#ifndef GRAPH_FILL_H
#define GRAPH_FILL_H
#ifndef GRAPH_INIT_H_INCLUDED
#include<graph/init.h>
#endif
std::queue<Point>q;
void FillGraph(Point st){
    if(st.x<0||st.x>=maxn||st.y<0||st.y>=maxn) return;
    q.push(st);
    while(!q.empty()){
        Point p = q.front(); q.pop();
        if(g[p.y][p.x].b!=1.0||g[p.y][p.x].r!=1.0||g[p.y][p.x].g!=1.0) continue;
        // 填充当前点
        setpixel(p.x,p.y,1,curcolor);
        curpoints.push_back({p.x,p.y});
        // 检查上方点
        if(p.y-1 >=0 && g[p.y-1][p.x].b==1.0&&g[p.y-1][p.x].r==1.0&&g[p.y-1][p.x].g==1.0)q.push({p.x, p.y-1});
            
        // 检查下方点
        if(p.y+1<maxn && g[p.y+1][p.x].b==1.0&&g[p.y+1][p.x].r==1.0&&g[p.y+1][p.x].g==1.0)q.push({p.x, p.y+1});
            
        // 检查左侧点
        if(p.x - 1>=0 &&g[p.y][p.x-1].b==1.0&&g[p.y][p.x-1].r==1.0&&g[p.y][p.x-1].g==1.0)q.push({p.x-1, p.y});
            
        // 检查右侧点
        if(p.x+1 < maxn && g[p.y][p.x+1].b==1.0&&g[p.y][p.x+1].r==1.0&&g[p.y][p.x+1].g==1.0)q.push({p.x+1, p.y});
    }
}
void Fill_Mouse_Pressed(GLFWwindow* window, int button, int action){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && mode == 3){
        detectposition(window, xpos, ypos);
        FillGraph({static_cast<int>(xpos),static_cast<int>(ypos)});//填充算法
        graphics.push_back({curpoints,mode,curcolor,1});//将当前图形加入到图形列表中
       
        curpoints.clear();//清空当前图形的点
    }
}
void Fill_Keyboard_Pressed(int key, int action){
    if(key == GLFW_KEY_F && action == GLFW_PRESS){//切换到填充模式
        mode=3;
        curpoints.clear(); // 清空当前点
        std::cout<<"Fill Mode"<<std::endl;
    }
}
#endif
