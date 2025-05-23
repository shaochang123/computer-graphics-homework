#ifndef GRAPH_INIT_H_INCLUDED
#define GRAPH_INIT_H_INCLUDED
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <math.h>
#include <map>
#include <queue>
#include <vector>
#include <stack>
#include <fstream>
#include<cstdlib>
#include<ctime>
#include<thread>
#include<atomic>
#include<algorithm>
#include<stdlib.h>
#include<string.h>
void initRandom() {
    srand(static_cast<unsigned>(time(nullptr)));
}
#define M_PI 3.14159265358979323846
struct Point {int x, y;};
struct Color{
    float r,g,b;//颜色值
    Color(float _r=0.0f,float _g=0.0f,float _b=0.0f):r(_r),g(_g),b(_b){}//构造函数
};
// 3x3变换矩阵 (使用齐次坐标系)
struct Matrix3x3 {
    float m[3][3];
    // 构造单位矩阵
    Matrix3x3() {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;
    }
    
    // 矩阵乘法
    Matrix3x3 operator*(const Matrix3x3& other) const {
        Matrix3x3 result;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                result.m[i][j] = 0.0f;
                for (int k = 0; k < 3; ++k) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }
};
// 然后再定义使用它们的结构体
struct graphic {
    std::vector<Point> points;      // 此图形的所有点
    int mode;                       // 图形类型
    Color color = {0, 0, 0};        // 图形颜色
    int width = 1;                  // 线宽
    Matrix3x3 transform = Matrix3x3();  // 初始化为单位矩阵
};
#define maxn 500 // 定义像素网格大小
Color g[maxn][maxn],curcolor={0.0,0.0,0.0},selectedColor = {129.0/255,148.0/255,240.0/255};
int mode=0,curwidth=1,ChooseIdx = -1;//g是到时候显示在窗口上的像素网格，cnt是每个图形的时间戳（为了撤回），mode是模式，w是线宽
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
// void inline render(){
    
//     glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲区
//     glMatrixMode(GL_MODELVIEW);//设置模型视图矩阵
//     glLoadIdentity();//重置当前的模型视图矩阵
//     // 遍历每个像素，绘制矩形
//     for (int i = 0; i < maxn; i++) {
//         for (int j = 0; j < maxn; j++) {
//             // 设置颜色
//             glColor3f(g[i][j].r, g[i][j].g, g[i][j].b);
//             // 绘制矩形（每个像素占1x1的正方形）
//             glBegin(GL_QUADS);
//             glVertex2f(j, i);        // 左下角
//             glVertex2f(j+1, i);      // 右下角
//             glVertex2f(j+1, i+1);    // 右上角
//             glVertex2f(j, i+1);      // 左上角
//             glEnd();
//         }
//     }
// }
GLFWwindow* init(int width, int height){
    if (!glfwInit()) return NULL;
    GLFWwindow* window = glfwCreateWindow(width, height, "fight simulation", NULL, NULL);
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
// 应用变换矩阵到点
Point applyTransform(const Point& p, const Matrix3x3& transform) {
    // 将点转换为齐次坐标
    float x = p.x;
    float y = p.y;
    float w = 1.0f;
    
    // 应用变换矩阵
    float newX = x * transform.m[0][0] + y * transform.m[0][1] + w * transform.m[0][2];
    float newY = x * transform.m[1][0] + y * transform.m[1][1] + w * transform.m[1][2];
    float newW = x * transform.m[2][0] + y * transform.m[2][1] + w * transform.m[2][2];
    
    // 齐次坐标归一化（除以w）
    if (newW != 0.0f) {
        newX /= newW;
        newY /= newW;
    }
    
    return {static_cast<int>(newX), static_cast<int>(newY)};
}
#endif // GRAPH_INIT_H_INCLUDED