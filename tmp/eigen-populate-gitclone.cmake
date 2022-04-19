
if(NOT "/Users/minhao/Workspace/Face_Mask_Detector/src/eigen-populate-stamp/eigen-populate-gitinfo.txt" IS_NEWER_THAN "/Users/minhao/Workspace/Face_Mask_Detector/src/eigen-populate-stamp/eigen-populate-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/Users/minhao/Workspace/Face_Mask_Detector/src/eigen-populate-stamp/eigen-populate-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/Users/minhao/Workspace/Face_Mask_Detector/eigen"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/Users/minhao/Workspace/Face_Mask_Detector/eigen'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"  clone --no-checkout --progress --config "advice.detachedHead=false" "https://gitlab.com/libeigen/eigen.git" "eigen"
    WORKING_DIRECTORY "/Users/minhao/Workspace/Face_Mask_Detector"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://gitlab.com/libeigen/eigen.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git"  checkout 008ff3483a8c5604639e1c4d204eae30ad737af6 --
  WORKING_DIRECTORY "/Users/minhao/Workspace/Face_Mask_Detector/eigen"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '008ff3483a8c5604639e1c4d204eae30ad737af6'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/Users/minhao/Workspace/Face_Mask_Detector/eigen"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/Users/minhao/Workspace/Face_Mask_Detector/eigen'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/Users/minhao/Workspace/Face_Mask_Detector/src/eigen-populate-stamp/eigen-populate-gitinfo.txt"
    "/Users/minhao/Workspace/Face_Mask_Detector/src/eigen-populate-stamp/eigen-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/Users/minhao/Workspace/Face_Mask_Detector/src/eigen-populate-stamp/eigen-populate-gitclone-lastrun.txt'")
endif()

