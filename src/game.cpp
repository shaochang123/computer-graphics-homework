#include<game/player.h>
#include<game/menu.h>
#include<game/attack.h>
#include<game/audio.h>
#include<game/epoch.h>
// 键盘回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void flushwindow();
void inline render(GLFWwindow* window);
void inline isgameover(GLFWwindow* window,bool arg);
int main(){
    initRandom();
    const int width = 500, height = 500;//设置窗口大小
    glfwSwapInterval(1);//垂直同步
    GLFWwindow* window = init(width, height);
    // 注册键盘回调函数
    glfwSetKeyCallback(window, key_callback);
    audio.playBackgroundMusic("resource/bgm.mp3", true); // 播放背景音乐
    while(!glfwWindowShouldClose(window)) {
        stime = glfwGetTime();
        flushwindow(); 
        if(you.hp<=0){isgameover(window,false);}//游戏失败
        if(enemyhp<=0){isgameover(window,true);}//游戏胜利
        if(gamemode==1){processepoch(level,window,render);you.move(window);you.iscollided();}//移动操作
        render(window);                                         // 最后显示到屏幕
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    audio.stopBackgroundMusic();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
// 在按键回调中添加音效
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if(key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        if(key ==GLFW_KEY_D){
            audio.playSound("resource/switch.wav"); // 播放音效
            if(you.Debug){
                you.Debug = false;
            }
            else{
                you.Debug = true;
            }
        }
        if(gamemode==0){
           menu.move(window,key,scancode,action,mods);
           if(key == GLFW_KEY_Z){
                
                if(Idx==0){//进入回合
                    audio.playSound("resource/snd_damage_c.wav"); // 播放音效
                    gamemode = 1;
                    int newlevel =rand()%5;
                    while(newlevel==level)newlevel = rand()%5;
                    level = newlevel;
                    double curtime  = stime;
                    double curenemyhp=enemyhp;
                    while(glfwGetTime()-curtime<=0.5){
                        
                        stime = glfwGetTime();
                        flushwindow();
                        enemyhp = curenemyhp - (stime-curtime)*13;
                        menu.renderenemyhp(enemyhp);
                        render(window);
                        glfwSwapBuffers(window);
                        glfwPollEvents();
                    }
                    curtime = glfwGetTime();
                    while(glfwGetTime()-curtime<=0.5){
                        stime = glfwGetTime();
                        flushwindow();
                        menu.renderenemyhp(enemyhp);
                        render(window);
                        glfwSwapBuffers(window);
                        glfwPollEvents();
                    }
                    selectepoch(level,window,render);
                    
                }
                if(Idx==1){//吃药，不增回合
                    
                    if(food==0)return;
                    audio.playSound("resource/heal.wav"); // 播放音效
                    food--;
                    gamemode=1;
                    you.hp+=40;
                    if(you.hp>=100)you.hp=100;
                    int newlevel = rand()%5;
                    while(newlevel==level)newlevel = rand()%5;
                    level = newlevel;
                    selectepoch(level,window,render);
                }
                if(Idx==2){//饶恕
                    level = -1;
                    selectepoch(level,window,render);
                    gamemode=1;
                }
            }
        }
    }
}
void inline render(GLFWwindow* window){
    glClear(GL_COLOR_BUFFER_BIT);                     // 清空OpenGL缓冲区
    
    if(gamemode==1)you.renderPlayer();//渲染玩家
    menu.renderbar();//画出菜单
    menu.renderhp(you.hp);//画出hp
    if(gamemode==0&&Idx==0)menu.renderImage("resource/fight_active.png",50,25,120,50);
    else menu.renderImage("resource/fight.png",50,25,120,50);
    if(gamemode==0&&Idx==1)menu.renderImage("resource/item_active.png",190,26,120,50);
    else menu.renderImage("resource/item.png",190,25,120,50);
    if(gamemode==0&&Idx==2)menu.renderImage("resource/mercy_active.png",330,25,120,50);
    else menu.renderImage("resource/mercy.png",330,25,120,50);
    menu.renderImage("resource/sans.png",200,260,100,120);
    glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲区
    glMatrixMode(GL_MODELVIEW);//设置模型视图矩阵
    glLoadIdentity();//重置当前的模型视图矩阵
    // 遍历每个像素，绘制矩形
    for (int i = 0; i < maxn; i++) {
        for (int j = 0; j < maxn; j++) {
            // 设置颜色
            glColor3f(g[i][j].r, g[i][j].g, g[i][j].b);
            // 绘制矩形（每个像素占1x1的正方形）
            glBegin(GL_QUADS);
            glVertex2f(j, i);        // 左下角
            glVertex2f(j+1, i);      // 右下角
            glVertex2f(j+1, i+1);    // 右上角
            glVertex2f(j, i+1);      // 左上角
            glEnd();
        }
    }

}
void inline isgameover(GLFWwindow* window, bool arg) {//判断游戏结束或者胜利
    if (arg == false) {
        // 游戏失败处理
        playaudio.playBackgroundMusic("resource/gameover.wav"); // 播放音效
    } else {
        // 游戏胜利处理
       playaudio.playBackgroundMusic("resource/Congratulations.wav"); // 播放胜利音效
    }
    
    // 显示结束画面
    render(window);
    glfwSwapBuffers(window);
    // 等待按键
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            // 重置游戏状态
            you.hp = 100;
            you.x = maxn/2;
            you.y = maxn/2-100;
            you.switchstat('r');
            level = 0;
            gamemode = 0;
            Idx = 0;
            LMenu = 50;
            RMenu=450;
            DMenu=100;
            UMenu=250;
            bone.clear();
            bluebone.clear();
            orangebone.clear();
            enemyhp = 100; // 胜利后重置总回合数
            food = 5; // 重置食物数量
            stime = glfwGetTime();
            audio.playBackgroundMusic("resource/bgm.mp3", true);
            break;
        }
        // if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        //     // 退出游戏
        //     glfwSetWindowShouldClose(window, GLFW_TRUE);
        //     break;
        // }
        // 可以适当sleep防止CPU占用过高
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
