#include <glad/glad.h>
#define GLFW_INCLUDE_NONE  // 添加这一行，阻止GLFW包含gl.h
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>

// 着色器代码
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    
    out vec3 ourColor;
    
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    
    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        ourColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 ourColor;
    out vec4 FragColor;
    
    void main() {
        FragColor = vec4(ourColor, 1.0);
    }
)";

const char* planeVertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    
    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)";

const char* planeFragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    
    uniform vec3 planeColor;
    
    void main() {
        FragColor = vec4(planeColor, 0.5); // 半透明
    }
)";

const char* projectionVertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 projectionMatrix; // 投影矩阵
    
    void main() {
        // 计算投影后的位置
        vec4 projectedPos = projectionMatrix * vec4(aPos, 1.0);
        projectedPos /= projectedPos.w;
        
        gl_Position = projection * view * model * vec4(projectedPos.xyz, 1.0);
    }
)";

const char* projectionFragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    
    void main() {
        FragColor = vec4(0.7, 0.0, 0.7, 1.0); // 紫色投影
    }
)";

// 窗口尺寸
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 相机参数
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// 鼠标控制参数
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;
bool firstMouse = true;

// 投影平面参数
glm::vec3 planePosition = glm::vec3(0.0f, -2.0f, 0.0f);
glm::vec3 planeNormal = glm::vec3(0.0f, 1.0f, 0.0f);
float planeSize = 4.0f;

// 投影光源位置（用于计算3D物体的投影）
glm::vec3 lightPos = glm::vec3(0.0f, 5.0f, 0.0f);

// 处理输入
void processInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // 相机移动
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
    // 投影平面移动
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        planePosition.y += cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        planePosition.y -= cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        planePosition.x -= cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        planePosition.x += cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        planePosition.z -= cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        planePosition.z += cameraSpeed;
    
    // 调整投影光源位置
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        lightPos.y += cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        lightPos.y -= cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        lightPos.x -= cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        lightPos.x += cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        lightPos.z -= cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        lightPos.z += cameraSpeed;
}

// 鼠标回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw += xoffset;
    pitch += yoffset;
    
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

// 创建着色器程序
unsigned int createShaderProgram(const char* vertexShader, const char* fragmentShader) {
    // 编译顶点着色器
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShader, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "错误::顶点着色器编译失败\n" << infoLog << std::endl;
    }
    
    // 编译片段着色器
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShader, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "错误::片段着色器编译失败\n" << infoLog << std::endl;
    }
    
    // 链接着色器
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "错误::着色器程序链接失败\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    return shaderProgram;
}

// 计算投影矩阵
glm::mat4 calculateShadowMatrix(glm::vec3 lightPos, glm::vec3 planePos, glm::vec3 planeNormal) {
    planeNormal = glm::normalize(planeNormal);
    float d = glm::dot(planeNormal, planePos);
    
    float dot = glm::dot(planeNormal, lightPos) - d;
    
    // 构建阴影矩阵
    glm::mat4 shadowMat;
    shadowMat[0][0] = dot - planeNormal.x * lightPos.x;
    shadowMat[0][1] = -planeNormal.x * lightPos.y;
    shadowMat[0][2] = -planeNormal.x * lightPos.z;
    shadowMat[0][3] = -planeNormal.x;
    
    shadowMat[1][0] = -planeNormal.y * lightPos.x;
    shadowMat[1][1] = dot - planeNormal.y * lightPos.y;
    shadowMat[1][2] = -planeNormal.y * lightPos.z;
    shadowMat[1][3] = -planeNormal.y;
    
    shadowMat[2][0] = -planeNormal.z * lightPos.x;
    shadowMat[2][1] = -planeNormal.z * lightPos.y;
    shadowMat[2][2] = dot - planeNormal.z * lightPos.z;
    shadowMat[2][3] = -planeNormal.z;
    
    shadowMat[3][0] = -d * lightPos.x;
    shadowMat[3][1] = -d * lightPos.y;
    shadowMat[3][2] = -d * lightPos.z;
    shadowMat[3][3] = dot - d;
    
    return shadowMat;
}

