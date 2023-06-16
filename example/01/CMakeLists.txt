set(EXECUTABLE_NAME example-01)

file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES *.hpp)

add_executable(${EXECUTABLE_NAME} ${INCLUDE_FILES} ${SOURCE_FILES})

# 链接 lib
target_link_libraries(${EXECUTABLE_NAME}  PRIVATE glfw)
target_link_libraries(${EXECUTABLE_NAME}  PRIVATE glad)
target_link_libraries(${EXECUTABLE_NAME}  PRIVATE pthread)