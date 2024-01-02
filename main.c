#include <math.h>

#include "SDL2/SDL.h"

#define PI 3.1415926535

float px, py, p_d_x, p_d_y, p_a;

void init()
{
    px = 300;
    py = 300;
    p_d_x = cos(p_a) * 5;
    p_d_y = sin(p_a) * 5;
}

// The player will be represented by a little yellow rectangle
void draw_player(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect player = {px, py, 8, 8};
    SDL_RenderFillRect(renderer, &player);

    // Calculate the center of the player rectangle
    float player_center_x = px + player.w / 2.0;
    float player_center_y = py + player.h / 2.0;

    // Calculate the endpoint of the line in front of the player
    float line_length = 20;  // You can adjust the length of the line
    float line_end_x = player_center_x + line_length * cos(p_a);
    float line_end_y = player_center_y + line_length * sin(p_a);

    // Draw the line in front of the player
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Red color for the line
    SDL_RenderDrawLine(renderer, player_center_x, player_center_y, line_end_x, line_end_y);
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


void draw_rays_3D(SDL_Renderer *renderer)
{
    int r, m_x, m_y, m_p, d_o_f;
    float r_x, r_y, r_a, x_o, y_o;

    r_a = p_a;
    for (r = 0; r < 1; r++)
    {
        d_o_f = 0;
        float a_tan = -1 / tan(r_a);
        if (r_a > PI)
        {
            r_y = ( ( (int)py >> 6) << 6) - 0.0001;
            r_x = (py - r_y) * a_tan + px;
            y_o = -64;
            x_o = -y_o * a_tan;
        }
        if (r_a < PI)
        {
            r_y = ( ( (int)py >> 6) << 6) + 64;
            r_x = (py - r_y) * a_tan + px;
            y_o = 64;
            x_o = -y_o * a_tan;
        }
        if (r_a == 0 || r_a == PI)
        {
            r_x = px;
            r_y = py;
            d_o_f = 8;
        }
        while ( d_o_f < 8 )
        {
            m_x = (int) (r_x) >> 6;
            m_y = (int) (r_y) >> 6;
            m_p = m_y * map_x + m_x;
            if ( m_p < map_x * map_y && map[m_p] == 1)
            {
                d_o_f = 8;
            }
            else
            {
                r_x += x_o;
                r_y += y_o;
                d_o_f += 1;
            }

        }
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Red color for the line
        SDL_RenderDrawLine(renderer, px, py, r_x, r_y);
    }
}



void render_screen(SDL_Renderer *renderer)
{
    // Render a white screen
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);

    SDL_RenderClear(renderer);

    draw_2D_map(renderer);

    draw_player(renderer);

    draw_rays_3D(renderer);

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
                        px += p_d_x;
                        py += p_d_y;
                        break;
                    }
                    case SDLK_a:
                    {
                        p_a -= 0.1;
                        if (p_a < 0)
                        {
                            p_a += (2 * PI);
                        }
                        p_d_x = cos(p_a) * 5;
                        p_d_y = sin(p_a) * 5;
                        break;
                    }
                    case SDLK_s:
                    {
                        px -= p_d_x;
                        py -= p_d_y;
                        break;
                    }
                    case SDLK_d:
                    {
                        p_a += 0.1;
                        if (p_a > (2 * PI))
                        {
                            p_a -= (2 * PI);
                        }
                        p_d_x = cos(p_a) * 5;
                        p_d_y = sin(p_a) * 5;
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