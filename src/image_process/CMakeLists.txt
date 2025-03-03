add_library(lib_image_process
SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/water_level_process_new.cpp
)

target_include_directories(lib_image_process
PUBLIC
    ${ALL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(lib_image_process 
PRIVATE 
    ${ALL_LIBS}
)

list(APPEND ALL_LIBS lib_image_process)
set (ALL_LIBS ${ALL_LIBS} PARENT_SCOPE)