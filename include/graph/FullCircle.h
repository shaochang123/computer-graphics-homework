#ifndef GRAPH_FULL_CIRCLE_H_INCLUDED
#define GRAPH_FULL_CIRCLE_H_INCLUDED
#ifndef GRAPH_INIT_H_INCLUDED
#include<graph/init.h>
#endif
#ifndef GRAPH_Line_H_INCLUDED
#include<graph/Line.h>
#endif
#ifndef GRAPH_Circle_H_INCLUDED
#include<graph/Circle.h>
#endif
// 整圆模式下绘制像素网格
void drawFullArc(GLFWwindow *window){
    detectposition(window, xpos, ypos);
    if(curpoints.size()==1){
        double r=sqrt((xpos-curpoints[0].x)*(xpos-curpoints[0].x)+(ypos-curpoints[0].y)*(ypos-curpoints[0].y));//半径
        drawarc(curpoints[0], r, 0, 2*M_PI);//起始角度和终止角度都为0和2π，表示画一个完整的圆
    }
    else if(curpoints.size()==2){
        double r=sqrt((curpoints[1].x-curpoints[0].x)*(curpoints[1].x-curpoints[0].x)+(curpoints[1].y-curpoints[0].y)*(curpoints[1].y-curpoints[0].y));//半径
        drawarc(curpoints[0], r, 0, 2*M_PI);//起始角度和终止角度都为0和2π，表示画一个完整的圆
    }
    render();
}
void Full_Circle_Mouse_Pressed(GLFWwindow* window, int button, int action){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && mode == 2){
        if(curpoints.size()==0){
            detectposition(window, xpos, ypos);
            curpoints.push_back({static_cast<int>(xpos), static_cast<int>(ypos)});
        }
        else if(curpoints.size()==1){//确定半径
            drawFullArc(window);
            curpoints.push_back({static_cast<int>(xpos), static_cast<int>(ypos)});//将当前点加入到当前图形的点中
            graphics.push_back({curpoints,mode,curcolor,curwidth});
            getcenposition(graphics.back());
            curpoints.clear();
        }
    }
}

#endif
