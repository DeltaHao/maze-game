#pragma once
#include<iostream>
#include<SDL.h>
#include<SDL_ttf.h>
class StringInput
{
public:
    std::string str;//�洢�ַ��� 
    SDL_Surface* surf;//�ı�����
    SDL_Rect* rect; //�����λ��
    TTF_Font* font; 
    bool show;

    StringInput(int x, int y, int w, int h);//��ʼ������
    ~StringInput(); //�������
    std::string handleInput(SDL_Event* evt); //��������
    void render(SDL_Renderer*); //����Ļ����ʾ��Ϣ
};