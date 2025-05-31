#include"memo.h"
#include<locale>

using namespace std;



void Menu(SDL_Renderer* renderer, TTF_Font*font, bool& musicOn,bool& soundOn,bool& exitGame) {
	DrawBackground(renderer, "assets/backgrounds/menu.jpg");
	int selected = 0;
	SDL_Event event;
	bool inMenu = true;

	while (inMenu) {
		string opt1 = "Start";
		string opt2 = musicOn ? "Music: ON" : "Music:OFF";
		string opt3 = soundOn ? "Sound: ON" : "Sound: OFF";
		string opt4 = "Leave Game";
		const char* options[] = {
			opt1.c_str(),
			opt2.c_str(),
			opt3.c_str(),
			opt4.c_str(),
		};

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				exitGame = true;
				return;
			}
			if(event.type == SDL_KEYDOWN){
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					selected = (selected - 1 + 4) % 4; break;   
				case SDLK_DOWN:
					selected = (selected + 1) % 4; break;
				case SDLK_RETURN:
					switch (selected) {
					case 0:
						return;
					case 1:
						musicOn = !musicOn;
						if (musicOn) {
							Mix_ResumeMusic(); }
						else {
							Mix_PauseMusic(); }
						break;
					case 2:
						soundOn = !soundOn; break;
					case 3:
						exitGame = true;
						return;
					}
					break;

				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		SDL_Color color = { 0,0,0 };
		for (int i = 0; i < 4; i++) {
			SDL_Surface* surface = TTF_RenderUTF8_Blended(font, options[i], color);
			SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_Rect rect = {
				(SCREEN_WIDTH - surface->w) / 2,
				200 + i * 60,
				surface->w,
				surface->h
			};

			if (i == selected) {
				SDL_SetRenderDrawColor(renderer, 180, 200, 255, 255);
				SDL_RenderFillRect(renderer, &rect);
			}
			SDL_RenderCopy(renderer, text, NULL, &rect);
			SDL_FreeSurface(surface);
			SDL_DestroyTexture(text);
		}
		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}
}


int VyborCarty(SDL_Renderer* renderer, TTF_Font* font) {
	DrawBackground(renderer, "assets/backgrounds/menu.jpg");
	setlocale(LC_ALL, "RUS");
	const char* vybor[] = { "3 x 2","4 x 3","4 x 5","5 x 6" };
	int cardCol[] = { 6,12,20,30 };

	int selectIndex = 0;
	bool quit = false;
	SDL_Event event;

	SDL_Color textColor = { 0,0,0 };
	SDL_Texture* textures[4];//вот тут я не совсем уверена
	SDL_Rect rects[4];

	int spacing = 20;
	int verticalOffset = 50;
	for (int i = 0; i < 4; i++) {
		SDL_Surface* surface = TTF_RenderUTF8_Blended(font, vybor[i], textColor);
		textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
		rects[i].x = (SCREEN_WIDTH - surface->w) / 2;
		rects[i].y = (SCREEN_HEIGHT - (3 * surface->h + 2 * spacing)) / 2 - verticalOffset + i * (surface->h + spacing);
		rects[i].w = surface->w;
		rects[i].h = surface->h;
		SDL_FreeSurface(surface);
	}

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				return 1;
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					selectIndex = (selectIndex - 1 + 4) % 4; break;

				case SDLK_DOWN:
					selectIndex = (selectIndex + 1) % 4; break;
				case SDLK_RETURN:
					quit = true; break;
				}
			}
		}


		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		for (int i = 0; i < 4; ++i) {
			if (i == selectIndex)
				SDL_SetRenderDrawColor(renderer, 180, 220, 255, 255);
			else
				SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);

			SDL_RenderFillRect(renderer, &rects[i]);
			SDL_RenderCopy(renderer, textures[i], NULL, &rects[i]);
		}
		SDL_RenderPresent(renderer);
	}
	for (int i = 0; i < 4; ++i) {
		SDL_DestroyTexture(textures[i]);
	}
	return cardCol[selectIndex];
}

