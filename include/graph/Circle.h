#ifndef GRAPH_CIRCLE_H_INCLUDED
#define GRAPH_CIRCLE_H_INCLUDED
#ifndef GRAPH_INIT_H_INCLUED
#include<graph/init.h>
#endif
#ifndef GRAPH_Line_H_INCLUED
#include<graph/Line.h>
#endif
uint8_t ArcStep = 0;//绘制圆弧的步骤
Point Arco, ArcA;//圆弧的圆心和圆弧上的一个点
// 圆弧绘制算法
void checkAndDraw(int px, int py, Point center, double startRad, double endRad) {
    if (px < 0 || px >= maxn || py < 0 || py >= maxn) return;
    int dx = px - center.x;
    int dy = center.y - py;
    double theta = atan2(dy, dx);
    if (theta < 0) theta += 2 * M_PI;
    bool inArc = (theta >= startRad && theta <= endRad) || //在起始角度和终止角度之间
                (startRad > endRad && (theta >= startRad || theta <= endRad));//起始角度到终止角度跨越了0度，就要特殊考虑
    
    if (inArc)setpixel(px,py,w);
}
// 优化后的圆弧绘制算法
void drawarc(Point center, int r, double startRad, double endRad) {
    int x = 0, y = r;
    int d = 1 - r;
    
    while (x <= y) {
        //这里按照象限分了八个区域，更方便处理
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
// 圆弧模式下绘制像素网格
void drawArc(GLFWwindow *window){
    double xpos, ypos;
    detectposition(window, xpos, ypos);
    if(ArcStep==1){
        drawLineBresenham(Arco, {static_cast<int>(xpos), static_cast<int>(ypos)},true);//最后的参数变为true，表示画虚线
    }
    else if(ArcStep==2){
        double r=sqrt((ArcA.x-Arco.x)*(ArcA.x-Arco.x)+(ArcA.y-Arco.y)*(ArcA.y-Arco.y));//半径
        double startRad = atan2(Arco.y - ArcA.y, ArcA.x - Arco.x);//起始角度
        double endRad = atan2(Arco.y - ypos, xpos - Arco.x);//终止角度
        if (startRad < 0) startRad += 2 * M_PI;//保证角度在0到2π之间
        if(endRad < 0) endRad += 2*M_PI;//保证角度在0到2π之间
        drawarc(Arco, r, startRad, endRad);
    }
    render();
}
void Circle_Mouse_Pressed(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && mode == 1){
        if(ArcStep==0){//确定圆心
            double xpos, ypos;
            detectposition(window, xpos, ypos);
            Arco.x=static_cast<int>(xpos);Arco.y=static_cast<int>(ypos);
        }
        else if(ArcStep==1){//确定圆弧上的一个点(固定半径)
            double xpos, ypos;
            detectposition(window, xpos, ypos);
            ArcA.x=static_cast<int>(xpos);ArcA.y=static_cast<int>(ypos);
        }
        else if(ArcStep==2){//圆弧画好后，cnt++
            drawArc(window);
            Cnt2Color.push_back(currentColor);//绑定对应颜色
            cnt++;
        } 
        ArcStep=(ArcStep+1)%3;//每次点击左键，ArcStep+1，画完了重置为0
    }
}
void Circle_Keyboard_Pressed(int key, int mods, int action){
    if(key == GLFW_KEY_C && action == GLFW_PRESS){//切换到圆弧模式
        ArcStep = 0;
        mode=1;
        std::cout<<"Circle Mode"<<std::endl;
    }
}
#endif
