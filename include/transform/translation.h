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
#endif