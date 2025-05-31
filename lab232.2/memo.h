#pragma once
#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include <SDL_image.h>
#include <iostream>
#include  <cstdlib>
#include <ctime>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 770;
#define MAX_SIZE 30
void Menu(SDL_Renderer* renderer, TTF_Font* font, bool& musicOn, bool& soundOn, bool& exitGame);

int VyborCarty(SDL_Renderer* renderer, TTF_Font* font);
int VyborSloznosti(SDL_Renderer* renderer, TTF_Font* font, int cardCol);

typedef struct {
	int id;
	bool Perevernuta;
	bool Naydena;
	SDL_Rect rect;
}Card;


void DrawCards(SDL_Renderer* renderer, Card cards[], int cardCol, int selectedIndex, SDL_Texture* textures[], SDL_Texture* backTexture);
void CartochnoePole(Card cards[], int cardCol, int screenW, int screenH, SDL_Renderer* renderer);
void FormatTime(int seconds, char* buffer, int bufferSize);
void DrawUI(SDL_Renderer* renderer, TTF_Font* font, int timeLeft, int score);


bool ShowEndScreen(SDL_Renderer* renderer, TTF_Font* font, const char* message);

bool PauseMenu(SDL_Renderer* renderer, TTF_Font* font, bool& musicOn, bool& soundOn, bool& gameRunning);
#endif
