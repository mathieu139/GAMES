#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WIDTH 1000
#define HEIGHT 750

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Rect circle_rect;
SDL_Rect moving_circle_rect;

bool game_active = false;
float current_score = 0;
float best_score = 0;
int pause_count = 0;
int perfect_score_count = 0;

void draw_circle(SDL_Renderer* renderer, int x, int y, int radius, int thickness, Uint8 r, Uint8 g, Uint8 b) {
    for (int w = 0; w < thickness; w++) {
        for (int i = 0; i < radius * 2; i++) {
            for (int j = 0; j < radius * 2; j++) {
                int dx = radius - i;
                int dy = radius - j;
                if ((dx*dx + dy*dy) <= (radius * radius)) {
                    SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                }
            }
        }
        radius--;
    }
}

void handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game_active = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_SPACE) {
                    if (game_active) {
                        game_active = false;
                        best_score = (best_score > current_score) ? best_score : current_score;
                        pause_count++;
                        if (current_score == 100) {
                            perfect_score_count++;
                        }
                    } else {
                        game_active = true;
                        current_score = 0;
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (game_active) {
                        game_active = false;
                        best_score = (best_score > current_score) ? best_score : current_score;
                        pause_count++;
                        if (current_score == 100) {
                            perfect_score_count++;
                        }
                    } else {
                        game_active = true;
                        current_score = 0;
                    }
                }
                break;
        }
    }
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Jeu du rond", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    circle_rect.x = WIDTH / 2;
    circle_rect.y = HEIGHT / 2;
    circle_rect.w = 2 * 30;
    circle_rect.h = 2 * 30;

    moving_circle_rect.x = WIDTH - 30 - 20;
    moving_circle_rect.y = HEIGHT / 2 - 20;
    moving_circle_rect.w = 2 * 20;
    moving_circle_rect.h = 2 * 20;

    game_active = false;
    current_score = 0;
    best_score = 0;
    pause_count = 0;
    perfect_score_count = 0;

    SDL_Event event;
    bool running = true;
    while (running) {
        handle_events();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        draw_circle(renderer, circle_rect.x + circle_rect.w / 2, circle_rect.y + circle_rect.h / 2, circle_rect.w / 2, 5, 0, 0, 0);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        draw_circle(renderer, circle_rect.x + circle_rect.w / 2, circle_rect.y + circle_rect.h / 2, circle_rect.w / 2 - 5, 0, 255, 0, 0);

        if (game_active) {
            moving_circle_rect.x += 10;
            if (moving_circle_rect.x + moving_circle_rect.w > WIDTH || moving_circle_rect.x < 0) {
                moving_circle_rect.x -= 10;
            }

            int distance = abs(moving_circle_rect.x + moving_circle_rect.w / 2 - circle_rect.x - circle_rect.w / 2);
            if (distance > circle_rect.w / 2 - 5 + moving_circle_rect.w / 2) {
                current_score = 0;
            } else {
                current_score = 100 - ((distance - (circle_rect.w / 2 - 5 - moving_circle_rect.w / 2)) / (2 * moving_circle_rect.w) * 100);
                current_score = (current_score > 0) ? current_score : 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &moving_circle_rect);

        char current_score_text[50];
        sprintf(current_score_text, "Score : %.2f%%", current_score);
        SDL_Color red = {255, 0, 0};
        SDL_Surface* current_score_surface = TTF_RenderText_Solid(font, current_score_text, red);
        SDL_Texture* current_score_texture = SDL_CreateTextureFromSurface(renderer, current_score_surface);
        SDL_Rect current_score_rect = {10, 10, current_score_surface->w, current_score_surface->h};
        SDL_RenderCopy(renderer, current_score_texture, NULL, &current_score_rect);
        SDL_FreeSurface(current_score_surface);
        SDL_DestroyTexture(current_score_texture);

        char best_score_text[50];
        sprintf(best_score_text, "Meilleur score : %.2f%%", best_score);
        SDL_Surface* best_score_surface = TTF_RenderText_Solid(font, best_score_text, red);
        SDL_Texture* best_score_texture = SDL_CreateTextureFromSurface(renderer, best_score_surface);
        SDL_Rect best_score_rect = {10, 50, best_score_surface->w, best_score_surface->h};
        SDL_RenderCopy(renderer, best_score_texture, NULL, &best_score_rect);
        SDL_FreeSurface(best_score_surface);
        SDL_DestroyTexture(best_score_texture);

        char pause_count_text[50];
        sprintf(pause_count_text, "Nombre d'éssais : %d", pause_count);
        SDL_Surface* pause_count_surface = TTF_RenderText_Solid(font, pause_count_text, red);
        SDL_Texture* pause_count_texture = SDL_CreateTextureFromSurface(renderer, pause_count_surface);
        SDL_Rect pause_count_rect = {10, 90, pause_count_surface->w, pause_count_surface->h};
        SDL_RenderCopy(renderer, pause_count_texture, NULL, &pause_count_rect);
        SDL_FreeSurface(pause_count_surface);
        SDL_DestroyTexture(pause_count_texture);

        char perfect_score_count_text[50];
        sprintf(perfect_score_count_text, "Scores parfaits : %d", perfect_score_count);
        SDL_Surface* perfect_score_count_surface = TTF_RenderText_Solid(font, perfect_score_count_text, red);
        SDL_Texture* perfect_score_count_texture = SDL_CreateTextureFromSurface(renderer, perfect_score_count_surface);
        SDL_Rect perfect_score_count_rect = {10, 130, perfect_score_count_surface->w, perfect_score_count_surface->h};
        SDL_RenderCopy(renderer, perfect_score_count_texture, NULL, &perfect_score_count_rect);
        SDL_FreeSurface(perfect_score_count_surface);
        SDL_DestroyTexture(perfect_score_count_texture);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


