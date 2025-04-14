cmake_minimum_required(VERSION 3.15) # Minimum version requirement
project(ECommerceDemoGUI LANGUAGES CXX C) # Project name and languages (C needed for glad.c)

set(CMAKE_CXX_STANDARD 17)             # Use C++17
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Source Files ---
# Add your project's C++ source files
set(PROJECT_SOURCES
    src/Trie.cpp
    src/ProductCatalog.cpp
    src/main_gui.cpp
    src/glad.c # Include the GLAD source file
)

# Add ImGui source files and backends
set(IMGUI_SOURCES
    extern/imgui/imgui.cpp
    extern/imgui/imgui_draw.cpp
    extern/imgui/imgui_tables.cpp
    extern/imgui/imgui_widgets.cpp
    extern/imgui/backends/imgui_impl_glfw.cpp
    extern/imgui/backends/imgui_impl_opengl3.cpp
)

# --- Include Directories ---
# Tell CMake where to find header files
include_directories(
    src # For your own headers (Trie.h etc.)
    extern/imgui # For imgui.h and backend headers
    glad # For glad.h, KHR/khrplatform.h
)

# --- Find Dependencies ---
# Find GLFW package
find_package(glfw3 3.3 REQUIRED)

# Find OpenGL (standard CMake module)
find_package(OpenGL REQUIRED)

# --- Create Executable ---
add_executable(e_commerce_gui ${PROJECT_SOURCES} ${IMGUI_SOURCES})

# --- Link Libraries ---
target_link_libraries(e_commerce_gui PRIVATE
    glfw # Link GLFW
    OpenGL::GL # Link OpenGL (modern CMake way)
)

# Add platform-specific libraries (OpenGL needs these sometimes)
if (WIN32)
    # Example: target_link_libraries(e_commerce_gui PRIVATE ...)
elseif (APPLE)
    target_link_libraries(e_commerce_gui PRIVATE "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
elseif (UNIX AND NOT APPLE)
    # Linux linking often handled by OpenGL/glfw discovery, but might need -lGL, -lX11 etc. if issues arise
    # Check glfw documentation if needed.
endif()

# Optional: Set output directory (CMake generates build files elsewhere)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})