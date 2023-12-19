#ifndef TERMINAL_INTERFACE_H
#define TERMINAL_INTERFACE_H

void init_terminal();
void restore_terminal();

void clear_terminal();
void print_str(int x, int y, char *str);
unsigned int read_input(char *buf, unsigned int count);

#endif
