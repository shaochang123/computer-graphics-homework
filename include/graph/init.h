#ifndef GRAPH_INIT_H_INCLUDED
#define GRAPH_INIT_H_INCLUDED
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <math.h>
#include <queue>
#include <vector>
#include <fstream>
#define M_PI 3.14159265358979323846
struct Point {int x, y;};
// 在文件开头的全局变量部分添加
struct Color {
    float r, g, b;
};
Color currentColor = {0.0f, 0.0f, 0.0f}; // 默认为黑色
std::vector<Color>Cnt2Color;
#define maxn 200 // 定义像素网格大小
uint8_t g[maxn][maxn],cnt=1,mode=0,w=1;//g是到时候显示在窗口上的像素网格，cnt是每个图形的时间戳（为了撤回），mode是模式，ArcStep是绘制圆弧的步骤
void setpixel(int x,int y, int w=1){
    for(int i =x-w+1;i<=x+w-1;i++){
        for(int j=y-w+1;j<=y+w-1;j++){
            if(i>=0&&i<maxn&&j>=0&&j<maxn&&g[i][j]==0)g[i][j]=cnt;
            
        }
    }
    
}
// 保存图像函数
void saveImage(const char* filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
    // 写入文件头
    file.put(0).put(0).put(2); // 无压缩的TGA格式
    file.put(0).put(0);
    file.put(0).put(0);
    file.put(0).put(0);
    file.put(0).put(0);
    file.put(0).put(0);
    file.put(0).put(0);
    file.put(0).put(0);
    file.put(maxn & 0x00FF).put((maxn & 0xFF00) >> 8);
    file.put(maxn & 0x00FF).put((maxn & 0xFF00) >> 8);
    file.put(24); // 24位颜色深度
    file.put(0);  
}
// 转换函数，将窗口坐标转换为g数组的索引
void detectposition(GLFWwindow *window, double &xpos, double &ypos) {
    glfwGetCursorPos(window, &xpos, &ypos);
    // 将窗口坐标转换为OpenGL坐标
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    xpos = xpos / width * maxn + 1;
    ypos = maxn - ypos / height * maxn + 1;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // 调整视口大小
    glViewport(0, 0, width, height);
    // 重新设置正交投影
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();//重置当前的投影矩阵
    glOrtho(0.0, maxn, 0.0, maxn, -1.0, 1.0);//设置正交投影
}
// 渲染函数，将g数组的像素网格渲染到窗口上
void render(){
    glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲区
    glMatrixMode(GL_MODELVIEW);//设置模型视图矩阵
    glLoadIdentity();//重置当前的模型视图矩阵

    // 遍历每个像素，绘制矩形
    for (int i = 0; i < maxn; i++) {
        for (int j = 0; j < maxn; j++) {
            // 设置颜色
            if (g[j][i]) {
                if(g[j][i]!=cnt)glColor3f(Cnt2Color[g[j][i]-1].r,Cnt2Color[g[j][i]-1].g,Cnt2Color[g[j][i]-1].b); // 历史图形颜色
                else glColor3f(currentColor.r,currentColor.g,currentColor.b);
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
void flushwindow(){
    // 刷新屏幕
    for(int i=0;i<maxn;i++){
        for(int j=0;j<maxn;j++){
            if(g[i][j]==cnt)g[i][j]=0;

        }
    }
}
void ReturnToBack(int key, int mods, int action){
    if (key == GLFW_KEY_Z && mods == GLFW_MOD_CONTROL && action == GLFW_PRESS) {//撤回操作
        // 将 g 上面为 cnt 的格子变为白色
        for (int i = 0; i < maxn; i++) {
            for (int j = 0; j < maxn; j++) {
                if (g[i][j] == cnt-1) {
                    g[i][j] = 0;
                }
            }
        }
        if(!Cnt2Color.empty())Cnt2Color.pop_back();
        cnt--;//退时间戳
        if(cnt<1)cnt=1;//防止撤回过多
    }
}
void Save2Image(int key, int mods, int action){
    if (key == GLFW_KEY_S && mods == GLFW_MOD_CONTROL && action == GLFW_PRESS) {//保存图像
        saveImage("output.tga");
    }
}
void ConvertWidth(int key,int mods,int action){
    if(key == GLFW_KEY_1 && action == GLFW_PRESS){
        w = 1;
        std::cout<<"Width = 1 now\n";
    }
    if(key == GLFW_KEY_2 && action == GLFW_PRESS){
        w = 2;
        std::cout<<"Width = 2 now\n";
    }
    if(key == GLFW_KEY_3 && action == GLFW_PRESS){
        w = 3;
        std::cout<<"Width = 3 now\n";
    }
    if(key == GLFW_KEY_4 && action == GLFW_PRESS){
        w = 4;
        std::cout<<"Width = 4 now\n";
    }
    if(key == GLFW_KEY_5 && action == GLFW_PRESS){
        w = 5;
        std::cout<<"Width = 5 now\n";
    }
}
void ConvertColor(int key, int mods, int action) {
    if (mods=GLFW_MOD_CONTROL && action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_R: // 红色
                currentColor = {1.0f, 0.0f, 0.0f};
                std::cout << "Color set to Red\n";
                break;
            case GLFW_KEY_G: // 绿色
                currentColor = {0.0f, 1.0f, 0.0f};
                std::cout << "Color set to Green\n";
                break;
            case GLFW_KEY_B: // 蓝色
                currentColor = {0.0f, 0.0f, 1.0f};
                std::cout << "Color set to Blue\n";
                break;
            case GLFW_KEY_Y: // 黄色
                currentColor = {1.0f, 1.0f, 0.0f};
                std::cout << "Color set to Yellow\n";
                break;
            case GLFW_KEY_C: // 青色
                currentColor = {0.0f, 1.0f, 1.0f};
                std::cout << "Color set to Cyan\n";
                break;
            case GLFW_KEY_M: // 洋红色
                currentColor = {1.0f, 0.0f, 1.0f};
                std::cout << "Color set to Magenta\n";
                break;
            case GLFW_KEY_K: // 黑色
                currentColor = {0.0f, 0.0f, 0.0f};
                std::cout << "Color set to Black\n";
                break;
            case GLFW_KEY_W: // 白色
                currentColor = {1.0f, 1.0f, 1.0f};
                std::cout << "Color set to White\n";
                break;
        }
    }
}
GLFWwindow* init(int width, int height){
    if (!glfwInit()) return NULL;
    GLFWwindow* window = glfwCreateWindow(width, height, "painting toolbox", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    // 注册窗口大小回调函数，这个函数可以在你拉伸窗口后调整图像大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // 初始化投影
    framebuffer_size_callback(window, width, height);
    return window;
}
#endif // GRAPH_INIT_H_INCLUDED