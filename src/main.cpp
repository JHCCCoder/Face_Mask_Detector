#include <paddle_api.h>

using namespace paddle::lite_api;

int main() {
    MobileConfig config;
    config.set_model_from_buffer("/Users/minhao/Workspace/Face_Mask_Detector/assets/model.nb");
    auto predictor = CreatePaddlePredictor<MobileConfig>(config);

    return 0;
}