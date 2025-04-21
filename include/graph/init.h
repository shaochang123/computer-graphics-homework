#ifndef GRAPH_INIT_H_INCLUDED
#define GRAPH_INIT_H_INCLUDED
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <math.h>
#include <queue>
#include <vector>
#include <stack>
#include <fstream>
#define M_PI 3.14159265358979323846
struct Point {int x, y;};
struct Color{
    float r,g,b;//颜色值
    Color(float _r=0.0f,float _g=0.0f,float _b=0.0f):r(_r),g(_g),b(_b){}//构造函数
};
struct graphic{
    std::vector<Point>points;//此图形的所有点
    int mode;//图形类型 0为直线
    Color color={0,0,0};//图形颜色
    int width=1;//线宽
    int dx=0,dy=0;
};
std::vector<Point>curpoints;//当前图形的所有点
std::vector<graphic>graphics;//所有图形
std::stack<graphic>graphicsbackup;//撤回的图形
#define maxn 600 // 定义像素网格大小
Color g[maxn][maxn],curcolor={0.0,0.0,0.0},selectedColor = {129.0/255,148.0/255,240.0/255};
int mode=0,curwidth=1,ChooseIdx = -1;//g是到时候显示在窗口上的像素网格，cnt是每个图形的时间戳（为了撤回），mode是模式，w是线宽
double xpos,ypos;//鼠标坐标
#include<transform/selectgraph.h>
void setpixel(int x,int y, int w=1,Color colorr={0.0,0.0,0.0}){
    for(int i =x-w+1;i<=x+w-1;i++){
        for(int j=y-w+1;j<=y+w-1;j++){
            if(i>=0&&i<maxn&&j>=0&&j<maxn)g[j][i]=colorr;//将像素点涂黑
        }
    }
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
            glColor3f(g[i][j].r, g[i][j].g, g[i][j].b);
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
            g[i][j]={1.0,1.0,1.0};
        }
    }
}
void ChangeWidth(int key,int action){
    if(key == GLFW_KEY_UP && action == GLFW_PRESS) {
        if(mode!=-1){
            curwidth++;
            std::cout << "Line width increased to: " << curwidth << std::endl;
        }
        else {
            if(ChooseIdx>=0&&ChooseIdx<graphics.size())graphics[ChooseIdx].width++;
            std::cout << "Line width increased to: " << graphics[ChooseIdx].width << std::endl;
        }
        
    }
    else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        if(curwidth > 1 && mode!=-1) {
            curwidth--;
            std::cout << "Line width decreased to: " << curwidth << std::endl;
        }
        else{
            if(graphics[ChooseIdx].width>1){
                if(ChooseIdx>=0&&ChooseIdx<graphics.size())graphics[ChooseIdx].width--;
                std::cout << "Line width decreased to: " << graphics[ChooseIdx].width << std::endl;
            }
        }
    }
}
void ChangeColor(int key,int action){
    if(key == GLFW_KEY_R && action == GLFW_PRESS) {
        if(mode!=-1)curcolor = {1.0f, 0.0f, 0.0f}; // 红色
        else if(ChooseIdx>=0&&ChooseIdx<graphics.size())graphics[ChooseIdx].color = {1.0, 0.0, 0.0};
        std::cout << "Color changed to red" << std::endl;
    }
    else if(key == GLFW_KEY_G && action == GLFW_PRESS) {
        if(mode!=-1)curcolor = {0.0f, 1.0f, 0.0f}; // 绿色
        else if(ChooseIdx>=0&&ChooseIdx<graphics.size())graphics[ChooseIdx].color = {0.0, 1.0, 0.0};
        std::cout << "Color changed to green" << std::endl;
    }
    else if(key == GLFW_KEY_B && action == GLFW_PRESS) {
        if(mode!=-1)curcolor = {0.0f, 0.0f, 1.0f}; // 蓝色
        else if(ChooseIdx>=0&&ChooseIdx<graphics.size())graphics[ChooseIdx].color = {0.0, 0.0, 1.0};
        std::cout << "Color changed to blue" << std::endl;
    }
}
void saveImage(int key, int mods, int action){
    if(key == GLFW_KEY_S && mods == GLFW_MOD_CONTROL && action == GLFW_PRESS) {
        std::ofstream out("image.txt");
        if(out.is_open()) {
            for(int i = 0; i < maxn; i++) {
                for(int j = 0; j < maxn; j++) {
                    out << g[i][j].r << " " << g[i][j].g << " " << g[i][j].b << "\n";
                }
            }
            out.close();
            std::cout << "Image saved to image.txt" << std::endl;
        } else {
            std::cerr << "Failed to open file for saving" << std::endl;
        }
    }
}
void backup(int key,int mods, int action){
    if (key == GLFW_KEY_Z && mods == GLFW_MOD_CONTROL &&action == GLFW_PRESS) {
        if(!graphics.empty()){
            graphicsbackup.push(graphics.back());
            graphics.pop_back();
            if(ChooseIdx>=graphics.size())ChooseIdx--;
            std::cout << "Undo last action" << std::endl;
        }
        else{
            std::cout << "No actions to undo" << std::endl;
        }
    }
}
void frontup(int key, int mods, int action){
    if (key == GLFW_KEY_Y && mods == GLFW_MOD_CONTROL && action == GLFW_PRESS) {
        if(!graphicsbackup.empty()){
            graphics.push_back(graphicsbackup.top());
            graphicsbackup.pop();
            std::cout << "Redo last action" << std::endl;
        }
        else{
            std::cout << "No actions to redo" << std::endl;
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
    for(int i=0;i<maxn;i++){
        for(int j=0;j<maxn;j++){
            g[i][j]={1,1,1};//初始化像素网格为白色
        }
    }
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