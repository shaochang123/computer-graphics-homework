#include<graph/Line.h>
#include<graph/Circle.h>
#include<graph/Fill.h>
#include<graph/Polygon.h>
#include<graph/Crop.h>
#include<graph/FullCircle.h>
#include<transform/translation.h>
#include<game/player.h>
#include<game/menu.h>
#include<game/attack.h>
#include<game/audio.h>
int level = 0;
player you;
Menu menu;
Audio audio;
std::vector<normalbone>bone;
int Idx=0;
double starttime=-1;
// 键盘回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void flushwindow();
int main(){
    initRandom();
    const int width = 500, height = 500;//设置窗口大小
    glfwSwapInterval(1);//垂直同步
    GLFWwindow* window = init(width, height);
    // 注册键盘回调函数
    glfwSetKeyCallback(window, key_callback);
    audio.playBackgroundMusic("resource/bgm.mp3", true); // 播放背景音乐
    
    while(!glfwWindowShouldClose(window)) {
        flushwindow(); 
        if(you.hp<=0 && mode!=2) {
            audio.stopBackgroundMusic();
            audio.playSound("resource/gameover.wav"); // 播放游戏结束音效
            mode = 2; // 切换到游戏结束模式
        }
        if(mode==2){
            //结束界面(待写)
            render();                                         // 最后显示到屏幕
            glfwSwapBuffers(window);
            glfwPollEvents();
            continue;
        }
        if(level==1&&mode==1&&glfwGetTime()-starttime<=10.0){
            for(int j=0;j<bone.size();j++){
                bone[j].g.transform = Matrix3x3(); // 重置变换矩阵
                bone[j].rotate(glfwGetTime()-bone[j].st);
                bone[j].move(glfwGetTime()-bone[j].st);
                bone[j].render();
                
            }
           
        }
        else if(level==2&&mode==1&&glfwGetTime()-starttime<=10.0){
            if(you.stat=='r')you.switchstat();
        }
        else{
            bone.clear();
            starttime = -1;
            mode=0;
        }
        glClear(GL_COLOR_BUFFER_BIT);                     // 清空OpenGL缓冲区
                                           // 先清空屏幕缓冲区
        if(mode==1)you.move(window);
        if(mode==1)you.renderPlayer();
        menu.renderbar();
        menu.renderhp(you.hp);
        // menu.renderText();
        if(mode==0&&Idx==0)menu.renderImage("resource/fight_active.png",50,25,120,50);
        else menu.renderImage("resource/fight.png",50,25,120,50);
        if(mode==0&&Idx==1)menu.renderImage("resource/item_active.png",190,26,120,50);
        else menu.renderImage("resource/item.png",190,25,120,50);
        if(mode==0&&Idx==2)menu.renderImage("resource/mercy_active.png",330,25,120,50);
        else menu.renderImage("resource/mercy.png",330,25,120,50);
        menu.renderImage("resource/sans.png",200,260,100,120);
        render();                                         // 最后显示到屏幕
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    audio.stopBackgroundMusic();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
void flushwindow() {
    // 刷新屏幕
    for(int i=0; i<maxn; i++) {
        for(int j=0; j<maxn; j++) {
            g[i][j] = {0.0, 0.0, 0.0};
        }
    }
}
// 在按键回调中添加音效
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if(key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        if(mode==0){
            if(key ==GLFW_KEY_LEFT){
                Idx--;
                if(Idx<0)Idx=0;
                audio.playSound("resource/switch.wav"); // 播放音效
                
            }
            if(key ==GLFW_KEY_RIGHT){
                Idx++;
                if(Idx>2)Idx=2;
                audio.playSound("resource/switch.wav"); // 播放音效
                
            }
            if(key == GLFW_KEY_Z){
                audio.playSound("resource/switch.wav"); // 播放音效
                if(Idx==0){
                    you.x=maxn/2;
                    you.y=maxn/2-100;
                    
                    mode = 1;
                    if(level==1){
                        starttime = glfwGetTime();
                        level++;
                    }
                    if(level==0){
                        starttime = glfwGetTime();
                        for(int i=0;i<10;i++){
                            graphic g = {{{50-i*100, 100},{50-i*100,250}}, 0, {1.0f,1.0f,1.0f},3};
                            normalbone b(g,10+rand()%(30-10),'r',glfwGetTime());
                            bone.push_back(b);
                        }
                        level++;
                    }
                }
                if(Idx==1){//吃药，不增回合
                    you.hp+=40;
                    if(you.hp>=100)you.hp=100;
                   
                    you.x=maxn/2;
                    you.y=maxn/2-100;
                    mode = 1;
                    if(level==1){
                        starttime = glfwGetTime();
                        for(int i=0;i<10;i++){
                            graphic g = {{{50-i*100, 100},{50-i*100,250}}, 0, {1.0f,1.0f,1.0f},3};
                            normalbone b(g,10+rand()%(30-10),'r',glfwGetTime());
                            bone.push_back(b);
                        }
                    }
                    if(level==2){
                        starttime = glfwGetTime();
                        for(int i=0;i<10;i++){
                            graphic g = {{{50-i*100, 100},{50-i*100,250}}, 0, {1.0f,1.0f,1.0f},3};
                            normalbone b(g,50,'r',glfwGetTime());
                            bone.push_back(b);
                        }
                    }
                }
                if(Idx==2){

                }
            }
        }
    }
}
