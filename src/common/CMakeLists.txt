add_library(lib_common 
SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/ImageSaver.cpp
)

target_include_directories(lib_common 
PUBLIC 
    ${ALL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(lib_common
PUBLIC 
    ${ALL_LIBS}
)