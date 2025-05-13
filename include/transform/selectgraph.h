#ifndef TRANSFORM_SELECTGRAPH_H_INCLUDED
#define TRANSFORM_SELECTGRAPH_H_INCLUDED
#ifndef GRAPH_INIT_H_INCLUDED
#include <graph/init.h>
#endif
// 检查点是否在控制点附近
bool isNearControlPoint1(const Point& mousePos, const Point& controlPoint, int threshold = 10) {
    Point control = applyTransform(controlPoint, graphics[ChooseIdx].transform);
    int dx = mousePos.x - control.x;
    int dy = mousePos.y - control.y;
    return (dx*dx + dy*dy) <= threshold*threshold;
}
// 创建均匀B样条的节点向量
std::vector<double> createUniformKnots1(int n, int k) {
    // n: 控制点数量-1, k: B样条阶数
    int m = n + k + 1; // 节点向量大小
    std::vector<double> knots(m);
    
    for (int i = 0; i < m; i++) {
        if (i < k) {
            knots[i] = 0.0; // 开始的k个节点为0
        } else if (i <= n) {
            knots[i] = static_cast<double>(i - k + 1) / (n - k + 2); // 中间节点均匀分布
        } else {
            knots[i] = 1.0; // 最后的k个节点为1
        }
    }
    
    return knots;
}
// 计算B样条基函数 N_i,k(t) 的递归实现
double bsplineBasisFunction1(int i, int k, double t, const std::vector<double>& knots) {
    // 0阶B样条
    if (k == 1) {
        if ((t >= knots[i] && t < knots[i+1]) || 
            (std::abs(t - knots[i+1]) < 1e-10 && std::abs(t - knots.back()) < 1e-10)) {
            return 1.0;
        }
        return 0.0;
    }
    
    // 计算k阶B样条基函数值
    double d1 = 0.0, d2 = 0.0;
    
    // 左项
    double denominator1 = knots[i+k-1] - knots[i];
    if (std::abs(denominator1) > 1e-10) { // 避免除以0
        d1 = (t - knots[i]) / denominator1 * bsplineBasisFunction1(i, k-1, t, knots);
    }
    
    // 右项
    double denominator2 = knots[i+k] - knots[i+1];
    if (std::abs(denominator2) > 1e-10) { // 避免除以0
        d2 = (knots[i+k] - t) / denominator2 * bsplineBasisFunction1(i+1, k-1, t, knots);
    }
    
    return d1 + d2;
}
// 检测鼠标位置函数
Point applyTransform1(const Point& p, const Matrix3x3& transform) {
    // 将点转换为齐次坐标
    float x = p.x;
    float y = p.y;
    float w = 1.0f;
    
    // 应用变换矩阵
    float newX = x * transform.m[0][0] + y * transform.m[0][1] + w * transform.m[0][2];
    float newY = x * transform.m[1][0] + y * transform.m[1][1] + w * transform.m[1][2];
    float newW = x * transform.m[2][0] + y * transform.m[2][1] + w * transform.m[2][2];
    
    // 齐次坐标归一化（除以w）
    if (newW != 0.0f) {
        newX /= newW;
        newY /= newW;
    }
    
    return {static_cast<int>(newX), static_cast<int>(newY)};
}
void detectposition1(GLFWwindow *window, double &xpos, double &ypos) {
    glfwGetCursorPos(window, &xpos, &ypos);
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
   
    xpos = xpos / width * maxn + 1;
    ypos = maxn - ypos / height * maxn + 1;
}

// 改变已选择的图形的函数
void ChangeSelectedGraph(int key, int action){
    if(key == GLFW_KEY_LEFT && action == GLFW_PRESS){
        ChooseIdx = (ChooseIdx-1+graphics.size())%graphics.size();
    }
    else if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
        ChooseIdx = (ChooseIdx+1)%graphics.size();
    }
    else if(key == GLFW_KEY_DELETE && action == GLFW_PRESS){
        if(mode == -1 && !graphics.empty()){
            graphics.erase(graphics.begin()+ChooseIdx);
            if(ChooseIdx >= graphics.size()) ChooseIdx = graphics.size()-1;
        }
    }
    if(key == GLFW_KEY_1 && mode == -1 && ChooseIdx != -1 && graphics[ChooseIdx].mode == 6){
        graphics[ChooseIdx].key = (graphics[ChooseIdx].key+1)%4;
    }
}

