#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

struct termios saved;

void disable_raw() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &saved);
}

void enable_raw() {
	tcgetattr(STDIN_FILENO, &saved);
	atexit(disable_raw);

	struct termios raw = saved;
	raw.c_lflag &= ~(ECHO | ICANON);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
	enable_raw();

	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q' ) {
		if ( iscntrl(c) ) {
			printf("%d\n", c);
		} else {
			printf("%d ('%c')\n", c, c);
		}

	}
	return 0;
}



