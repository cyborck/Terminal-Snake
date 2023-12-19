#include "terminal_interface.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

int original_stdin_fd_flags = 0;
struct termios original_term = {0};

void init_terminal() {
    // safe current state of the terminal so it can be restored later
    original_stdin_fd_flags = fcntl(STDIN_FILENO, F_GETFL);
    tcgetattr(STDIN_FILENO, &original_term);

    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    // disable terminal echo (user input will not be displayed in the terminal)
    term.c_lflag &= ~ECHO;
    // 'activate' non blocking mode in the terminal
    term.c_lflag &= ~ICANON;  // disable canonical mode
    term.c_cc[VMIN] = 0;      // set the minimum number of characters to 0
    term.c_cc[VTIME] = 0;     // set the timeout to 0
    
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    // set terminal's file-descriptor to non-blocking
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, flags);

    // hide the cursor
    printf("\e[?25l");
    fflush(stdout);
}

void restore_terminal() {
    // restore terminal settings
    fcntl(STDIN_FILENO, F_SETFL, original_stdin_fd_flags);
    tcsetattr(STDIN_FILENO, TCSANOW, &original_term);

    // show cursor again
    printf("\e[?25h");
    // clear terminal and set cursor to (0,0)
    printf("\e[2J\e[H");
    fflush(stdout);
}

void clear_terminal() {
    printf("\e[2J");
    fflush(stdout);
}

void print_str(int x, int y, char *str) {
    printf("\e[%d;%dH%s", y + 1, x + 1, str);
    fflush(stdout);
}

unsigned int read_input(char *buf, unsigned int count) {
    unsigned int size = read(STDIN_FILENO, buf, count);
    if (size < count)
        buf[size] = '\0';
    return size;
}
