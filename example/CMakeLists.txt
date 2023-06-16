# 获取当前目录下所有子目录
file(GLOB subdirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(subdir ${subdirs})
    # 如果是目录，则作为子目录添加到CMake中
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
        add_subdirectory(${subdir})
    endif()
endforeach()