#define _USE_MATH_DEFINES
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cstring>

struct Point { int x, y; };
const int maxn = 200;
uint8_t g[maxn][maxn] = {0}, cnt = 1, mode = 0;
bool isMousePressed = false;
Point st, arcO, arcA;
int arcStep = 0;

// 新增：像素设置函数
void setPixel(int x, int y) {
    if (x >= 0 && x < maxn && y >= 0 && y < maxn)
        g[x][y] = cnt;
}

// 修改后的坐标系转换函数
void convertCoords(GLFWwindow* window, double& x, double& y) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    x = x / width * maxn;
    y = maxn - y / height * maxn;
}

// 修改后的 Bresenham 直线算法
void drawLineBresenham(Point start, Point end) {
    int dx = abs(end.x - start.x), sx = start.x < end.x ? 1 : -1;
    int dy = -abs(end.y - start.y), sy = start.y < end.y ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        setPixel(start.x, start.y);
        if (start.x == end.x && start.y == end.y) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; start.x += sx; }
        if (e2 <= dx) { err += dx; start.y += sy; }
    }
}

// 改进的角度判断函数
void checkAndDraw(int px, int py, Point center, double startRad, double endRad) {
    if (px < 0 || px >= maxn || py < 0 || py >= maxn) return;
    
    int dx = px - center.x;
    int dy = center.y - py;
    double theta = atan2(dy, dx);
    
    if (theta < 0) theta += 2 * M_PI;
    bool inArc = (theta >= startRad && theta <= endRad) || 
                (startRad > endRad && (theta >= startRad || theta <= endRad));
    
    if (inArc && g[px][py] == 0) g[px][py] = cnt;
}

// 优化后的圆弧绘制算法
void drawArc(Point center, int r, double startRad, double endRad) {
    int x = 0, y = r;
    int d = 1 - r;
    
    while (x <= y) {
        checkAndDraw(center.x + x, center.y + y, center, startRad, endRad);
        checkAndDraw(center.x - x, center.y + y, center, startRad, endRad);
        checkAndDraw(center.x + x, center.y - y, center, startRad, endRad);
        checkAndDraw(center.x - x, center.y - y, center, startRad, endRad);
        checkAndDraw(center.x + y, center.y + x, center, startRad, endRad);
        checkAndDraw(center.x - y, center.y + x, center, startRad, endRad);
        checkAndDraw(center.x + y, center.y - x, center, startRad, endRad);
        checkAndDraw(center.x - y, center.y - x, center, startRad, endRad);
        
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

// 改进的鼠标回调函数
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        convertCoords(window, x, y);
        Point current = { static_cast<int>(x), static_cast<int>(y) };

        if (mode == 2) {
            switch (arcStep) {
                case 0:
                    arcO = current;
                    arcStep = 1;
                    std::cout << "With the center of the circle selected, click to determine the radius\n";
                    break;
                case 1:
                    arcA = current;
                    arcStep = 2;
                    std::cout << "radius is selected,click to determine the destination\n";
                    break;
                case 2: {
                    int dx = arcA.x - arcO.x;
                    int dy = arcA.y - arcO.y;
                    int r = sqrt(dx*dx + dy*dy);
                    
                    double startRad = atan2(arcO.y - arcA.y, arcA.x - arcO.x);
                    double endRad = atan2(arcO.y - current.y, current.x - arcO.x);
                    
                    startRad = startRad < 0 ? startRad + 2*M_PI : startRad;
                    endRad = endRad < 0 ? endRad + 2*M_PI : endRad;
                    
                    drawArc(arcO, r, startRad, endRad);
                    cnt++;
                    arcStep = 0;
                    break;
                }
            }
        } else {
            if (isMousePressed) {
                if (mode == 0) drawLineBresenham(st, current);
                else if (mode == 1) {
                    int r = sqrt(pow(current.x-st.x,2) + pow(current.y-st.y,2));
                    drawArc(st, r, 0, 2*M_PI);
                }
                cnt++;
                isMousePressed = false;
            } else {
                isMousePressed = true;
                st = current;
            }
        }
    }
}

// 键盘回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_L: 
                mode = 0; 
                std::cout << "line mode\n";
                break;
            case GLFW_KEY_C: 
                mode = 1; 
                std::cout << "circle mode\n";
                break;
            case GLFW_KEY_A: 
                mode = 2; 
                arcStep = 0;
                std::cout << "part of circle(three steps)\n";
                break;
            case GLFW_KEY_Z:
                cnt = cnt > 1 ? cnt-1 : 1;
                memset(g, 0, sizeof(g)); 
                std::cout << "reset\n";
                break;
        }
    }
}

// OpenGL 渲染函数
void render(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // 绘制网格线
    glBegin(GL_LINES);
    glColor3f(0.2f, 0.2f, 0.2f);
    for (int i = 0; i < maxn; i++) {
        glVertex2f(i, 0);
        glVertex2f(i, maxn);
        glVertex2f(0, i);
        glVertex2f(maxn, i);
    }
    glEnd();
    
    // 绘制像素点
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int x = 0; x < maxn; ++x) {
        for (int y = 0; y < maxn; ++y) {
            if (g[x][y] != 0) {
                glVertex2i(x, y);
            }
        }
    }
    glEnd();
}

// 窗口尺寸回调
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, maxn, 0, maxn, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 800, "painting toolbox", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // 初始化投影
    framebuffer_size_callback(window, maxn, maxn);
    
    while (!glfwWindowShouldClose(window)) {
        render(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}