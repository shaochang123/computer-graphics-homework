#ifndef GAME_EPOCH_H_INCLUDED
#define GAME_EPOCH_H_INCLUDED
#ifndef GAME_INIT_H_INCLUDED
#include<game/init.h>
#endif
#ifndef GAME_ATTACK_H_INCLUDED
#include<game/attack.h>
#endif
void selectepoch(int level){
    starttime = glfwGetTime();//开始的回合
    you.x=maxn/2;
    you.y=maxn/2-100;
    if(level==0){//回合0
        you.switchstat('r');
        for(int i=0;i<20;i++){//召唤骨头
            
            float sp = rand()%50;
            graphic g = {{{50-i*100, 100},{50-i*100,250}}, 0, {1.0f,1.0f,1.0f},1};// 在左边创建一个骨头
            normalbone b(g,sp);
            bone.push_back(b);
        }
    }
    else if(level==1){//回合1，此时玩家是蓝色模式
        you.switchstat('b');
        for(int i=0;i<40;i++){//召唤骨头
            int height = (rand()%70)+100;
            graphic g = {{{50-i*200, 100},{50-i*200,height}}, 0, {1.0f,1.0f,1.0f},3};// 在左边创建一个骨头
            graphic g1 = {{{450+i*200,100},{450+i*200,height}}, 0, {1.0f,1.0f,1.0f},3};// 在右边创建一个骨头
            graphic g2 = {{{50-i*200, height+20},{50-i*200,250}}, 0, {1.0f,1.0f,1.0f},3};// 在左边创建一个骨头
            graphic g3 = {{{450+i*200,height+20},{450+i*200,250}}, 0, {1.0f,1.0f,1.0f},3};// 在右边创建一个骨头
            normalbone b(g);
            normalbone b1(g1);
            normalbone b2(g2);
            normalbone b3(g3);
            bone.push_back(b);
            bone.push_back(b2);
            bone.push_back(b1);
            bone.push_back(b3);
        }
    }
    else if(level==-1){//饶恕
        you.switchstat('r');
    }
}
void inline processepoch(int level){
    if(level==0){//回合0
        if(stime-starttime>=10.0){//回合结束
            Idx=0;
            if(!bone.empty())bone.clear();
            gamemode=0;
        }
        else{
            for(int j=0;j<bone.size();j++){
                bone[j].g.transform = Matrix3x3(); // 重置变换矩阵
                bone[j].rotate(stime-bone[j].st,200);
                bone[j].move(stime-bone[j].st,200);
                bone[j].render();
            }
        }
    }
    else if(level==1){//回合1
        if(stime-starttime>=10.0){//回合结束
            Idx=0;
            if(!bone.empty())bone.clear();
            gamemode=0;
        }
        else{
            for(int j=0;j<bone.size();j++){
                bone[j].g.transform = Matrix3x3(); // 重置变换矩阵
                if(j%4<=1)bone[j].move(stime-bone[j].st,100,'r');
                else bone[j].move(stime-bone[j].st,100,'l');
                bone[j].render();
            }
        }
    }
    else if(level==-1){
        you.drophp();
    }
}
#endif