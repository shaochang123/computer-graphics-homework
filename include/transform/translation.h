#ifndef TRANSFORM_TRANSLATION_H
#define TRANSFORM_TRANSLATION_H
#ifndef CAL_MATRIX_H
#include "cal/matrix.h"
#endif
#ifndef GRAPH_INIT_H_INCLUED
#include<graph/init.h>
#endif
#include <cmath>

// 应用旋转矩阵到一个点
void rotatePoint(float& x, float& y, float xcenter, float ycenter, float angle) {
    // 将角度转换为弧度
    float radian = angle * M_PI / 180.0f;
    
    // 计算三角函数值（避免重复计算）
    float cosAngle = cosf(radian);
    float sinAngle = sinf(radian);
    
    // 保存原始坐标
    float xOrig = x;
    float yOrig = y;
    
    // 使用标准的旋转公式：
    // 1. 将点平移到以旋转中心为原点的坐标系
    // 2. 应用旋转
    // 3. 平移回原始坐标系
    x = xcenter + (xOrig - xcenter) * cosAngle - (yOrig - ycenter) * sinAngle;
    y = ycenter + (xOrig - xcenter) * sinAngle + (yOrig - ycenter) * cosAngle;
}

// 修改旋转函数，仅更新角度和中心点，不修改原始坐标
void RotateGraphic(GLFWwindow* window, double yOffset) {
    if (mode != -1 || ChooseIdx < 0 || ChooseIdx >= graphics.size())
        return;
    
    // 更新旋转角度
    graphics[ChooseIdx].rotationAngle += static_cast<float>(yOffset * 5.0f); // 控制旋转速度
    
    // 如果角度超过360度，归一化到0-360度范围
    if (graphics[ChooseIdx].rotationAngle >= 360.0f)
        graphics[ChooseIdx].rotationAngle -= 360.0f;
    if (graphics[ChooseIdx].rotationAngle < 0.0f)
        graphics[ChooseIdx].rotationAngle += 360.0f;
    
    // 计算并储存旋转中心点
    float xcenter = 0.0f, ycenter = 0.0f;
    
    // 根据图形类型确定旋转中心
    if (graphics[ChooseIdx].mode == 2 || graphics[ChooseIdx].mode == 1) {
        // 对于圆和圆弧，使用第一个点作为旋转中心（圆心）
        xcenter = graphics[ChooseIdx].points[0].x;
        ycenter = graphics[ChooseIdx].points[0].y;
    } else {
        // 对于其他图形，使用所有点的平均位置作为旋转中心
        for (const auto& point : graphics[ChooseIdx].points) {
            xcenter += point.x;
            ycenter += point.y;
        }
        xcenter /= graphics[ChooseIdx].points.size();
        ycenter /= graphics[ChooseIdx].points.size();
    }
    
    // 保存旋转中心
    graphics[ChooseIdx].rotationCenterX = xcenter;
    graphics[ChooseIdx].rotationCenterY = ycenter;
    
    
}

// 获取旋转后的点坐标（不修改原始点）
Point getRotatedPoint(const Point& point, float centerX, float centerY, float angle) {
    if (angle == 0.0f) return point; // 如果没有旋转，直接返回原点
    
    // 将角度转换为弧度
    float radian = angle * M_PI / 180.0f;
    
    // 计算三角函数值
    float cosAngle = cosf(radian);
    float sinAngle = sinf(radian);
    
    // 计算旋转后的坐标
    Point rotated;
    rotated.x = centerX + (point.x - centerX) * cosAngle - (point.y - centerY) * sinAngle;
    rotated.y = centerY + (point.x - centerX) * sinAngle + (point.y - centerY) * cosAngle;
    
    return rotated;
}



// 鼠标滚轮回调函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (mode == -1 && ChooseIdx >= 0 && ChooseIdx < graphics.size()) {
        RotateGraphic(window, yoffset);
    }
}

