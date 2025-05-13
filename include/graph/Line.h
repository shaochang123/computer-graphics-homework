#ifndef GRAPH_LINE_H_INCLUDED
#define GRAPH_LINE_H_INCLUDED
#ifndef GRAPH_INIT_H_INCLUED
#include<graph/init.h>
#endif
// Bresenham直线算法
void drawLineBresenham(Point start, Point end, bool arg=false, int w =curwidth, Color color=curcolor,int a=10,int b =10) {//这里的arg是为了区分是否画虚线，为True时画虚线
    const int step = 3*w;
    a*=w;
    b*=w;
    const int dx = abs(end.x - start.x), sx = start.x < end.x ? 1 : -1;
    const int dy = -abs(end.y - start.y), sy = start.y < end.y ? 1 : -1;
    int err = dx + dy, e2,mark=1;
    while (true) {
        if(arg==true&&(mark-1)%step==0){
            setpixel(start.x,start.y,w,color);
        }
        else if(arg==false&&(mark-1)%b<a){
            setpixel(start.x,start.y,w,color);
        }
        if (start.x == end.x && start.y == end.y) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; start.x += sx; }
        if (e2 <= dx) { err += dx; start.y += sy; }
        mark++;
    }
}
//直线中点算法
void middleLine(Point start, Point end,int w =curwidth, Color color=curcolor,int a=10,int b=10) {
    int dx = abs(end.x - start.x);
    int dy = abs(end.y - start.y);
    int sx = start.x < end.x ? 1 : -1;
    int sy = start.y < end.y ? 1 : -1;
    a*=w;
    b*=w;
    bool steep = dy > dx; // 判断斜率是否大于1
    if (steep) {
        std::swap(dx, dy); // 交换dx和dy
    }

    int d = 2 * dy - dx; // 初始决策变量
    int x = start.x, y = start.y,mark=0;

    for (int i = 0; i <= dx; i++) {
        // 绘制点
        if (x >= 0 && x < maxn && y >= 0 && y < maxn) {
            if(mark%b<a)setpixel(x, y, w, color);
        }
        // 更新决策变量和坐标
        if (d > 0) {
            if (steep) {
                x += sx;
            } else {
                y += sy;
            }
            d -= 2 * dx;
        }
        if (steep) {
            y += sy;
        } else {
            x += sx;
        }
        d += 2 * dy;
        mark++;
    }
}
void drawLine(GLFWwindow *window){
    detectposition(window, xpos, ypos);
    if(curpoints.size() == 1){//如果已经按下鼠标（确定好了第一个点），那么就以按下的点和当前鼠标位置为直线的两个端点
        if(mode==0)drawLineBresenham(curpoints[0], {static_cast<int>(xpos), static_cast<int>(ypos)},false,curwidth,curcolor,cura,curb);
        if(mode==7)middleLine(curpoints[0], {static_cast<int>(xpos), static_cast<int>(ypos)},curwidth,curcolor,cura,curb);//如果想用中点算法画，就把上一行注释掉，把这一行取消注释
    }
    render();
}
void Line_Mouse_Pressed(GLFWwindow* window, int button, int action){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && (mode == 0||mode == 7) ){
        detectposition(window, xpos, ypos);
        curpoints.push_back({static_cast<int>(xpos), static_cast<int>(ypos)});
        if(curpoints.size() == 2){
            drawLine(window);
            graphics.push_back({curpoints,mode,curcolor,curwidth});
            curpoints.clear();
        }
    }
}
#endif