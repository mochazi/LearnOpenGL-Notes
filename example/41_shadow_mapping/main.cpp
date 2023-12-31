#include "41_shadow_mapping.hpp"

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

// camera value
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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
    Shader simpleShadowShader("../shader/shadow_map_vert.glsl", "../shader/shadow_map_frag.glsl");
    Shader finalShaderShader("../shader/shadow_final_vert.glsl", "../shader/shadow_final_frag.glsl");

    Shader quadShader("../shader/shadow_quad_vert.glsl", "../shader/shadow_quad_frag.glsl");

    PlaneGeometry groundGeometry(10.0, 10.0);            // 地面
    PlaneGeometry quadGeometry(6.0, 6.0);                // 测试面板
    BoxGeometry boxGeometry(1.0, 1.0, 1.0);              // 箱子
    BoxGeometry floorGeometry(10.0, 0.0001, 10.0);       // 箱子
    SphereGeometry pointLightGeometry(0.06, 10.0, 10.0); // 点光源位置显示

    unsigned int woodMap = loadTexture("../images/texture/wood.png");           // 地面
    unsigned int brickMap = loadTexture("../images/texture/brick_diffuse.jpg"); // 砖墙

    float factor = 0.0;

    // ------------------------------------------------
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    quadShader.use();
    quadShader.setInt("depthMap", 0);

    // ------------------------------------------------
    finalShaderShader.use();
    finalShaderShader.setInt("diffuseTexture", 0);
    finalShaderShader.setInt("shadowMap", 1);

    glm::vec3 lightPosition = glm::vec3(-2.0f, 3.0f, -1.0f); // 光照位置
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

        float radius = 5.0f;
        float camX = sin(glfwGetTime() * 0.5) * radius;
        float camZ = cos(glfwGetTime() * 0.5) * radius;
        lightPosition = glm::vec3(lightPosition.x + glm::sin(glfwGetTime()) * 0.03, lightPosition.y, lightPosition.z);

        glm::mat4 model = glm::mat4(1.0f);
        // ++++++++++++++++++++++++++++++++++++++++++++++++ 渲染深度贴图
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 7.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPosition, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightSpaceMatrix = lightProjection * lightView;
        simpleShadowShader.use();
        simpleShadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        // 绘制场景
        glBindTexture(GL_TEXTURE_2D, woodMap);

        simpleShadowShader.setMat4("model", model);
        drawMesh(floorGeometry);

        glBindTexture(GL_TEXTURE_2D, brickMap);
        model = glm::translate(model, glm::vec3(0.0, 0.5, 0.0));
        simpleShadowShader.setMat4("model", model);
        drawMesh(boxGeometry);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // ++++++++++++++++++++++++++++++++++++++++++++++++

        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        finalShaderShader.use();
        finalShaderShader.setMat4("view", view);
        finalShaderShader.setMat4("projection", projection);
        finalShaderShader.setVec3("viewPos", camera.Position);

        finalShaderShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        finalShaderShader.setVec3("lightPos", lightPosition); // 光源位置
        finalShaderShader.setFloat("uvScale", 4.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        model = glm::mat4(1.0f);
        finalShaderShader.setMat4("model", model);
        drawMesh(floorGeometry);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, brickMap);
        model = glm::translate(model, glm::vec3(0.0, 0.5, 0.0));
        finalShaderShader.setMat4("model", model);
        finalShaderShader.setFloat("uvScale", 1.0f);
        drawMesh(boxGeometry);

        // 显示深度贴图
        // *************************************************
        // quadShader.use();
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, depthMap);
        // quadShader.setMat4("view", view);

        // model = glm::mat4(1.0f);
        // quadShader.setFloat("near_plane", near_plane);
        // quadShader.setFloat("far_plane", far_plane);
        // quadShader.setMat4("model", model);
        // quadShader.setMat4("projection", projection);
        // drawMesh(quadGeometry);
        // *************************************************

        drawLightObject(lightObjectShader, pointLightGeometry, lightPosition);

        // 渲染 gui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    groundGeometry.dispose();
    pointLightGeometry.dispose();
    glfwTerminate();

    return 0;
}