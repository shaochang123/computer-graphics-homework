#ifndef GAME_EPOCH_H_INCLUDED
#define GAME_EPOCH_H_INCLUDED
#ifndef GAME_INIT_H_INCLUDED
#include<game/init.h>
#endif
#ifndef GAME_ATTACK_H_INCLUDED
#include<game/attack.h>
#endif

// 定义渲染函数的函数指针类型
typedef void (*RenderFunc)(GLFWwindow*);

// 修改函数声明，添加render函数指针参数
void selectepoch(int level, GLFWwindow* window, RenderFunc renderFunc);
void inline processepoch(int level, GLFWwindow* window, RenderFunc renderFunc);

// 实现函数时使用传入的renderFunc
void selectepoch(int level, GLFWwindow* window, RenderFunc renderFunc){
    starttime = glfwGetTime();//开始的回合
    you.x=maxn/2;
    you.y=maxn/2-100;
    if(level==0){//回合0
        you.switchstat('r');
        for(int i=0;i<25;i++){//召唤骨头
            
            float sp = rand()%50;
            graphic g = {{{50-i*100, 100},{50-i*100,250}}, 0, {1.0f,1.0f,1.0f},2};// 在左边创建一个骨头
            normalbone b(g,sp);
            bone.push_back(b);
        }
        for(int i=1;i<25;i++){//召唤骨头
            float sp = rand()%50;
            graphic g = {{{450+i*300, 100},{450+i*300,250}}, 0, {0.0f,0.0f,1.0f},2};// 在右边创建一个蓝色骨头
            normalbone b(g,sp);
            bluebone.push_back(b);
        }
        thunder t({you.x,175},'h',40,150);
        thunderbone.push_back(t);
        thunder t1({you.x,175},'h',40,150,stime+1);
        thunderbone.push_back(t1);
    }
    else if(level==1){//回合1，此时玩家是蓝色模式
        you.switchstat('b');
        for(int i=0;i<40;i++){//召唤骨头
            int height = (rand()%50)+105;
            graphic g = {{{50-i*290, 100},{50-i*290,height}}, 0, {1.0f,1.0f,1.0f},3};// 在左边创建一个骨头
            graphic g1 = {{{450+i*290,100},{450+i*290,height}}, 0, {1.0f,1.0f,1.0f},3};// 在右边创建一个骨头
            graphic g2 = {{{50-i*290, height+30},{50-i*290,250}}, 0, {1.0f,1.0f,1.0f},3};// 在左边创建一个骨头
            graphic g3 = {{{450+i*290,height+30},{450+i*290,250}}, 0, {1.0f,1.0f,1.0f},3};// 在右边创建一个骨头
            float s1 = 1+rand()%60;
            float s2 = 1+rand()%60;
            normalbone b(g,s1);
            normalbone b1(g1,s2);
            normalbone b2(g2,s1);
            normalbone b3(g3,s2);
            bone.push_back(b);
            bone.push_back(b2);
            bone.push_back(b1);
            bone.push_back(b3);
        }
    }
    else if(level==2){
        you.switchstat('r');
        graphic circle = {{{250,150},{270,150}},2,{1.0f,1.0f,1.0f},2};// 在左边创建一个骨头
        normalbone b(circle);
        bone.push_back(b);
    }
    else if(level==3){
        you.switchstat('r');
        double curtime=stime;
        int curLmenu = LMenu;
        int curRmenu = RMenu;
        while(glfwGetTime()-curtime<=0.5){
            stime = glfwGetTime();
            flushwindow();
            LMenu = curLmenu + (glfwGetTime()-curtime)*250;
            RMenu = curRmenu - (glfwGetTime()-curtime)*250;
            renderFunc(window);  // 使用传入的renderFunc
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        graphic g = {{{137, 175},{363,175}}, 0, {1.0f,1.0f,1.0f},3};// 创建旋转骨头
        normalbone b(g);
        bone.push_back(b);
        for(int i=0;i<20;i++){//召唤骨头
            float sp = rand()%50;
            graphic g = {{{175-i*300, 100},{175-i*300,250}}, 0, {0.0f,0.0f,1.0f},2};// 在右边创建一个蓝色骨头
            normalbone b(g,sp);
            bluebone.push_back(b);
        }
    }
    else if(level==4){
        you.switchstat('b');
        for(int i=0;i<50;i++){//召唤骨头
            graphic g = {{{50-i*200, 100},{50-i*200,250}}, 0, {1.0f,1.0f,1.0f},3};// 在左边创建一个骨头
            normalbone b(g,40);
            bone.push_back(b);
        }
        for(int i=0;i<50;i++){//召唤骨头
            graphic g = {{{-50-i*200, 100},{-50-i*200,110}}, 0, {1.0f,1.0f,1.0f},2};// 在左边创建一个骨头
            normalbone b(g,40);
            bone.push_back(b);
        }
    }
    else if(level==5){
        you.switchstat('r');
        processtime = starttime;
        int x = rand()%maxn;
        int y = rand()%maxn;
        you.switchstat('r');
        graphic g = {{{x,y}},6,{1.0f,1.0f,1.0f},2};
        normalbone b(g,40);
        bone.push_back(b);
    }
    else if(level==6){
        you.switchstat('r');
        for(int i=1;i<=5;i++){
            graphic g = {{{-2000, 100+25*i},{2000,100+25*i}}, 0, {1.0f,1.0f,1.0f},2};// 在左边创建一个骨头
            normalbone b(g,10);
            bone.push_back(b);
        }
        you.y = 105;
        thunder t({you.x,175},'h',25,150);
        thunderbone.push_back(t);
        thunder t1({250,you.y},'v',400,25);
        thunderbone.push_back(t1);
    }
    else if(level==7){
        you.switchstat('r');
        for(int i=1;i<=7;i++){
            graphic g = {{{50+50*i, -2000},{50+50*i,2000}}, 0, {1.0f,1.0f,1.0f},2};// 在左边创建一个骨头
            normalbone b(g,10);
            bone.push_back(b);
        }
        you.x=55;
        thunder t({you.x,175},'h',50,150);
        thunderbone.push_back(t);
        thunder t1({250,you.y},'v',400,50);
        thunderbone.push_back(t1);
    }
    else if(level==8){
        you.switchstat('r');
        
        // 创建100个大三角形，初始位置在右上角窗口外
        for(int i = 0; i < 100; i++) {
            int startX =  500;  // 右上角窗口外
            int startY = rand()%550 + 150;        // 窗口上方
            int triangleSize = 30;            // 三角形大小
            
            // 创建等边三角形的顶点
            graphic g = {{{startX, startY - triangleSize},           // 顶点
                          {startX - triangleSize, startY + triangleSize/2},  // 左下
                          {startX + triangleSize, startY + triangleSize/2}}, // 右下
                         4, {1.0f, 1.0f, 1.0f}, 1};
            
            trianglebone tb(g, 0.8, stime + 3*(i/2), '4', false);  // 延迟生成
            Trianglebone.push_back(tb);
            startX = 0;  // 右上角窗口外
            startY = rand()%550 + 150;        // 窗口上方
            triangleSize = 30;            // 三角形大小
            
            // 创建等边三角形的顶点
            graphic g2 = {{{startX, startY - triangleSize},           // 顶点
                          {startX - triangleSize, startY + triangleSize/2},  // 左下
                          {startX + triangleSize, startY + triangleSize/2}}, // 右下
                         4, {1.0f, 1.0f, 1.0f}, 1};
            trianglebone tb2(g2, 0.8, stime + 3*(i/2), '2', false);  // 延迟生成
            Trianglebone.push_back(tb2);
        }
    }
    else if(level==-1){//饶恕
        you.switchstat('r');
    }
}
void inline processepoch(int level,GLFWwindow* window, RenderFunc renderFunc){//处理回合
    if(level==0){//回合0
        if(stime-starttime>=15.0){//回合结束
            Idx=0;
            if(!bone.empty())bone.clear();
            if(!bluebone.empty())bluebone.clear();
            if(!orangebone.empty())orangebone.clear();
            if(!thunderbone.empty())thunderbone.clear();
            if(!Trianglebone.empty())Trianglebone.clear();
            gamemode=0;
        }
        else{
            for(int j=0;j<bone.size();j++){
                bone[j].g.transform = Matrix3x3(); // 重置变换矩阵
                bone[j].rotate(stime-bone[j].st,200);
                bone[j].move(stime-bone[j].st,200);
                bone[j].render();
            }
            for(int j=0;j<bluebone.size();j++){
                bluebone[j].g.transform = Matrix3x3(); // 重置变换矩阵

                bluebone[j].move(stime-bluebone[j].st,200,'l');
                bluebone[j].render();
            }
            for(int j=0;j<thunderbone.size();j++){
                thunderbone[j].render();
                
                if(stime-thunderbone[j].startTime>=4.0){
                    thunderbone[j].center = {int(you.x),175};
                    thunderbone[j].startTime = stime;
                    thunderbone[j].isPlayed = false;
                }
            }
        }
    }
    else if(level==1){//回合1
        if(stime-starttime>=10.0){//回合结束
            Idx=0;
            if(!bone.empty())bone.clear();
            if(!bluebone.empty())bluebone.clear();
            if(!orangebone.empty())orangebone.clear();
            if(!thunderbone.empty())thunderbone.clear();
            if(!Trianglebone.empty())Trianglebone.clear();
            gamemode=0;
        }
        else{
            for(int j=0;j<bone.size();j++){
                bone[j].g.transform = Matrix3x3(); // 重置变换矩阵
                if(j%4<=1)bone[j].move(stime-bone[j].st,200,'r');
                else bone[j].move(stime-bone[j].st,200,'l');
                bone[j].render();
            }
        }
    }
    else if(level==2){//回合2
        if(stime-starttime>=10.0){//回合结束
            Idx=0;
            if(!bone.empty())bone.clear();
            if(!bluebone.empty())bluebone.clear();
            if(!orangebone.empty())orangebone.clear();
            if(!thunderbone.empty())thunderbone.clear();
            if(!Trianglebone.empty())Trianglebone.clear();
            gamemode=0;
        }
        else{
            Point st = {60,120};
            for(int i=0;i<bone.size();i++){
                bone[i].g.transform = Matrix3x3(); // 重置变换矩阵
                bone[i].move(stime-bone[i].st,18,'r');
                bone[i].move(stime-bone[i].st,8,'u');
                bone[i].render();
            }
            menu.renderbar();
            FillGraph(st);
        }
    }
    else if(level==3){//回合3
        if(stime-starttime>=10.0){//回合结束
            Idx=0;
            if(!bone.empty())bone.clear();
            if(!bluebone.empty())bluebone.clear();
            if(!orangebone.empty())orangebone.clear();
            if(!thunderbone.empty())thunderbone.clear();
            if(!Trianglebone.empty())Trianglebone.clear();
            double curtime=stime;
            int curLmenu = LMenu;
            int curRmenu = RMenu;
            while(glfwGetTime()-curtime<=0.5){
                stime = glfwGetTime();
                flushwindow();
                LMenu = curLmenu - (glfwGetTime()-curtime)*250;
                RMenu = curRmenu + (glfwGetTime()-curtime)*250;
                renderFunc(window);  // 使用传入的renderFunc
                glfwSwapBuffers(window);
                glfwPollEvents();
            }
            gamemode=0;
        }
        else{
            for(int j=0;j<bluebone.size();j++){
                bluebone[j].g.transform = Matrix3x3(); // 重置变换矩阵
                bluebone[j].move(stime-bluebone[j].st,200,'r');
                bluebone[j].render();
            }
            for(int j=0;j<bone.size();j++){
                bone[j].g.transform = Matrix3x3(); // 重置变换矩阵
                bone[j].rotate(stime-bone[j].st,190);
                bone[j].render();
            }
        }
    }
    else if(level==4){
        if(stime-starttime>=20.0){//回合结束
            Idx=0;
            if(!bone.empty())bone.clear();
            if(!bluebone.empty())bluebone.clear();
            if(!orangebone.empty())orangebone.clear();
            if(!thunderbone.empty())thunderbone.clear();
            if(!Trianglebone.empty())Trianglebone.clear();
            gamemode=0;
        }
        else{
            for(int j=0;j<50;j++){
                bone[j].g.transform = Matrix3x3(); // 重置变换矩阵
                bone[j].move(stime-bone[j].st,150);
                bone[j].scale(0.15*cos(2.5*(stime-bone[j].st))+0.85);
                bone[j].render();
            }
            for(int j=50;j<bone.size();j++){
                bone[j].g.transform = Matrix3x3(); // 重置变换矩阵
                bone[j].move(stime-bone[j].st,150);
                bone[j].render();
            }
        }
    }
    else if(level==5){
        if(stime-starttime>=17.0){//回合结束
            Idx=0;
            if(!bone.empty())bone.clear();
            if(!bluebone.empty())bluebone.clear();
            if(!orangebone.empty())orangebone.clear();
            if(!thunderbone.empty())thunderbone.clear();
            if(!Trianglebone.empty())Trianglebone.clear();
            gamemode=0;
        }
        else{
            if(stime-processtime>=1.0){
                processtime = stime;
                int dx = bone[0].g.points[bone[0].g.points.size()-1].x-you.x;
                int dy = bone[0].g.points[bone[0].g.points.size()-1].y-you.y;
                double distance = sqrt(dx*dx+dy*dy);
                int newx = bone[0].g.points[bone[0].g.points.size()-1].x-50.0*dx/distance;
                int newy = bone[0].g.points[bone[0].g.points.size()-1].y-50.0*dy/distance;
                // if(bone[0].g.points.size()>=3)bone[0].g.points.pop_back();
                bone[0].g.points.push_back({newx,newy});
                 audio.playSound("./resource/mus_st_meatfactory.ogg");
            }
            bone[0].render();
           
        }
    }
    else if(level==6){
        if(stime-starttime>=20.0){//回合结束
            Idx=0;
            if(!bone.empty())bone.clear();
            if(!bluebone.empty())bluebone.clear();
            if(!orangebone.empty())orangebone.clear();
            if(!thunderbone.empty())thunderbone.clear();
            if(!Trianglebone.empty())Trianglebone.clear();
            gamemode=0;
        }
        else{
            for(int j=0;j<bone.size();j++){
                bone[j].g.transform = Matrix3x3(); // 重置变换矩阵
                if(j%2==0)bone[j].move(stime-bone[j].st,50,'r');
                else bone[j].move(stime-bone[j].st,50,'l');
                bone[j].render(true);
            }
            for(int j=0;j<thunderbone.size();j++){
                thunderbone[j].render();
                
                if(stime-thunderbone[j].startTime>=4.0){
                    if(j==0){
                        thunderbone[j].center = {int(you.x),175};
                        thunderbone[j].startTime = stime;
                        thunderbone[j].isPlayed = false;
                    }
                    else{
                        thunderbone[j].center = {250,int(you.y)};
                        thunderbone[j].startTime = stime;
                        thunderbone[j].isPlayed = false;
                    }
                    
                }
            }
        }
    }
    else if(level==7){
        if(stime-starttime>=20.0){//回合结束
            Idx=0;
            if(!bone.empty())bone.clear();
            if(!bluebone.empty())bluebone.clear();
            if(!orangebone.empty())orangebone.clear();
            if(!thunderbone.empty())thunderbone.clear();
            if(!Trianglebone.empty())Trianglebone.clear();
            gamemode=0;
        }
        else{
            for(int j=0;j<bone.size();j++){
                bone[j].g.transform = Matrix3x3(); // 重置变换矩阵
                if(j%2==0)bone[j].move(stime-bone[j].st,50,'u');
                else bone[j].move(stime-bone[j].st,50,'d');
                bone[j].render(true);
            }
            for(int j=0;j<2;j++){
                thunderbone[j].render();
                
                if(stime-thunderbone[j].startTime>=4.0){
                    if(j==0){
                        thunderbone[j].center = {int(you.x),175};
                        thunderbone[j].startTime = stime;
                        thunderbone[j].isPlayed = false;
                    }
                    else{
                        thunderbone[j].center = {250,int(you.y)};
                        thunderbone[j].startTime = stime;
                        thunderbone[j].isPlayed = false;
                    }
                }
            }
        }
    }
    else if(level==8){
        if(stime-starttime>=15.0){//回合结束
            Idx=0;
            if(!bone.empty())bone.clear();
            if(!bluebone.empty())bluebone.clear();
            if(!orangebone.empty())orangebone.clear();
            if(!thunderbone.empty())thunderbone.clear();
            if(!Trianglebone.empty())Trianglebone.clear();
            gamemode=0;
        }
        
        // 处理现有三角形
        for(int j = 0; j < Trianglebone.size(); j++){
            Trianglebone[j].g.transform = Matrix3x3(); // 重置变换矩阵
            Trianglebone[j].rotate(stime-Trianglebone[j].st, 300);
            Trianglebone[j].move(stime-Trianglebone[j].st, 150);
            
            // 检查大三角形是否到达分裂点（只有大三角形且未分裂才检查）
            if(!Trianglebone[j].isSmall && !Trianglebone[j].hasSplit) {
                Point center = Trianglebone[j].getCenter();
                if(center.y <= 100) {
                    // 播放分裂音效
                    audio.playSound("resource/mus_creepy_ambience.ogg");
                        
                    // 创建4个小三角形
                    std::vector<trianglebone> splitTriangles = Trianglebone[j].split();
                    for(auto& newTriangle : splitTriangles) {
                        Trianglebone.push_back(newTriangle);
                    }
                    // 标记为已分裂
                    Trianglebone[j].hasSplit = true;
                }
            }
                
                // 渲染三角形（只渲染未分裂的或小三角形）
            if(!Trianglebone[j].hasSplit || Trianglebone[j].isSmall) {
                Trianglebone[j].render();
            }
            
        }
    }
    else if(level==-1){
        you.drophp();
    }
}
#endif