#ifndef GAME_ATTACK_H_INCLUDED
#define GAME_ATTACK_H_INCLUDED
#ifndef GAME_INIT_H_INCLUDED
#include<game/init.h>
#endif
class normalbone{
    public:
    graphic g;//这个骨头的图形（一般是直线）
    float speed;//速度
    double st;//起始时间
    normalbone(graphic g={{{0,0},{0,0}},0,{0,0,0}},float s=1.0,double st=stime):g(g),speed(s),st(st){}//构造函数;
    
    void move(double distance,double k=1.0,char dir='r') {
        // 计算移动距离
       // float distance = speed * t* k;//当前骨头的时间乘以速度,k是放大倍数
        float dx = 0.0f, dy = 0.0f;
        distance*=k;
        distance+=speed;
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
    
    void rotate(double angle,double k=1.0,bool dir=true) {
        // 计算旋转角度
        //double angle = t * speed*k;//k是放大倍数
        angle*=k;
        angle+=speed;
        // 计算旋转中心（线段中点）
        float centerX = (g.points[0].x + g.points[1].x) / 2.0f;
        float centerY = (g.points[0].y + g.points[1].y) / 2.0f;
        Point cur = {static_cast<int>(centerX), static_cast<int>(centerY)};
        cur = applyTransform(cur, g.transform);
        // 创建旋转矩阵
        Matrix3x3 rotationMatrix ;
        if(dir)rotationMatrix = createRotationMatrix(cur.x, cur.y, angle);
        else rotationMatrix = createRotationMatrix(cur.x, cur.y, -angle);
        // 累积变换（新变换 = 旋转矩阵 * 当前变换）
        g.transform = rotationMatrix * g.transform;
    }
    void scale(double scaleFactor,double k=1.0) {
        // 计算缩放比例
        // float scaleFactor = 1.0f + t * speed*k;//k是放大倍数
        
        // 创建缩放矩阵
        Matrix3x3 scaleMatrix;
        float centerX = (g.points[0].x + g.points[1].x) / 2.0f;
        float centerY = (g.points[0].y + g.points[1].y) / 2.0f;
        Point cur = {static_cast<int>(centerX), static_cast<int>(centerY)};
        cur = applyTransform(cur, g.transform);
        scaleMatrix = createScalingMatrix(cur.x,cur.y,scaleFactor);
        
        // 累积变换（新变换 = 缩放矩阵 * 当前变换）
        g.transform = scaleMatrix * g.transform;
    }
    void render() {//渲染这个骨头
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
std::vector<normalbone>bone;//骨头攻击
#endif