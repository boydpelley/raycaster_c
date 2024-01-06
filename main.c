#include <math.h>

#include "SDL2/SDL.h"

#include "Textures/All_Textures.ppm"

#define PI 3.1415926535
#define P2 (PI/2)
#define P3 (3*PI/2)
#define DR 0.0174533

typedef struct
{
    int w, a, s, d;
}button_keys; button_keys keys;

double px, py, p_d_x, p_d_y, p_a;

void init()
{
    px = 300;
    py = 300;
    p_d_x = cos(p_a) * 2;
    p_d_y = sin(p_a) * 2;
}


int map_x = 8;
int map_y = 8;
int map_size = 64;
int map_w [] =
        {
                1,1,1,1,1,2,1,1,
                1,0,0,1,0,0,0,1,
                3,0,0,2,0,0,0,1,
                3,0,0,1,0,2,0,1,
                1,3,4,2,0,0,0,1,
                2,0,0,0,0,1,0,1,
                1,0,0,0,0,0,0,1,
                1,2,1,3,1,1,1,1
        };

int mapF[]=          //floors
        {
                0,0,0,0,0,0,0,0,
                0,0,0,0,1,1,0,0,
                0,0,0,0,2,0,0,0,
                0,0,0,0,0,0,0,0,
                0,0,2,0,0,0,0,0,
                0,0,0,0,0,0,0,0,
                0,1,1,1,1,0,0,0,
                0,0,0,0,0,0,0,0,
        };

void updatePlayerPosition()
{
    int x_offset = (p_d_x < 0) ? -20 : 20;
    int y_offset = (p_d_y < 0) ? -20 : 20;
    int ipx = px / 64.0, ipx_add_xo = (px + x_offset)/64.0, ipx_sub_xo = (px - x_offset)/64.0;
    int ipy = py / 64.0, ipy_add_yo = (py + y_offset)/64.0, ipy_sub_yo = (py - y_offset)/64.0;

    if (keys.w)
    {
        if (map_w[(int)(ipy_add_yo) * map_x + (int)(ipx)] == 0) py += p_d_y;
        if (map_w[(int)(ipy) * map_x + (int)(ipx_add_xo)] == 0) px += p_d_x;
    }
    if (keys.a) {
        p_a -= 0.1;
        if (p_a < 0) {
            p_a += (2 * PI);
        }
        p_d_x = cos(p_a) * 2;
        p_d_y = sin(p_a) * 2;
    }
    if (keys.s)
    {
        if (map_w[(int)(ipy_sub_yo) * map_x + (int)(ipx)] == 0) py -= p_d_y;
        if (map_w[(int)(ipy) * map_x + (int)(ipx_sub_xo)] == 0) px -= p_d_x;
    }
    if (keys.d) {
        p_a += 0.1;
        if (p_a > (2 * PI)) {
            p_a -= (2 * PI);
        }
        p_d_x = cos(p_a) * 2;
        p_d_y = sin(p_a) * 2;
    }
}

