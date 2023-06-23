add_library(imgui "")

target_sources(imgui
PRIVATE
    src/imgui.cpp
    src/imgui_demo.cpp
    src/imgui_draw.cpp
    src/imgui_impl_glfw.cpp
    src/imgui_impl_opengl3.cpp
    src/imgui_widgets.cpp
PUBLIC
    include/imgui/imconfig.h
    include/imgui/imgui.h
    include/imgui/imgui_impl_glfw.h
    include/imgui/imgui_impl_opengl3.h
    include/imgui/imgui_internal.h
    include/imgui/imstb_rectpack.h
    include/imgui/imstb_textedit.h
    include/imgui/imstb_truetype.h
)

set_target_properties(imgui PROPERTIES VERSION v1.78)

target_link_libraries(imgui  PRIVATE glad)
target_link_libraries(imgui  PRIVATE glfw)

target_include_directories(imgui PUBLIC include)