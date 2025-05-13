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
    double cenx=0;
    double ceny=0;
    int key = 0;//专属于bezier
    int a=10;
    int b=10;
    bool isdefault = true;//是否是默认中心
};
std::vector<Point>curpoints;//当前图形的所有点
std::vector<graphic>graphics;//所有图形
std::stack<graphic>graphicsbackup;//撤回的图形
#define maxn 500 // 定义像素网格大小
Color g[maxn][maxn],curcolor={0.0,0.0,0.0},selectedColor = {129.0/255,148.0/255,240.0/255};
int mode=0,curwidth=1,ChooseIdx = -1,cura=10,curb=10;//g是到时候显示在窗口上的像素网格，cnt是每个图形的时间戳（为了撤回），mode是模式，w是线宽
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
void inline render(){
    
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
void ui() {                  
    
     // 设置初始位置，但只影响首次显示
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 420), ImGuiCond_FirstUseEver);
    ImGuiWindowFlags window_flags = 0;
    ImGui::Begin("Drawing Toolbox",nullptr, window_flags);
    // 模式选择按钮区域
    ImGui::Text("Drawing Mode:");
    if (ImGui::Button("Line Mode(Bresenham)")){mode=0;curpoints.clear();std::cout<<"Line(Bresenham) Mode"<<std::endl;}
    ImGui::SameLine();
    if (ImGui::Button("Line Mode(middleline)")){mode=7;curpoints.clear();std::cout<<"Line(middleline) Mode"<<std::endl;}
    ImGui::SameLine();
    if (ImGui::Button("Arc Mode")){mode=1;curpoints.clear();std::cout<<"Arc Mode"<<std::endl;}
    ImGui::SameLine();
    if (ImGui::Button("Crop Mode(Cohen-Sutherland)")){mode=5;curpoints.clear();std::cout<<"Crop Mode(Cohen-Sutherland)"<<std::endl;}
    if (ImGui::Button("Crop Mode(middle point)")){mode=8;curpoints.clear();std::cout<<"Crop Mode(middle point)"<<std::endl;}
    ImGui::SameLine();
    if (ImGui::Button("Circle Mode")){mode=2;curpoints.clear();std::cout<<"Circle Mode"<<std::endl;}
    if (ImGui::Button("Fill Mode")) {mode=3;curpoints.clear();std::cout<<"Fill Mode"<<std::endl;}
    ImGui::SameLine();
    if (ImGui::Button("Polygon Mode")){mode=4;curpoints.clear();std::cout<<"Polygon Mode"<<std::endl;}
    ImGui::SameLine();
    if (ImGui::Button("Bezier Mode")){mode=6;curpoints.clear();std::cout<<"Bezier Mode"<<std::endl;}
    ImGui::SameLine();
    if (ImGui::Button("Select Mode")){mode=-1;curpoints.clear();std::cout<<"Select Mode"<<std::endl;}
    
    ImGui::Separator();
    
    // 颜色调整区域
    ImGui::Text("Color Control:");
    
    // 创建临时变量存储当前颜色值（值范围0.0-1.0）
    static float color[3] = {0.0f, 0.0f, 0.0f};
    
    // 根据模式和选择状态获取当前颜色
    if (mode != -1) {
        // 在绘图模式下，使用当前颜色
        color[0] = curcolor.r;
        color[1] = curcolor.g;
        color[2] = curcolor.b;
    } else if (ChooseIdx >= 0 && ChooseIdx < graphics.size()) {
        // 在选择模式下，使用选中图形的颜色
        color[0] = graphics[ChooseIdx].color.r;
        color[1] = graphics[ChooseIdx].color.g;
        color[2] = graphics[ChooseIdx].color.b;
    }
    
    // 显示滑块控制RGB颜色
    bool colorChanged = false;
    colorChanged |= ImGui::SliderFloat("Red", &color[0], 0.0f, 1.0f);
    colorChanged |= ImGui::SliderFloat("Green", &color[1], 0.0f, 1.0f);
    colorChanged |= ImGui::SliderFloat("Blue", &color[2], 0.0f, 1.0f);
    
    // 根据模式将更改应用到相应的颜色变量
    if (colorChanged) {
        if (mode != -1) {
            // 更新当前绘图颜色
            curcolor = {color[0], color[1], color[2]};
        } else if (ChooseIdx >= 0 && ChooseIdx < graphics.size()) {
            // 更新选中图形的颜色
            graphics[ChooseIdx].color = {color[0], color[1], color[2]};
        }
    }
    
    ImGui::Separator();
    
    // 线宽控制
    int wid = (mode != -1) ? curwidth : 
                (ChooseIdx >= 0 && ChooseIdx < graphics.size()) ? graphics[ChooseIdx].width : 1;
    if (ImGui::SliderInt("Line Width", &wid, 1, 10)) {
        if (mode != -1) {
            curwidth = wid;
        } else if (ChooseIdx >= 0 && ChooseIdx < graphics.size()) {
            graphics[ChooseIdx].width = wid;
        }
    }
    // 线型控制
    int a = (mode != -1) ? cura : 
                (ChooseIdx >= 0 && ChooseIdx < graphics.size()) ? graphics[ChooseIdx].a : 1;
    if (ImGui::SliderInt("Line type", &a, 1, 10)) {
        if (mode != -1) {
            cura = a;
        } else if (ChooseIdx >= 0 && ChooseIdx < graphics.size()) {
            graphics[ChooseIdx].a = a;
        }
    }
    ImGui::Separator();
    
    // 显示当前模式
    const char* modeNames[] = {"Line(Bresenham)", "Arc", "Circle", "Fill", "Polygon", "Crop", "Bezier", "Line(middleline)", "Selection"};
    int displayMode = (mode == -1) ? 8 : mode;
    if (displayMode >= 0 && displayMode < 9) {
        ImGui::Text("Current Mode: %s", modeNames[displayMode]);
        if(displayMode==8)ImGui::Text("%d",ChooseIdx);
    }
    
    // 显示键盘快捷键信息
    ImGui::Separator();
    ImGui::Text("Keyboard Shortcuts:");
    ImGui::BulletText("Ctrl+S: Save image");
    ImGui::BulletText("Ctrl+Z: Undo");
    ImGui::BulletText("Ctrl+Y: Redo");
    
    ImGui::End();
}
// 计算3x3矩阵的逆矩阵
Matrix3x3 inverse(const Matrix3x3& mat) {
    Matrix3x3 inv;
    float m[3][3];
    // 拷贝原矩阵
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            m[i][j] = mat.m[i][j];

    float det =
        m[0][0]*(m[1][1]*m[2][2]-m[1][2]*m[2][1]) -
        m[0][1]*(m[1][0]*m[2][2]-m[1][2]*m[2][0]) +
        m[0][2]*(m[1][0]*m[2][1]-m[1][1]*m[2][0]);

    if (fabs(det) < 1e-8) // 不可逆
        return Matrix3x3();

    float invDet = 1.0f / det;

    inv.m[0][0] =  (m[1][1]*m[2][2]-m[1][2]*m[2][1]) * invDet;
    inv.m[0][1] = -(m[0][1]*m[2][2]-m[0][2]*m[2][1]) * invDet;
    inv.m[0][2] =  (m[0][1]*m[1][2]-m[0][2]*m[1][1]) * invDet;

    inv.m[1][0] = -(m[1][0]*m[2][2]-m[1][2]*m[2][0]) * invDet;
    inv.m[1][1] =  (m[0][0]*m[2][2]-m[0][2]*m[2][0]) * invDet;
    inv.m[1][2] = -(m[0][0]*m[1][2]-m[0][2]*m[1][0]) * invDet;

    inv.m[2][0] =  (m[1][0]*m[2][1]-m[1][1]*m[2][0]) * invDet;
    inv.m[2][1] = -(m[0][0]*m[2][1]-m[0][1]*m[2][0]) * invDet;
    inv.m[2][2] =  (m[0][0]*m[1][1]-m[0][1]*m[1][0]) * invDet;

    return inv;
}
#endif // GRAPH_INIT_H_INCLUDED