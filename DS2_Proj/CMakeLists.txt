cmake_minimum_required(VERSION 3.15)
project(ECommerceDemoGUI LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Source Files ---
set(PROJECT_SOURCES
    src/Trie.cpp
    src/ProductCatalog.cpp
    src/main_gui.cpp
    src/glad.c
)

# ImGui source files
set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

# --- Include Directories ---
include_directories(
    src
    glad
    imgui
    imgui/backends
    imgui/stb  
    C:/glfw-3.4.bin.WIN64/include
)

# --- Link Directories ---
link_directories(
    C:/glfw-3.4.bin.WIN64/lib-mingw-w64
)

# --- Find OpenGL ---
find_package(OpenGL REQUIRED)

# --- Executable ---
add_executable(e_commerce_gui ${PROJECT_SOURCES} ${IMGUI_SOURCES})

# --- Link Libraries ---
target_link_libraries(e_commerce_gui PRIVATE
    glfw3
    OpenGL::GL
)

# Platform-specific stuff (optional)
if (WIN32)
    # Windows-specific libs if needed (not necessary right now)
elseif (APPLE)
    target_link_libraries(e_commerce_gui PRIVATE "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
elseif (UNIX AND NOT APPLE)
    # Linux-specific
endif()

# Optional output directory
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
