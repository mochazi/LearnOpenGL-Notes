set(EXECUTABLE_NAME_1 example_04_shader_glsl_1)
set(EXECUTABLE_NAME_2 example_04_shader_glsl_2)

add_executable(${EXECUTABLE_NAME_1} main1.cpp 04_shader_glsl.cpp 04_shader_glsl.hpp)
add_executable(${EXECUTABLE_NAME_2} main2.cpp 04_shader_glsl.cpp 04_shader_glsl.hpp)
set_target_properties(${EXECUTABLE_NAME_1}  PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set_target_properties(${EXECUTABLE_NAME_2}  PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# 链接 lib
target_link_libraries(${EXECUTABLE_NAME_1}  PRIVATE glfw)
target_link_libraries(${EXECUTABLE_NAME_1}  PRIVATE glad)
target_link_libraries(${EXECUTABLE_NAME_1}  PRIVATE pthread)

target_link_libraries(${EXECUTABLE_NAME_2}  PRIVATE glfw)
target_link_libraries(${EXECUTABLE_NAME_2}  PRIVATE glad)
target_link_libraries(${EXECUTABLE_NAME_2}  PRIVATE pthread)