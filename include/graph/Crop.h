#ifndef GRAPH_FILL_H
#define GRAPH_FILL_H
#ifndef GRAPH_INIT_H_INCLUDED
#include<graph/init.h>
#endif
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

// 浮点数版本的区域码计算
int computeOutCode(double x, double y, int xmin, int ymin, int xmax, int ymax) {
    int code = INSIDE;
    if (x < xmin) code |= LEFT;
    else if (x > xmax) code |= RIGHT;
    if (y < ymin) code |= BOTTOM;
    else if (y > ymax) code |= TOP;
    return code;
}

// Cohen-Sutherland裁剪
void cropcohen(std::vector<Point> points) {
    if (points.size() != 2) return;
    int xmin = std::min(points[0].x, points[1].x);
    int xmax = std::max(points[0].x, points[1].x);
    int ymin = std::min(points[0].y, points[1].y);
    int ymax = std::max(points[0].y, points[1].y);

    for (int i = 0; i < graphics.size(); i++) {
        graphic cur = graphics[i];
        if (cur.mode != 0 && cur.mode != 7) { i++; continue; }

        // 使用浮点数坐标
        std::pair<double, double> p0 = {static_cast<double>(cur.points[0].x), static_cast<double>(cur.points[0].y)};
        std::pair<double, double> p1 = {static_cast<double>(cur.points[1].x), static_cast<double>(cur.points[1].y)};

        int out0 = computeOutCode(p0.first, p0.second, xmin, ymin, xmax, ymax);
        int out1 = computeOutCode(p1.first, p1.second, xmin, ymin, xmax, ymax);

        // 全部在框外，删除
        if ((out0 & out1) != 0) {
            graphics.erase(graphics.begin() + i);
            i--;
            continue;
        }

        // 全部在框内，保留
        if (out0 == 0 && out1 == 0) {
            
            continue;
        }

        // 部分在框内，裁剪后保留
        std::pair<double, double> cp0 = p0, cp1 = p1;
        int outcode0 = out0, outcode1 = out1;
        bool accept = false;

        while (true) {
            if (!(outcode0 | outcode1)) { accept = true; break; }
            else if (outcode0 & outcode1) { break; }
            else {
                int outcodeOut = outcode0 ? outcode0 : outcode1;
                double x, y;
                if (outcodeOut & TOP) {
                    x = cp0.first + (cp1.first - cp0.first) * (ymax - cp0.second) / (cp1.second - cp0.second);
                    y = ymax;
                } else if (outcodeOut & BOTTOM) {
                    x = cp0.first + (cp1.first - cp0.first) * (ymin - cp0.second) / (cp1.second - cp0.second);
                    y = ymin;
                } else if (outcodeOut & RIGHT) {
                    y = cp0.second + (cp1.second - cp0.second) * (xmax - cp0.first) / (cp1.first - cp0.first);
                    x = xmax;
                } else { // LEFT
                    y = cp0.second + (cp1.second - cp0.second) * (xmin - cp0.first) / (cp1.first - cp0.first);
                    x = xmin;
                }
                if (outcodeOut == outcode0) {
                    cp0.first = x; cp0.second = y;
                    outcode0 = computeOutCode(cp0.first, cp0.second, xmin, ymin, xmax, ymax);
                } else {
                    cp1.first = x; cp1.second = y;
                    outcode1 = computeOutCode(cp1.first, cp1.second, xmin, ymin, xmax, ymax);
                }
            }
        }

        if (accept) {
            cur.points = {
                {static_cast<int>(std::round(cp0.first)), static_cast<int>(std::round(cp0.second))},
                {static_cast<int>(std::round(cp1.first)), static_cast<int>(std::round(cp1.second))}
            };
            graphics.erase(graphics.begin() + i);
            graphics.push_back(cur);
            i--;
        } else {
            graphics.erase(graphics.begin() + i);
            i--;
        }
    }
}

// 递归找端点到框内的交点（保证一端在内一端在外）
Point midpoint_find(Point p_in, Point p_out, int xmin, int ymin, int xmax, int ymax) {
    std::pair<double, double> a = {static_cast<double>(p_in.x), static_cast<double>(p_in.y)};
    std::pair<double, double> b = {static_cast<double>(p_out.x), static_cast<double>(p_out.y)};
    for (int iter = 0; iter < 50; ++iter) {
        std::pair<double, double> mid = { (a.first + b.first) / 2.0, (a.second + b.second) / 2.0 };
        int code_mid = computeOutCode(mid.first, mid.second, xmin, ymin, xmax, ymax);
        if (code_mid == 0) {
            a = mid;
        } else {
            b = mid;
        }
        double dx = a.first - b.first;
        double dy = a.second - b.second;
        if (dx*dx + dy*dy < 0.25) break;
    }
    return {static_cast<int>(std::round(a.first)), static_cast<int>(std::round(a.second))};
}

