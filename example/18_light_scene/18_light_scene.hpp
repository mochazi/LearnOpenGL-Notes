#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tool/shader.h>
#include <tool/camera.h>
#include <geometry/BoxGeometry.h>
#include <geometry/PlaneGeometry.h>
#include <geometry/SphereGeometry.h>
#include <tool/gui.h>
#include <iostream>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

// camera value
extern Camera camera;


// delta time
extern float deltaTime;
extern float lastTime;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_calback(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double x, double y);

#endif //EXAMPLE_H
