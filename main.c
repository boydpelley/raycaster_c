#include "SDL2/SDL.h"

float px, py;

void init()
{
    px = 300;
    py = 300;
}

// The player will be represented by a little yellow rectangle
void draw_player(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect player = {px, py, 8, 8};
    SDL_RenderFillRect(renderer, &player);
}

int map_x = 8;
int map_y = 8;
int map_size = 64;
int map [] =
{
        1,1,1,1,1,1,1,1,
        1,0,1,0,0,0,0,1,
        1,0,1,0,0,0,0,1,
        1,0,1,0,0,0,0,1,
        1,0,0,0,0,0,0,1,
        1,0,0,0,0,1,0,1,
        1,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1
};

void draw_2D_map(SDL_Renderer *renderer)
{
    int x, y, x_o, y_o;
    for (y = 0; y < map_y; y++)
    {
        for (x = 0; x < map_x; x++)
        {
            if(map[y*map_y+x] == 1)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            x_o = x * map_size;
            y_o = y * map_size;
            SDL_Rect rect = {x_o, y_o, map_size, map_size};
            SDL_RenderFillRect(renderer, &rect);

            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
}


void render_screen(SDL_Renderer *renderer)
{
    // Render a white screen
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);

    SDL_RenderClear(renderer);

    draw_2D_map(renderer);

    draw_player(renderer);

    SDL_RenderPresent(renderer);
}


short process_events(SDL_Window * window)
{
    SDL_Event event;

    short done = 0;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_WINDOWEVENT_CLOSE:
            {
                if (window) {
                    SDL_DestroyWindow(window);
                    window = NULL;
                    done = 1;
                }
            }
            break;
            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    {
                        done = 1;
                        break;
                    }
                    case SDLK_w:
                    {
                        py -= 5;
                        break;
                    }
                    case SDLK_a:
                    {
                        px -= 5;
                        break;
                    }
                    case SDLK_s:
                    {
                        py += 5;
                        break;
                    }
                    case SDLK_d:
                    {
                        px += 5;
                        break;
                    }
                }
            }
            break;
            case SDL_QUIT:
            {
                done = 1;
            }
            break;
        }
    }
    return done;
}

int main(int argc, char *argv[])
{

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO);;
    init();

    window = SDL_CreateWindow("Raycaster Window",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              1024,
                              512,
                              0
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    short done = 0;

    while (!done)
    {
        done = process_events(window);

        render_screen(renderer);
    }
    return 0;
}