// 修正版中点分割裁剪
bool midpoint_clip(Point p0, Point p1, int xmin, int ymin, int xmax, int ymax, Point& q0, Point& q1) {
    int out0 = computeOutCode(p0.x, p0.y, xmin, ymin, xmax, ymax);
    int out1 = computeOutCode(p1.x, p1.y, xmin, ymin, xmax, ymax);

    // 全部在外且在同一区域，直接返回false
    if ((out0 & out1) != 0) return false;
    // 全部在内部，直接返回原线段
    if (out0 == 0 && out1 == 0) {
        q0 = p0; q1 = p1; return true;
    }

    // 只要有交点（即两端不全在同一区域外），都递归找两端交点
    Point in0 = p0, in1 = p1;
    if (out0 != 0) in0 = midpoint_find(p1, p0, xmin, ymin, xmax, ymax);
    if (out1 != 0) in1 = midpoint_find(p0, p1, xmin, ymin, xmax, ymax);
    q0 = in0;
    q1 = in1;
    return true;
}

void cropmiddle(std::vector<Point> points) {
    if (points.size() != 2) return;
    int xmin = std::min(points[0].x, points[1].x);
    int xmax = std::max(points[0].x, points[1].x);
    int ymin = std::min(points[0].y, points[1].y);
    int ymax = std::max(points[0].y, points[1].y);
    int cnt=0;
    for (int i = 0; i < graphics.size() && cnt<graphics.size()+5; i++) {
        graphic cur = graphics[i];
        if (cur.mode != 0 && cur.mode != 7) { continue; }
        Point p0 = cur.points[0];
        Point p1 = cur.points[1];
        Point q0, q1;
        if (midpoint_clip(p0, p1, xmin, ymin, xmax, ymax, q0, q1)) {
            cur.points = {q0, q1};
            graphics.erase(graphics.begin() + i);
            graphics.push_back(cur);
            i--;
        } else {
            graphics.erase(graphics.begin() + i);
            i--;
        }
        cnt++;
    }
}
// Sutherland-Hodgman算法实现
std::vector<Point> clipPolygonSutherlandHodgman(const std::vector<Point>& polygon, int xmin, int ymin, int xmax, int ymax) {
    // 如果多边形为空，直接返回
    if (polygon.empty()) return {};
    
    // 定义裁剪函数 (针对单条边裁剪)
    auto clipAgainstLine = [](const std::vector<Point>& inputPolygon, int edge, int xmin, int ymin, int xmax, int ymax) {
        std::vector<Point> outputPolygon;
        
        // 处理多边形的每条边
        for (size_t i = 0; i < inputPolygon.size(); i++) {
            // 当前点和下一个点
            Point current = inputPolygon[i];
            Point next = inputPolygon[(i + 1) % inputPolygon.size()];
            
            // 判断点是否在裁剪边内部
            bool currentInside = false, nextInside = false;
            
            switch (edge) {
                case LEFT:   
                    currentInside = current.x >= xmin;
                    nextInside = next.x >= xmin;
                    break;
                case RIGHT:  
                    currentInside = current.x <= xmax;
                    nextInside = next.x <= xmax;
                    break;
                case BOTTOM: 
                    currentInside = current.y >= ymin;
                    nextInside = next.y >= ymin;
                    break;
                case TOP:    
                    currentInside = current.y <= ymax;
                    nextInside = next.y <= ymax;
                    break;
            }
            
            // 计算交点
            auto computeIntersection = [](const Point& p1, const Point& p2, int edge, int xmin, int ymin, int xmax, int ymax) {
                double x, y;
                
                switch (edge) {
                    case LEFT:   // x = xmin
                        if (p2.x == p1.x) // 垂直线
                            return Point{xmin, p1.y};
                        y = p1.y + (p2.y - p1.y) * (xmin - p1.x) / static_cast<double>(p2.x - p1.x);
                        return Point{xmin, static_cast<int>(std::round(y))};
                        
                    case RIGHT:  // x = xmax
                        if (p2.x == p1.x)
                            return Point{xmax, p1.y};
                        y = p1.y + (p2.y - p1.y) * (xmax - p1.x) / static_cast<double>(p2.x - p1.x);
                        return Point{xmax, static_cast<int>(std::round(y))};
                        
                    case BOTTOM: // y = ymin
                        if (p2.y == p1.y)
                            return Point{p1.x, ymin};
                        x = p1.x + (p2.x - p1.x) * (ymin - p1.y) / static_cast<double>(p2.y - p1.y);
                        return Point{static_cast<int>(std::round(x)), ymin};
                        
                    case TOP:    // y = ymax
                        if (p2.y == p1.y)
                            return Point{p1.x, ymax};
                        x = p1.x + (p2.x - p1.x) * (ymax - p1.y) / static_cast<double>(p2.y - p1.y);
                        return Point{static_cast<int>(std::round(x)), ymax};
                        
                    default:
                        return p1;
                }
            };
            
            // 根据不同情况处理
            if (currentInside && nextInside) {
                // 两点都在内部，添加终点
                outputPolygon.push_back(next);
            } else if (currentInside && !nextInside) {
                // 当前点在内部，下一点在外部，添加交点
                outputPolygon.push_back(computeIntersection(current, next, edge, xmin, ymin, xmax, ymax));
            } else if (!currentInside && nextInside) {
                // 当前点在外部，下一点在内部，添加交点和下一点
                outputPolygon.push_back(computeIntersection(current, next, edge, xmin, ymin, xmax, ymax));
                outputPolygon.push_back(next);
            }
            // 如果两点都在外部，不添加任何点
        }
        
        return outputPolygon;
    };
    
    // 依次对四条边进行裁剪
    std::vector<Point> result = polygon;
    result = clipAgainstLine(result, LEFT, xmin, ymin, xmax, ymax);
    result = clipAgainstLine(result, RIGHT, xmin, ymin, xmax, ymax);
    result = clipAgainstLine(result, BOTTOM, xmin, ymin, xmax, ymax);
    result = clipAgainstLine(result, TOP, xmin, ymin, xmax, ymax);
    
    return result;
}
// 多边形裁剪算法 (Sutherland-Hodgman)
void clipPolygon(std::vector<Point> clipWindow) {
    if (clipWindow.size() != 2) return; // 需要两个点定义裁剪窗口

    int xmin = std::min(clipWindow[0].x, clipWindow[1].x);
    int xmax = std::max(clipWindow[0].x, clipWindow[1].x);
    int ymin = std::min(clipWindow[0].y, clipWindow[1].y);
    int ymax = std::max(clipWindow[0].y, clipWindow[1].y);
    
    int cnt = 0;
    for (int i = 0; i < graphics.size() && cnt < graphics.size()+5; i++) {
        graphic cur = graphics[i];
        cnt++;
        
        // 只处理多边形
        if (cur.mode != 4) continue;
        
        std::vector<Point> clippedPolygon = clipPolygonSutherlandHodgman(cur.points, xmin, ymin, xmax, ymax);
        
        if (clippedPolygon.size() < 3) { // 裁剪后不是有效多边形
            graphics.erase(graphics.begin() + i);
            i--;
        } else {
            cur.points = clippedPolygon;
            graphics.erase(graphics.begin() + i);
            graphics.push_back(cur);
            i--;
        }
    }
}