void Translation(GLFWwindow* window) {
    if (mode != -1) return;
    
    if (ChooseIdx < 0 || ChooseIdx >= graphics.size()) return;
    
    bool moved = false;
    if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) {
        for (int i = 0; i < graphics[ChooseIdx].points.size(); i++) {
            graphics[ChooseIdx].points[i].x -= 1;
        }
        graphics[ChooseIdx].rotationCenterX -= 1; // 更新旋转中心
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS) {
        for (int i = 0; i < graphics[ChooseIdx].points.size(); i++) {
            graphics[ChooseIdx].points[i].x += 1;
        }
        graphics[ChooseIdx].rotationCenterX += 1; // 更新旋转中心
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS) {
        for (int i = 0; i < graphics[ChooseIdx].points.size(); i++) {
            graphics[ChooseIdx].points[i].y += 1;
        }
        graphics[ChooseIdx].rotationCenterY += 1; // 更新旋转中心
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
        for (int i = 0; i < graphics[ChooseIdx].points.size(); i++) {
            graphics[ChooseIdx].points[i].y -= 1;
        }
        graphics[ChooseIdx].rotationCenterY -= 1; // 更新旋转中心
        moved = true;
    }
    
    // 如果移动了，重新计算旋转中心
    if (moved && graphics[ChooseIdx].rotationAngle != 0.0f) {
        // 更新旋转中心点（仅当图形有旋转时才需要）
        float xcenter = 0.0f, ycenter = 0.0f;
        
        if (graphics[ChooseIdx].mode == 2 || graphics[ChooseIdx].mode == 1) {
            xcenter = graphics[ChooseIdx].points[0].x;
            ycenter = graphics[ChooseIdx].points[0].y;
        } else {
            for (const auto& point : graphics[ChooseIdx].points) {
                xcenter += point.x;
                ycenter += point.y;
            }
            xcenter /= graphics[ChooseIdx].points.size();
            ycenter /= graphics[ChooseIdx].points.size();
        }
        
        graphics[ChooseIdx].rotationCenterX = xcenter;
        graphics[ChooseIdx].rotationCenterY = ycenter;
    }
    
    
}

// 判断点是否在直线上（使用距离判断）
bool isPointOnLine(const Point& point, const Point& lineStart, const Point& lineEnd, int threshold = 5) {
    // 如果线段很短，考虑为点
    if (lineStart.x == lineEnd.x && lineStart.y == lineEnd.y) {
        return (abs(point.x - lineStart.x) <= threshold && abs(point.y - lineStart.y) <= threshold);
    }
    
    // 计算点到线段的距离
    float A = point.x - lineStart.x;
    float B = point.y - lineStart.y;
    float C = lineEnd.x - lineStart.x;
    float D = lineEnd.y - lineStart.y;
    
    // 线段长度的平方
    float dot = A * C + B * D;
    float len_sq = C * C + D * D;
    float param = -1;
    
    if (len_sq != 0) // 避免除以零
        param = dot / len_sq;
    
    float xx, yy;
    
    if (param < 0) {
        xx = lineStart.x;
        yy = lineStart.y;
    }
    else if (param > 1) {
        xx = lineEnd.x;
        yy = lineEnd.y;
    }
    else {
        xx = lineStart.x + param * C;
        yy = lineStart.y + param * D;
    }
    
    float dx = point.x - xx;
    float dy = point.y - yy;
    
    float distance = sqrt(dx * dx + dy * dy);
    
    return (distance <= threshold);
}

// 判断点是否在圆上
bool isPointOnCircle(const Point& point, const Point& center, float radius, int threshold = 5) {
    float dx = point.x - center.x;
    float dy = point.y - center.y;
    float distance = sqrt(dx * dx + dy * dy);
    
    // 跜离在半径附近的点被认为在圆上
    return (abs(distance - radius) <= threshold);
}