void draw_2D_map(SDL_Renderer *renderer)
{
    int x, y, x_o, y_o;
    for (y = 0; y < map_y; y++)
    {
        for (x = 0; x < map_x; x++)
        {
            if(map_w[y*map_y+x] > 0)
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

double dist(double a_x, double a_y, double b_x, double b_y, double c)
{
    c = ( sqrt( (b_x - a_x) * (b_x - a_x) + (b_y - a_y) * (b_y - a_y) ) );
    return c;
}

void draw_rays_3D(SDL_Renderer *renderer)
{
    int r, m_x, m_y, m_p, d_o_f;
    double r_x, r_y, r_a, x_o, y_o;

    r_a = p_a - DR * 30;
    if (r_a < 0)
    {
        r_a += 2 * PI;
    }
    if (r_a > 2 * PI)
    {
        r_a -= 2 * PI;
    }

    for (r = 0; r < 60; r++)
    {
        int vmt = 0, hmt = 0; // For textures

        d_o_f = 0;
        double dis_h = 1000000, hx = px, hy = py, dis_t;
        double a_tan = -1 / tan(r_a);

        r_y = floor(py / map_size) * map_size + (r_a > PI ? -0.0001 : map_size);
        r_x = (py - r_y) * a_tan + px;
        y_o = (r_a > PI) ? -map_size : map_size;
        x_o = -y_o * a_tan;

        while (d_o_f < 8)
        {
            m_x = (int)(r_x) / map_size;
            m_y = (int)(r_y) / map_size;
            m_p = m_y * map_x + m_x;

            if (m_p > 0 && m_p < map_x * map_y && map_w[m_p] > 0)
            {
                hmt = map_w[m_p] - 1;
                hx = r_x;
                hy = r_y;
                dis_h = dist(px, py, hx, hy, r_a);
                d_o_f = 8;
            }
            else
            {
                r_x += x_o;
                r_y += y_o;
                d_o_f += 1;
            }
        }

        d_o_f = 0;
        double dis_v = 1000000, vx = px, vy = py;
        double n_tan = -tan(r_a);

        r_x = (r_a > P2 && r_a < P3) ? floor(px / map_size) * map_size - 0.0001 : floor(px / map_size) * map_size + map_size;
        r_y = (px - r_x) * n_tan + py;
        x_o = (r_a > P2 && r_a < P3) ? -map_size : map_size;
        y_o = -x_o * n_tan;

        while (d_o_f < 8)
        {
            m_x = (int)(r_x) / map_size;
            m_y = (int)(r_y) / map_size;
            m_p = m_y * map_x + m_x;

            if (m_p > 0 && m_p < map_x * map_y && map_w[m_p] > 0)
            {
                vmt = map_w[m_p] - 1;
                vx = r_x;
                vy = r_y;
                dis_v = dist(px, py, vx, vy, r_a);
                d_o_f = 8;
            }
            else
            {
                r_x += x_o;
                r_y += y_o;
                d_o_f += 1;
            }
        }

        double shade = 1;
        if (dis_v < dis_h)
        {
            hmt = vmt;
            r_x = vx;
            r_y = vy;
            dis_t = dis_v;
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            shade = 0.5;
        }
        else
        {
            vmt = hmt;
            r_x = hx;
            r_y = hy;
            dis_t = dis_h;
            SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
            shade = 1;
        }

        SDL_RenderDrawLine(renderer, px + 4, py + 4, r_x, r_y);

        // Draw 3D Walls
        double c_a = p_a - r_a;
        if (c_a < 0)
        {
            c_a += 2 * PI;
        }
        if (c_a > 2 * PI)
        {
            c_a -= 2 * PI;
        }
        dis_t = dis_t * cos(c_a);
        double line_h = ((double)map_size * 320) / dis_t;

        double ty_step = 32.0 / (double)line_h;
        double ty_offset = 0;

        if (line_h > 320)
        {
            ty_offset = (line_h - 320) / 2;
            line_h = 320;
        }
        double line_o = 160 - line_h / 2;

        double ty = ty_offset * ty_step + hmt * 32;
        double tx;
        if (shade == 1)
        {
            tx = (int)(r_x / 2.0) % 32;
            if (r_a > 180) tx = 31 - tx;
        }
        else
        {
            tx = (int)(r_y / 2.0) % 32;
            if (r_a > 90 && r_a < 270) tx = 31 - tx;
        }

        // Draw walls
        for (int y = 0; y < line_h; y++)
        {
            int pixel=((int)ty*32+(int)tx)*3+(hmt*32*32*3);
            int red   =All_Textures[pixel+0]*shade;
            int green =All_Textures[pixel+1]*shade;
            int blue  =All_Textures[pixel+2]*shade;

            SDL_SetRenderDrawColor(renderer, red, green, blue, 255);

            int rect_w = 8;
            int rect_h = 8;
            int x_rect = r * 8 + 530;
            int y_rect = y + (int)line_o;
            int w_rect = rect_w;
            int h_rect = rect_h;

            SDL_Rect rect = {x_rect, y_rect, w_rect, h_rect};
            SDL_RenderFillRect(renderer, &rect);

            ty += ty_step;
        }

        // Draw Floors
        for (int y_floor = line_o + line_h; y_floor < 320; y_floor++) {
        /*
            double dy = y_floor - (320 / 2.0);
            double deg = degToRad(r_a);
            double r_a_fix = cos(degToRad(FixAng(p_a - r_a)));
            tx = px / 2 + cos(deg) * 158 * 32 / dy / r_a_fix;
            ty = py / 2 - sin(deg) * 158 * 32 / dy / r_a_fix;

            int mapIndex = (int)(ty / 32.0) * map_x + (int)(tx / 32.0);
            int mp = mapF[mapIndex] * 32 * 32;

            double c = all_textures[((int)(ty)&31) * 32 + ((int)(tx)&31) + mp] * 200;
        */
            int x_rect = r * 8 + 530;
            int y_rect = y_floor;
            int w_rect = 8;
            int h_rect = 8;

            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

            SDL_Rect floor = { x_rect, y_rect, w_rect, h_rect};
            SDL_RenderFillRect(renderer, &floor);

            y_rect = 320 - y_floor;
            SDL_Rect ceil = { x_rect, y_rect, w_rect, h_rect};
            SDL_RenderFillRect(renderer, &ceil);
        }


        r_a += DR;
        if (r_a < 0)
        {
            r_a += 2 * PI;
        }
        if (r_a > 2 * PI)
        {
            r_a -= 2 * PI;
        }
    }
}

// The player will be represented by a little yellow rectangle
void draw_player(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect player = {px, py, 8, 8};
    SDL_RenderFillRect(renderer, &player);

    // Calculate the center of the player rectangle
    double player_center_x = px + player.w / 2.0;
    double player_center_y = py + player.h / 2.0;

    // Calculate the endpoint of the line in front of the player
    double line_length = 20;  // You can adjust the length of the line
    double line_end_x = player_center_x + line_length * cos(p_a);
    double line_end_y = player_center_y + line_length * sin(p_a);

    draw_rays_3D(renderer);

    // Draw the line in front of the player
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Yellow color for the line
    SDL_RenderDrawLine(renderer, player_center_x, player_center_y, line_end_x, line_end_y);
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


short process_events(SDL_Window *window)
{
    SDL_Event event;

    short done = 0;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_WINDOWEVENT_CLOSE:
            {
                if (window)
                {
                    SDL_DestroyWindow(window);
                    window = NULL;
                    done = 1;
                }
            } break;
            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    {
                        done = 1;
                        break;
                    }
                    case SDLK_e:
                    {
                        int x_o = 0;
                        if (p_d_x < 0) x_o = -25;
                        else x_o = 25;

                        int y_o = 0;
                        if (p_d_y < 0) y_o = -25;
                        else y_o = 25;

                        int ipx = px / 64.0, ipx_add_xo = (px + x_o) / 64.0;
                        int ipy = py / 64.0, ipy_add_yo = (py + y_o) / 64.0;

                        if (map_w[ipy_add_yo * map_x + ipx_add_xo] == 4) map_w[ipy_add_yo * map_x + ipx_add_xo] = 0;
                    }
                }
                // Update the corresponding key state in the struct
                if (event.key.keysym.sym == SDLK_w) keys.w = 1;
                if (event.key.keysym.sym == SDLK_a) keys.a = 1;
                if (event.key.keysym.sym == SDLK_s) keys.s = 1;
                if (event.key.keysym.sym == SDLK_d) keys.d = 1;
            } break;
            case SDL_KEYUP: {
                // Update the corresponding key state in the struct
                if (event.key.keysym.sym == SDLK_w) keys.w = 0;
                if (event.key.keysym.sym == SDLK_a) keys.a = 0;
                if (event.key.keysym.sym == SDLK_s) keys.s = 0;
                if (event.key.keysym.sym == SDLK_d) keys.d = 0;
            } break;
            case SDL_QUIT: {
                done = 1;
            } break;
        }
    }

    updatePlayerPosition(); // Update player position based on key states

    return done;
}

int main(int argc, char *argv[])
{

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO);
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