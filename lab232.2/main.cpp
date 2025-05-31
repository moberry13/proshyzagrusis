#include "memo.h"
#include<locale.h>



#define MAX_SIZE 30

int main(int argc, char* argv[]) {

    setlocale(LC_ALL, "RUS");
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_Init(0);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 256);

    TTF_Font* font = TTF_OpenFont("Roboto-Regular.ttf", 24);

    Mix_Music* backgroundSound = Mix_LoadMUS("NewJeans - ASAP.mp3");
    Mix_Chunk* flipSound = Mix_LoadWAV("perevorot.ogg");
    Mix_Chunk* matchSound = Mix_LoadWAV("matchSound.WAV");



    Mix_VolumeMusic(30); // фоновая музыка потише
    Mix_VolumeChunk(flipSound, 128); // эффекты погромче
    Mix_VolumeChunk(matchSound, 128);

    SDL_Window* window = SDL_CreateWindow(u8"Мемо",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* cardTextures[15];

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Texture* backTexture = NULL;
    SDL_Surface* backSurface = IMG_Load("backcard.jpg");
    if (!backSurface) {
        printf_s("Error with card back", IMG_GetError());
    }
    else {
        backTexture = SDL_CreateTextureFromSurface(renderer, backSurface);
        SDL_FreeSurface(backSurface);
    }



    bool musicOn = true;
    bool soundOn = true;

    bool gameRunning = true;
    while (gameRunning) { 
        Mix_PlayMusic(backgroundSound, -1); // играет бесконечно

        bool exitGameFromMenu = false;
        Menu(renderer, font, musicOn, soundOn, exitGameFromMenu);
        if (exitGameFromMenu) break;

        int cardCol = VyborCarty(renderer, font);
        int timeLimit = VyborSloznosti(renderer, font, cardCol);

        int score = 0;
        int timeLeft = timeLimit; 

        char filename[100];
        const char* folder = NULL;
        switch (cardCol) {
        case 6: folder = "card_6/";break;
        case 12: folder = "card_12"; break;
        case 20: folder = "card_20"; break;
        case 30: folder = "card_30"; break;
        }
        snprintf(filename, sizeof(filename), "%s/%d.jpg", folder, i + 1);

        for (int i = 0; i < cardCol / 2; ++i) {
           
            snprintf(filename, sizeof(filename), "%d.jpg", i + 1);
            SDL_Surface* surface = IMG_Load(filename);
            if (!surface) {
                printf("Ошибка загрузки %s: %s\n", i + 1, IMG_GetError());
                exit(1);
            }

            cardTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }


        Card cards[30];
        CartochnoePole(cards, cardCol, SCREEN_WIDTH, SCREEN_HEIGHT);
        bool running = true;
        SDL_Event e;
        int selected = 0;

        Uint32 startTicks = SDL_GetTicks(); // старт времени
        Uint32 lastSecond = startTicks;

        int firstOpen = -1;
        int secondOpen = -1;
        Uint32 revealTime = 0;
        bool pauseBeforeHide = false;

        while (running) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT)
                    running = false;

                if (e.type == SDL_KEYDOWN) {
                    int cols = (cardCol == 6) ? 3 : (cardCol == 12) ? 4 :
                        (cardCol == 20) ? 5 : 6;

                    switch (e.key.keysym.sym) {
                    case SDLK_LEFT:  if (selected % cols > 0) selected--; break;
                    case SDLK_RIGHT: if (selected % cols < cols - 1) selected++; break;
                    case SDLK_UP:    if (selected - cols >= 0) selected -= cols; break;
                    case SDLK_DOWN:  if (selected + cols < cardCol) selected += cols; break;
                    //case SDLK_ESCAPE: running = false; break;
                    case SDLK_RETURN:
                        if (!cards[selected].Perevernuta && !cards[selected].Naydena && !pauseBeforeHide) {
                            cards[selected].Perevernuta = true;
                            if (soundOn) {
                                Mix_PlayChannel(-1, flipSound, 0);
                            }
                            if (firstOpen == -1) {
                                firstOpen = selected;
                            }
                            else if (secondOpen == -1 && selected != firstOpen) {
                                secondOpen = selected;
                                pauseBeforeHide = true;
                                revealTime = SDL_GetTicks();
                            }
                        }
                        break;
                    case SDLK_ESCAPE:
                        bool exitGame = false;
                        bool resume = PauseMenu(renderer, font, musicOn, soundOn, exitGame);
                        if (exitGame) {
                            gameRunning = false;
                            running = false;
                        }
                        else if (!resume) {
                            running = false;
                        }
                        
                        break;
                   }
                }
            }

            Uint32 currentTicks = SDL_GetTicks();
            if (currentTicks - lastSecond >= 1000) { // прошла 1 секунда
                timeLeft--;
                lastSecond = currentTicks;

                if (timeLeft <= 0) {
                    bool allFound = true;
                    for (int i = 0; i < cardCol; ++i) {
                        if (!cards[i].Naydena) {
                            allFound = false;
                            break;
                        }
                    }

                    if (!allFound) {
                        bool restart = ShowEndScreen(renderer, font, "You lose!");
                        running = false;
                        if (!restart) gameRunning = false;
                        break;
                    }
                }

            }

            if (pauseBeforeHide && SDL_GetTicks() - revealTime > 1000) {
                if (cards[firstOpen].id == cards[secondOpen].id) {
                    cards[firstOpen].Naydena = true;
                    cards[secondOpen].Naydena = true;
                    score += 10;
                    if (soundOn) {
                        Mix_PlayChannel(-1, matchSound, 0);
                    }

                     
                }
                else {
                    cards[firstOpen].Perevernuta = false;
                    cards[secondOpen].Perevernuta = false;
                }

                firstOpen = -1;
                secondOpen = -1;
                pauseBeforeHide = false;

                bool allFound = true;
                for (int i = 0; i < cardCol; ++i) {
                    if (!cards[i].Naydena) {
                        allFound = false;
                        break;
                    }
                }

                bool restart = false;
                if (allFound) {
                    restart = ShowEndScreen(renderer, font, "You won!");
                    running = false;
                    if (!restart) gameRunning = false;
                    break;
                }

                                        
            }


            SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
            SDL_RenderClear(renderer);

            DrawUI(renderer, font, timeLeft, score);
            DrawCards(renderer, cards, cardCol, selected, cardTextures, backTexture);

            SDL_RenderPresent(renderer);
        }
        // После окончания игрового цикла
        for (int i = 0; i < cardCol / 2; ++i) {
            SDL_DestroyTexture(cardTextures[i]);
        }
      

    }

    Mix_FreeChunk(flipSound);
    Mix_FreeChunk(matchSound);
    Mix_FreeMusic(backgroundSound);
 

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_CloseFont(font);
    Mix_CloseAudio();
    TTF_Quit();

    return 0;
}