void drawrec(GLFWwindow *window){
    double xpos,ypos;
    detectposition(window,xpos,ypos);
    int x = static_cast<int>(xpos);
    int y = static_cast<int>(ypos);
    if(curpoints.size()==1){
        Point ld = {std::min(x,curpoints[0].x),std::min(y,curpoints[0].y)};
        Point lu = {std::min(x,curpoints[0].x),std::max(y,curpoints[0].y)};
        Point ru = {std::max(x,curpoints[0].x),std::max(y,curpoints[0].y)};
        Point rd = {std::max(x,curpoints[0].x),std::min(y,curpoints[0].y)};
        drawLineBresenham(ld,lu,false,1,{1.0,0.0,0.0});
        drawLineBresenham(ld,rd,false,1,{1.0,0.0,0.0});
        drawLineBresenham(lu,ru,false,1,{1.0,0.0,0.0});
        drawLineBresenham(rd,ru,false,1,{1.0,0.0,0.0});
    }
    render();
}
void crop_mouse_pressed(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS &&( mode == 5||mode==8||mode==9)) {
        double xpos,ypos;
        detectposition(window,xpos,ypos);
        curpoints.push_back({static_cast<int>(xpos), static_cast<int>(ypos)});
        if(curpoints.size()==2){
            if(mode==5)cropcohen(curpoints);
            else if(mode==8)cropmiddle(curpoints);
            else if(mode==9)clipPolygon(curpoints);
            curpoints.clear();
        }
    }
}
#endif