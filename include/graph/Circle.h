#ifndef GRAPH_CIRCLE_H_INCLUDED
#define GRAPH_CIRCLE_H_INCLUDED
#ifndef GRAPH_INIT_H_INCLUED
#include<graph/init.h>
#endif
#ifndef GRAPH_Line_H_INCLUED
#include<graph/Line.h>
#endif
// 圆弧绘制算法
void checkAndDraw(int px, int py, Point center, double startRad, double endRad,int w,Color color) {
    if (px < 0 || px >= maxn || py < 0 || py >= maxn) return;
    int dx = px - center.x;
    int dy = center.y - py;
    double theta = atan2(dy, dx);
    if (theta < 0) theta += 2 * M_PI;
    bool inArc = (theta >= startRad && theta <= endRad) || //在起始角度和终止角度之间
                (startRad > endRad && (theta >= startRad || theta <= endRad));//起始角度到终止角度跨越了0度，就要特殊考虑
    
    if (inArc)setpixel(px,py,w,color);
}
// 优化后的圆弧绘制算法
void drawarc(Point center, int r, double startRad, double endRad, int w=curwidth, Color color=curcolor) {
    int x = 0, y = r;
    int d = 1 - r;
    
    while (x <= y) {
        //这里按照象限分了八个区域，更方便处理
        checkAndDraw(center.x + x, center.y + y, center, startRad, endRad,w,color);
        checkAndDraw(center.x - x, center.y + y, center, startRad, endRad,w,color);
        checkAndDraw(center.x + x, center.y - y, center, startRad, endRad,w,color);
        checkAndDraw(center.x - x, center.y - y, center, startRad, endRad,w,color);
        checkAndDraw(center.x + y, center.y + x, center, startRad, endRad,w,color);
        checkAndDraw(center.x - y, center.y + x, center, startRad, endRad,w,color);
        checkAndDraw(center.x + y, center.y - x, center, startRad, endRad,w,color);
        checkAndDraw(center.x - y, center.y - x, center, startRad, endRad,w,color);
        
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}
// 圆弧模式下绘制像素网格
void drawArc(GLFWwindow *window){
    detectposition(window, xpos, ypos);
    if(curpoints.size()==1){
        drawLineBresenham(curpoints[0], {static_cast<int>(xpos), static_cast<int>(ypos)},true);//最后的参数变为true，表示画虚线
    }
    else if(curpoints.size()==2){
        double r=sqrt((curpoints[1].x-curpoints[0].x)*(curpoints[1].x-curpoints[0].x)+(curpoints[1].y-curpoints[0].y)*(curpoints[1].y-curpoints[0].y));//半径
        double startRad = atan2(curpoints[0].y - curpoints[1].y, curpoints[1].x - curpoints[0].x);
        double endRad = atan2(curpoints[0].y - ypos, xpos - curpoints[0].x);
        if (startRad < 0) startRad += 2 * M_PI;//保证角度在0到2π之间
        if(endRad < 0) endRad += 2*M_PI;//保证角度在0到2π之间
        drawarc(curpoints[0], r, startRad, endRad,curwidth);
    }
    render();
}
void Circle_Mouse_Pressed(GLFWwindow* window, int button, int action){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && mode == 1){
        if(curpoints.size()==0){
            detectposition(window, xpos, ypos);
            curpoints.push_back({static_cast<int>(xpos), static_cast<int>(ypos)});
        }
        else if(curpoints.size()==1){//确定圆弧上的一个点(固定半径)
            detectposition(window, xpos, ypos);
            curpoints.push_back({static_cast<int>(xpos), static_cast<int>(ypos)});
        }
        else if(curpoints.size()==2){
            drawArc(window);
            detectposition(window, xpos, ypos);
            curpoints.push_back({static_cast<int>(xpos), static_cast<int>(ypos)});
            graphics.push_back({curpoints,mode,curcolor,curwidth});
            curpoints.clear();
        } 
    }
}
void Circle_Keyboard_Pressed(int key, int action){
    if(key == GLFW_KEY_C && action == GLFW_PRESS){//切换到圆弧模式
        mode=1;
        curpoints.clear(); // 清空当前点
        std::cout<<"Circle Mode"<<std::endl;
    }
}
#endif
