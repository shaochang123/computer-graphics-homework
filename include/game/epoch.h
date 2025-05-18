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
    else if(level==-1){//饶恕
        you.switchstat('r');
    }
}
void inline processepoch(int level,GLFWwindow* window, RenderFunc renderFunc){//处理回合
    if(level==0){//回合0
        if(stime-starttime>=10.0){//回合结束
            Idx=0;
            if(!bone.empty())bone.clear();
            if(!bluebone.empty())bluebone.clear();
            if(!orangebone.empty())orangebone.clear();
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
        }
    }
    else if(level==1){//回合1
        if(stime-starttime>=10.0){//回合结束
            Idx=0;
            if(!bone.empty())bone.clear();
            if(!bluebone.empty())bluebone.clear();
            if(!orangebone.empty())orangebone.clear();
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
    else if(level==-1){
        you.drophp();
    }
}
#endif