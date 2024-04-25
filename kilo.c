/*** includes ***/
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>


/*** data ***/
struct termios og_termios;


/*** terminal ***/
void die(const char*s){
    perror(s);
    exit(1);

}

void disableRawMode(){
    if (tcsetattr(STDERR_FILENO, TCSAFLUSH, &og_termios) == -1)
        die("tcsetattr");
}

void enableRawMode(){

    if (tcgetattr(STDIN_FILENO, &og_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);

    struct termios raw = og_termios;
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= ~CS8;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");

}


/*** intialize ***/
int main(){
    enableRawMode();
    while (1){
    char c;
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
        if (iscntrl(c)){
            printf("%d\r\n", c);
        }else{
            printf("%d('%c')\r\n", c, c);
        }
        if (c == 'q') break;
    } 
    return 0;
}
