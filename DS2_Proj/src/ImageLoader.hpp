#ifndef IMAGELOADER_HPP
#define IMAGELOADER_HPP

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>  // If using GLFW, as it includes OpenGL types as well

#include <imgui.h>

bool loadImage(const std::string& path, GLuint& textureID, ImVec2& size);

#endif