int VyborSloznosti(SDL_Renderer* renderer, TTF_Font* font, int cardCol) {
	DrawBackground(renderer, "assets/backgrounds/menu.jpg");
	const char* vybor[] = { "Easy level","Normal level","Hard level" };
	int indexVybora = 0;
	bool quit = false;
	SDL_Event event;

	int easy = 0, normal = 0, hard = 0;
	switch (cardCol) {
	case 6: easy = 120; normal = 90; hard = 60; break;
	case 12: easy = 180; normal = 120; hard = 90; break;
	case 20: easy = 210; normal = 150; hard = 90; break;
	case 30: easy = 180; normal = 120; hard = 90; break;
	}

	int times[] = { easy,normal,hard };

	SDL_Color textColor = { 0,0,0 };
	SDL_Texture* textures[3];
	SDL_Rect rects[3];
	int verticalOffset = 50;
	for (int i = 0; i < 3; i++) {
		int spacing = 20;
		SDL_Surface* surface = TTF_RenderUTF8_Blended(font, vybor[i], textColor);
		textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
		rects[i].x = (SCREEN_WIDTH - surface->w) / 2;
		rects[i].y = (SCREEN_HEIGHT - (3 * surface->h + 2 * spacing)) / 2 - verticalOffset + i * (surface->h + spacing);
		rects[i].w = surface->w;
		rects[i].h = surface->h;
		SDL_FreeSurface(surface);
	}

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return -1;
			}
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					indexVybora = (indexVybora - 1 + 3) % 3; break;
				case SDLK_DOWN:
					indexVybora = (indexVybora + 1) % 3; break;
				case SDLK_RETURN:
					quit = true; break;
				}
			}
		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		for (int i = 0; i < 3; ++i) {
			if (i == indexVybora)
				SDL_SetRenderDrawColor(renderer, 180, 255, 180, 255);
			else
				SDL_SetRenderDrawColor(renderer, 220, 220, 200, 255);
			SDL_RenderFillRect(renderer, &rects[i]);
			SDL_RenderCopy(renderer, textures[i], NULL, &rects[i]);
		}
		SDL_RenderPresent(renderer);
	}
	for (int i = 0; i < 3; ++i) {
		SDL_DestroyTexture(textures[i]);
	}
	return times[indexVybora];
}

void CartochnoePole(Card cards[], int cardCol, int screenW, int screenH,SDL_Renderer* renderer) {
	DrawBackground(renderer, "background/bg6.jpg");
	int pairIDS[30];
	for (int i = 0; i < cardCol / 2; ++i) {
		pairIDS[2 * i] = i + 1;
		pairIDS[2 * i + 1] = i + 1;
	}

	srand((unsigned int)time(NULL));
	for (int i = cardCol - 1; i > 0; --i) {
		int j = rand() % (i + 1);
		int temp = pairIDS[i];
		pairIDS[i] = pairIDS[j];
		pairIDS[j] = temp;
	}

	for (int i = 0; i < cardCol; ++i) {
		cards[i].id = pairIDS[i];
		cards[i].Perevernuta = false;
		cards[i].Naydena = false;
	}
	int rows = 1, cols = cardCol;
	if (cardCol == 6) { rows = 2; cols = 3; }
	if (cardCol == 12) { rows = 3; cols = 4; }
	if (cardCol == 20) { rows = 4; cols = 5; }
	if (cardCol == 30) { rows = 5; cols = 6; }

	int padding = 10;
	int cardW = (SCREEN_WIDTH - (cols + 1) * padding) / cols;
	int cardH = (SCREEN_HEIGHT - 80 - (rows + 1) * padding) / rows;

	for (int i = 0; i < cardCol; ++i) {
		int row = i / cols;
		int col = i % cols;
		cards[i].rect.x = padding + col * (cardW + padding);
		cards[i].rect.y = 80 + padding + row * (cardH + padding);
		cards[i].rect.w = cardW;
		cards[i].rect.h = cardH;
	}
}

void DrawCards(SDL_Renderer* renderer, Card cards[], int cardCol, int selectedIndex, SDL_Texture* textures[], SDL_Texture* backTexture) {
	for (int i = 0; i < cardCol; ++i) {
		if (cards[i].Naydena) {
			SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Найденная пара
			SDL_RenderFillRect(renderer, &cards[i].rect);
		}
		else if (cards[i].Perevernuta)
			SDL_RenderCopy(renderer, textures[cards[i].id - 1], NULL, &cards[i].rect);
			//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Открыта
		else {
			if (backTexture) {
				SDL_RenderCopy(renderer, backTexture, NULL, &cards[i].rect);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // fallback
				SDL_RenderFillRect(renderer, &cards[i].rect);
			}
		}


		if (i == selectedIndex) {
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Подсветка
			SDL_RenderDrawRect(renderer, &cards[i].rect);
		}
	}
}

void FormatTime(int seconds, char* buffer, int bufferSize) {
	int minutes = seconds / 60;
	int secs = seconds % 60;
	snprintf(buffer, bufferSize, "%02d:%02d", minutes, secs);
}


void DrawUI(SDL_Renderer* renderer, TTF_Font* font, int timeLeft, int score) {
	SDL_Rect panel = { 0, 0, SCREEN_WIDTH, 80 };
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
	SDL_RenderFillRect(renderer, &panel);

	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	SDL_Rect timerBox = { 50, 20, 200, 40 };
	SDL_Rect scoreBox = { 420, 20, 200, 40 };
	SDL_RenderFillRect(renderer, &timerBox);
	SDL_RenderFillRect(renderer, &scoreBox);

	// === Вывод текста ===
	SDL_Color textColor = { 0, 0, 0 }; // чёрный

	// Время
	char timeBuffer[10];
	FormatTime(timeLeft, timeBuffer, sizeof(timeBuffer));
	char fullTimeText[50];
	snprintf(fullTimeText, sizeof(fullTimeText), "Time: %s", timeBuffer);
	SDL_Surface* timeSurface = TTF_RenderUTF8_Blended(font, fullTimeText, textColor);
	SDL_Texture* timeTexture = SDL_CreateTextureFromSurface(renderer, timeSurface);
	SDL_Rect timeTextRect = { timerBox.x + 10, timerBox.y + 5, timeSurface->w, timeSurface->h };
	SDL_RenderCopy(renderer, timeTexture, NULL, &timeTextRect);
	SDL_FreeSurface(timeSurface);
	SDL_DestroyTexture(timeTexture);

	// Очки
	char scoreText[50];
	snprintf(scoreText, sizeof(scoreText), "Score: %d", score);
	SDL_Surface* scoreSurface = TTF_RenderUTF8_Blended(font, scoreText, textColor);
	SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
	SDL_Rect scoreTextRect = { scoreBox.x + 10, scoreBox.y + 5, scoreSurface->w, scoreSurface->h };
	SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreTextRect);
	SDL_FreeSurface(scoreSurface);
	SDL_DestroyTexture(scoreTexture);
}