// 检测距离辅助函数
bool isCloseToPoint(double x, double y, double clickX, double clickY, int threshold = 5) {
    double dx = x - clickX;
    double dy = y - clickY;
    return (dx * dx + dy * dy) <= threshold * threshold;
}

// 鼠标点击选择图形函数
void SelectGraphByClick(GLFWwindow* window, int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && mode == -1) {
        double clickX, clickY;
        detectposition1(window, clickX, clickY);
        
        const int threshold = 5; // 选择临界值
        int closestGraphIdx = -1;
        double minDistance = threshold * threshold + 1; // 初始距离设为比阈值大
        
        // 遍历所有图形，找到最近的
        for (int i = 0; i < graphics.size(); i++) {
            // 创建一个标记来跟踪此图形是否被选中
            bool graphSelected = false;
            double shortestDistSq = minDistance;
            
            // 根据图形类型进行不同的绘制逻辑（类似flushwindow中的逻辑）
            if (graphics[i].mode == 0 || graphics[i].mode == 7) { // 直线
                // 获取变换后的点
                Point p1 = applyTransform1(graphics[i].points[0], graphics[i].transform);
                Point p2 = applyTransform1(graphics[i].points[1], graphics[i].transform);
                
                // 模拟绘制线段的过程，但只检查距离不真正绘制
                const int dx = abs(p2.x - p1.x), sx = p1.x < p2.x ? 1 : -1;
                const int dy = -abs(p2.y - p1.y), sy = p1.y < p2.y ? 1 : -1;
                int err = dx + dy, e2;
                Point currentPoint = p1;
                
                while (true) {
                    // 检查当前点与鼠标点击位置的距离
                    double distSq = pow(currentPoint.x - clickX, 2) + pow(currentPoint.y - clickY, 2);
                    if (distSq < shortestDistSq) {
                        shortestDistSq = distSq;
                        if (distSq <= threshold * threshold) {
                            graphSelected = true;
                        }
                    }
                    
                    if (currentPoint.x == p2.x && currentPoint.y == p2.y) break;
                    
                    e2 = 2 * err;
                    if (e2 >= dy) { err += dy; currentPoint.x += sx; }
                    if (e2 <= dx) { err += dx; currentPoint.y += sy; }
                }
            }
            else if (graphics[i].mode == 2) { // 整圆
                Point center = applyTransform1(graphics[i].points[0], graphics[i].transform);
                Point radiusPoint = applyTransform1(graphics[i].points[1], graphics[i].transform);
                int radius = sqrt(pow(radiusPoint.x - center.x, 2) + pow(radiusPoint.y - center.y, 2));
                
                // 检查点是否在圆上（简化版)
                double dist = sqrt(pow(clickX - center.x, 2) + pow(clickY - center.y, 2));
                if (abs(dist - radius) <= threshold) {
                    shortestDistSq = 0; // 表示我们找到了匹配
                    graphSelected = true;
                }
            }
            else if (graphics[i].mode == 1) { // 圆弧 - 修复部分
                Point center = applyTransform1(graphics[i].points[0], graphics[i].transform);
                Point p1 = applyTransform1(graphics[i].points[1], graphics[i].transform);
                Point p2 = applyTransform1(graphics[i].points[2], graphics[i].transform);
                
                double radius = sqrt(pow(p1.x - center.x, 2) + pow(p1.y - center.y, 2));
                
                // 计算圆弧的角度范围
                double startAngle = atan2(p1.y - center.y, p1.x - center.x);
                double endAngle = atan2(p2.y - center.y, p2.x - center.x);
                
                // 规范化角度到[0, 2π]
                if (startAngle < 0) startAngle += 2 * M_PI;
                if (endAngle < 0) endAngle += 2 * M_PI;
                
                // 查找是否在圆弧上
                double angle = atan2(clickY - center.y, clickX - center.x);
                if (angle < 0) angle += 2 * M_PI; // 规范化角度
                
                double dist = sqrt(pow(clickX - center.x, 2) + pow(clickY - center.y, 2)); // 修复距离计算
                
                // 检查是否在圆弧上
                if (abs(dist - radius) <= threshold) {
                    // 检查角度是否在范围内
                    bool inArc = (angle >= startAngle && angle <= endAngle) || //在起始角度和终止角度之间
                    (startAngle > endAngle && (angle >= startAngle || angle <= endAngle));//起始角度到终止角度跨越了0度，就要特殊考虑
                    
                    if (inArc) {
                        shortestDistSq = 0;
                        graphSelected = true;
                    }
                }
            }
            else if (graphics[i].mode == 3) { // 填充区域 - 新增部分
                // 应用变换到所有填充点
                std::vector<Point> transformedPoints;
                for (const auto& point : graphics[i].points) {
                    transformedPoints.push_back(applyTransform1(point, graphics[i].transform));
                }
                
                // 检查点击点是否在填充区域的任何点附近
                for (const auto& point : transformedPoints) {
                    double dist_sq = pow(point.x - clickX, 2) + pow(point.y - clickY, 2);
                    if (dist_sq < shortestDistSq) {
                        shortestDistSq = dist_sq;
                        if (dist_sq <= threshold * threshold) {
                            graphSelected = true;
                        }
                    }
                }
            }
            else if (graphics[i].mode == 4) { // 多边形
                // 检查点击点是否在多边形边缘附近
                std::vector<Point> transformedPoints;
                for (const auto& point : graphics[i].points) {
                    transformedPoints.push_back(applyTransform1(point, graphics[i].transform));
                }
                
                // 方法1: 检查点是否在多边形边缘
                for (size_t j = 0; j < transformedPoints.size(); j++) {
                    Point p1 = transformedPoints[j];
                    Point p2 = transformedPoints[(j + 1) % transformedPoints.size()];
                    
                    // 计算点到线段的距离 - 修正计算方式
                    double line_length = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
                    if (line_length == 0) continue;
                    
                    // 计算点到直线的距离
                    double distance = fabs((p2.x - p1.x) * (p1.y - clickY) - (p1.x - clickX) * (p2.y - p1.y)) / line_length;
                    
                    // 检查点是否在线段范围内
                    double dot_product = ((clickX - p1.x) * (p2.x - p1.x) + (clickY - p1.y) * (p2.y - p1.y)) / (line_length * line_length);
                    
                    if (dot_product >= 0 && dot_product <= 1 && distance <= threshold) {
                        shortestDistSq = distance * distance; // 使用实际距离
                        graphSelected = true;
                    }
                }
                
                // 方法2: 检查点是否在多边形内部
                if (!graphSelected) {
                    bool inside = false;
                    for (size_t j = 0, k = transformedPoints.size() - 1; j < transformedPoints.size(); k = j++) {
                        if (((transformedPoints[j].y > clickY) != (transformedPoints[k].y > clickY)) &&
                            (clickX < (transformedPoints[k].x - transformedPoints[j].x) * (clickY - transformedPoints[j].y) / 
                                      (transformedPoints[k].y - transformedPoints[j].y) + transformedPoints[j].x)) {
                            inside = !inside;
                        }
                    }
                    
                    if (inside) {
                        shortestDistSq = 0; // 点在多边形内
                        graphSelected = true;
                    }
                }
            }
            else if (graphics[i].mode == 6) { // 贝塞尔曲线 - 保持原有逻辑
                std::vector<Point> transformedPoints;
                for (const auto& point : graphics[i].points) {
                    transformedPoints.push_back(applyTransform1(point, graphics[i].transform));
                }
                
                // 检查点是否在控制点附近
                for (const auto& point : transformedPoints) {
                    double dist_sq = pow(clickX - point.x, 2) + pow(clickY - point.y, 2);
                    if (dist_sq < shortestDistSq) {
                        shortestDistSq = dist_sq;
                        if (dist_sq <= threshold * threshold) {
                            graphSelected = true;
                        }
                    }
                }
                
                // 检查点是否在贝塞尔曲线上（简化计算）
                if (!graphSelected) {
                    const double step = 0.01; // 步长
                    for (double t = 0; t <= 1.0; t += step) {
                        // 创建临时数组用于递归计算
                        std::vector<std::pair<double,double>> points;
                        for (const auto& p : transformedPoints) {
                            points.push_back({static_cast<double>(p.x), static_cast<double>(p.y)});
                        }
                        
                        // de Casteljau算法计算曲线上的点
                        for (int r = 1; r < points.size(); r++) {
                            for (int j = 0; j < points.size() - r; j++) {
                                points[j].first = (1 - t) * points[j].first + t * points[j + 1].first;
                                points[j].second = (1 - t) * points[j].second + t * points[j + 1].second;
                            }
                        }
                        
                        double dist_sq = pow(clickX - points[0].first, 2) + pow(clickY - points[0].second, 2);
                        if (dist_sq < shortestDistSq) {
                            shortestDistSq = dist_sq;
                            if (dist_sq <= threshold * threshold) {
                                graphSelected = true;
                            }
                        }
                    }
                }
            }
            else if(graphics[i].mode==10){
                std::vector<Point> transformedPoints;
                for (const auto& point : graphics[i].points) {
                    transformedPoints.push_back(applyTransform1(point, graphics[i].transform));
                }
                // 检查点是否在控制点附近
                for (const auto& point : transformedPoints) {
                    double dist_sq = pow(clickX - point.x, 2) + pow(clickY - point.y, 2);
                    if (dist_sq < shortestDistSq) {
                        shortestDistSq = dist_sq;
                        if (dist_sq <= threshold * threshold) {
                            graphSelected = true;
                        }
                    }
                }
                int n = transformedPoints.size() - 1; // 控制点数量减1
                
                // 创建节点向量
                std::vector<double> knots = createUniformKnots1(n, graphics[i].reserved);
                
                // t的步长，步长越小曲线越平滑
                const double step = 0.001;
                
                // 从第一个有效参数开始
                double tStart = knots[graphics[i].reserved-1];
                double tEnd = knots[n+1]; // 最后一个有效节点
                
                Point prevPoint;
                bool firstPoint = true;
                
                // 对参数t进行均匀采样
                for (double t = tStart; t <= tEnd; t += step) {
                    double x = 0.0, y = 0.0;
                    
                    // 计算曲线上的点
                    for (int j = 0; j <= n; j++) {
                        double basis = bsplineBasisFunction1(j, graphics[i].reserved, t, knots);
                        x += basis * transformedPoints[j].x;
                        y += basis * transformedPoints[j].y;
                    }
                    
                    // 绘制点
                    Point currentPoint = {static_cast<int>(std::round(x)), static_cast<int>(std::round(y))};
                    double dist_sq = pow(clickX - currentPoint.x, 2) + pow(clickY - currentPoint.y, 2);
                    if (dist_sq < shortestDistSq) {
                        shortestDistSq = dist_sq;
                        if (dist_sq <= threshold * threshold) {
                            graphSelected = true;
                        }
                    }
                    prevPoint = currentPoint;
                    firstPoint = false;
                    // 连接相邻点获得更平滑的曲线
                    
                }
            }
            // 如果找到比当前最近的图形，更新索引
            if (graphSelected && shortestDistSq < minDistance) {
                minDistance = shortestDistSq;
                closestGraphIdx = i;
            }
            
        }
        
        // 更新选中的图形索引
        if (closestGraphIdx != -1) {
            ChooseIdx = closestGraphIdx;
            printf("choose %d\n", closestGraphIdx);
        } else {
            printf("no graph is chosen\n");
        }
    }
}
#endif