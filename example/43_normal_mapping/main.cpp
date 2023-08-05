#include "43_normal_mapping.hpp"

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

// camera value
Camera camera(glm::vec3(0.0f, 7.0f, 1.5f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -80.0f);

// delta time
float deltaTime = 0.0f;
float lastTime = 0.0f;

float lastX = SCREEN_WIDTH / 2.0f; // 鼠标上一帧的位置
float lastY = SCREEN_HEIGHT / 2.0f;

int main(int argc, char *argv[])
{
    glfwInit();
    // 设置主要和次要版本
    const char *glsl_version = "#version 330";

    // 片段着色器将作用域每一个采样点（采用4倍抗锯齿，则每个像素有4个片段（四个采样点））
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 窗口对象
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // -----------------------
    // 创建imgui上下文
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    // 设置样式
    ImGui::StyleColorsDark();
    // 设置平台和渲染器
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // -----------------------

    // 设置视口
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_PROGRAM_POINT_SIZE);

    // 启用混合
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 启用gamma校正
    // glEnable(GL_FRAMEBUFFER_SRGB);

    // 深度测试
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // 鼠标键盘事件
    // 1.注册窗口变化监听
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // 2.鼠标事件
    glfwSetCursorPosCallback(window, mouse_callback);
    // 3.将鼠标隐藏
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader sceneShader("../shader/scene_vert.glsl", "../shader/scene_frag.glsl");
    Shader lightObjectShader("../shader/light_object_vert.glsl", "../shader/light_object_frag.glsl");

    BoxGeometry boxGeometry(1.0, 1.0, 1.0);      // 箱子
    BoxGeometry floorGeometry(10.0, 0.01, 10.0); // 箱子
    PlaneGeometry planeGeometry(1.0, 1.0);
    SphereGeometry pointLightGeometry(0.06, 10.0, 10.0); // 点光源位置显示

    unsigned int woodDiffuseMap = loadTexture("../images/texture/wood.png");             // 地面
    unsigned int brickDiffuseMap = loadTexture("../images/texture/brickwall.jpg");       // 砖墙
    unsigned int brickNormalMap = loadTexture("../images/texture/brickwall_normal.jpg"); // 砖墙

    float factor = 0.0;

    sceneShader.use();
    sceneShader.setInt("diffuseMap", 0);
    sceneShader.setInt("normalMap", 1);

    glm::vec3 lightPosition = glm::vec3(-2.0f, 2.0f, 2.0f); // 光照位置
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastTime;
        lastTime = currentFrame;

        // 在标题中显示帧率信息
        // *************************************************************************
        int fps_value = (int)round(ImGui::GetIO().Framerate);
        int ms_value = (int)round(1000.0f / ImGui::GetIO().Framerate);

        std::string FPS = std::to_string(fps_value);
        std::string ms = std::to_string(ms_value);
        std::string newTitle = "LearnOpenGL - " + ms + " ms/frame " + FPS;
        glfwSetWindowTitle(window, newTitle.c_str());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // *************************************************************************

        // 渲染指令
        // ...
        glClearColor(25.0 / 255.0, 25.0 / 255.0, 25.0 / 255.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightPosition = glm::vec3(lightPosition.x + glm::sin(glfwGetTime()) * 0.03, lightPosition.y, lightPosition.z);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        sceneShader.use();
        sceneShader.setMat4("projection", projection);
        sceneShader.setMat4("view", view);
        sceneShader.setVec3("viewPos", camera.Position);
        sceneShader.setVec3("lightPos", lightPosition); // 光源位置
        sceneShader.setFloat("strength", 0.01);         // 环境光强度

        // 绘制地面
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, brickDiffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, brickNormalMap);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(5, 5, 5));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));

        sceneShader.setFloat("uvScale", 1.0f);
        sceneShader.setMat4("model", model);
        drawMesh(planeGeometry);

        // // 绘制箱子
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, brickDiffuseMap);

        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, brickNormalMap);

        // model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(0.0, 0.5, 0.0));
        // sceneShader.setFloat("uvScale", 1.0f);
        // sceneShader.setMat4("model", model);
        // drawMesh(boxGeometry);

        drawLightObject(lightObjectShader, pointLightGeometry, lightPosition);

        // 渲染 gui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    boxGeometry.dispose();
    floorGeometry.dispose();
    pointLightGeometry.dispose();

    glfwTerminate();

    return 0;
}