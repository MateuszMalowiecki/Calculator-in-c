void init_ncurses() {
	initscr();

	// general ncurses configuration
	noecho();
	cbreak();
	curs_set(0);
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_GREEN);

	refresh();
}

void finalize_ncurses() {
	endwin();
}

WINDOW* create_newwin(int height, int width, int starty, int startx){

	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);
	wrefresh(local_win);

	return local_win;
}

// wrapper around creating sub-window (that is window inside its parent window) with given height, with and relative position
WINDOW* create_subwin(WINDOW* parent, int height, int width, int startrely, int startrelx) {
	WINDOW* local_subwin = derwin(parent, height, width, startrely, startrelx);
	box(local_subwin, 0 , 0);
	wrefresh(local_subwin);

	return local_subwin;
}

// function which release window resources
void destroy_win(WINDOW* local_win){
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(local_win);
	delwin(local_win);
}

// function that initialize caluculator input window (field)
void init_input_w_() {
	calc.input_win.w = create_subwin(calc.main_win, CALC_LINES / 4, CALC_COLS - 4, 1, 2);
	calc.input_win.cursor_pos_y = 2;
	calc.input_win.cursor_pos_x = CALC_COLS - 7;
	calc.input_win.buffer_idx = 0;
	calc.input_win.buffer[0] = '\0';
}

// function that initialize all calculator components
void init_calc() {

	// input sub-window
	calc.main_win = create_newwin(CALC_LINES, CALC_COLS, (LINES - CALC_LINES) / 2, (COLS - CALC_COLS) / 2);
	init_input_w_();

	// number keypad sub-windows
	int keypad_bttn_width = 5;
	int keypad_bttn_height = 3;
	int keypad_shift_y = -2;
	int keypad_shift_x = 0;
	calc.button_num[0] = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 4 + keypad_shift_y, 2 + keypad_shift_x);
	calc.button_num[1] = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 7 + keypad_shift_y, 2 + keypad_shift_x);
	calc.button_num[4] = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 10 + keypad_shift_y, 2 + keypad_shift_x);
	calc.button_num[7] = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 13 + keypad_shift_y, 2 + keypad_shift_x);
	calc.button_num[2] = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 7 + keypad_shift_y, 8 + keypad_shift_x);
	calc.button_num[5] = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 10 + keypad_shift_y, 8 + keypad_shift_x);
	calc.button_num[8] = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 13 + keypad_shift_y, 8 + keypad_shift_x);
	calc.button_num[3] = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 7 + keypad_shift_y, 14 + keypad_shift_x);
	calc.button_num[6] = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 10 + keypad_shift_y, 14 + keypad_shift_x);
	calc.button_num[9] = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 13 + keypad_shift_y, 14 + keypad_shift_x);

	for(int i = 0; i < 10; ++i) {
		mvwprintw(calc.button_num[i], 1, 2, "%d", i);
		wrefresh(calc.button_num[i]);
	}

	// a dot sub-window
	calc.button_dot = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 4 + keypad_shift_y, 8 + keypad_shift_x);
	mvwprintw(calc.button_dot, 1, 2, ".");
	wrefresh(calc.button_dot);

	// operators keypad sub-windows
	calc.button_binop[0] = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 4 + keypad_shift_y, 14 + keypad_shift_x);
	mvwprintw(calc.button_binop[0], 1, 2, "%%");

	calc.button_binop[1] = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 4 + keypad_shift_y, 20 + keypad_shift_x);
	mvwprintw(calc.button_binop[1], 1, 2, "+");

	calc.button_binop[2] = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 7 + keypad_shift_y, 20 + keypad_shift_x);
	mvwprintw(calc.button_binop[2], 1, 2, "-");

	calc.button_binop[3] = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 10 + keypad_shift_y, 20 + keypad_shift_x);
	mvwprintw(calc.button_binop[3], 1, 2, "*");

	calc.button_binop[4] = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 13 + keypad_shift_y, 20 + keypad_shift_x);
	mvwprintw(calc.button_binop[4], 1, 2, "/");

	for(int i = 0; i < 5; ++i) {
		wrefresh(calc.button_binop[i]);
	}

	// others
	calc.button_eval = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width * 2 + 1, CALC_LINES - 4 + keypad_shift_y, 30 + keypad_shift_x);
	mvwprintw(calc.button_eval, 1, 5, "=");
	wrefresh(calc.button_eval);

	calc.button_fsquare = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 7 + keypad_shift_y, 30 + keypad_shift_x);
	mvwprintw(calc.button_fsquare, 1, 1, "x^2");
	wrefresh(calc.button_fsquare);

	calc.button_fsqrt = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 7 + keypad_shift_y, 36 + keypad_shift_x);
	mvwprintw(calc.button_fsqrt, 1, 1, "sqr");
	wrefresh(calc.button_fsqrt);

	calc.button_brack[0] = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 10 + keypad_shift_y, 30 + keypad_shift_x);
	mvwprintw(calc.button_brack[0], 1, 2, "(");
	wrefresh(calc.button_brack[0]);

	calc.button_brack[1] = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width, CALC_LINES - 10 + keypad_shift_y, 36 + keypad_shift_x);
	mvwprintw(calc.button_brack[1], 1, 2, ")");
	wrefresh(calc.button_brack[1]);

	calc.button_c = create_subwin(calc.main_win, keypad_bttn_height, keypad_bttn_width * 2 + 1, CALC_LINES - 13 + keypad_shift_y, 30 + keypad_shift_x);
	mvwprintw(calc.button_c, 1, 5, "C");
	wrefresh(calc.button_c);

	calc.reserved_space = create_subwin(calc.main_win, 12, 30, CALC_LINES - 13 + keypad_shift_y, 46 + keypad_shift_x);
	mvwprintw(calc.reserved_space, 5, 11, "RESERVED");
	wrefresh(calc.reserved_space);
}

bool is_window_click(WINDOW* w, MEVENT* m) {
    return w->_begx <= m->x && w->_begx + w->_maxx >= m->x && w->_begy <= m->y && w->_begy + w->_maxy >= m->y;
}
