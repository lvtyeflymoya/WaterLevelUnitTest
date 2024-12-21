add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/base)

list(APPEND ALL_LIBS
    lib_base
)

target_include_directories(${PROJECT_NAME} PUBLIC ${ALL_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC ${ALL_LIBS})