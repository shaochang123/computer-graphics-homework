#ifndef GAME_INIT_H_INCLUDED
#define GAME_INIT_H_INCLUDED
#include<graph/init.h>
#include<graph/Line.h>
#include<graph/Circle.h>
#include<graph/Fill.h>
#include<graph/Polygon.h>
#include<graph/Crop.h>
#include<graph/FullCircle.h>
#include<transform/translation.h>
int level;//现在是第几轮
int total=10;//总共多少轮
int Idx=0;//选择菜单时的指向
int gamemode=0;//游戏模式，0为菜单，1为游戏
double stime;//游戏时长
double starttime;//回合开始的时间
int LMenu = 50,RMenu=450,DMenu=100,UMenu=250;//菜单位置
int food = 5;//食物数量
#endif