#define _CRT_SECURE_NO_WARNINGS

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <ctime>
#include <windows.h>

#include "Button.h"
#include "Grid.h"

const int SCREEN_WIDTH = 265;
const int SCREEN_HEIGHT = 100;
const int MARGIN = 10;

const SDL_Color BLACK = { 0, 0, 0, 255 };
const SDL_Color WHITE = { 255, 255, 255, 255 };

std::vector<SDL_Color> COLORBGBT = {
                                     { 70, 84, 192, 255 },
                                     { 50, 123, 168, 255 },
                                     { 26, 67, 78, 255 }
                                   };

std::vector<SDL_Color> BACKGROUND = {
                                      { 54, 55, 62, 255 },
                                      { 0, 68, 166, 255 },
                                      { 255, 255, 255, 255 }
                                    };

std::vector<SDL_Color> TEXTCOLOR = {
                                     { 255, 255, 255, 255 },
                                     { 255, 255, 255, 255 },
                                     { 0, 0, 0, 255 }
                                   };

void formatTime(int& x, std::string& X);
void formatTime(tm* tmL, std::string& hour, std::string& min, std::string& sec);
void updateTime(int& min, std::string& min_s, int& sec, std::string& sec_s, std::string& actualT);
void updateTime(time_t& currentT, std::string& actualT);
void updateColor(Grid& g, int colorCounter, bool bg = true);
void formatChrono(double time, std::string& txt, double diff = 0);

