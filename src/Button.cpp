#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "Button.h"

Button::Button(int x, int y, int w, int h, const char* text, TTF_Font* font, SDL_Color colorTxt)
    : posx(x), posy(y), width(w), height(h), text(text), font(font), colorTxt(colorTxt) {}

Button::Button(const char* text, TTF_Font* font, SDL_Color colorTxt)
    : text(text), font(font), colorTxt(colorTxt) {}

Button::Button(int x, int y, int w, int h, const char* imgSrc)
    : posx(x), posy(y), width(w), height(h), imgSrc(imgSrc) {}

Button::Button(const char* imgSrc)
    : imgSrc(imgSrc) {}


int Button::create(SDL_Renderer* renderer) {
    if (this->hidden) return 0;

    if (this->showBd) this->showBorders(renderer);
    if (this->showBg) this->showBackgroundColor(renderer, colorBg);

    if (this->imgSrc != "") {
        SDL_Surface* img = IMG_Load(imgSrc);
        if (!img) {
            std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
            return 1;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, img);
        if (texture == NULL) {
            std::cout << "Failed to load texture: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(img);
            return 1;
        }
        SDL_FreeSurface(img);

        SDL_Rect destRect = { this->posx, this->posy, this->width, this->height };
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
    }

    else {
        SDL_Surface* text_surface = TTF_RenderText_Blended(font, text, colorTxt);
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

        SDL_FreeSurface(text_surface);

        SDL_Rect source_texture;
        SDL_Rect dest_texture;

        source_texture.x = source_texture.y = 0;

        SDL_QueryTexture(text_texture, NULL, NULL, &source_texture.w, &source_texture.h);

        SDL_Rect box_texture = { this->posx, this->posy, this->width, this->height };

        SDL_RenderDrawRect(renderer, &box_texture);

        dest_texture.x = this->posx + (box_texture.w - source_texture.w) / 2;
        dest_texture.y = this->posy + (box_texture.h - source_texture.h) / 2;

        dest_texture.w = source_texture.w;
        dest_texture.h = source_texture.h;

        SDL_RenderCopy(renderer, text_texture, &source_texture, &dest_texture);

        SDL_DestroyTexture(text_texture);
    }
}

void Button::setBorders(bool set) {
    this->showBd = set;
}

void Button::showBorders(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect rect = {
        posx, posy,
        width, height
    };

    SDL_RenderDrawRect(renderer, &rect);
}

void Button::setBackgroundColor(SDL_Color color, bool set) {
    this->colorBg = color;
    this->showBg = set;
}

void Button::setTextColor(SDL_Color color) {
    this->colorTxt = color;
}

void Button::showBackgroundColor(SDL_Renderer* renderer, SDL_Color colorBg) {
    SDL_SetRenderDrawColor(renderer, colorBg.r, colorBg.g, colorBg.b, colorBg.a);

    SDL_Rect rect = {
        posx, posy,
        width, height
    };

    SDL_RenderFillRect(renderer, &rect);
}

void Button::setText(const char* txt) {
    this->text = txt;
}

void Button::setImgSrc(const char* src) {
    this->imgSrc = src;
}

void Button::setHidden(bool set) {
    this->hidden = set;
}

void Button::setDimensions(int x = -1, int y = -1, int w = -1, int h = -1) {
    if (x >= 0) this->posx = x;
    if (y >= 0) this->posy = y;
    if (w >= 0) this->width = w;
    if (h >= 0) this->height = h;
}

void Button::setPosx(int x) {
    this->posx = x;
}

void Button::setPosy(int y) {
    this->posy = y;
}

void Button::setWidth(int w) {
    this->width = w;
}

void Button::setHeight(int h) {
    this->height = h;
}

const char* Button::getText() {
    return this->text;
}

int Button::getPosx() {
    return this->posx;
}

int Button::getPosy() {
    return this->posy;
}

int Button::getWidth() {
    return this->width;
}

int Button::getHeight() {
    return this->height;
}

bool Button::isHidden() {
    return this->hidden;
}

int Button::isHovered(int mousex, int mousey) {
    if (this->hidden) return 0;

    if (mousex >= posx && mousex <= posx + width) {
        if (mousey >= posy && mousey <= posy + height) {
            return 1;
        }
    }

    return 0;
}