// 判断点是否在圆弧上
bool isPointOnArc(const Point& point, const Point& center, float radius, float startAngle, float endAngle, int threshold = 5) {
    // 首先检查点是否在圆上
    if (!isPointOnCircle(point, center, radius, threshold)) {
        return false;
    }
    
    // 计算点的角度
    float angle = atan2f(point.y - center.y, point.x - center.x);
    if (angle < 0) angle += 2 * M_PI;
    
    // 检查角度是否在弧的范围内
    if (startAngle >= endAngle) {
        return (angle <= startAngle && angle >= endAngle);
    } else {
        // 处理跨越0度的情况
        return (angle <= startAngle || angle >= endAngle);
    }
}

// 判断点是否在多边形内部（射线法）
bool isPointInPolygon(const Point& point, const std::vector<Point>& vertices) {
    bool inside = false;
    size_t n = vertices.size();
    
    if (n < 3) return false; // 不是一个完整的多边形
    
    for (size_t i = 0, j = n - 1; i < n; j = i++) {
        if ((vertices[i].y > point.y) != (vertices[j].y > point.y) &&
            (point.x < (vertices[j].x - vertices[i].x) * (point.y - vertices[i].y) / 
                       (vertices[j].y - vertices[i].y) + vertices[i].x))
        {
            inside = !inside;
        }
    }
    
    return inside;
}

// 鼠标点击选择图形的函数
void SelectGraphByClick(GLFWwindow* window, int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && mode == -1) {
        double xpos, ypos;
        detectposition(window, xpos, ypos);
        Point clickPoint = {static_cast<int>(xpos), static_cast<int>(ypos)};
        
        // 遍历所有图形，检查点击是否命中
        for (int i = 0; i < graphics.size(); i++) {
            bool hit = false;
            
            // 根据图形类型进行不同的命中测试
            if (graphics[i].mode == 0) { // 直线
                hit = isPointOnLine(clickPoint, graphics[i].points[0], graphics[i].points[1]);
            }
            else if (graphics[i].mode == 2) { // 整圆
                float radius = sqrt(pow(graphics[i].points[1].x - graphics[i].points[0].x, 2) + 
                                  pow(graphics[i].points[1].y - graphics[i].points[0].y, 2));
                hit = isPointOnCircle(clickPoint, graphics[i].points[0], radius);
            }
            else if (graphics[i].mode == 1) { // 圆弧
                float radius = sqrt(pow(graphics[i].points[1].x - graphics[i].points[0].x, 2) + 
                                  pow(graphics[i].points[1].y - graphics[i].points[0].y, 2));
                
                // 计算圆弧的开始和结束角度
                float startAngle = atan2f(graphics[i].points[1].y - graphics[i].points[0].y, 
                                        graphics[i].points[1].x - graphics[i].points[0].x);
                float endAngle = atan2f(graphics[i].points[2].y - graphics[i].points[0].y, 
                                      graphics[i].points[2].x - graphics[i].points[0].x);
                if (startAngle < 0) startAngle += 2 * M_PI;
                if (endAngle < 0) endAngle += 2 * M_PI;
                
                hit = isPointOnArc(clickPoint, graphics[i].points[0], radius, startAngle, endAngle);
            }
            else if (graphics[i].mode == 4) { // 多边形
                hit = isPointInPolygon(clickPoint, graphics[i].points);
            }
            else if (graphics[i].mode == 3) { // 填充区域
                // 简单检查：如果点击的像素颜色与填充区域颜色相似，则认为命中
                for (const auto& p : graphics[i].points) {
                    if (abs(p.x - clickPoint.x) <= 1 && abs(p.y - clickPoint.y) <= 1) {
                        hit = true;
                        break;
                    }
                }
            }   
            if (hit) {
                ChooseIdx = i; // 更新选中的图形索引
                printf("choose graph %d\n", i);
                return; // 找到一个命中的图形后立即返回，不再检查其他图形
            }
        }
    
        // 如果没有找到命中的图形，保持当前选择
        printf("no graph is chooesn\n");
    }
}

#endif