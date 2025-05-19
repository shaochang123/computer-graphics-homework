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
    char dir;//方向
    normalbone(graphic g={{{0,0},{0,0}},0,{0,0,0}},float s=1.0,double st=stime,char dir='r'):g(g),speed(s),st(st),dir(dir){
        
    }//构造函数;
    
    void move(double distance,double k=1.0,char dir='r') {
        // 计算移动距离
       // float distance = speed * t* k;//当前骨头的时间乘以速度,k是放大倍数
       // 保存移动前的位置
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
        // 保存移动前的位置
        
        angle*=k;
        angle+=speed;
        // 计算旋转中心（线段中点）
        float centerX = 0.0f, centerY = 0.0f;
        if(g.mode==1){
            centerX = g.points[0].x;
            centerY = g.points[0].y;
        }
        else{
            centerX = (g.points[0].x + g.points[1].x) / 2.0f;
            centerY = (g.points[0].y + g.points[1].y) / 2.0f;
        }
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
        scaleFactor*=k;
        // 创建缩放矩阵
        // 保存移动前的位置
       
        Matrix3x3 scaleMatrix;
        float centerX = 0.0f, centerY = 0.0f;
        if(g.mode==1){
            centerX = g.points[0].x;
            centerY = g.points[0].y;
        }
        else{
            centerX = (g.points[0].x + g.points[1].x) / 2.0f;
            centerY = (g.points[0].y + g.points[1].y) / 2.0f;
        }
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
        if(g.mode==0)drawLineBresenham(tempG.points[0], tempG.points[1], false, 
                         tempG.width, tempG.color);
        
        if(g.mode==1){
            Point center = applyTransform(g.points[0], g.transform);
            Point start = applyTransform(g.points[1], g.transform);
            Point end = applyTransform(g.points[2], g.transform);
            
            double r = sqrt((start.x - center.x) * (start.x - center.x) + 
                         (start.y - center.y) * (start.y - center.y));
                      
            double startRad = atan2(center.y - start.y, start.x - center.x);
            double endRad = atan2(center.y - end.y, end.x - center.x);
            
            if (startRad < 0) startRad += 2 * M_PI;
            if (endRad < 0) endRad += 2 * M_PI;
            
            drawarc(center, r, startRad, endRad, g.width, g.color);
        }
        if(g.mode==2){
            double r=sqrt((tempG.points[1].x-tempG.points[0].x)*(tempG.points[1].x-tempG.points[0].x)+(tempG.points[1].y-tempG.points[0].y)*(tempG.points[1].y-tempG.points[0].y));//半径
            
            drawarc(tempG.points[0], r, 0, 2*M_PI,tempG.width,tempG.color);//起始角度和终止角度都为0和2π，表示画一个完整的圆
        }
    }
    bool detectPlayer(float x,float y){
        // 获取变换后的实际位置
        Point p1 = applyTransform(g.points[0], g.transform);
        Point p2 = applyTransform(g.points[1], g.transform);
        
        // 玩家判定区域
        int playerLeft = x - 4;
        int playerRight = x + 4;
        int playerTop = y + 4;
        int playerBottom = y - 4;
        
        // 对于橙色骨头，检查是否与玩家相交
        if (dir == 'l' || dir == 'r') {
            // 水平骨头，检查x轴区间重叠
            int boneLeft = std::min(p1.x, p2.x);
            int boneRight = std::max(p1.x, p2.x);
            int boneY = (p1.y + p2.y) / 2;
            
            if (boneRight >= playerLeft && boneLeft <= playerRight && 
                boneY >= playerBottom && boneY <= playerTop) {
                return true;
            }
        } 
        else if (dir == 'u' || dir == 'd') {
            // 垂直骨头，检查y轴区间重叠
            int boneX = (p1.x + p2.x) / 2;
            int boneTop = std::max(p1.y, p2.y);
            int boneBottom = std::min(p1.y, p2.y);
            
            if (boneX >= playerLeft && boneX <= playerRight && 
                boneTop >= playerBottom && boneBottom <= playerTop) {
                return true;
            }
        }
        
        return false;
    }
};
class thunder{//类似龙骨炮的攻击

};
std::vector<normalbone>bone;//骨头攻击
std::vector<normalbone>bluebone;
std::vector<normalbone>orangebone;
#endif