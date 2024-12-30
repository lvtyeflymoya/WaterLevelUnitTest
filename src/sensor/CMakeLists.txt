add_library(lib_sensor 
SHARED 
    ${CMAKE_CURRENT_SOURCE_DIR}/ImageSensor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LocalImage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HikCamera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RTSPCamera.cpp
)

target_include_directories(lib_sensor 
PUBLIC 
    ${ALL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(lib_sensor 
PRIVATE 
    ${ALL_LIBS}
)

# 这里的ALL_LIBS包含了lib_base, lib_common, lib_sensor
list(APPEND ALL_LIBS lib_sensor)
set (ALL_LIBS ${ALL_LIBS} PARENT_SCOPE)