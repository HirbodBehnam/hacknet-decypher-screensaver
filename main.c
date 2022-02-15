#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <ncursesw/ncurses.h>

// This struct defines a line which must be changed in the animation
typedef struct {
    // The row/column column
    int number;
    // Is this column or row
    bool is_vertical;
} changing_line;

// The number of lines which must be changed in screen must be between these two values
int changing_lines_max, changing_lines_min;
// At each time a random amount of lines will change. This is stored in this variable
int changing_lines_size;
// I think you know what are these
int screen_vertical_size, screen_horizontal_size;
// The list of lines which must change in each screen
changing_line *changing_lines;
// Each frame is this milliseconds long
#define FRAME_TIME 16 // milliseconds (60fps)
// How long a screen must last
#define SCREEN_TIME 1000 // milliseconds

void setup_sizes();

void main_loop();

void print_all_screen();

void generate_changing_lines();

void change_changing_lines();

char random_char();

void mvputchar(int x, int y, char c);


int main() {
    srand(time(NULL)); // seed the rng
    initscr();
    noecho();
    curs_set(0);
    timeout(FRAME_TIME);
    setup_sizes();
    main_loop();
    endwin();
    return 0;
}

/**
 * This function will setup the screen sizes and allocate memory for changing_lines array
 */
void setup_sizes() {
    getmaxyx(stdscr, screen_vertical_size, screen_horizontal_size);
    changing_lines_max = (screen_horizontal_size + screen_vertical_size) / 20; // 5% of lines must move
    changing_lines = malloc(changing_lines_max * sizeof(changing_line));
    changing_lines_min = 1;
}

/**
 * Main loop will loop the screen until it's done (a key is pressed)
 */
void main_loop() {
    while (1) {
        print_all_screen(); // print a new screen
        refresh();
        generate_changing_lines(); // generate which lines we must change
        if (getch() != ERR)
            return;
        for (int i = 0; i < SCREEN_TIME / FRAME_TIME; i++) {
            change_changing_lines(); // change them
            refresh();
            if (getch() != ERR)
                return;
        }
    }
}

/**
 * Print a new screen
 */
void print_all_screen() {
    char buffer[screen_horizontal_size + 1];
    buffer[screen_horizontal_size] = '\0';
    for (int i = 0; i < screen_vertical_size; i++) {
        for (int j = 0; j < screen_horizontal_size; j++)
            buffer[j] = random_char();
        mvprintw(i, 0, buffer);
    }
}

/**
 * Randomly generate the lines which must be
 */
void generate_changing_lines() {
    changing_lines_size = (rand() % (changing_lines_max - changing_lines_min)) + changing_lines_min;
    for (int i = 0; i < changing_lines_size; i++) {
        changing_lines[i].is_vertical = rand() % 2;
        int mod = changing_lines[i].is_vertical ? screen_horizontal_size : screen_vertical_size;
        changing_lines[i].number = rand() % mod;
    }
}

/**
 * This function will randomly fill the lines of changing_lines with random numbers
 */
void change_changing_lines() {
    for (int i = 0; i < changing_lines_size; i++) {
        if (changing_lines[i].is_vertical) {
            for (int j = 0; j < screen_vertical_size; j++)
                mvputchar(j, changing_lines[i].number, random_char());
        } else {
            for (int j = 0; j < screen_horizontal_size; j++)
                mvputchar(changing_lines[i].number, j, random_char());
        }
    }
}

/**
 * Basically something like mvprintw(x,y,a) but prints only one char
 * @param x The x to put char
 * @param y The y to put char
 * @param c The char to put
 */
void mvputchar(int x, int y, char c) {
    char buffer[] = {c, '\0'};
    mvprintw(x, y, buffer);
}

/**
 * Generates a random character which is either a number or space
 * @return The generated char
 */
char random_char() {
    char a = rand() % 11;
    if (a == 10)
        return ' ';
    else
        return (char) (a + '0');
}