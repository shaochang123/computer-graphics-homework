#ifndef GRAPH_CIRCLE_H_INCLUDED
#define GRAPH_CIRCLE_H_INCLUDED
#ifndef GRAPH_INIT_H_INCLUED
#include<graph/init.h>
#endif
#ifndef GRAPH_Line_H_INCLUED
#include<graph/Line.h>
#endif
// 圆弧绘制算法
void checkAndDraw(int px, int py, Point center, double startRad, double endRad,int w,Color color) {
    if (px < 0 || px >= maxn || py < 0 || py >= maxn) return;
    int dx = px - center.x;
    int dy = center.y - py;
    double theta = atan2(dy, dx);
    if (theta < 0) theta += 2 * M_PI;
    bool inArc = (theta >= startRad && theta <= endRad) || //在起始角度和终止角度之间
                (startRad > endRad && (theta >= startRad || theta <= endRad));//起始角度到终止角度跨越了0度，就要特殊考虑
    
    if (inArc)setpixel(px,py,w,color);
}
// 优化后的圆弧绘制算法
void drawarc(Point center, int r, double startRad, double endRad, int w=curwidth, Color color=curcolor) {
    int x = 0, y = r;
    int d = 1 - r;
    
    while (x <= y) {
        //这里按照象限分了八个区域，更方便处理
        checkAndDraw(center.x + x, center.y + y, center, startRad, endRad,w,color);
        checkAndDraw(center.x - x, center.y + y, center, startRad, endRad,w,color);
        checkAndDraw(center.x + x, center.y - y, center, startRad, endRad,w,color);
        checkAndDraw(center.x - x, center.y - y, center, startRad, endRad,w,color);
        checkAndDraw(center.x + y, center.y + x, center, startRad, endRad,w,color);
        checkAndDraw(center.x - y, center.y + x, center, startRad, endRad,w,color);
        checkAndDraw(center.x + y, center.y - x, center, startRad, endRad,w,color);
        checkAndDraw(center.x - y, center.y - x, center, startRad, endRad,w,color);
        
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}
#endif
