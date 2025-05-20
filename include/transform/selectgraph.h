#ifndef TRANSFORM_SELECTGRAPH_H_INCLUDED
#define TRANSFORM_SELECTGRAPH_H_INCLUDED
#ifndef GRAPH_INIT_H_INCLUDED
#include <graph/init.h>
#endif


void Select(int key,int action){
    if (key == GLFW_KEY_S  && action == GLFW_PRESS) {//切换为选择模式
        mode = -1;
        
        ChooseIdx = 0;
        std::cout<<"start to select"<<std::endl;
    }
}
#endif