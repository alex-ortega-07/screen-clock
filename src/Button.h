#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

class Button {
public:
    Button(int x, int y, int w, int h, const char* text, TTF_Font* font, SDL_Color colorTxt);
    Button(const char* text, TTF_Font* font, SDL_Color colorTxt);
    Button(int x, int y, int w, int h, const char* imgSrc);
    Button(const char* imgSrc);

    int create(SDL_Renderer* renderer);

    void setBorders(bool set);
    void setBackgroundColor(SDL_Color color, bool set);
    void setTextColor(SDL_Color color);
    void setText(const char* txt);
    void setImgSrc(const char* src);
    void setHidden(bool set);

    void setDimensions(int x, int y, int w, int h);
    void setPosx(int x);
    void setPosy(int y);
    void setWidth(int w);
    void setHeight(int h);

    int getPosx();
    int getPosy();
    int getWidth();
    int getHeight();

    const char* getText();

    bool isHidden();
    int isHovered(int mousex, int mousey);

private:
    void showBorders(SDL_Renderer* renderer);
    void showBackgroundColor(SDL_Renderer* renderer, SDL_Color colorBg);

    int posx, posy;
    int width, height;

    const char* text = "";
    const char* imgSrc = "";

    bool showBd = false;
    bool showBg = false;
    bool hidden = false;

    TTF_Font* font;

    SDL_Color colorTxt;
    SDL_Color colorBg = { 255, 255, 255, 255 };
};
