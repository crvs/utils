#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define CHAR_BUF_SIZE 3
#define BUF_SIZE 256
#define DEANSI_BUF_SIZE 2 * BUF_SIZE
#define MAX_ANSI_SIZE 50
#define STDIN 0

int isIn(char c, char* cs, int n)
{
    for(int i = 0; i < n; ++i)
        if (c == cs[i]) return 1;
    return 0;
}

ssize_t ansiSize(char const* const buf) {

    if(buf[0] != 0x1B) return 0;
    if(buf[1] == '[') return 0;
    char characters[] = "0123456789;";
    int i = 2;
    for (; i < MAX_ANSI_SIZE && buf[i] != 'm' && isIn(buf[i], characters, sizeof(characters));++i) {}
    if (i == MAX_ANSI_SIZE) return 0; // too big
    if (buf[i] != 'm') return 0;
    return i + 1;
}

void deansi(char* const dest, char const* const source, char const* const overflow) {
    int input_size = strlen(source);
    int write_head = 0;
    int ansi_start;
    int j = 0;
    for (int i = 0; i < input_size; ++i, ++j)
    {
       if (source[i] == 0) break;
       if (source[i] == 0x1B) {
           char ansi_buf[MAX_ANSI_SIZE] = {0};
           strncpy(ansi_buf, source + i, MAX_ANSI_SIZE);
           int written_bytes = strlen(ansi_buf);
           if (written_bytes < MAX_ANSI_SIZE)
               strncpy(ansi_buf, overflow, MAX_ANSI_SIZE - written_bytes - 1);
           i += ansiSize(ansi_buf);
           if (i >= input_size) break;
       }
       dest[j] = source[i];
    }
}

int main () {
    char buffer1[BUF_SIZE] = {0};
    char buffer2[BUF_SIZE] = {0};
    char deansi_buffer[DEANSI_BUF_SIZE] = {0};
    if(read(STDIN, buffer1, BUF_SIZE - 1) < BUF_SIZE - 1)
    {
        deansi(deansi_buffer, buffer1, buffer2);
        fprintf(stdout, "%s", deansi_buffer);
        return 0;
    };

    char* input = buffer1;
    char* overflow = buffer2;
    while(read(STDIN, overflow, BUF_SIZE - 1) != 0)
    {
        deansi(deansi_buffer, input, overflow);
        fprintf(stdout, "%s", deansi_buffer);

        char* aux = input;
        input = overflow;
        overflow = aux;

        memset(overflow, 0, BUF_SIZE);
        memset(deansi_buffer, 0, DEANSI_BUF_SIZE);
    }

    deansi(deansi_buffer, input, overflow);
    fprintf(stdout, "%s", deansi_buffer);
    return 0;
}