int main() {
    // 初始化GLFW
    if (!glfwInit()) {
        std::cout << "GLFW初始化失败" << std::endl;
        return -1;
    }
    
    // 设置OpenGL版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D投影演示", NULL, NULL);
    if (!window) {
        std::cout << "窗口创建失败" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // 设置视口大小随窗口变化
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });
    
    // 设置鼠标回调
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLAD初始化失败" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 启用混合
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // 创建着色器程序
    unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    unsigned int planeShaderProgram = createShaderProgram(planeVertexShaderSource, planeFragmentShaderSource);
    unsigned int projectionShaderProgram = createShaderProgram(projectionVertexShaderSource, projectionFragmentShaderSource);
    
    // 创建立方体顶点数据
    float cubeVertices[] = {
        // 位置                 // 颜色
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f
    };
    
    unsigned int cubeIndices[] = {
        0, 1, 2, 2, 3, 0,       // 前面
        4, 5, 6, 6, 7, 4,       // 后面
        8, 9, 10, 10, 11, 8,    // 左面
        12, 13, 14, 14, 15, 12, // 右面
        16, 17, 18, 18, 19, 16, // 底面
        20, 21, 22, 22, 23, 20  // 顶面
    };
    
    // 创建投影平面顶点数据
    float planeVertices[] = {
        -planeSize/2, 0.0f, -planeSize/2,
         planeSize/2, 0.0f, -planeSize/2,
         planeSize/2, 0.0f,  planeSize/2,
        -planeSize/2, 0.0f,  planeSize/2
    };
    
    unsigned int planeIndices[] = {
        0, 1, 2,
        0, 2, 3
    };
    
    // 创建立方体VAO
    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);
    
    glBindVertexArray(cubeVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
    
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // 创建平面VAO
    unsigned int planeVAO, planeVBO, planeEBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glGenBuffers(1, &planeEBO);
    
    glBindVertexArray(planeVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices, GL_STATIC_DRAW);
    
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // 解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // 渲染循环
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    while (!glfwWindowShouldClose(window)) {
        // 计算帧时间
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // 处理输入
        processInput(window, deltaTime);
        
        // 清除颜色缓冲和深度缓冲
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // 设置相机视图矩阵
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
        // 设置投影矩阵
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        // 立方体位置
        glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, 0.0f);
        
        // 渲染立方体
        glUseProgram(shaderProgram);
        
        // 更新着色器变量
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePos);
        model = glm::rotate(model, glm::radians(currentFrame * 50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        // 计算阴影投影矩阵
        glm::mat4 shadowMatrix = calculateShadowMatrix(lightPos, planePosition, planeNormal);
        
        // 渲染立方体投影
        glUseProgram(projectionShaderProgram);
        
        modelLoc = glGetUniformLocation(projectionShaderProgram, "model");
        viewLoc = glGetUniformLocation(projectionShaderProgram, "view");
        projectionLoc = glGetUniformLocation(projectionShaderProgram, "projection");
        unsigned int shadowMatrixLoc = glGetUniformLocation(projectionShaderProgram, "projectionMatrix");
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(shadowMatrixLoc, 1, GL_FALSE, glm::value_ptr(shadowMatrix));
        
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        // 渲染投影平面
        glUseProgram(planeShaderProgram);
        
        model = glm::mat4(1.0f);
        // 应用旋转让平面始终垂直于法线方向
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 axis = glm::cross(up, planeNormal);
        float angle = glm::acos(glm::dot(up, planeNormal));
        
        model = glm::translate(model, planePosition);
        if (glm::length(axis) > 0.01f)  // 避免零向量
            model = glm::rotate(model, angle, glm::normalize(axis));
        
        modelLoc = glGetUniformLocation(planeShaderProgram, "model");
        viewLoc = glGetUniformLocation(planeShaderProgram, "view");
        projectionLoc = glGetUniformLocation(planeShaderProgram, "projection");
        unsigned int planeColorLoc = glGetUniformLocation(planeShaderProgram, "planeColor");
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(planeColorLoc, 0.5f, 0.8f, 0.8f);  // 设置平面颜色
        
        glBindVertexArray(planeVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // 绘制光源位置标记点
        glUseProgram(shaderProgram);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        // 交换缓冲并检查事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // 清理资源
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteBuffers(1, &planeEBO);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(planeShaderProgram);
    glDeleteProgram(projectionShaderProgram);
    
    glfwTerminate();
    return 0;
}