#ifndef GAME_PLAYER_H_INCLUDED
#define GAME_PLAYER_H_INCLUDED
#ifndef GRAPH_INIT_H_INCLUDED
#include<graph/init.h>
#endif
#ifndef GAME_AUDIO_H
#include<game/audio.h>
#endif
Audio playaudio;
class player{
    private:
    float speed;
    
    public:
    char stat;//status为状态
    float x,y,hp;
    player(int hp=100,int x=maxn/2,int y=maxn/2 -100,float speed=1.5,char status='r'):hp(hp),x(x),y(y),speed(speed),stat(status){}//构造函数
    // 在 init.h 中添加渲染 player 的函数
    void renderPlayer();
    void switchstat();
    void move(GLFWwindow* glWindow);
};
void player::renderPlayer(){
    Color color = {1.0, 0.0, 0.0}; // 红色
    if(stat=='b')color = {0.0, 0.0, 1.0}; // 蓝色
    Color white = {1.0, 1.0, 1.0}; // 白色
    
    for(int i=x-4;i<=x+4;i++){
        setpixel(i,y,1,color);
    }
    
    for(int i=x-3;i<=x+3;i++){
        setpixel(i,y-1,1,color);
        
            
    }
    
    for(int i=x-2;i<=x+2;i++){
        setpixel(i,y-2,1,color);
        
            
    }
    
    for(int i=x-1;i<=x+1;i++){
        setpixel(i,y-3,1,color);
        
           
    }
    
    for(int i=x-3;i<=x+3;i++)if(i!=x){
        setpixel(i,y+1,1,color);
      
    }
    
    setpixel(x-2,y+2,1,color);
    setpixel(x+2,y+2,1,color);
    setpixel(x,y-4,1,color);
    
    
}
void player::move(GLFWwindow* window){
    int oldX=x,oldY=y;
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && y+speed<=248-3){
        y+=speed;
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && y-speed>=104+3){
        y-=speed;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && x-speed>=54+3){
        x-=speed;
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && x+speed<=446-3){
        x+=speed;
    }
    
    // 连续碰撞检测 - 检查移动路径上是否有骨头
    // 沿着移动路径采样多个点，检查是否与骨头碰撞
    int steps = std::max(abs(x - oldX), abs(y - oldY)) * 2; // 采样点数量
    if (steps > 1) {
        for (int i = 1; i < steps; i++) {
            float t = (float)i / steps;
            int checkX = oldX + (x - oldX) * t;
            int checkY = oldY + (y - oldY) * t;
            
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
                playaudio.playSound("resource/hit.wav"); // 播放音效
                hp--;
                break;
            }
        }
    }
}
void player::switchstat(){
    playaudio.playSound("resource/switch.wav"); // 播放音效
    if(stat=='r')stat='b';
    else stat='r';
}
#endif // GAME_PLAYER_H_INCLUDED