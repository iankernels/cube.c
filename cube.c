#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define WIDTH 480
#define HEIGHT 360
#define WINDOW_SCALE 2

float A, B, C;

float cubeWidth = 45;

float zBuffer[WIDTH * HEIGHT];
unsigned char buffer[WIDTH * HEIGHT];

int backgroundASCIICode = ' ';
float distanceFromCamera = 100.0f;

float horizontalOffset;
float K1 = 40.0f;

float incrementSpeed = 0.5f;

float x, y, z;
float ooz;

int xp, yp;
int idx;

float calculateX(float i, float j, float k) {
    return j * sinf(A) * sinf(B) * cosf(C) - k * cosf(A) * sinf(B) * cosf(C) +
           j * cosf(A) * sinf(C) + k * sinf(A) * sinf(C) + i * cosf(B) * cosf(C);
}

float calculateY(float i, float j, float k) {
    return j * cosf(A) * cosf(C) + k * sinf(A) * cosf(C) -
           j * sinf(A) * sinf(B) * sinf(C) + k * cosf(A) * sinf(B) * sinf(C) -
           i * cosf(B) * sinf(C);
}

float calculateZ(float i, float j, float k) {
    return k * cosf(A) * cosf(B) - j * sinf(A) * cosf(B) + i * sinf(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, unsigned char ch) {
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCamera;

    ooz = 1.0f / z;

    xp = (int)(WIDTH / 2.0f + horizontalOffset + K1 * ooz * x);
    yp = (int)(HEIGHT / 2.0f + K1 * ooz * y);

    idx = xp + yp * WIDTH;
    if (idx >= 0 && idx < WIDTH * HEIGHT) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init failed. %s\n", SDL_GetError());
        return 1;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_Window* window = SDL_CreateWindow("Cube",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH * WINDOW_SCALE, HEIGHT * WINDOW_SCALE, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!window) {
        printf("Window error. %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer) {
        printf("Renderer error. %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = 0;
        }

        memset(buffer, ' ', WIDTH * HEIGHT);
        memset(zBuffer, 0, sizeof(zBuffer));

        horizontalOffset = 0.0f;

        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed) {
                calculateForSurface(cubeX, cubeY, -cubeWidth, '@');
                calculateForSurface(cubeWidth, cubeY, cubeX, '$');
                calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
                calculateForSurface(-cubeX, cubeY, cubeWidth, '#');
                calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
                calculateForSurface(cubeX, cubeWidth, cubeY, '+');
            }
        }

        A += 0.03f;
        B += 0.02f;
        C += 0.01f;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < WIDTH * HEIGHT; i++) {
            if (buffer[i] != ' ') {
                int x = i % WIDTH;
                int y = i / WIDTH;

                switch (buffer[i]) {
                    case '@': SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break;
                    case '$': SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); break;
                    case '~': SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); break;
                    case '#': SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); break;
                    case ';': SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); break;
                    case '+': SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); break;
                    default: SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); break;
                }

                SDL_Rect rect = { x, y, 1, 1 };
                SDL_RenderFillRect(renderer, &rect);
            }
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 fps
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
