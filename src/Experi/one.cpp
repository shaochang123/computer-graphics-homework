#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

struct Point {
    int x, y;
};
#define maxn 200
int g[maxn][maxn];

void setPixel(Point p) {
    g[p.x-1][p.y-1] = 1;
}

// Bresenham直线算法
void drawLineBresenham(Point start, Point end) {
    int dx = abs(end.x - start.x), sx = start.x < end.x ? 1 : -1;
    int dy = -abs(end.y - start.y), sy = start.y < end.y ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        setPixel(start);
        if (start.x == end.x && start.y == end.y) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; start.x += sx; }
        if (e2 <= dx) { err += dx; start.y += sy; }
    }
}

// 绘制像素网格
void draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 遍历每个像素，绘制矩形
    for (int i = 0; i < maxn; i++) {
        for (int j = 0; j < maxn; j++) {
            // 设置颜色
            if (g[j][i] == 1) {
                glColor3f(0.0f, 0.0f, 0.0f); // 黑色
            } else {
                glColor3f(1.0f, 1.0f, 1.0f); // 白色
            }

            // 绘制矩形（每个像素占1x1的正方形）
            glBegin(GL_QUADS);
            glVertex2f(j, i);        // 左下角
            glVertex2f(j+1, i);      // 右下角
            glVertex2f(j+1, i+1);    // 右上角
            glVertex2f(j, i+1);      // 左上角
            glEnd();
        }
    }
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // 调整视口大小
    glViewport(0, 0, width, height);

    // 重新设置正交投影
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, maxn, 0.0, maxn, -1.0, 1.0);
}
int main() {
    // 初始化GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(800, 800, "Bresenham Algorithm", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 注册窗口大小回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 设置正交投影
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, maxn, 0.0, maxn, -1.0, 1.0); // 坐标范围为[0, maxn]x[0, maxn]

    // 绘制直线（初始化阶段）
    drawLineBresenham({1, 20}, {200, 200});

    // 主循环
    while (!glfwWindowShouldClose(window)) {
        // 渲染
        draw();
        // 刷新屏幕
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 清理并退出
    glfwTerminate();
    return 0;
}