add_library(lib_model_inference
SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/BaseInference.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SegmentationInference.cpp
)

target_include_directories(lib_model_inference
PUBLIC
    ${ALL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(lib_model_inference 
PRIVATE 
    ${ALL_LIBS}
)

list(APPEND ALL_LIBS lib_model_inference)
set (ALL_LIBS ${ALL_LIBS} PARENT_SCOPE)