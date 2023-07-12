#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tool/shader.h>
#include <geometry/BoxGeometry.h>
#include <geometry/PlaneGeometry.h>
#include <geometry/SphereGeometry.h>
#include <tool/gui.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

#endif //EXAMPLE_H
