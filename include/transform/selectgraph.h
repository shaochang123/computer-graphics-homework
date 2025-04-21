#ifndef TRANSFORM_SELECTGRAPH_H_INCLUDED
#define TRANSFORM_SELECTGRAPH_H_INCLUDED
#ifndef GRAPH_INIT_H_INCLUDED
#include <graph/init.h>
#endif

void ChangeSelectedGraph(int key, int action){
    if(key ==GLFW_KEY_LEFT && action == GLFW_PRESS){
        ChooseIdx = (ChooseIdx-1+graphics.size())%graphics.size();
    }
    else if(key ==GLFW_KEY_RIGHT && action == GLFW_PRESS){
        ChooseIdx = (ChooseIdx+1)%graphics.size();
    }
    else if(key == GLFW_KEY_DELETE && action == GLFW_PRESS){
        if(mode == -1 && !graphics.empty()){
            graphics.erase(graphics.begin()+ChooseIdx);
            if(ChooseIdx>=graphics.size()) ChooseIdx = graphics.size()-1;
        }
    }
}
void Select(int key,int action){
    if (key == GLFW_KEY_S  && action == GLFW_PRESS) {//切换为选择模式
        mode = -1;
        ChooseIdx = 0;
        std::cout<<"start to select"<<std::endl;
    }
}
#endif