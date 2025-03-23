#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
struct Point {int x, y;}st;
#define maxn 200 // 定义像素网格大小
uint8_t g[maxn][maxn],cnt=1,mode=0;
bool isMousePressed = false; // 鼠标是否按下
// Bresenham直线算法
void detectposition(GLFWwindow *window, double &xpos, double &ypos) {
    glfwGetCursorPos(window, &xpos, &ypos);
    // 将窗口坐标转换为OpenGL坐标
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    xpos = xpos / width * maxn;
    ypos = (height - ypos) / height * maxn;
}

void drawLineBresenham(Point start, Point end) {
    int dx = abs(end.x - start.x), sx = start.x < end.x ? 1 : -1;
    int dy = -abs(end.y - start.y), sy = start.y < end.y ? 1 : -1;
    int err = dx + dy, e2;
    while (true) {
        if(start.x-1>=0&&start.y-1>=0&&start.x-1<maxn&&start.y-1<maxn&&g[start.x-1][start.y-1]==0)g[start.x-1][start.y-1] = cnt;//画点
        if (start.x == end.x && start.y == end.y) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; start.x += sx; }
        if (e2 <= dx) { err += dx; start.y += sy; }
    }
}
void drawcircle(Point center,int r){
    int x=0,y=r;
    int d=1-r;
    while(x<=y){
        if(center.x+x>=0&&center.y+y>=0&&center.x+x<maxn&&center.y+y<maxn&&g[center.x+x][center.y+y]==0)g[center.x+x][center.y+y]=cnt;
        if(center.x+x>=0&&center.y-y>=0&&center.x+x<maxn&&center.y-y<maxn&&g[center.x+x][center.y-y]==0)g[center.x+x][center.y-y]=cnt;
        if(center.x-x>=0&&center.y+y>=0&&center.x-x<maxn&&center.y+y<maxn&&g[center.x-x][center.y+y]==0)g[center.x-x][center.y+y]=cnt;
        if(center.x-x>=0&&center.y-y>=0&&center.x-x<maxn&&center.y-y<maxn&&g[center.x-x][center.y-y]==0)g[center.x-x][center.y-y]=cnt;
        if(center.x+y>=0&&center.y+x>=0&&center.x+y<maxn&&center.y+x<maxn&&g[center.x+y][center.y+x]==0)g[center.x+y][center.y+x]=cnt;
        if(center.x+y>=0&&center.y-x>=0&&center.x+y<maxn&&center.y-x<maxn&&g[center.x+y][center.y-x]==0)g[center.x+y][center.y-x]=cnt;
        if(center.x-y>=0&&center.y+x>=0&&center.x-y<maxn&&center.y+x<maxn&&g[center.x-y][center.y+x]==0)g[center.x-y][center.y+x]=cnt;
        if(center.x-y>=0&&center.y-x>=0&&center.x-y<maxn&&center.y-x<maxn&&g[center.x-y][center.y-x]==0)g[center.x-y][center.y-x]=cnt;
        if(d<0){
            d+=2*x+3;
        }
        else{
            d+=2*(x-y)+5;
            y--;
        }
        x++;
    }
}
// 绘制像素网格
void drawLine(GLFWwindow *window) {
    double xpos, ypos;
    detectposition(window, xpos, ypos);
    if(isMousePressed ==true){
        drawLineBresenham(st, {static_cast<int>(xpos), static_cast<int>(ypos)});
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 遍历每个像素，绘制矩形
    for (int i = 0; i < maxn; i++) {
        for (int j = 0; j < maxn; j++) {
            // 设置颜色
            if (g[j][i]) {
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
void drawCircle(GLFWwindow *window){
    double xpos, ypos;
    detectposition(window, xpos, ypos);
    if(isMousePressed ==true){
        int r=static_cast<int>(sqrt((xpos-st.x)*(xpos-st.x)+(ypos-st.y)*(ypos-st.y)));
        drawcircle(st,r);
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 遍历每个像素，绘制矩形
    for (int i = 0; i < maxn; i++) {
        for (int j = 0; j < maxn; j++) {
            // 设置颜色
            if (g[j][i]) {
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

// 鼠标点击回调函数
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if(isMousePressed == true){
            if(mode ==0)
                drawLine(window);
            else
                drawCircle(window);
            cnt++;
            isMousePressed =false;
            st.x=0;st.y=0;
            return;
        }
        isMousePressed = true;
        double xpos,ypos;
        detectposition(window, xpos, ypos);
        st.x=static_cast<int>(xpos);st.y=static_cast<int>(ypos);
    }
}

// 键盘回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_Z && mods == GLFW_MOD_CONTROL && action == GLFW_PRESS) {
        // 将 g 上面为 cnt 的格子变为白色
        for (int i = 0; i < maxn; i++) {
            for (int j = 0; j < maxn; j++) {
                if (g[i][j] == cnt-1) {
                    g[i][j] = 0;
                }
            }
        }
        cnt--;
        if(cnt<1)cnt=1;//防止撤回过多
    }
    if(key == GLFW_KEY_L && action == GLFW_PRESS){
        mode=0;
        std::cout<<"Line Mode"<<std::endl;

    }
    if(key == GLFW_KEY_C && action == GLFW_PRESS){
        mode=1;
        std::cout<<"Circle Mode"<<std::endl;
    }
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

    // 注册窗口大小回调函数，这个函数可以在你拉伸窗口后调整图像大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 注册鼠标点击回调函数，用来鼠标点击画图
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    // 注册键盘回调函数
    glfwSetKeyCallback(window, key_callback);

    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 设置正交投影
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, maxn, 0.0, maxn, -1.0, 1.0); // 坐标范围为[0, maxn]x[0, maxn]

    // 绘制直线（键盘输入绘制）
    // drawLineBresenham({1, 20}, {200, 200});

    // 主循环
    while (!glfwWindowShouldClose(window)) {
        if(mode == 0){
            drawLine(window);//渲染直线
        }
        else{
            drawCircle(window);//渲染圆
        }
        // 刷新屏幕
        for(int i=0;i<maxn;i++){
            for(int j=0;j<maxn;j++){
                if(g[i][j]==cnt)g[i][j]=0;
            }
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // 清理并退出
    glfwTerminate();
    return 0;
}