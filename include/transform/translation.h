#ifndef TRANSFORM_TRANSLATION_H
#define TRANSFORM_TRANSLATION_H
#ifndef GRAPH_INIT_H_INCLUED
#include<graph/init.h>
#endif
#include <cmath>
// 创建平移矩阵
Matrix3x3 createTranslationMatrix(float dx, float dy) {
    Matrix3x3 mat;
    mat.m[0][0] = 1.0f; mat.m[0][1] = 0.0f; mat.m[0][2] = dx;
    mat.m[1][0] = 0.0f; mat.m[1][1] = 1.0f; mat.m[1][2] = dy;
    mat.m[2][0] = 0.0f; mat.m[2][1] = 0.0f; mat.m[2][2] = 1.0f;
    return mat;
}

// 创建旋转矩阵（围绕指定中心点旋转）
Matrix3x3 createRotationMatrix(float centerX, float centerY, float angleDegrees) {
    // 首先平移到原点
    Matrix3x3 toOrigin = createTranslationMatrix(-centerX, -centerY);
    
    // 然后旋转
    float angleRadians = angleDegrees * M_PI / 180.0f;
    float cosA = cosf(angleRadians);
    float sinA = sinf(angleRadians);
    
    Matrix3x3 rotate;
    rotate.m[0][0] = cosA;  rotate.m[0][1] = -sinA; rotate.m[0][2] = 0.0f;
    rotate.m[1][0] = sinA;  rotate.m[1][1] = cosA;  rotate.m[1][2] = 0.0f;
    rotate.m[2][0] = 0.0f;  rotate.m[2][1] = 0.0f;  rotate.m[2][2] = 1.0f;
    
    // 最后平移回原来的位置
    Matrix3x3 fromOrigin = createTranslationMatrix(centerX, centerY);
    
    // 组合变换：先平移到原点，然后旋转，再平移回原位置
    return fromOrigin * rotate * toOrigin;
}

// 创建缩放矩阵（围绕指定中心点缩放）
Matrix3x3 createScalingMatrix(float centerX, float centerY, float scale) {
    // 首先平移到原点
    Matrix3x3 toOrigin = createTranslationMatrix(-centerX, -centerY);
    
    // 然后缩放
    Matrix3x3 scaling;
    scaling.m[0][0] = scale; scaling.m[0][1] = 0.0f;  scaling.m[0][2] = 0.0f;
    scaling.m[1][0] = 0.0f;  scaling.m[1][1] = scale; scaling.m[1][2] = 0.0f;
    scaling.m[2][0] = 0.0f;  scaling.m[2][1] = 0.0f;  scaling.m[2][2] = 1.0f;
    
    // 最后平移回原来的位置
    Matrix3x3 fromOrigin = createTranslationMatrix(centerX, centerY);
    
    // 组合变换：先平移到原点，然后缩放，再平移回原位置
    return fromOrigin * scaling * toOrigin;
}
void RotateGraphic(GLFWwindow* window, double yOffset) {
    if (mode != -1 || ChooseIdx < 0 || ChooseIdx >= graphics.size())
        return;
    
    // 计算旋转角度
    float angle = static_cast<float>(yOffset * 5.0f);
    
    // 计算图形中心作为旋转中心
    float centerX = 0.0f, centerY = 0.0f;
    
    // 根据图形类型确定旋转中心
    if (graphics[ChooseIdx].mode == 2 || graphics[ChooseIdx].mode == 1) {
        centerX = graphics[ChooseIdx].points[0].x;
        centerY = graphics[ChooseIdx].points[0].y;
    } else {
        for (const auto& point : graphics[ChooseIdx].points) {
            centerX += point.x;
            centerY += point.y;
        }
        centerX /= graphics[ChooseIdx].points.size();
        centerY /= graphics[ChooseIdx].points.size();
    }

    Point newP = applyTransform({static_cast<int>(centerX),static_cast<int>(centerY)},graphics[ChooseIdx].transform);
    centerX = newP.x;
    centerY = newP.y;
    // 创建旋转矩阵
    Matrix3x3 rotationMatrix = createRotationMatrix(centerX, centerY, angle);
    // 应用旋转矩阵（新变换 = 当前变换矩阵 * 新操作矩阵）
    graphics[ChooseIdx].transform = rotationMatrix * graphics[ChooseIdx].transform;
  
}

void TranslateGraphic(float deltaX, float deltaY) {
    if (mode != -1 || ChooseIdx < 0 || ChooseIdx >= graphics.size())
        return;
    
    // 创建平移矩阵
    Matrix3x3 translationMatrix = createTranslationMatrix(deltaX, deltaY);
    graphics[ChooseIdx].transform = translationMatrix * graphics[ChooseIdx].transform;
    
}

void ScaleGraphic(GLFWwindow* window, float scaleFactor) {
    if (mode != -1 || ChooseIdx < 0 || ChooseIdx >= graphics.size())
        return;
    
    // 计算图形中心作为缩放中心
    float centerX = 0.0f, centerY = 0.0f;
    
    if (graphics[ChooseIdx].mode == 2 || graphics[ChooseIdx].mode == 1) {
        centerX = graphics[ChooseIdx].points[0].x;
        centerY = graphics[ChooseIdx].points[0].y;
    } else {
        for (const auto& point : graphics[ChooseIdx].points) {
            centerX += point.x;
            centerY += point.y;
        }
        centerX /= graphics[ChooseIdx].points.size();
        centerY /= graphics[ChooseIdx].points.size();
    }
    Point newP = applyTransform({static_cast<int>(centerX),static_cast<int>(centerY)},graphics[ChooseIdx].transform);
    centerX = newP.x;
    centerY = newP.y;
    // 创建缩放矩阵
    Matrix3x3 scalingMatrix = createScalingMatrix(centerX, centerY, scaleFactor);
    
    graphics[ChooseIdx].transform = scalingMatrix * graphics[ChooseIdx].transform;
}
// 鼠标滚轮回调函数 - 用于旋转图形
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // 只在选择模式下处理滚轮事件
    if (mode == -1) {
        RotateGraphic(window, yoffset);
    }
}
void Translation(GLFWwindow* window) {
    if (mode != -1) return;
    
    if (ChooseIdx < 0 || ChooseIdx >= graphics.size()) return;
    
    bool moved = false;
    float deltaX = 0.0f, deltaY = 0.0f;
    
    // 处理平移按键
    if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) {
        deltaX = -1.0f;
        moved = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS) {
        deltaX = 1.0f;
        moved = true;
    }
    
    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS) {
        deltaY = 1.0f;
        moved = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
        deltaY = -1.0f;
        moved = true;
    }
    
    // 如果有平移操作
    if (moved && (deltaX != 0.0f || deltaY != 0.0f)) {
        TranslateGraphic(deltaX, deltaY);
    }
    
    // 处理缩放按键
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
        ScaleGraphic(window, 1.05f);
        moved = true;
    }
    
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        ScaleGraphic(window, 0.95f);
        moved = true;
    }
    
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        // 重置变换矩阵为单位矩阵
        Matrix3x3 identityMatrix;
        graphics[ChooseIdx].transform = Matrix3x3();
        printf("reset\n");
        moved = true;
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