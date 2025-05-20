#ifndef GRAPH_LINE_H_INCLUDED
#define GRAPH_LINE_H_INCLUDED
#ifndef GRAPH_INIT_H_INCLUED
#include<graph/init.h>
#endif
// Bresenham直线算法
void drawLineBresenham(Point start, Point end, bool arg=false, int w =curwidth, Color color=curcolor) {//这里的arg是为了区分是否画虚线，为True时画虚线
    const int step = 3*w;
    const int dx = abs(end.x - start.x), sx = start.x < end.x ? 1 : -1;
    const int dy = -abs(end.y - start.y), sy = start.y < end.y ? 1 : -1;
    int err = dx + dy, e2,mark=1;
    while (true) {
        if((mark-1)%step==0){
            setpixel(start.x,start.y,w,color);
        }
        //把对应的像素点“涂黑”
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
        if (x >= 0 && x < maxn && y >= 0 && y < maxn) {
            g[x][y] = 1; // 将像素点涂黑
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
#endif