
#ifndef FACEMASKDETECTOR_PIPELINE_H
#define FACEMASKDETECTOR_PIPELINE_H

#include <CppTimer.h>

#include <condition_variable>  //NOLINT [build/c++11]
#include <deque>
#include <mutex>   //NOLINT [build/c++11]
#include <thread>  //NOLINT [build/c++11]
#include <vector>
#include <opencv2/opencv.hpp>

/**
 * @brief A synchronising buffer and results structure
 *
 */
namespace Buffer {

/**
 * @brief Result when calling `Buffer::pop()`
 *
 * The structure has a field to indicate the validity of the result. It is
 * possible for the buffer to return a result when the owning pipeline is shut
 * down. In this situation the result could be invalid as the call to `pop()`
 * may be blocking until data is available. In this situation a `valid` flag of
 * `false` means the `value` field should not be used. A `valid` flag of `true`
 * means the `value` is useable from the buffer's perspective.
 *
 * @tparam T Type of elements on the buffer
 */
    template <typename T>
    struct PopResult {
        T value;     ///< Actual popped result
        bool valid;  ///< Indicates validity of the result
    };

/**
 * @brief A synchronising buffer to be used as a communication mechanism between
 * threads
 *
 * The buffer ensures that elements are in order based on the element's `id`
 * field. Attempting to `push` an element that is not next in line will block
 * until the missing element has been pushed by a different thread. An object of
 * this class may therefore be used to share the load at any stage in a pipeline
 * between multiple threads without needing to worry about issues in the order
 * of elements.
 *
 * It is implemented as a circular buffer.
 *
 * @tparam T The type for elements in the buffer. This must provide an `id`
 * field that is a `uint8_t`.
 */
    template <typename T>
    class Buffer {
    private:
        std::mutex lock_in;    ///< `std::mutex` for input to the object
        std::mutex lock_out;   ///< `std::mutex` for output of the object
        std::vector<T> queue;  ///< Underlying queueing mechanism

        size_t front_index = 0;  ///< Front (next to be popped) of the buffer
        size_t back_index = 0;   ///< Back (where to push) of the buffer

        /**
         * @brief Flag to indicate if the pipeline is running
         *
         * When this becomes `false` any blocked threads should be allowed to quit,
         * i.e., infinite loops should exit
         *
         */
        bool* running;

        /**
         * @brief ID number for the newest frame on the queue
         *
         * The next frame that will be added to the queue will therefore be `id + 1`.
         * An ID of `255` is defined to be followed by an ID of `0`, meaning there are
         * no problems due to an integer overflow.
         *
         */
        uint8_t id = -1;

        /**
         * @brief Get the current number of elements in the buffer
         *
         * @return `size_t` number of elements in the buffer
         */
        size_t size(void) {
            int size = back_index - front_index;
            if (size < 0) {
                return (queue.size() + size);
            }
            return size;
        }

        /**
         * @brief Flag to indicate if the buffer is full
         *
         */
        bool full = false;

    public:
        /**
         * @brief Construct a new `Buffer` object
         *
         * @param running_ptr Pointer to the `Pipeline::Pipeline::running` flag
         * @param max_size Maximum desired useable size of underlying memory
         */
        explicit Buffer(bool* running_ptr, size_t max_size)
                : running(running_ptr), queue(max_size) {}

        /**
         * @brief Push a frame to the queue
         *
         * This blocks if the frame's ID is not the next in the series, until the
         * missing frame has been pushed by another thread. Also blocks if the maximum
         * size is reached, until elements are popped.
         *
         * @tparam frame Frame to be pushed to the queue
         */
        void push(T frame) {
            while (*running) {
                lock_in.lock();
                if ((uint8_t)(id + 1) == frame.id && !full) {
                    queue.at(back_index) = frame;
                    back_index = (back_index + 1) % queue.size();
                    if (back_index == front_index) {
                        full = true;
                    }

                    id++;
                    lock_in.unlock();
                    break;
                }
                lock_in.unlock();
            }
        }

        /**
         * @brief Push a frame to the queue (doesn't block if queue is full)
         *
         * This blocks if the frame's ID is not the next in the series, until the
         * missing frame has been pushed by another thread. It returns if the queue is
         * full.
         *
         * @param frame Frame to be pushed to the queue
         * @return `true` If pushing was successful
         * @return `false` If the frame was not pushed, i.e., the queue was full; or
         * the pipeline is halting
         */
        bool try_push(T frame) {
            while (*running) {
                // Immediately return if the queue is full
                lock_in.lock();
                if (full) {
                    lock_in.unlock();
                    return false;
                }

                if ((uint8_t)(id + 1) == frame.id) {
                    queue.at(back_index) = frame;
                    back_index = (back_index + 1) % queue.size();
                    if (front_index == back_index) {
                        full = true;
                    }

                    id++;
                    lock_in.unlock();
                    return true;
                } else {
                    lock_in.unlock();
                }
            }
            return false;
        }

