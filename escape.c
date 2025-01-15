#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define CHAR_BUF_SIZE 3
#define BUF_SIZE 256
#define ESCAPED_BUF_SIZE 2 * BUF_SIZE
#define STDIN 0

ssize_t escapeChar(char const c, char * const buf) {
    // zero-out buffer
    switch(c)
    {
       case(0x00) :
       case(0x01) :
       case(0x02) :
       case(0x03) :
       case(0x04) :
       case(0x05) :
       case(0x06) :  { break; }
       case(0x07) :  { strncpy(buf,   "\\a", 2); break; }
       case(0x08) :  { strncpy(buf,   "\\b", 2); break; }
       case(0x09) :  { strncpy(buf,   "\\t", 2); break; }
       case(0x0A) :  { strncpy(buf,   "\\n", 2); break; }
       case(0x0B) :  { strncpy(buf,   "\\v", 2); break; }
       case(0x0C) :  { strncpy(buf,   "\\f", 2); break; }
       case(0x0D) :  { strncpy(buf,   "\\r", 2); break; }
       case(0x0E) :
       case(0x0F) :
       case(0x10) :
       case(0x11) :
       case(0x12) :
       case(0x13) :
       case(0x14) :
       case(0x15) :
       case(0x16) :
       case(0x17) :
       case(0x18) :
       case(0x19) :
       case(0x1A) :
       case(0x1B) :
       case(0x1C) :
       case(0x1D) :
       case(0x1E) :
       case(0x1F) :  { return 0; }
       case(0x20) :  { buf[0] = ' '; break; }
       case(0x21) :  { buf[0] = '!'; break; }
       case(0x22) :  { strncpy(buf, "\\\"", 2); break; }
       case(0x23) :  { buf[0] = '#'; break; }
       case(0x24) :  { buf[0] = '$'; break; }
       case(0x25) :  { buf[0] = '%'; break; }
       case(0x26) :  { buf[0] = '&'; break; }
       case(0x27) :  { strncpy(buf,"\\\'", 2); break; }
       case(0x28) :  { buf[0] = '('; break; }
       case(0x29) :  { buf[0] = ')'; break; }
       case(0x2A) :  { buf[0] = '*'; break; }
       case(0x2B) :  { buf[0] = '+'; break; }
       case(0x2C) :  { buf[0] = ','; break; }
       case(0x2D) :  { buf[0] = '-'; break; }
       case(0x2E) :  { buf[0] = '.'; break; }
       case(0x2F) :  { buf[0] = '/'; break; }
       case(0x30) :  { buf[0] = '0'; break; }
       case(0x31) :  { buf[0] = '1'; break; }
       case(0x32) :  { buf[0] = '2'; break; }
       case(0x33) :  { buf[0] = '3'; break; }
       case(0x34) :  { buf[0] = '4'; break; }
       case(0x35) :  { buf[0] = '5'; break; }
       case(0x36) :  { buf[0] = '6'; break; }
       case(0x37) :  { buf[0] = '7'; break; }
       case(0x38) :  { buf[0] = '8'; break; }
       case(0x39) :  { buf[0] = '9'; break; }
       case(0x3A) :  { buf[0] = ':'; break; }
       case(0x3B) :  { buf[0] = ';'; break; }
       case(0x3C) :  { buf[0] = '<'; break; }
       case(0x3D) :  { buf[0] = '='; break; }
       case(0x3E) :  { buf[0] = '>'; break; }
       case(0x3F) :  { buf[0] = '?'; break; }
       case(0x40):   { buf[0] = '@'; break; }
       case(0x41):   { buf[0] = 'A'; break; }
       case(0x42):   { buf[0] = 'B'; break; }
       case(0x43):   { buf[0] = 'C'; break; }
       case(0x44):   { buf[0] = 'D'; break; }
       case(0x45):   { buf[0] = 'E'; break; }
       case(0x46):   { buf[0] = 'F'; break; }
       case(0x47):   { buf[0] = 'G'; break; }
       case(0x48):   { buf[0] = 'H'; break; }
       case(0x49):   { buf[0] = 'I'; break; }
       case(0x4A):   { buf[0] = 'J'; break; }
       case(0x4B):   { buf[0] = 'K'; break; }
       case(0x4C):   { buf[0] = 'L'; break; }
       case(0x4D):   { buf[0] = 'M'; break; }
       case(0x4E):   { buf[0] = 'N'; break; }
       case(0x4F):   { buf[0] = 'O'; break; }
       case(0x50):   { buf[0] = 'P'; break; }
       case(0x51):   { buf[0] = 'Q'; break; }
       case(0x52):   { buf[0] = 'R'; break; }
       case(0x53):   { buf[0] = 'S'; break; }
       case(0x54):   { buf[0] = 'T'; break; }
       case(0x55):   { buf[0] = 'U'; break; }
       case(0x56):   { buf[0] = 'V'; break; }
       case(0x57):   { buf[0] = 'W'; break; }
       case(0x58):   { buf[0] = 'X'; break; }
       case(0x59):   { buf[0] = 'Y'; break; }
       case(0x5A):   { buf[0] = 'Z'; break; }
       case(0x5B):   { buf[0] = '['; break; }
       case(0x5C):   { buf[0] = '\\'; break; }
       case(0x5D):   { buf[0] = ']'; break; }
       case(0x5E):   { buf[0] = '^'; break; }
       case(0x5F):   { buf[0] = '_'; break; }
       case(0x60):   { buf[0] = '`'; break; }
       case(0x61):   { buf[0] = 'a'; break; }
       case(0x62):   { buf[0] = 'b'; break; }
       case(0x63):   { buf[0] = 'c'; break; }
       case(0x64):   { buf[0] = 'd'; break; }
       case(0x65):   { buf[0] = 'e'; break; }
       case(0x66):   { buf[0] = 'f'; break; }
       case(0x67):   { buf[0] = 'g'; break; }
       case(0x68):   { buf[0] = 'h'; break; }
       case(0x69):   { buf[0] = 'i'; break; }
       case(0x6A):   { buf[0] = 'j'; break; }
       case(0x6B):   { buf[0] = 'k'; break; }
       case(0x6C):   { buf[0] = 'l'; break; }
       case(0x6D):   { buf[0] = 'm'; break; }
       case(0x6E):   { buf[0] = 'n'; break; }
       case(0x6F):   { buf[0] = 'o'; break; }
       case(0x70):   { buf[0] = 'p'; break; }
       case(0x71):   { buf[0] = 'q'; break; }
       case(0x72):   { buf[0] = 'r'; break; }
       case(0x73):   { buf[0] = 's'; break; }
       case(0x74):   { buf[0] = 't'; break; }
       case(0x75):   { buf[0] = 'u'; break; }
       case(0x76):   { buf[0] = 'v'; break; }
       case(0x77):   { buf[0] = 'w'; break; }
       case(0x78):   { buf[0] = 'x'; break; }
       case(0x79):   { buf[0] = 'y'; break; }
       case(0x7A):   { buf[0] = 'z'; break; }
       case(0x7B):   { buf[0] = '{'; break; }
       case(0x7C):   { buf[0] = '|'; break; }
       case(0x7D):   { buf[0] = '}'; break; }
       case(0x7E):   { buf[0] = '~'; break; }
       default: {break;}
    } return strlen(buf);
}

void escapeString(char* const dest, char const* const source) {
    int input_size = strlen(source);
    int write_head = 0;
    for (int i = 0; i < input_size; ++i)
    {
       if (source[i] == 0) break;
       char escape_buf[CHAR_BUF_SIZE] = {0};
       ssize_t n = escapeChar(source[i], escape_buf);
       if (n == 0) continue;
       strncpy(dest + write_head, escape_buf, n);
       write_head += n;
    }
}

int main () {
    char buffer[BUF_SIZE] = {0};
    char escaped_buffer[ESCAPED_BUF_SIZE] = {0};
    while(read(STDIN, buffer, BUF_SIZE - 1) != 0)
    {
        escapeString(escaped_buffer, buffer);
        fprintf(stdout, "%s", escaped_buffer);

        memset(buffer, 0, BUF_SIZE);
        memset(escaped_buffer, 0, ESCAPED_BUF_SIZE);
    }
    // fclose(stdin_f);
    return 0;
}
