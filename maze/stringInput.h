#pragma once
#include<iostream>
#include<SDL.h>
#include<SDL_ttf.h>
class StringInput
{
public:
    std::string str;//存储字符串 
    SDL_Surface* surf;//文本表面
    SDL_Rect* rect; //输入框位置
    TTF_Font* font; 
    bool show;

    StringInput(int x, int y, int w, int h);//初始化变量
    ~StringInput(); //完成清理
    std::string handleInput(SDL_Event* evt); //处理输入
    void render(SDL_Renderer*); //在屏幕上显示信息
};