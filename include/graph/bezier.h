#ifndef GRAPH_BEZIER_H_INCLUDED
#define GRAPH_BEZIER_H_INCLUDED
#ifndef GRAPH_INIT_H_INCLUDED
#include<graph/init.h>
#endif
#ifndef GRAPH_Line_H_INCLUDED
#include<graph/Line.h>
#endif

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
#endif