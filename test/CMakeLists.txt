# 指定要创建可执行文件的 cpp 文件列表
list(APPEND ALL_TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_ImageStitch.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_ImageSaver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_ThreadPool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_detailed_stitch.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_highlevel_stitch.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_LocalImage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_inference.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_folder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_WaterLevelDataWriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_WaterLevelDataReader.cpp
    
    # ${CMAKE_CURRENT_SOURCE_DIR}/test_HIKSDK.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/test_HikCamera.cpp
)

# message("ALL_TEST_FILES: ${ALL_TEST_FILES}")
# message("ALL_INCLUDE_DIRS: ${ALL_INCLUDE_DIRS}")
# message("ALL_LIBS: ${ALL_LIBS}")

create_executables_from_list(ALL_TEST_FILES)