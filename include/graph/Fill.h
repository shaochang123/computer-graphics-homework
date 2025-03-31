#ifndef GRAPH_FILL_H
#define GRAPH_FILL_H
#ifndef GRAPH_INIT_H_INCLUDED
#include<graph/init.h>
#endif
std::queue<Point>q;
void FillGraph(Point st){
    if(st.x-1<0||st.x-1>=maxn||st.y-1<0||st.y-1>=maxn) return;
    q.push(st);
    while(!q.empty()){
        Point p = q.front(); q.pop();
        if(g[p.x-1][p.y-1]) continue;
        // 填充当前点
        g[p.x-1][p.y-1] = cnt;
        
        // 检查上方点
        if(p.y < maxn && g[p.x-1][p.y]==0) 
            q.push({p.x, p.y+1});
            
        // 检查下方点
        if(p.y > 1 && g[p.x-1][p.y-2]==0) 
            q.push({p.x, p.y-1});
            
        // 检查左侧点
        if(p.x > 1 && g[p.x-2][p.y-1]==0) 
            q.push({p.x-1, p.y});
            
        // 检查右侧点
        if(p.x < maxn && g[p.x][p.y-1]==0) 
            q.push({p.x+1, p.y});
    }
}
void Fill_Mouse_Pressed(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && mode == 2){
        double xpos,ypos;
        detectposition(window, xpos, ypos);
        st.x=static_cast<int>(xpos);st.y=static_cast<int>(ypos);
        FillGraph(st);//填充算法
        cnt++;
    }
}
void Fill_Keyboard_Pressed(int key, int mods, int action){
    if(key == GLFW_KEY_F && action == GLFW_PRESS){//切换到填充模式
        mode=2;
        std::cout<<"Fill Mode"<<std::endl;
    }
}
#endif
