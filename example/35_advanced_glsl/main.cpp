#include "35_advanced_glsl.hpp"

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

// camera value
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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
    // glfwWindowHint(GLFW_SAMPLES, 4);
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

    // 深度测试
    glEnable(GL_DEPTH_TEST);

    // 鼠标键盘事件
    // 1.注册窗口变化监听
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // 2.鼠标事件
    glfwSetCursorPosCallback(window, mouse_callback);
    // 3.将鼠标隐藏
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader sceneShader1("../shader/scene_vert.glsl", "../shader/scene1_frag.glsl");
    Shader sceneShader2("../shader/scene_vert.glsl", "../shader/scene2_frag.glsl");
    Shader sceneShader3("../shader/scene_vert.glsl", "../shader/scene3_frag.glsl");
    Shader sceneShader4("../shader/scene_vert.glsl", "../shader/scene4_frag.glsl");

    SphereGeometry sphereGeometry(1.0, 50.0, 50.0); // 圆球
    BoxGeometry boxGeometry(1.0, 1.0, 1.0);         // 盒子

    float fov = 45.0f; // 视锥体的角度
    glm::vec3 view_translate = glm::vec3(0.0, 0.0, -5.0);
    ImVec4 clear_color = ImVec4(25.0 / 255.0, 25.0 / 255.0, 25.0 / 255.0, 1.0); // 25, 25, 25

    unsigned int uvMap = loadTexture("../images/texture/uv_grid_directx.jpg");
    unsigned int triMap = loadTexture("../images/texture/tri_pattern.jpg");

    // 获取绑定点
    unsigned int uniformBlockIndex_1 = glGetUniformBlockIndex(sceneShader1.ID, "Matrices");
    unsigned int uniformBlockIndex_2 = glGetUniformBlockIndex(sceneShader2.ID, "Matrices");
    unsigned int uniformBlockIndex_3 = glGetUniformBlockIndex(sceneShader3.ID, "Matrices");
    unsigned int uniformBlockIndex_4 = glGetUniformBlockIndex(sceneShader4.ID, "Matrices");

    // 将顶点着色器中uniform块设置为绑定点0
    glUniformBlockBinding(sceneShader1.ID, uniformBlockIndex_1, 0);
    glUniformBlockBinding(sceneShader2.ID, uniformBlockIndex_2, 0);
    glUniformBlockBinding(sceneShader3.ID, uniformBlockIndex_3, 0);
    glUniformBlockBinding(sceneShader4.ID, uniformBlockIndex_4, 0);

    // 创建uniform缓冲对象，并将其绑定到绑定点0
    unsigned int uboMatrices;
    glGenBuffers(1, &uboMatrices);

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW); // 分配内存
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4)); //将特定范围链接到绑定点0

    // 进入渲染循环之前填充投影矩阵
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

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
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, uvMap);
        glBindVertexArray(boxGeometry.VAO);

        float rotate = glfwGetTime() * 0.2f;
        glm::qua<float> qu = glm::qua<float>(glm::vec3(rotate, rotate, rotate));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f));
        model = model * glm::mat4_cast(qu);
        sceneShader1.use();
        sceneShader1.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, boxGeometry.indices.size(), GL_UNSIGNED_INT, 0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f));
        model = model * glm::mat4_cast(qu);
        sceneShader2.use();
        sceneShader2.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, boxGeometry.indices.size(), GL_UNSIGNED_INT, 0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f));
        model = model * glm::mat4_cast(qu);
        sceneShader3.use();
        sceneShader3.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, boxGeometry.indices.size(), GL_UNSIGNED_INT, 0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f));
        model = model * glm::mat4_cast(qu);
        sceneShader4.use();
        sceneShader4.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, boxGeometry.indices.size(), GL_UNSIGNED_INT, 0);

        // 渲染 gui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}