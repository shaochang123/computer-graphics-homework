#ifndef GRAPH_BSPLINE_H_INCLUDED
#define GRAPH_BSPLINE_H_INCLUDED
#ifndef GRAPH_INIT_H_INCLUDED
#include <graph/init.h>
#endif
#ifndef GRAPH_BEZIER_H_INCLUDED
#include <graph/bezier.h>
#endif
#include <graph/Line.h>
#include <vector>
#include <algorithm>
#include <cmath>
// 计算B样条基函数 N_i,k(t) 的递归实现
double bsplineBasisFunction(int i, int k, double t, const std::vector<double>& knots) {
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
        d1 = (t - knots[i]) / denominator1 * bsplineBasisFunction(i, k-1, t, knots);
    }
    
    // 右项
    double denominator2 = knots[i+k] - knots[i+1];
    if (std::abs(denominator2) > 1e-10) { // 避免除以0
        d2 = (knots[i+k] - t) / denominator2 * bsplineBasisFunction(i+1, k-1, t, knots);
    }
    
    return d1 + d2;
}

// 创建标准均匀B样条节点向量（无clamp）
std::vector<double> createUniformKnots(int n, int k) {
    // n: 控制点数量-1, k: 阶数
    int m = n + k + 1; // 节点数
    std::vector<double> knots(m);
    for (int i = 0; i < m; ++i) {
        knots[i] = static_cast<double>(i) / (m - 1);
    }
    return knots;
}

// 绘制B样条曲线
void drawBSplineCurve(const std::vector<Point>& controlPoints, int bswidth, Color bscolor, int order = 4) {
    if (controlPoints.size() < order) return;
    int n = controlPoints.size() - 1;
    std::vector<double> knots = createUniformKnots(n, order);

    const double step = 0.001;
    double tStart = knots[order-1];
    double tEnd = knots[n+1]; // 不包括最后一个节点

    Point prevPoint;
    bool firstPoint = true;
    for (double t = tStart; t < tEnd; t += step) { // 注意这里是 t < tEnd
        double x = 0.0, y = 0.0;
        for (int i = 0; i <= n; i++) {
            double basis = bsplineBasisFunction(i, order, t, knots);
            x += basis * controlPoints[i].x;
            y += basis * controlPoints[i].y;
        }
        Point currentPoint = {static_cast<int>(std::round(x)), static_cast<int>(std::round(y))};
        setpixel(currentPoint.x, currentPoint.y, bswidth, bscolor);
        if (!firstPoint) {
            drawLineBresenham(prevPoint, currentPoint, false, bswidth, bscolor);
        }
        prevPoint = currentPoint;
        firstPoint = false;
    }
}

// 实时绘制B样条曲线（鼠标移动时预览）
void drawBSpline(GLFWwindow *window) {
    detectposition(window, xpos, ypos);
    if (curpoints.size() > bsplineOrder) {
        curpoints.push_back({static_cast<int>(xpos), static_cast<int>(ypos)});
        drawBSplineCurve(curpoints, curwidth, curcolor, bsplineOrder);
        curpoints.pop_back(); // 删除最后一个点
    }
    
    // 绘制控制多直线
    if (curpoints.size() >= 2) {
        for (int j = 0; j < curpoints.size() - 1; j++) {
            drawLineBresenham(curpoints[j], curpoints[j+1], false, 1, {1.0f, 0.0f, 0.0f});
        }
    }
    
    // 绘制最后一个控制点到当前鼠标位置的线
    if (curpoints.size() >= 1) {
        drawLineBresenham(curpoints[curpoints.size()-1], {static_cast<int>(xpos), static_cast<int>(ypos)}, false, 1, {1.0f, 0.0f, 0.0f});
    }
    
    render();
}

// 处理B样条曲线的鼠标按下事件
void BSpline_Mouse_Pressed(GLFWwindow* window, int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && mode == 10) {
        detectposition(window, xpos, ypos);
        curpoints.push_back({static_cast<int>(xpos), static_cast<int>(ypos)});
    }
    
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && mode == 10) {
        // B样条曲线至少需要阶数个控制点
        if (curpoints.size() <= bsplineOrder) {
            std::cout << "Need at least " << bsplineOrder << " points for order-" << bsplineOrder << " B-spline." << std::endl;
            return;
        }
        
        detectposition(window, xpos, ypos);
        curpoints.push_back({static_cast<int>(xpos), static_cast<int>(ypos)});
        drawBSpline(window);
        
        // 保存图形数据，添加order信息到reserved字段
        graphic g = {curpoints, mode, curcolor, curwidth};
        g.reserved = bsplineOrder; // 存储阶数
        graphics.push_back(g);
        getcenposition(graphics.back());
        curpoints.clear();
    }
}
// 处理B样条曲线在编辑模式下的鼠标操作
void BSpline_Edit_Mouse_Handler(GLFWwindow* window, int button, int action, int mods) {
    // 确保在编辑模式且选中了B样条曲线
    if (mode != -1 || ChooseIdx == -1 || graphics[ChooseIdx].mode != 10) return;
    
    detectposition(window, xpos, ypos);
    Point currentPos = {static_cast<int>(xpos), static_cast<int>(ypos)};
    
    // 鼠标中键按下，检查点击控制点
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        selectedPointIndex = -1;
        for (int i = 0; i < graphics[ChooseIdx].points.size(); i++) {
            if (isNearControlPoint(currentPos, graphics[ChooseIdx].points[i])) {
                selectedPointIndex = i;
                break;
            }
        }
    }
    
    // 鼠标中键松开，重置选中状态
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
        selectedPointIndex = -1;
    }
    
    // 编辑后重新绘制B样条曲线
    if (selectedPointIndex != -1 && graphics[ChooseIdx].mode == 10) {
        int order = graphics[ChooseIdx].reserved > 0 ? graphics[ChooseIdx].reserved : 4;
        drawBSplineCurve(graphics[ChooseIdx].points, graphics[ChooseIdx].width, graphics[ChooseIdx].color, order);
    }
}


#endif