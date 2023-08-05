set(EXECUTABLE_NAME example_47_deferred_shading)

file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES *.hpp)

add_executable(${EXECUTABLE_NAME} ${INCLUDE_FILES} ${SOURCE_FILES})

set_target_properties(${EXECUTABLE_NAME}  PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# 链接 lib
target_link_libraries(${EXECUTABLE_NAME}  PRIVATE glfw)
target_link_libraries(${EXECUTABLE_NAME}  PRIVATE glad)
target_link_libraries(${EXECUTABLE_NAME}  PRIVATE glm)
target_link_libraries(${EXECUTABLE_NAME}  PRIVATE imgui)
target_link_libraries(${EXECUTABLE_NAME}  PRIVATE tool)
target_link_libraries(${EXECUTABLE_NAME}  PRIVATE geometry)
target_link_libraries(${EXECUTABLE_NAME}  PRIVATE assimp)
target_link_libraries(${EXECUTABLE_NAME}  PRIVATE mesh)
target_link_libraries(${EXECUTABLE_NAME}  PRIVATE pthread)