int main(int argv, char* args[]) {
    SDL_Window* win;
    SDL_Renderer* renderer;
    TTF_Font* font;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

    win = SDL_CreateWindow("Clock", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        std::cout << "Window could not be created. SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
    if (TTF_Init()) {
        std::cout << "TTF could not be initialized. TTF_Error" << SDL_GetError() << std::endl;
        return 1;
    }

    font = TTF_OpenFont("res/font/Inter_24pt-SemiBold.ttf", 44);
    if (!font) {
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
        return 1;
    }

    SDL_SetWindowAlwaysOnTop(win, SDL_TRUE);
    SDL_SetWindowBordered(win, SDL_FALSE);
    SDL_SetWindowOpacity(win, 0.9);


    time_t currentTime;
    time(&currentTime);

    tm* tmLocal = localtime(&currentTime);

    std::string actualTime = std::to_string(tmLocal->tm_hour) + ":" + std::to_string(tmLocal->tm_min) + ":" + std::to_string(tmLocal->tm_sec);
    
    int colorCounter = 0;

    Grid gridClock(MARGIN, MARGIN, 200, SCREEN_HEIGHT - 2 * MARGIN);
    Button clockLabel(actualTime.c_str(), font, TEXTCOLOR.at(colorCounter));
    gridClock.add(clockLabel);
    gridClock.setMargin(MARGIN);
    gridClock.updateDistribution(renderer);

    Grid gridMenu(200 + 2 * MARGIN, MARGIN, 35, MARGIN + 35 * 2);
    Button quitBt(0, 0, 35, 35, "res/assets/close.png");
    Button menuBt(0, 0, 35, 35, "res/assets/menu.png");
    gridMenu.add(quitBt);
    gridMenu.add(menuBt);
    gridMenu.setMargin(MARGIN);
    gridMenu.setHidden(false);
    gridMenu.updateDistribution(renderer);

    Grid gridConfig(MARGIN, MARGIN, 200, SCREEN_HEIGHT - 2 * MARGIN);
    Button moveBt(0, 0, 35, 35, "res/assets/move.png");
    Button colorBt(0, 0, 35, 35, "res/assets/color.png");
    Button timerBt(0, 0, 35, 35, "res/assets/timer.png");
    gridConfig.add(moveBt);
    gridConfig.add(colorBt);
    gridConfig.add(timerBt);
    gridConfig.setMargin(MARGIN);
    gridConfig.setHidden(true);
    gridConfig.updateDistribution(renderer);

    std::string chronoLabelTxt = "00:00";
    clock_t startTime = 0, elapsedTime = 0, diff = 0, diffAcumulative = 0;
    bool chronoStarted = false;

    Grid gridChrono1(MARGIN, MARGIN, 200 - 35 - 2 * MARGIN, SCREEN_HEIGHT - 2 * MARGIN);
    Button chronoLabel(chronoLabelTxt.c_str(), font, TEXTCOLOR.at(colorCounter));
    gridChrono1.add(chronoLabel);
    gridChrono1.setMargin(MARGIN);
    gridChrono1.setHidden(true);
    gridChrono1.updateDistribution(renderer);

    Grid gridChrono2(200 - 35, MARGIN, 35, SCREEN_HEIGHT - 2 * MARGIN);
    Button playTimeBt(0, 0, 35, 35, "res/assets/play.png");
    Button resetTimeBt(0, 0, 35, 35, "res/assets/reset.png");
    gridChrono2.add(playTimeBt);
    gridChrono2.setMargin(MARGIN);
    gridChrono2.setHidden(true);
    gridChrono2.updateDistribution(renderer);
    

    bool mouseState = false;
    bool isRunning = true;
    while (isRunning) {
        const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

        SDL_Event event;
        POINT mouse; // mouse location relative to the entire screen
        SDL_Point mousePos; // mouse location relative to the window
        SDL_Point winPos;

        SDL_SetRenderDrawColor(renderer, BACKGROUND.at(colorCounter).r, BACKGROUND.at(colorCounter).g, BACKGROUND.at(colorCounter).b, BACKGROUND.at(colorCounter).a);
        SDL_RenderClear(renderer);


        updateTime(currentTime, actualTime);
        clockLabel.setText(actualTime.c_str());
        clockLabel.setTextColor(TEXTCOLOR.at(colorCounter));

        updateColor(gridClock, colorCounter, false);
        gridClock.updateDistribution(renderer);

        gridMenu.setHidden(false);
        updateColor(gridMenu, colorCounter);
        gridMenu.updateDistribution(renderer);

        updateColor(gridConfig, colorCounter);
        gridConfig.updateDistribution(renderer);

        if(chronoStarted) elapsedTime = clock();
        formatChrono(static_cast<double>((elapsedTime - startTime) / CLOCKS_PER_SEC), chronoLabelTxt, static_cast<double>((diffAcumulative) / CLOCKS_PER_SEC));
        
        updateColor(gridChrono1, colorCounter, false);
        gridChrono1.updateDistribution(renderer);

        updateColor(gridChrono2, colorCounter);
        gridChrono2.updateDistribution(renderer);


        SDL_PollEvent(&event);
        SDL_GetMouseState(&mousePos.x, &mousePos.y);
        if (event.type == SDL_QUIT) isRunning = false;

        if (event.type == SDL_MOUSEBUTTONUP) {
            if (quitBt.isHovered(mousePos.x, mousePos.y)) isRunning = false;

            if (menuBt.isHovered(mousePos.x, mousePos.y)) {
                if (!gridConfig.isHidden()) {
                    gridClock.setHidden(false);
                    gridConfig.setHidden(true);
                    gridChrono1.setHidden(true);
                    gridChrono2.setHidden(true);
                }

                else if (!gridClock.isHidden()) {
                    gridClock.setHidden(true);
                    gridConfig.setHidden(false);
                    gridChrono1.setHidden(true);
                    gridChrono2.setHidden(true);
                }

                else if (!gridChrono1.isHidden()) {
                    gridClock.setHidden(true);
                    gridConfig.setHidden(false);
                    gridChrono1.setHidden(true);
                    gridChrono2.setHidden(true);
                }

                if(!gridConfig.isHidden() || !gridChrono1.isHidden()) menuBt.setImgSrc("res/assets/undo.png");
                else menuBt.setImgSrc("res/assets/menu.png");
            }

            if (colorBt.isHovered(mousePos.x, mousePos.y)) {
                if (colorCounter < BACKGROUND.size()) colorCounter++;
                if (colorCounter >= BACKGROUND.size()) colorCounter = 0;
            }

            if (timerBt.isHovered(mousePos.x, mousePos.y)) {
                gridClock.setHidden(true);
                gridConfig.setHidden(true);
                gridChrono1.setHidden(false);
                gridChrono2.setHidden(false);

                startTime = elapsedTime = 0;
                chronoStarted = false;

                if (gridChrono2.getBtList().size() == 2) gridChrono2.removeLast();
                playTimeBt.setImgSrc("res/assets/play.png");
            }

            if (resetTimeBt.isHovered(mousePos.x, mousePos.y)) {
                startTime = elapsedTime = clock();
                diffAcumulative = 0;
                chronoStarted = false;

                if (gridChrono2.getBtList().size() == 2) gridChrono2.removeLast();
                playTimeBt.setImgSrc("res/assets/play.png");
            }

            if (playTimeBt.isHovered(mousePos.x, mousePos.y)) {
                chronoStarted = !chronoStarted;

                if (!chronoStarted) {
                    playTimeBt.setImgSrc("res/assets/play.png");

                    diff = elapsedTime - startTime;
                }

                else {
                    playTimeBt.setImgSrc("res/assets/pause.png");

                    if (elapsedTime == startTime) {
                        if (gridChrono2.getBtList().size() == 1) gridChrono2.add(resetTimeBt);

                        startTime = clock();
                    }

                    else {
                        startTime = clock();
                        diffAcumulative += diff;
                        diff = 0;
                    }
                }
            }

            mouseState = false;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (moveBt.isHovered(mousePos.x, mousePos.y)) {
                mouseState = true;
            }
        }

        if (mouseState) {
            GetCursorPos(&mouse);
            SDL_GetWindowPosition(win, &winPos.x, &winPos.y);
            SDL_SetWindowPosition(win, mouse.x - moveBt.getPosx() - moveBt.getWidth() / 2, mouse.y - moveBt.getPosy() - moveBt.getHeight() / 2);
        }

        if (keyboard_state[SDL_SCANCODE_ESCAPE]) isRunning = false;

        else if (keyboard_state[SDL_SCANCODE_M]) {
            GetCursorPos(&mouse);
            SDL_GetWindowPosition(win, &winPos.x, &winPos.y);
            SDL_SetWindowPosition(win, mouse.x - SCREEN_WIDTH / 2, mouse.y - SCREEN_HEIGHT / 2);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    TTF_Quit();
    SDL_Quit();
    return 0;
}

void formatTime(int& x, std::string& X) {
    if (x < 10) X = "0" + std::to_string(x);
    else X = std::to_string(x);
}

void formatTime(tm* tmL, std::string& hour, std::string& min, std::string& sec) {
    hour = std::to_string(tmL->tm_hour);
    min = std::to_string(tmL->tm_min);
    sec = std::to_string(tmL->tm_sec);

    formatTime(tmL->tm_hour, hour);
    formatTime(tmL->tm_min, min);
    formatTime(tmL->tm_sec, sec);
}

void updateTime(int& min, std::string& min_s, int& sec, std::string& sec_s, std::string& actualT) {
    formatTime(min, min_s);
    formatTime(sec, sec_s);

    actualT = min_s + ":" + sec_s;
}

void updateTime(time_t& currentT,std::string& actualT) {
    time(&currentT);
    tm* tmLocal = localtime(&currentT);

    std::string hour, min, sec;
    formatTime(tmLocal, hour, min, sec);
    
    actualT = hour + ":" + min + ":" + sec;
}

void formatChrono(double time, std::string& txt, double diff) {
    time += diff;

    int min = static_cast<int>(time) / 60;
    int sec = static_cast<int>(time) % 60;

    std::string min_s, sec_s;
    updateTime(min, min_s, sec, sec_s, txt);
}

void updateColor(Grid& g, int colorCounter, bool bg) {
    std::vector<std::reference_wrapper<Button>> btList = g.getBtList();

    for (Button& bt : btList) {
        if (bg) bt.setBackgroundColor(COLORBGBT.at(colorCounter), true);
        bt.setTextColor(TEXTCOLOR.at(colorCounter));
    }
}
