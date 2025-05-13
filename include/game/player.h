#ifndef GAME_PLAYER_H_INCLUDED
#define GAME_PLAYER_H_INCLUDED
#ifndef game_INIT_H_INCLUDED
#include<game/init.h>
#endif
#ifndef GAME_AUDIO_H
#include<game/audio.h>
#endif
#ifndef GAME_ATTACK_H
#include<game/attack.h>
#endif
Audio playaudio;
class player{
    public:
    float speed;
    char stat;//status为状态
    float x,y,hp,oldx,oldy;
    bool CanJump = true;//是否可以跳跃
    float vy = 0.0f;         // 垂直速度
    bool isJumping = false;  // 是否正在跳跃
    float jumpMaxY = maxn/2 + 80; // 最大跳跃高度（可调整）
    
    // 添加滞空属性
    float floatTime = 0.0f;     // 当前滞空时间
    float maxFloatTime = 0.3f;  // 最大滞空时间（秒）
    bool isFloating = false;    // 是否正在滞空
    
    player(int hp=100,int x=maxn/2,int y=maxn/2 -100,float speed=2.0,char status='r'):hp(hp),x(x),y(y),oldx(x),oldy(y),speed(speed),stat(status){}//构造函数
    void renderPlayer();
    void switchstat(char statt);
    void move(GLFWwindow* glWindow);
    void drophp();
    void iscollided();
    
};
void player::renderPlayer(){
    Color color = {1.0, 0.0, 0.0}; // 红色
    if(stat=='b')color = {0.0, 0.0, 1.0}; // 蓝色
    
    for(int i=x-4;i<=x+4;i++){setpixel(i,y,1,color);}
    for(int i=x-3;i<=x+3;i++){setpixel(i,y-1,1,color);}
    for(int i=x-2;i<=x+2;i++){setpixel(i,y-2,1,color);  }
    for(int i=x-1;i<=x+1;i++){setpixel(i,y-3,1,color);  }
    for(int i=x-3;i<=x+3;i++)if(i!=x){setpixel(i,y+1,1,color);}
    setpixel(x-2,y+2,1,color);setpixel(x+2,y+2,1,color);setpixel(x,y-4,1,color);
}
void player::drophp(){
    hp--;
    playaudio.playSound("resource/hit.wav"); // 播放音效
}
void player::move(GLFWwindow* window){
    oldx = x;
    oldy = y;
    const float upgravity = -0.01f;
    const float gravity = -0.08f;       // 重力加速度
    const float jumpSpeed = speed*0.8;  // 跳跃初速度
    const float groundY = DMenu + 7;    // 地面y坐标
    
    if(stat == 'b'){
        // 跳跃 - 现有代码保留
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !isJumping && y <= UMenu-5 && CanJump){
            vy = jumpSpeed;
            isJumping = true;
            CanJump = false;
            isFloating = false;  // 刚起跳，重置滞空状态
            floatTime = 0;       // 重置滞空时间
        }
        
        // 松开上键逻辑 - 现有代码保留
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && isJumping && vy > 0){
            vy = 0.0f; // 立即下落
            isFloating = true;  // 不进入滞空状态
            
        }
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE){
            CanJump = true; // 允许再次跳跃
        }
        // 应用重力和竖直速度
        if(isJumping){
            // 上升阶段
            if(vy > 0){
                y += vy;
                vy += upgravity;
                
                // 判断是否到达最高点
                if(vy <= 0){
                    vy = 0;  // 停止上升
                    isFloating = true;  // 进入滞空状态
                }
            }
            // 滞空阶段
            else if(isFloating){
                floatTime += 1.0f/60.0f;  // 假设以60FPS运行
                
                // 滞空时间到期，开始下落
                if(floatTime >= maxFloatTime){
                    isFloating = false;
                    vy = gravity;  // 开始下落，初始速度较小
                }
            }
            // 下落阶段
            else {
                y += vy;
                vy += gravity;
                
                // 检查是否落地
                if(y <= groundY){
                    y = groundY;
                    vy = 0;
                    isJumping = false;
                    isFloating = false;
                }
            }
        }
        
        // 左右移动代码保持不变...
        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && x-speed>=LMenu+7){
            x -= speed;
            
        }
        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && x+speed<=RMenu-7){
            x += speed;
            
        }
    }
    else{
        // 红色模式，原有逻辑
        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && x-speed>=LMenu+7){
            x -= speed;
        }
        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && x+speed<=RMenu-7){
            x += speed;
        }
        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && y-speed>=DMenu+7){
            y -= speed;
        }
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && y+speed<=UMenu-5){
            y += speed;
        }
    }
}
void player::iscollided(){
    // 连续碰撞检测 - 检查移动路径上是否有骨头
    // 沿着移动路径采样多个点，检查是否与骨头碰撞
    bool isMoving = (x != oldx || y != oldy);
    if(x==oldx&&y==oldy){//没动，如果还是原来的方案，不会有采样点
        int oldX=oldx;
        int oldY=oldy;
        if(g[oldY][oldX].r>0.9f&&g[oldY][oldX].g>0.9f&&g[oldY][oldX].b>0.9f){
            drophp();
        }
        return;
    }
    // 像素级检测 (仅用于补充基础检测)
    if (isMoving) {
        int checkX = x;
        int checkY = y;
        
        // 检查当前位置的碰撞
        if (checkX >= 0 && checkX < maxn && checkY >= 0 && checkY < maxn) {
            
            // 蓝色骨头 - 静止时不扣血
            if (g[checkY][checkX].b > 0.9f && g[checkY][checkX].r < 0.3f && g[checkY][checkX].g < 0.3f) {
                drophp();
            }
            
        }
    }
    if(!isMoving){
        
        for(auto& b : orangebone){
           if(b.detectPlayer(x,y))drophp();
        }
    }
    int steps = std::max(abs(x - oldx), abs(y - oldy)) * 3; // 采样点数量
    if (steps > 1) {
        for (int i = 1; i < steps; i++) {
            float t = (float)i / steps;
            int checkX = oldx + (x - oldx) * t;
            int checkY = oldy + (y - oldy) * t;
            
            // 检查此点是否与骨头碰撞
            bool pathCollided = false;
            for (int dx = -4; dx <= 4; dx++) {
                for (int dy = -4; dy <= 4; dy++) {
                    int px = checkX + dx;
                    int py = checkY + dy;
                    if (px >= 0 && px < maxn && py >= 0 && py < maxn) {
                        if (g[py][px].r > 0.9f && g[py][px].g > 0.9f && g[py][px].b > 0.9f) {
                            pathCollided = true;
                            break;
                        }
                    }
                }
                if (pathCollided) break;
            }
            
            if (pathCollided) {
                drophp();
                break;
            }
        }
    }
    
}
void player::switchstat(char statt){//转换模式
    if(statt=='b'){
        // if(stat=='r')playaudio.playSound("resource/switch.wav"); // 播放音效
        stat=statt;
        y = DMenu + 7;
    }
    if(statt=='r'){
        // if(stat=='b')playaudio.playSound("resource/switch.wav"); // 播放音效
        stat=statt;
    }
}
player you;//玩家
#endif // GAME_PLAYER_H_INCLUDED