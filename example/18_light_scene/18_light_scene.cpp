#include "18_light_scene.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // 相机按键控制
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.Position += cameraSpeed * camera.Front;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.Position -= cameraSpeed * camera.Front;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.Position -= glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.Position += glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
    }
}

// 鼠标移动监听
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    // cout << "xpos " << xpos << endl;
    // cout << "ypos " << ypos << endl;
}
void mouse_button_calback(GLFWwindow *window, int button, int action, int mods)
{

    if (action == GLFW_PRESS)
        switch (button)
        {
            case GLFW_MOUSE_BUTTON_LEFT:
                // cout << "mouse left" << endl;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                // cout << "mouse middle" << endl;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                // cout << "mouse right" << endl;
                break;
        }
}

void cursor_position_callback(GLFWwindow *window, double x, double y)
{
    float xpos = float((x - SCREEN_WIDTH / 2) / SCREEN_WIDTH) * 2;
    float ypos = float(0 - (y - SCREEN_HEIGHT / 2) / SCREEN_HEIGHT) * 2;

    // cout << "xpos " << xpos << endl;
    // cout << "ypos " << ypos << endl;
    return;
}

void scroll_callback(GLFWwindow *window, double x, double y)
{
    // cout << "x " << x << endl;
    // cout << "y " << y << endl;
    return;
}