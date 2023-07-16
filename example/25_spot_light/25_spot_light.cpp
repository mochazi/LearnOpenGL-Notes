#include "25_spot_light.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <tool/stb_image.h>

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
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    // cout << "xpos " << xpos << endl;
    // cout << "ypos " << ypos << endl;
    camera.ProcessMouseMovement(xoffset, yoffset);
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

// 加载纹理贴图
unsigned int loadTexture(char const *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}