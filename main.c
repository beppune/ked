#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <errno.h>

struct termios saved;

void die(char *c) {
	perror(c);
	exit(1);
}

void disable_raw() {
	if( tcsetattr(STDIN_FILENO, TCSAFLUSH, &saved) == -1 )
		die("tcsetattr");
}

void enable_raw() {
	if( tcgetattr(STDIN_FILENO, &saved) == -1 ) die("tcgetattr");
	atexit(disable_raw);

	struct termios raw = saved;
	raw.c_cflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
	raw.c_oflag &= ~(OPOST);
	raw.c_lflag &= ~(ECHO | IEXTEN | ICANON | ISIG);
	raw.c_cflag |= (CS8);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	if( tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1 ) die("tcsetattr");
}

int main() {
	enable_raw();

	char c = '\0';
	if( read(STDIN_FILENO, &c, 1) == -1 ) die("read");
	while (1) {
		if ( iscntrl(c) ) {
			printf("%d\r\n", c);
		} else {
			printf("%d ('%c')\r\n", c, c);
		}
		if ( c == 'q' ) break;
	}
	return 0;
}



