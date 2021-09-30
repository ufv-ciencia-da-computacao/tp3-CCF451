#include "../lib/color.h"
#include <stdio.h>

void color_white(char str[]) {
    printf("\033[0;37m%s", str);
}

void color_green(char str[]) {
    printf("\033[0;32m%s", str);
}

void color_blue(char str[]) {
    printf("\033[0;34m%s", str);
}

void color_purple(char str[]) {
    printf("\033[0;35m%s", str);
}

void color_cyan(char str[]) {
    printf("\033[0;36m%s", str);
}

void color_yellow(char str[]) {
    printf("\033[0;33m%s", str);
}

void color_red(char str[]) {
    printf("\033[0;31m%s", str);
}

void color_black(char str[]) {    
    printf("\033[0;30m%s", str);
}