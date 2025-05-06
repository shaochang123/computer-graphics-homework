#include<graph/Line.h>
#include<graph/Circle.h>
#include<graph/Fill.h>
#include<graph/Polygon.h>
#include<graph/Crop.h>
#include<graph/FullCircle.h>
#include<transform/translation.h>
#include<graph/bezier.h>
// 鼠标点击回调函数
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Line_Mouse_Pressed(window, button, action);
    Circle_Mouse_Pressed(window, button, action);
    Full_Circle_Mouse_Pressed(window, button, action);
    Fill_Mouse_Pressed(window, button, action);
    Polygon_Mouse_Pressed(window, button, action);
    SelectGraphByClick(window, button, action);
    Bezier_Mouse_Pressed(window, button, action);
    Bezier_Edit_Mouse_Handler(window,button,action,mods);
}
// 键盘回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Line_Keyboard_Pressed(key, action);
    Circle_Keyboard_Pressed(key, action);
    Full_Circle_Keyboard_Pressed(key, action);
    Fill_Keyboard_Pressed(key, action);
    Polygon_Keyboard_Pressed(key, action);
    Select(key,action);
    ChangeSelectedGraph(key,action);
    backup(key,mods,action);
    frontup(key,mods,action);
    saveImage(key,mods,action);
    ChangeColor(key,action);
    ChangeWidth(key, action);
    Translation(window);
    Bezier_Keyboard_Pressed(key, action);
}
void flushwindow();
int main() {
    const int width = 800, height = 800;//设置窗口大小
    GLFWwindow* window = init(width, height);
    // 注册鼠标点击回调函数，用来鼠标点击画图
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // 注册鼠标滚轮回调函数，用来旋转图形
    glfwSetScrollCallback(window, scroll_callback);
    // 注册键盘回调函数
    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT); // 清空颜色缓冲区
        if(mode == 0) drawLine(window);//渲染直线
        else if(mode==1) drawArc(window);//渲染圆弧
        else if(mode==2) drawFullArc(window);//渲染整圆
        else if(mode==4) drawPolygon(window);//渲染填充
        else if(mode==6) drawBezier(window);
        else render();
        flushwindow();//刷新屏幕
        glfwSwapBuffers(window);//交换缓冲区
        glfwPollEvents();//处理所有未决事件
        if(mode!=-1)ChooseIdx = -1;
        if(mode==-1&&ChooseIdx!=-1&&graphics[ChooseIdx].mode==6&&selectedPointIndex!=-1){
            detectposition(window, xpos, ypos);
            graphics[ChooseIdx].points[selectedPointIndex] = {static_cast<int>(xpos), static_cast<int>(ypos)};
        }
    }
    glfwTerminate();std::cout<<"bye\n"<<std::endl;//bye
    return 0;
}


void flushwindow() {
    // 刷新屏幕
    for(int i=0; i<maxn; i++) {
        for(int j=0; j<maxn; j++) {
            g[i][j] = {1.0, 1.0, 1.0};
        }
    }
    for(int i=0; i<graphics.size(); i++) {
        Color choosecolor;
        if(i==ChooseIdx) {
            choosecolor = graphics[i].color;
            graphics[i].color = selectedColor;
        }
        
        // 获取当前变换矩阵
        Matrix3x3 transform = graphics[i].transform;
        
        if(graphics[i].mode == 0) { // 直线
            // 应用变换到端点
            Point p1 = applyTransform(graphics[i].points[0], transform);
            Point p2 = applyTransform(graphics[i].points[1], transform);
            
            drawLineBresenham(p1, p2, false, graphics[i].width, graphics[i].color);
        }
        else if(graphics[i].mode == 2) { // 整圆
            Point center = applyTransform(graphics[i].points[0], transform); 
            Point edge = applyTransform(graphics[i].points[1], transform);
            
            // 计算变换后的半径
            float radius = sqrt(pow(edge.x - center.x, 2) + pow(edge.y - center.y, 2));
            
            drawarc(center, radius, 0, 2 * M_PI, graphics[i].width, graphics[i].color);
        }
        else if(graphics[i].mode == 1) { // 圆弧
            Point center = applyTransform(graphics[i].points[0], transform);
            Point start = applyTransform(graphics[i].points[1], transform);
            Point end = applyTransform(graphics[i].points[2], transform);
            
            double r = sqrt((start.x - center.x) * (start.x - center.x) + 
                         (start.y - center.y) * (start.y - center.y));
                      
            double startRad = atan2(center.y - start.y, start.x - center.x);
            double endRad = atan2(center.y - end.y, end.x - center.x);
            
            if (startRad < 0) startRad += 2 * M_PI;
            if (endRad < 0) endRad += 2 * M_PI;
            
            drawarc(center, r, startRad, endRad, graphics[i].width, graphics[i].color);
        }
        else if(graphics[i].mode == 3) { // 填充区域
            // 应用变换到所有填充点
            std::vector<Point> transformedPoints;
            for (const auto& point : graphics[i].points) {
                transformedPoints.push_back(applyTransform(point, transform));
            }
            
            for (const auto& point : transformedPoints) {
                setpixel(point.x, point.y, graphics[i].width, graphics[i].color);
            }
        }
        else if(graphics[i].mode == 4) { // 多边形
            // 应用变换到所有多边形顶点
            std::vector<Point> transformedPoints;
            for (const auto& point : graphics[i].points) {
                transformedPoints.push_back(applyTransform(point, transform));
            }
            
            fillPolygon(transformedPoints, graphics[i].width, graphics[i].color);
        }
        else if(graphics[i].mode == 6) { // Bezier曲线
           
            std::vector<Point> transformedPoints;
            for (const auto& point : graphics[i].points) {
                transformedPoints.push_back(applyTransform(point, transform));
            }
            for(int j=0;j<transformedPoints.size()-1;j++){
                if(graphics[i].key==0||graphics[i].key==1)drawLineBresenham(transformedPoints[j], transformedPoints[j+1], false, 1,{1.0f,0.0f,0.0f});
            }
            drawBezierCurve(transformedPoints,graphics[i].width,graphics[i].color);
        }
        if(i == ChooseIdx) {
            graphics[i].color = choosecolor;
        }
    }
}