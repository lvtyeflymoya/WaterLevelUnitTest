add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/base)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/common)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sensor)


set (ALL_LIBS ${ALL_LIBS} PARENT_SCOPE)

target_include_directories(${PROJECT_NAME} PRIVATE ${ALL_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${ALL_LIBS})