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
#include <cmath>
#include <random>
#include "model.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

// camera value
extern Camera camera;

// delta time
extern float deltaTime;
extern float lastTime;

// 鼠标上一帧的位置
extern float lastX;
extern float lastY;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_calback(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double x, double y);
unsigned int loadTexture(char const *path);
unsigned int loadCubemap(vector<std::string> faces);
void drawSkyBox(Shader shader, BoxGeometry geometry, unsigned int cubeMap);
void drawLightObject(Shader shader, BufferGeometry geometry, glm::vec3 position);
void drawMesh(BufferGeometry geometry);
void RenderQuad();
float lerp(float a, float b, float f);
unsigned int loadHdrTexture(char const *path);

#endif //EXAMPLE_H