        /**
         * @brief Pop the oldest element in the queue and return it
         *
         * @return `PopResult<T>` Oldest frame on the queue. Note: the `valid` flag of
         * the result must be checked before use.
         */
        PopResult<T> pop() {
            PopResult<T> front = PopResult<T>{T{}, false};
            while (*running) {
                lock_out.lock();
                if (size() != 0 || full) {
                    front = PopResult<T>{queue.at(front_index), true};
                    front_index = (front_index + 1) % queue.size();
                    full = false;

                    lock_out.unlock();
                    break;
                }
                lock_out.unlock();
            }
            return front;
        }
    };
}  // namespace Buffer

/**
 * @brief A frame as returned by the `FrameGenerator`
 *
 */
struct RawFrame {
    uint8_t id;         ///< Frame ordering ID
    cv::Mat raw_image;  ///< Raw `cv::Mat` (OpenCV) image
};

/**
 * @brief Class to maintain access to the `cv::VideoCapture` used as the input
 * video stream. This class makes use of the `CppTimer` wrapper created
 * <a href="https://github.com/berndporr/cppTimer">here</a> to regularly
 * capture input frames at the set frame rate.
 */
class FrameGenerator : public CppTimer {
private:
    /**
     * @brief Video stream handle
     *
     */
    cv::VideoCapture cap;

    /**
     * @brief Identifier to keep track of frame ordering
     *
     * Each frame is labelled with an ID that increments by one from one frame to
     * the next. The FrameGenerator never skips and ID. Once the maximum value is
     * reached, the number overflows and wraps back to zero. Therefore the maximum
     * number possible for the ID is less than zero, i.e., it is always possible
     * to determine what the next expected frame ID is.
     *
     * Access to this should be protected by `lock`
     *
     */
    uint8_t id = 0;

    /**
     * @brief The most up-to-date frame retrieved from the camera
     *
     * Access to this should be protected by `lock`
     *
     */
    cv::Mat current_frame;

    /**
     * @brief Lock to protect the most current frame-related data
     *
     * Enables the synchronisation of `id` and `current_frame`
     *
     */
    std::mutex mutex;

    /**
     * @brief Condition variable to make callers of `next_frame()` wait until the
     * next frame is ready to be processed.
     *
     */
    std::condition_variable cv;

    /**
     * @brief Continuously running thread to ensure the class always has the
     * newest frame ready
     *
     * This cannot be in the body of the timer as OpenCV buffers camera frames.
     * The only reliable way, at the time of writing, to get the newest frame is
     * to constantly poll the camera so the OpenCV buffer is emptied. Not doing
     * this would result in outdated frames being used in the pipeline, leading to
     * a very noticeable system latency.
     *
     */
    std::thread thread;

    /**
     * @brief Implementation of how to retrieve the next frame from the camera
     *
     */
    void thread_body();

    /**
     * @brief Timer to notify waiting threads they can process the current frame
     *
     * When the timer fires, at most one thread currently waiting in a
     * `next_frame()` call will be notified and will start to process the frame.
     *
     * This timer controls the frame rate of the pipeline.
     *
     */
    void timerEvent();

    /**
     * @brief Flag to tell `thread_body` whether or not it should be running
     *
     */
    bool running = true;

public:
    /**
     * @brief Construct a new Frame Generator object
     *
     * @throw `std::runtime_error` if the camera cannot be accessed
     */
    FrameGenerator();

    /**
     * @brief Destroy the Frame Generator object
     *
     * Blocks until the `thread` that generates the frames is joined
     *
     */
    ~FrameGenerator();

    /**
     * @brief Notify the `FrameGenerator` that the frame rate has changed
     *
     * The underlying timer is stopped and restarted with the new frame delay
     *
     * @param new_frame_delay New delay from one frame to the next in ms
     */
    void updated_framerate(size_t new_frame_delay);

    /**
     * @brief Get the newest frame
     *
     * @return `RawFrame` The most up-to-date frame from the camera
     */
    RawFrame next_frame();
};


class Pipeline {

};


#endif //FACEMASKDETECTOR_PIPELINE_H
