add_library(mesh "")

target_sources(mesh
PRIVATE
    src/model.cpp
PUBLIC
    include/mesh.h
    include/model.h
)

target_link_libraries(mesh  PRIVATE glad)
target_link_libraries(mesh  PRIVATE glfw)
target_link_libraries(mesh  PRIVATE glm)
target_link_libraries(mesh  PRIVATE assimp)
target_link_libraries(mesh  PRIVATE tool)
target_link_libraries(mesh  PRIVATE bz2)

target_include_directories(mesh PUBLIC include)