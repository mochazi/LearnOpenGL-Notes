# 获取当前目录下所有子目录
file(GLOB subdirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(subdir ${subdirs})
    # 如果是目录，则作为子目录添加到CMake中
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})

        # assimp暂时不处理，后面单独添加
        if (${CMAKE_CURRENT_SOURCE_DIR}/${subdir} MATCHES  "assimp")
            continue()
        endif ()

        add_subdirectory(${subdir})
    endif()
endforeach()

# gcc13构建 assimp 空悬引用误报解决
# https://github.com/assimp/assimp/pull/5163/commits/ac2cf2f71b58c2a0d0fe697de1ec74b5928a20ac
IF(CMAKE_CXX_COMPILER_VERSION GREATER_EQUAL 13)
    MESSAGE(STATUS "GCC13 detected disabling \"-Wdangling-reference\" in Cpp files as it appears to be a false positive")
    ADD_COMPILE_OPTIONS("$<$<COMPILE_LANGUAGE:CXX>:-Wno-dangling-reference>")
ENDIF()

# 添加 assimp 库
add_subdirectory(assimp)