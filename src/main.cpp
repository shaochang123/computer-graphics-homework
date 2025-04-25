#include<graph/Line.h>
#include<graph/Circle.h>
#include<graph/Fill.h>
#include<graph/Polygon.h>
#include<graph/Crop.h>
#include<graph/FullCircle.h>
#include<transform/translation.h>
// 鼠标点击回调函数
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Line_Mouse_Pressed(window, button, action);
    Circle_Mouse_Pressed(window, button, action);
    Full_Circle_Mouse_Pressed(window, button, action);
    Fill_Mouse_Pressed(window, button, action);
    Polygon_Mouse_Pressed(window, button, action);
    SelectGraphByClick(window, button, action);
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
        else render();
        flushwindow();//刷新屏幕
        glfwSwapBuffers(window);//交换缓冲区
        glfwPollEvents();//处理所有未决事件
        if(mode!=-1)ChooseIdx = -1;
    
    }
    glfwTerminate();std::cout<<"bye\n"<<std::endl;//bye
    return 0;
}


void flushwindow(){
    // 刷新屏幕
    for(int i=0;i<maxn;i++){
        for(int j=0;j<maxn;j++){
            g[i][j]={1.0,1.0,1.0};
        }
    }
    for(int i=0;i<graphics.size();i++){
        Color choosecolor;
        if(i==ChooseIdx){choosecolor=graphics[i].color;graphics[i].color=selectedColor;}
        if(graphics[i].mode==0){//直线
            // 获取旋转后的两个端点
            Point p1 = getRotatedPoint(graphics[i].points[0], 
                                     graphics[i].rotationCenterX, 
                                     graphics[i].rotationCenterY, 
                                     graphics[i].rotationAngle);
            Point p2 = getRotatedPoint(graphics[i].points[1], 
                                     graphics[i].rotationCenterX, 
                                     graphics[i].rotationCenterY, 
                                     graphics[i].rotationAngle);
            
            // 使用旋转后的点绘制直线
            drawLineBresenham(p1, p2, false, graphics[i].width, graphics[i].color);
        }
        else if(graphics[i].mode==2){//整圆
            // 对于圆，旋转中心通常是圆心，不需要旋转圆心，但圆上的点需要旋转
            // 此处可能不需要特殊处理，因为圆是对称的
            drawarc(graphics[i].points[0], 
                   sqrt((graphics[i].points[1].x - graphics[i].points[0].x) * (graphics[i].points[1].x - graphics[i].points[0].x) + 
                        (graphics[i].points[1].y - graphics[i].points[0].y) * (graphics[i].points[1].y - graphics[i].points[0].y)), 
                   0, 2 * M_PI, graphics[i].width, graphics[i].color);
        }
        else if(graphics[i].mode==1){//圆弧
            // 获取旋转后的端点
            Point center = graphics[i].points[0]; // 圆心不需要旋转
            Point p1 = getRotatedPoint(graphics[i].points[1], 
                                     graphics[i].rotationCenterX, 
                                     graphics[i].rotationCenterY, 
                                     graphics[i].rotationAngle);
            Point p2 = getRotatedPoint(graphics[i].points[2], 
                                     graphics[i].rotationCenterX, 
                                     graphics[i].rotationCenterY, 
                                     graphics[i].rotationAngle);
            
            double r = sqrt((p1.x - center.x) * (p1.x - center.x) + 
                            (p1.y - center.y) * (p1.y - center.y)); // 半径
            
            double startRad = atan2(center.y - p1.y, p1.x - center.x);
            double endRad = atan2(center.y - p2.y, p2.x - center.x);
            
            if (startRad < 0) startRad += 2 * M_PI;
            if (endRad < 0) endRad += 2 * M_PI;
            
            drawarc(center, r, startRad, endRad, graphics[i].width, graphics[i].color);
        }
        else if(graphics[i].mode==3){//填充
            for(int j=0;j<graphics[i].points.size();j++){
                setpixel(graphics[i].points[j].x,graphics[i].points[j].y,graphics[i].width,graphics[i].color);
            }
        }
        else if(graphics[i].mode==4){//多边形
            // 创建旋转后的点集
            std::vector<Point> rotatedPoints;
            for (const auto& point : graphics[i].points) {
                rotatedPoints.push_back(getRotatedPoint(point, 
                                                    graphics[i].rotationCenterX, 
                                                    graphics[i].rotationCenterY, 
                                                    graphics[i].rotationAngle));
            }
            
            fillPolygon(rotatedPoints, graphics[i].width, graphics[i].color);
        }
        if(i==ChooseIdx)graphics[i].color=choosecolor;       
    }
}