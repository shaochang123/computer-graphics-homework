#ifndef TRANSFORM_TRANSLATION_H
#define TRANSFORM_TRANSLATION_H
#ifndef GRAPH_INIT_H_INCLUED
#include<graph/init.h>
#endif
#include <cmath>
// 拖动状态变量
static bool isDragging = false;
static double dragStartX = 0.0, dragStartY = 0.0;
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
    
    // 获取原始旋转中心
    Point centerPoint = {static_cast<int>(graphics[ChooseIdx].cenx), 
                         static_cast<int>(graphics[ChooseIdx].ceny)};
    
    // 应用当前变换获取变换后的中心
    Point transformedCenter;
    if(graphics[ChooseIdx].isdefault)transformedCenter = applyTransform(centerPoint, graphics[ChooseIdx].transform);
    else transformedCenter = centerPoint;
    // 创建旋转矩阵（围绕变换后的中心点）
    Matrix3x3 rotationMatrix = createRotationMatrix(transformedCenter.x, transformedCenter.y, angle);
    
    // 应用旋转矩阵
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
    double centerX = graphics[ChooseIdx].cenx, centerY = graphics[ChooseIdx].ceny;
    if(graphics[ChooseIdx].isdefault){
        Point newP = applyTransform({static_cast<int>(centerX),static_cast<int>(centerY)},graphics[ChooseIdx].transform);
        centerX = newP.x;
        centerY = newP.y;
    }
    // 创建缩放矩阵
    Matrix3x3 scalingMatrix = createScalingMatrix(centerX, centerY, scaleFactor);
    
    graphics[ChooseIdx].transform = scalingMatrix * graphics[ChooseIdx].transform;
}
void newcenter(GLFWwindow* window,int button,int action,int mods){
    if(button==GLFW_MOUSE_BUTTON_RIGHT && action==GLFW_PRESS && mode==-1){
        double xpos,ypos;
        detectposition(window,xpos,ypos);
        if(ChooseIdx>=0&&ChooseIdx<graphics.size()){
            graphics[ChooseIdx].cenx=xpos;
            graphics[ChooseIdx].ceny=ypos;
        }
        printf("new center: %lf %lf\n",xpos,ypos);
        graphics[ChooseIdx].isdefault = false;
    }
}
// 鼠标滚轮回调函数 - 用于旋转图形
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // 只在选择模式下处理滚轮事件
    if (mode == -1) {
        RotateGraphic(window, yoffset);
    }
}
// 处理图形拖动平移
void HandleDragTranslation(GLFWwindow* window, int button, int action, int mods) {
    // 只在选择模式下处理
    if (mode != -1 || ChooseIdx < 0 || ChooseIdx >= graphics.size()) 
        return;
    
    // 获取当前鼠标位置
    double currentX, currentY;
    detectposition(window, currentX, currentY);
    
    // 鼠标左键按下时开始拖动
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // 判断点击是否在已选图形上
        // 这里复用了 SelectGraphByClick 的逻辑，只是不改变已选择的图形
        if (ChooseIdx >= 0) {
            isDragging = true;
            dragStartX = currentX;
            dragStartY = currentY;
            // 可以设置光标样式为移动状态
            glfwSetCursor(window, glfwCreateStandardCursor(GLFW_HAND_CURSOR));
        }
    }
    
    // 鼠标左键释放时结束拖动
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        isDragging = false;
        // 恢复默认光标
        glfwSetCursor(window, NULL);
    }
}
// 鼠标移动回调函数
void cursor_position(GLFWwindow* window) {
    // 处理图形拖动
    if (isDragging && mode == -1 && ChooseIdx >= 0) {
        double currentX, currentY;
        detectposition(window, currentX, currentY);
        
        // 计算拖动偏移量
        float deltaX = currentX - dragStartX;
        float deltaY = currentY - dragStartY;
        
        if (deltaX != 0 || deltaY != 0) {
            // 应用平移变换
            TranslateGraphic(deltaX, deltaY);
            // 更新起始位置为当前位置
            dragStartX = currentX;
            dragStartY = currentY;
        }
    }
}
void Translation(GLFWwindow* window) {
    if (mode != -1) return;
    
    if (ChooseIdx < 0 || ChooseIdx >= graphics.size()) return;
    
    bool moved = false;
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
        getcenposition(graphics[ChooseIdx]);
        printf("reset\n");
        moved = true;
    }
}


#endif