#ifndef GRAPH_BEZIER_H_INCLUDED
#define GRAPH_BEZIER_H_INCLUDED
#ifndef GRAPH_INIT_H_INCLUDED
#include<graph/init.h>
#endif
#ifndef GRAPH_Line_H_INCLUDED
#include<graph/Line.h>
#endif

// 控制点的选中阈值（点击控制点的有效距离）
const int CONTROL_POINT_THRESHOLD = 10;

// 用于记录当前选中的控制点索引
int selectedPointIndex = -1; 

// 检查点是否在控制点附近
bool isNearControlPoint(const Point& mousePos, const Point& controlPoint, int threshold = CONTROL_POINT_THRESHOLD) {
    Point control = applyTransform(controlPoint, graphics[ChooseIdx].transform);
    int dx = mousePos.x - control.x;
    int dy = mousePos.y - control.y;
    return (dx*dx + dy*dy) <= threshold*threshold;
}

void drawBezierCurve(const std::vector<Point>& controlPoints,int bwidth,Color bcolor) {
    if (controlPoints.size() < 2) return; // 至少需要两个点才能绘制曲线
    
    // t的步长，步长越小曲线越平滑
    const double step = 0.0001;
    
    // 对t从0到1遍历
    for (double t = 0; t <= 1.0; t += step) {
        // 创建临时数组用于递归计算
        std::vector<std::pair<double,double>>points;
        for(int i=0;i<controlPoints.size();i++){
            points.push_back({static_cast<double>(controlPoints[i].x),static_cast<double>(controlPoints[i].y)});
        }
        // de Casteljau算法的递归计算
        for (int r = 1; r < points.size(); r++) {
            for (int i = 0; i < points.size() - r; i++) {
                points[i].first = (1 - t) * points[i].first + t * points[i + 1].first;
                points[i].second = (1 - t) * points[i].second + t * points[i + 1].second;
            }
        }
        
        // points[0]现在包含贝塞尔曲线上的点
        setpixel(static_cast<int>(points[0].first), static_cast<int>(points[0].second), bwidth, bcolor);
    }
}
void drawBezier(GLFWwindow *window){
    detectposition(window, xpos, ypos);
    if(curpoints.size()>=2){
        curpoints.push_back({static_cast<int>(xpos), static_cast<int>(ypos)});
        drawBezierCurve(curpoints,curwidth,curcolor);
        curpoints.pop_back(); // 删除最后一个点
    }
    if(curpoints.size()>=2)for(int j=0;j<curpoints.size()-1;j++)drawLineBresenham(curpoints[j],curpoints[j+1],false,1,{1.0f,0.0f,0.0f});
    if(curpoints.size()>=1)drawLineBresenham(curpoints[curpoints.size()-1],{static_cast<int>(xpos), static_cast<int>(ypos)},false,1,{1.0f,0.0f,0.0f});
    render();
}
void Bezier_Mouse_Pressed(GLFWwindow* window, int button, int action){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && mode == 6){
        detectposition(window, xpos, ypos);
        curpoints.push_back({static_cast<int>(xpos), static_cast<int>(ypos)});
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && mode == 6){
       if(curpoints.size() < 2) return; // 至少需要两个点才能绘制曲线
       detectposition(window, xpos, ypos);
       curpoints.push_back({static_cast<int>(xpos), static_cast<int>(ypos)});
       drawBezier(window);
       graphics.push_back({curpoints,mode,curcolor,curwidth});
       getcenposition(graphics.back());
       curpoints.clear();
    }
}

// 处理贝塞尔曲线在编辑模式下的鼠标操作
void Bezier_Edit_Mouse_Handler(GLFWwindow* window, int button, int action, int mods) {
    // 确保我们在编辑模式且选中了贝塞尔曲线
    if (mode != -1 || ChooseIdx == -1 || graphics[ChooseIdx].mode != 6 ) return;
    
    detectposition(window, xpos, ypos);
    Point currentPos = {static_cast<int>(xpos), static_cast<int>(ypos)};
    
    // 鼠标按下时，检查是否点击了控制点
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        selectedPointIndex = -1;
        for (int i = 0; i < graphics[ChooseIdx].points.size(); i++) {
            if (isNearControlPoint(currentPos, graphics[ChooseIdx].points[i])) {
                selectedPointIndex = i;
                break;
            }
        }
    }
    
    // 鼠标释放时，重置选中状态
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
        selectedPointIndex = -1;
    }
}



#endif