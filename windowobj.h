#ifndef _windowobj_h
#define _windowobj_h

#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include "newgraph.h"

#define TEXT_LENGTH 20
#define DEFAULT_COLOR "Black"
#define BUTTON_DOWN_COLOR "Red"

#define GENERAL_WINDOW_INFO struct Point vertice;\
                            double width;\
                            double length;\
                            char *color

struct general_window {
    WINDOW_INFO;
};
struct window_frame {
    WINDOW_INFO;
};

struct text_window {
    WINDOW_INFO;
    bool isCursorBlink;
    bool isDisplayerCursor;
};

struct button {
    WINDOW_INFO;
    char text[TEXT_LENGTH];
    char *move_response_color;
    char *click_response_color;
    void (*click_event)(void);
};

struct text {
    struct Point vertice;
    char *color;
    char text[TEXT_LENGTH];
};

struct window_frame *menu;
struct text_window *order_window;
struct text_window *len_window;
struct text *order_text;
struct text *len_text;
struct button *draw_button;

void init_window(void);

#endif