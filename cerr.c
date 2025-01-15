#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 256
#define ESCAPED_BUF_SIZE 2 * BUF_SIZE
#define STDIN 0

int main () {
    char buffer[BUF_SIZE] = {0};
    char escaped_buffer[ESCAPED_BUF_SIZE] = {0};
    while(read(STDIN, buffer, BUF_SIZE - 1) != 0)
    {
        fprintf(stderr, "%s", buffer);
        memset(buffer, 0, BUF_SIZE);
    }
    return 0;
}
