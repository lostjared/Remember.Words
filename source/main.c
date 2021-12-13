#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>
#include"remember.h"
#include "SDL.h"
#include "SDL_ttf.h"

void printtext(SDL_Renderer *ren, TTF_Font *font, const char *src, int x, int y,unsigned char r, unsigned char g, unsigned char b) {
    SDL_Color color = { r, g, b };
    SDL_Surface *surface = TTF_RenderText_Solid(font,src,color);

    if(!surface) {
        fprintf(stderr, "Error creating surface %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surface);

    if(!texture) {
        fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
        return;
    }

    SDL_Rect rc = { x, y, surface->w, surface->h };
    SDL_Rect rc_s = { 0, 0, surface->w, surface->h };
    SDL_RenderCopy(ren, texture, &rc_s, &rc);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void printtext_width(SDL_Renderer *ren, TTF_Font *font, int blink, const char *src, int x, int y, int w, unsigned char r, unsigned char g, unsigned char b) {
    char buffer[255][4096];
    int index = 0, pos = 0, offset = 0;
    int len = strlen(src);
    int width = x;
    int advance = 0;
    int minx,maxx,miny,maxy;
    int ypos = y;
    static int max_advance = 0;
    static int max_y = 0;
    while(pos < len) {
        if(TTF_GlyphMetrics(font,src[pos],&minx,&maxx,&miny,&maxy,&advance)==-1) {
            printf("%s\n",TTF_GetError());
            return;
        }
        else {

            if(maxy > max_y)
                max_y = maxy;
            if(advance > max_advance)
                max_advance = advance;

            if(width + max_advance > (w-25)) {
                width = x;
                buffer[index][offset] = 0;
                ++index;
                offset = 0;
                ypos += max_advance+max_y;
            } else {
                buffer[index][offset++] = src[pos++];
                width += advance;
            }
        }
        
    }
    buffer[index][offset] = 0;
    int yy = y;
    for (int i = 0; i <= index; ++i) {
        if(strlen(buffer[i]) > 0)
            printtext(ren, font, buffer[i], x, yy, r, g, b);
        yy += max_advance+max_y;
    }

    static int flash = 0;
    if(blink == 1 && ++flash > 15) {
        SDL_Rect rc = {strlen(src) == 0 ? width : width+8, ypos, 8, max_y+max_advance};
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderFillRect(ren, &rc);
        flash = 0;
    }
}

int main(int argc, char **argv) {
    printf("Program Useage:\n%s: timeout words_list.txt font_size width height\n", argv[0]);
    int font_size = 24;
    srand((unsigned int)time(0));
    int width = 1920, height = 1080;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    if(argc >= 4)
        font_size = atoi(argv[3]);

    if(argc >= 6) {
        width = atoi(argv[4]);
        height = atoi(argv[5]);
    }
    
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("Remember Words", 0, 0, width, height, SDL_WINDOW_SHOWN);
    if(!window) {
        fprintf(stderr, "Error creating Window: %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    TTF_Font *font = TTF_OpenFont("data/font.ttf", font_size);

    if(font == 0) {
        fprintf(stderr, "Error loading font ./data/font.ttf %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }
    
    SDL_Renderer *ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if(ren == 0) {
        SDL_Quit();
        return EXIT_FAILURE;
    }

    int show_words = 0;
    
    struct Remember rem;
    rem_init(&rem);
    
    if(argc >= 3)
        rem_buildlist(&rem, argv[2]);
    else
        rem_buildlist(&rem, "list.txt");

    //rem_printlist(&rem);
    rem_genwords(&rem);
    int active = 1;
    SDL_Event e;
    unsigned int time_ticks = 0;
    unsigned int timeout = 15;

    if(argc >= 2) {
        timeout = atoi(argv[1]);
    }

    SDL_StartTextInput();

    while(active == 1) {
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                    active = 0;
                    break;

                case SDL_TEXTINPUT: {
                    if(show_words == 2) {
                        for(int i = 0; i < strlen(e.text.text); ++i) {
                            int keycode = e.text.text[i];
                            if(isalpha(keycode) || keycode == ' ') {
                                    rem_addchar(&rem, keycode);
                            } 
                        }
                    }
                }
                    break;
          
                case SDL_KEYDOWN: {
                     if(e.key.keysym.sym == SDLK_ESCAPE) {
                         active = 0;
                         break;
                     }                   

                     if(show_words == 0) {
                         if(e.key.keysym.sym == SDLK_SPACE) {
                             show_words = 1;
                             time_ticks = SDL_GetTicks();
                             break;
                         }
                        break;
                     } else if(show_words == 3) {
                         if(e.key.keysym.sym == SDLK_SPACE) {
                             show_words = 0;
                             rem_restart(&rem);
                             rem_genwords(&rem);
                             timeout += 5;
                             break;
                         }
                         break;
                     } else if(show_words == 4) {
                         show_words = 0;
                         rem_restart(&rem);
                         rem.count = 1;
                         rem_genwords(&rem);
                         timeout = 15;
                         if(argc >= 2) 
                            timeout = atoi(argv[1]);
                         break;
                     } else if(show_words == 2) {
                        int keycode = e.key.keysym.sym;
                        if(keycode == SDLK_RETURN) {
                            if(rem_compare(&rem)) {
                                 show_words = 3;
                            } else {
                                 show_words = 4;
                             }
                        }
                       
                       if(keycode == SDLK_BACKSPACE) {
                            rem_delchar(&rem);
                        }
                    }
                }
                    break;
            }
        }
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        switch(show_words) {
            case 0: {
                char buffer[1024];
                sprintf(buffer, "Remember these %d words: (Press Space to Start)", rem.count-1);
                printtext(ren, font, buffer, 25, 25, 255, 0, 0);
                printtext_width(ren, font, 0, rem.match_buffer, 25, 60, width, 0, 255, 0);
            }
                break;
            case 1: {

                char buffer[1024];
                unsigned int cur_ticks = SDL_GetTicks();
                unsigned int diff_time = cur_ticks - time_ticks;
                static unsigned int num = 0;
                num += diff_time;
                int wait_ticks = timeout-(num/1000);
                time_ticks = cur_ticks;
                if(wait_ticks <= 0) {
                    show_words = 2;
                    num = 0;
                }
                sprintf(buffer, "Wait for %d Seconds then Retype the Text as you saw it...", wait_ticks);
                printtext(ren, font, buffer, 25, 25, 0, 0, 255);
            }
                break;
            case 2:
                printtext(ren, font, "Enter the words Exactly as you saw and press Enter", 25, 25, 255, 0, 255);
                printtext_width(ren, font, 1, rem.buffer, 25, 60, width, 255, 255, 255);
                break;
            case 3:
                printtext(ren, font, "You are Correct! Press Space to Continue....", 25, 25, 0, 255, 255);
                break;
            case 4:
                printtext(ren, font, "You are incorrect Press Space to Start Over...", 25, 25, 255, 255, 0);
            break;
        }
        SDL_RenderPresent(ren);
    }
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    rem_free(&rem);
    SDL_Quit();
    return 0;
}