#include "49_pbr_light.hpp"

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

// camera value
Camera camera(glm::vec3(0.0f, 13.0f, 22.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -30.0f);

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

    // 深度测试
    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LESS);

    // 鼠标键盘事件
    // 1.注册窗口变化监听
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // 2.鼠标事件
    glfwSetCursorPosCallback(window, mouse_callback);
    // 3.将鼠标隐藏
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader sceneShader("../shader/scene_vert.glsl", "../shader/scene_texture_frag.glsl");
//    Shader sceneShader("../shader/scene_vert.glsl", "../shader/scene_frag.glsl");
    Shader sceneTextureShader("../shader/scene_vert.glsl", "../shader/scene_texture_frag.glsl");
    Shader lightObjShader("../shader/light_object_vert.glsl", "../shader/light_object_frag.glsl");

    PlaneGeometry groundGeometry(10.0, 10.0);            // 地面
    BoxGeometry boxGeometry(1.0, 1.0, 1.0);              // 盒子
    SphereGeometry pointLightGeometry(0.17, 64.0, 64.0); // 点光源位置显示
    SphereGeometry objectGeometry(1.0, 64.0, 64.0);      // 圆球

    // 点光源的位置
    vector<glm::vec3> lightPositions{
            glm::vec3(-10.0f, 10.0f, 10.0f),
            glm::vec3(10.0f, 10.0f, 10.0f),
            glm::vec3(-10.0f, -10.0f, 10.0f),
            glm::vec3(10.0f, -10.0f, 10.0f),
    };

    // 点光源颜色
    vector<glm::vec3> lightColors{
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f),
    };

    int nrRows = 7;
    int nrColumns = 7;
    float spacing = 2.5;

    sceneShader.use();
    sceneShader.setVec3("albedo", 0.0f, 0.5f, 0.0f);
    sceneShader.setFloat("ao", 1.0f);

//     unsigned int albedoMap = loadTexture("../images/texture/solar/TexturesCom_PaintedConcreteFloor_1K_albedo.png");
//     unsigned int normalMap = loadTexture("../images/texture/solar/TexturesCom_PaintedConcreteFloor_1K_normal.png");
//     unsigned int metallicMap = loadTexture("../images/texture/solar/TexturesCom_PaintedConcreteFloor_1K_metallic.png");
//     unsigned int roughnessMap = loadTexture("../images/texture/solar/TexturesCom_PaintedConcreteFloor_1K_roughness.png");
//     unsigned int aoMap = loadTexture("../images/texture/solar/TexturesCom_PaintedConcreteFloor_1K_ao.png");

    unsigned int albedoMap = loadTexture("../images/texture/tiles/TexturesCom_Marble_TilesSquare8_512_albedo.png");
    unsigned int normalMap = loadTexture("../images/texture/tiles/TexturesCom_Marble_TilesSquare8_512_normal.png");
    unsigned int roughnessMap = loadTexture("../images/texture/tiles/TexturesCom_Marble_TilesSquare8_512_roughness.png");
    unsigned int metallicMap = 0;
    unsigned int aoMap = 0;

    // 设置贴图
    sceneShader.setInt("albedoMap", 0);
    sceneShader.setInt("normalMap", 1);
    sceneShader.setInt("metallicMap", 2);
    sceneShader.setInt("roughnessMap", 3);
    sceneShader.setInt("aoMap", 4);

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

        glClearColor(25.0 / 255.0, 25.0 / 255.0, 25.0 / 255.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        float radius = 5.0f;
        float camX = sin(glfwGetTime() * 0.5) * radius;
        float camZ = cos(glfwGetTime() * 0.5) * radius;

        lightPositions[1].x = camX;
        lightPositions[1].y = camZ;

        for (unsigned int i = 0; i < lightPositions.size(); i++)
        {
            glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 15.0, 0.0, 0.0);
            newPos = lightPositions[i];
            sceneShader.use();
            sceneShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
            sceneShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
        }

        sceneShader.setMat4("projection", projection);
        sceneShader.setMat4("view", view);
        sceneShader.setVec3("camPos", camera.Position);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, albedoMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, metallicMap);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, roughnessMap);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, aoMap);

        for (int row = 0; row < nrRows; ++row)
        {
            sceneShader.setFloat("metallic", (float)row / (float)nrRows);
            for (int col = 0; col < nrColumns; ++col)
            {

                sceneShader.setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((col - (nrColumns / 2)) * spacing, (row - (nrRows / 2)) * spacing, 0.0f));
                sceneShader.setMat4("model", model);

                // ........render
                drawMesh(objectGeometry);
            }
        }

        // 绘制灯光物体
        // --------------------------
        lightObjShader.use();
        lightObjShader.setMat4("view", view);
        lightObjShader.setMat4("projection", projection);

        for (unsigned int i = 0; i < lightPositions.size(); i++)
        {
            glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
            newPos = lightPositions[i];

            model = glm::mat4(1.0f);
            model = glm::translate(model, newPos);
            lightObjShader.setMat4("model", model);
            lightObjShader.setVec3("lightColor", lightColors[i]);
            drawMesh(pointLightGeometry);
        }
        // --------------------------

        // 渲染 gui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}