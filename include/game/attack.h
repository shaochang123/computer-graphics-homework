#ifndef GAME_ATTACK_H_INCLUDED
#define GAME_ATTACK_H_INCLUDED
#ifndef GRAPH_INIT_H_INCLUDED
#include<graph/init.h>
#endif
#ifndef GRAPH_LINE_H_INCLUDED
#include<graph/line.h>
#endif
class normalbone{
    private:
    public:
    graphic g;
    float speed;
    char dir;
    double st;
    normalbone(graphic g={{{0,0},{0,0}},0,{0,0,0}},float s=2.0,char dir='r',double st=glfwGetTime()):g(g),speed(s),dir(dir),st(st){}//构造函数;
    
    void move(double t) {
        // 计算移动距离
        float distance = speed * t*10;
        float dx = 0.0f, dy = 0.0f;
        
        // 根据方向计算位移
        switch(dir) {
            case 'r': dx = distance; break;
            case 'l': dx = -distance; break;
            case 'u': dy = distance; break;
            case 'd': dy = -distance; break;
        }
        
        // 创建并应用平移矩阵
        Matrix3x3 translationMatrix = createTranslationMatrix(dx, dy);
        
        // 累积变换（新变换 = 平移矩阵 * 当前变换）
        g.transform = translationMatrix * g.transform;
    }
    
    void rotate(double t) {
        // 计算旋转角度
        double angle = t * speed*10;
        
        // 计算旋转中心（线段中点）
        float centerX = (g.points[0].x + g.points[1].x) / 2.0f;
        float centerY = (g.points[0].y + g.points[1].y) / 2.0f;
        
        // 创建旋转矩阵
        Matrix3x3 rotationMatrix = createRotationMatrix(centerX, centerY, angle);
        
        // 累积变换（新变换 = 旋转矩阵 * 当前变换）
        g.transform = rotationMatrix * g.transform;
    }
    
    void render() {
        // 创建临时图形用于渲染
        graphic tempG = g;
        
        // 应用存储的变换到所有点
        for (auto &point : tempG.points) {
            point = applyTransform(point, g.transform);
        }
        
        // 渲染图形（这里假设有一个drawLine函数）
        drawLineBresenham(tempG.points[0], tempG.points[1], false, 
                         tempG.width, tempG.color);
    }
};
#endif