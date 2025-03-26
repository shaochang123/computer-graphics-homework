#ifndef GRAPH_LINE_H_INCLUDED
#define GRAPH_LINE_H_INCLUDED
#ifndef GRAPH_INIT_H_INCLUED
#include<graph/init.h>
#endif
bool isMousePressed = false; // 鼠标是否按下，实际上只为直线模式服务。
Point st;//直线初始点st
// Bresenham直线算法
void drawLineBresenham(Point start, Point end, bool arg=false) {//这里的arg是为了区分是否画虚线，为True时画虚线
    const int dx = abs(end.x - start.x), sx = start.x < end.x ? 1 : -1;
    const int dy = -abs(end.y - start.y), sy = start.y < end.y ? 1 : -1;
    int err = dx + dy, e2,mark=1;
    while (true) {
        if(start.x-1>=0&&start.y-1>=0&&start.x-1<maxn&&start.y-1<maxn&&g[start.x-1][start.y-1]==0&&mark%3!=0&&(mark+1)%3!=0)g[start.x-1][start.y-1] = cnt;//把对应的像素点“涂黑”
        //为何要加上 g[start.x-1][start.y-1] == 0？因为如果这个点已经被画过了，就不要再画了，否则在撤回时会导致另外一个图形出现断点
        if (start.x == end.x && start.y == end.y) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; start.x += sx; }
        if (e2 <= dx) { err += dx; start.y += sy; }
        if(arg)mark++;
    }
}
//直线中点算法
void middleLine(Point start, Point end) {
    int dx = abs(end.x - start.x);
    int dy = abs(end.y - start.y);
    int sx = start.x < end.x ? 1 : -1;
    int sy = start.y < end.y ? 1 : -1;

    bool steep = dy > dx; // 判断斜率是否大于1
    if (steep) {
        std::swap(dx, dy); // 交换dx和dy
    }

    int d = 2 * dy - dx; // 初始决策变量
    int x = start.x, y = start.y;

    for (int i = 0; i <= dx; i++) {
        // 绘制点
        if (x >= 0 && x < maxn && y >= 0 && y < maxn && g[x][y] == 0) {
            g[x][y] = cnt; // 将像素点涂黑
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
    }
}
void drawLine(GLFWwindow *window){
    double xpos, ypos;
    detectposition(window, xpos, ypos);
    if(isMousePressed ==true){//如果已经按下鼠标（确定好了第一个点），那么就以按下的点和当前鼠标位置为直线的两个端点
        drawLineBresenham(st, {static_cast<int>(xpos), static_cast<int>(ypos)});
        // middleLine(st, {static_cast<int>(xpos), static_cast<int>(ypos)});//如果想用中点算法画，就把上一行注释掉，把这一行取消注释
    }
    render();
}
void Line_Mouse_Pressed(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && mode == 0){
        if(isMousePressed == true){
            drawLine(window);
            //这里是防止画完直线后就被窗口刷新掉，所以在这里再画一次
            cnt++;
            isMousePressed =false;
            st.x=0;st.y=0;
            return;
        }
        isMousePressed = true;
        double xpos,ypos;
        detectposition(window, xpos, ypos);
        st.x=static_cast<int>(xpos);st.y=static_cast<int>(ypos);
    }
}
void Line_Keyboard_Pressed(int key, int mods, int action){
    if(key == GLFW_KEY_L && action == GLFW_PRESS){//切换到直线模式
        isMousePressed = false;
        mode=0;
        std::cout<<"Line Mode"<<std::endl;

    }
}
#endif