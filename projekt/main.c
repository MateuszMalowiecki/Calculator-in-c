#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#include <ncurses.h>
#include <gmp.h>

#define CALC_COLS 78
#define CALC_LINES 23
#define CALC_MAX_INPUT 70

typedef struct INPUT_WINDOW_S {
	WINDOW* w;
	int cursor_pos_x;
	int cursor_pos_y;

	char buffer[CALC_MAX_INPUT];
	int buffer_idx;
} WINDOW_INPUT;

typedef struct calc {
	WINDOW* main_win;
	WINDOW_INPUT input_win;
	WINDOW* button_num[10];
	WINDOW* button_dot;
	WINDOW* button_binop[5];	// 0 - modulo, 1 - addition, 2 - subtraction, 3 - multiplication, 4 - division

	WINDOW* button_eval;		// equal sign
	WINDOW* button_fsquare;	 // unary function square
	WINDOW* button_fsqrt;	   // unary function sqrt
	WINDOW* button_brack[2];	// 0 - opening bracket, 1 -- closing bracket
	WINDOW* button_c;

	WINDOW* reserved_space;
} calc_t;

typedef struct window_config {

	int width;
	int height;
	char label;
} window_config_t;

static calc_t calc;

#include "init.h"
#include "counitng.h"

	/* ==== SIGNATURES ==== */

void handle_keystrokes();

	/* MAIN */

int main(){

	init_ncurses();
	mpf_set_default_prec(128);

	init_calc();
    mousemask(ALL_MOUSE_EVENTS, NULL);

	handle_keystrokes();

	finalize_ncurses();

	return 0;
}
	/* FUNCTION DEFINITION */

void handle_keystrokes() {
	NCURSES_EXPORT(int) c;
	MEVENT m;

	bool flag_erease = false;
	bool flag_evalte = false;

	while((c = wgetch(calc.input_win.w)) != 27 && c != 'q') {

        if (c == KEY_MOUSE) {
            char binop[5] = {'%', '+', '-', '*', '/'};

            if(getmouse(&m) == OK && m.bstate & BUTTON1_PRESSED) {
                if (is_window_click(calc.button_dot, &m)) { c = '.'; }
                if (is_window_click(calc.button_brack[0], &m)) { c = '('; }
                if (is_window_click(calc.button_brack[1], &m)) { c = ')'; }
                if (is_window_click(calc.button_c, &m)) { c = 8; }
                if (is_window_click(calc.button_eval, &m)) { c = 10; }

                for(int i = 0; i < 5; ++i) {
                    if (is_window_click(calc.button_binop[i], &m)) { c = binop[i]; }
                }
                for(int i = 0; i < 10; ++i) {
                    if (is_window_click(calc.button_num[i], &m)) { c = '0' + i; }
                }
            }
        }

		if (isinput(c) && calc.input_win.buffer_idx < CALC_MAX_INPUT) {

			if (flag_erease) {
				// compensation
				--calc.input_win.cursor_pos_x;
				flag_erease = false;
			}

			if (flag_evalte) {
				calc.input_win.cursor_pos_x -= 1;
				flag_evalte = false;
			}

			werase(calc.input_win.w);
			box(calc.input_win.w, 0 , 0);

			calc.input_win.buffer[calc.input_win.buffer_idx++] = c;
			calc.input_win.buffer[calc.input_win.buffer_idx] = '\0';
			mvwprintw(calc.input_win.w, calc.input_win.cursor_pos_y, calc.input_win.cursor_pos_x--, "%s", calc.input_win.buffer);

			wrefresh(calc.input_win.w);


			continue;
		}

		if (iserease(c) && calc.input_win.buffer_idx > 0) {

			if (!flag_erease) {
				// compensation
				calc.input_win.cursor_pos_x += 1;
			}

			calc.input_win.buffer[--calc.input_win.buffer_idx] = '\0';
			werase(calc.input_win.w);
			box(calc.input_win.w, 0 , 0);

			mvwprintw(calc.input_win.w, calc.input_win.cursor_pos_y, ++calc.input_win.cursor_pos_x, "%s", calc.input_win.buffer);
			wrefresh(calc.input_win.w);

			flag_erease = true;
		}

		if (c == 10) {
            mpf_t e;
            mpf_init(e);
            evalONP(convertToONP(calc.input_win.buffer), e);

            sprintf(calc.input_win.buffer, "%s", convertToStrRep(e));
            calc.input_win.buffer_idx = strlen(calc.input_win.buffer);
            calc.input_win.cursor_pos_x = CALC_MAX_INPUT - calc.input_win.buffer_idx + 2;

            werase(calc.input_win.w);
			box(calc.input_win.w, 0 , 0);

			mvwprintw(calc.input_win.w, calc.input_win.cursor_pos_y, calc.input_win.cursor_pos_x, "%s", calc.input_win.buffer);

			flag_evalte = true;
		}
	}
}
