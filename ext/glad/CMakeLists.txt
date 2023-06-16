add_library(glad "")

set_target_properties(glad PROPERTIES VERSION glad-gl4.6-core)

target_sources(glad
PRIVATE
        src/glad.c
PUBLIC
        include/glad/glad.h
        include/KHR/khrplatform.h
)

target_include_directories(glad PUBLIC include)