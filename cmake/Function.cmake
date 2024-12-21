# 遍历一个list, 为每一项创建同名的可执行文件并添加头文件目录和链接库
function(create_executables_from_list cpp_files)
    foreach(cpp_file IN LISTS ${cpp_files})
        # 获取文件名，不带扩展名
        get_filename_component(exe_name ${cpp_file} NAME_WE)

        # 创建可执行文件
        add_executable(${exe_name} ${cpp_file})

        # 添加头文件目录
        target_include_directories(${exe_name} PUBLIC ${ALL_INCLUDE_DIRS})

        # 添加链接库
        target_link_libraries(${exe_name} PUBLIC ${ALL_LIBS})

    endforeach()
endfunction()