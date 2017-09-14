//
//  testmemfind.c
//  
//
//  Created by Mark Riordan on 9/13/17.
//
// cc -o testmemfind testmemfind.c

#include <stdio.h>
#include <string.h>

/* Find a string inside a buffer.
 * Entry: buffer   is the buffer to search
 *        buflen   is the # of bytes in the buffer
 *        pattern  is the string of bytes to search for
 *        patlen   is the # of bytes in pattern
 * Exit:  Returns -1 if found
 *        0 if not found at all
 *        N if the first N chars of pattern matched the last N chars in buffer.
 */
int memfind(unsigned char *buffer, size_t buflen, unsigned char *pattern, size_t patlen)
{
    int result = 0;
    size_t offset;
    do {
        unsigned char * pchFound = memchr(buffer, pattern[0], buflen);
        if(0 == pchFound) {
            result = 0;
            break;
        } else {
            offset = pchFound - buffer;
            size_t bytesFromEnd = buflen - offset;
            if(bytesFromEnd >= patlen) {
                if(0==memcmp(pchFound, pattern, patlen)) {
                    result = -1;
                    break;
                } else {
                    result = 0;
                }
            } else {
                size_t bytesToCompare = buflen - offset;
                if(0==memcmp(pchFound, pattern, bytesToCompare)) {
                    result = bytesToCompare;
                    break;
                } else {
                    result = 0;
                }
            }
        }
        if(0==result) {
            if(0==offset) {
                buffer++;
                buflen--;
            } else {
                buflen -= (pchFound - buffer);
                buffer = pchFound;
            }
        }
    } while(1);
    return result;
}

void testMemfind(char *buffer, char *pattern, int expected)
{
    int result;
    /*printf("Looking in %s for %s\n", buffer, pattern);*/
    result = memfind((unsigned char *) buffer, strlen(buffer),
                     (unsigned char *) pattern, strlen(pattern));
    if(result != expected) {
        printf("  ** failed %s %s got %d expected %d\n", buffer, pattern, result, expected);
    } else {
        printf("passed: %s %s %d\n", buffer, pattern, expected);
    }
}

int main(int argc, char *argv[])
{
    char *pat = "#IPS!";
    testMemfind("abcdef", pat, 0);
    testMemfind("#IPS!k", pat, -1);
    testMemfind("j#IPS!k", pat, -1);
    testMemfind("j#IPS", pat, 4);
    testMemfind("j##IPS", pat, 4);
    testMemfind("#j##IPS", pat, 4);
    testMemfind("j##", pat, 1);
    testMemfind("j##a", pat, 0);
    testMemfind("a#IPS!k#IPS!l", pat, -1);
}
