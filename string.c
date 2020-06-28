#include "source.h"


void *my_memset(void *b, int c, int len)
{
    unsigned char *p = b;

    while (len > 0)
    {
    *p = c;
    p++;
    len--;
    }

    return (b);
}

char *my_strcat(char *dest, const char *src)
{
    char *rdest = dest;
    while (*dest) dest++;
    while ((*dest++ = *src++));
    return rdest;
}

char* my_strncpy(char* d, const char* s, unsigned int n)
{
    unsigned int uLen = 0;
    while ( *s!= '\0' || uLen<n)
    {
        if(*s!= '\0' && uLen<n)
        {
            *d++ = *s++;
        } else
        {
            *d++='\0';
        }
        
        uLen++;
    }
     
    return d;

}

char *my_strcpy(char *dest, char *src)
{
    char *start = dest;
    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return start;
}

int my_strlen(char* c)
{
    int i = 0;
    while(*(c + i) != '\0') i++;
    return i;
}

int my_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

char* my_strstr(char *s1, char *s2)
{
    size_t n = strlen(s2);
    while(*s1)
        if(!memcmp(s1++,s2,n))
            return (char *) (s1-1);
    return 0;
}