void DrawBackground(SDL_Renderer* renderer, const char* path) {
	SDL_Texture* bg = IMG_LoadTexture(renderer, path);
	if (bg) {
		SDL_RenderCopy(renderer, bg, NULL, NULL);
		SDL_DestroyTexture(bg);
	}
}

bool ShowEndScreen(SDL_Renderer* renderer, TTF_Font* font, const char* message) {
	DrawBackground(renderer, "assets/backgrounds/menu.jpg");
	const char* options[] = { "Menu", "Leave game" };
	int selected = 0;

	bool running = true;
	SDL_Event e;

	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) return false;

			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_UP: selected = (selected - 1 + 2) % 2; break;
				case SDLK_DOWN: selected = (selected + 1) % 2; break;
				case SDLK_RETURN:
					return selected == 0; // true = главное меню, false = выход
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Заголовок: Победа/Поражение
		SDL_Color color = { 255, 255, 255 };
		SDL_Surface* titleSurf = TTF_RenderUTF8_Blended(font, message, color);
		SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, titleSurf);
		SDL_Rect titleRect = { (SCREEN_WIDTH - titleSurf->w) / 2, 150, titleSurf->w, titleSurf->h };
		SDL_RenderCopy(renderer, titleTex, NULL, &titleRect);
		SDL_FreeSurface(titleSurf);
		SDL_DestroyTexture(titleTex);

		// Кнопки
		for (int i = 0; i < 2; ++i) {
			SDL_Color btnColor = (i == selected) ? SDL_Color{ 100, 255, 100 } : SDL_Color{ 200, 200, 200 };
			SDL_Surface* optSurf = TTF_RenderUTF8_Blended(font, options[i], color);
			SDL_Texture* optTex = SDL_CreateTextureFromSurface(renderer, optSurf);
			SDL_Rect optRect = { (SCREEN_WIDTH - optSurf->w) / 2, 300 + i * 60, optSurf->w, optSurf->h };

			SDL_SetRenderDrawColor(renderer, btnColor.r, btnColor.g, btnColor.b, 255);
			SDL_RenderFillRect(renderer, &optRect);
			SDL_RenderCopy(renderer, optTex, NULL, &optRect);

			SDL_FreeSurface(optSurf);
			SDL_DestroyTexture(optTex);
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	return false;
}


bool PauseMenu(SDL_Renderer* renderer, TTF_Font* font, bool& musicOn, bool& soundOn, bool& exitGame) {
	DrawBackground(renderer, "assets/backgrounds/menu.jpg");
	int selected = 0;
	SDL_Event event;
	bool inPause = true;

	while (inPause) {
		// Обновляем опции каждый кадр
		string opt1 = "Continue";
		string opt2 = musicOn ? "Music: ON" : "Music: OFF";
		string opt3 = soundOn ? "Sound: ON" : "Sound: OFF";
		string opt4 = "Leave Game";
		const char* options[] = {
			opt1.c_str(),
			opt2.c_str(),
			opt3.c_str(),
			opt4.c_str(),			
		};

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				exitGame = true;
				return false;
			}
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_UP: selected = (selected - 1 + 4) % 4; break;
				case SDLK_DOWN: selected = (selected + 1) % 4; break;
				case SDLK_RETURN:
					switch (selected) {
					case 0: return true; // Continue
					case 1:
						musicOn = !musicOn;
						if (musicOn) Mix_ResumeMusic();
						else Mix_PauseMusic();
						break;
					case 2:
						soundOn = !soundOn;
						break;
					case 3:
						exitGame = true;
						return false;
					}
					break;
				case SDLK_ESCAPE:
					return true;
				}
			}
		}

		// Рисуем меню
		SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
		SDL_RenderClear(renderer);

		SDL_Color color = { 255, 255, 255 };

		for (int i = 0; i < 4; i++) {
			SDL_Surface* surface = TTF_RenderUTF8_Blended(font, options[i], color);
			SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_Rect rect = {
				(SCREEN_WIDTH - surface->w) / 2,
				200 + i * 60,
				surface->w,
				surface->h
			};

			if (i == selected) {
				SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
				SDL_RenderFillRect(renderer, &rect);
			}

			SDL_RenderCopy(renderer, text, NULL, &rect);
			SDL_FreeSurface(surface);
			SDL_DestroyTexture(text);
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	return true;
}
