#include "main.h"
#include <sstream>

void trim(char *str)
{
    int i;
    if(str == NULL) return;
    for(i = 0; i < strlen(str); i++)
    {
        if(str[i] < 0)
        {
            str[i] = '\0';
            break;
        }
    }
}

template <class T>
inline std::string to_string (const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

int IndexOf(char *str, char *index, int start)
{
    int i;
    if(str == NULL || index == NULL) return -1;
    if(start < 0) return -1;
    if(start > strlen(str)) return -1;
    for(i = start; i < strlen(str); i++)
    {
        if(str[i] == index[0])
        {
            return i;
        }
    }
    //Not found.
    return -1;
}

char *substr(char *str, int index)
{
    return substr_s(str, index, 0);
}

char *substr_s(char *str, int index, int len)
{
    int i;
    int f = 0;
    char *subsr = (char *)malloc((len > 0 ? len : strlen(str) - index) + 1);
    bzero(subsr, (len > 0 ? len : strlen(str) - index) + 1);
    if(index < 0) return NULL;
    for(i = index; i < (len > 0 ? index + len : strlen(str)); i++)
    {
        char m = str[i];
        subsr[f] = (char)m;
        f++;
    }
    trim(subsr);
    return subsr;
}

//Our better function, called "str_indexof()"
int str_indexof(char *str, char *search)
{
    return str_indexof(str, search, 0);
}
int str_indexof(char *str, char *search, int start)
{
    int srch = 0;
    int i;
    if(str == NULL) return -1;
    for(i = start; i < strlen(str); i++)
    {
        //Start searching.
        if(str[i] == search[srch])
        {
            if(srch == strlen(search) - 1)
            {
                //We're done, let's return what we have.
                return i;
            }
            //good, now increment.
            srch++;
        }
        else
        {
            //Shit, back to 0;
            srch = 0;
        }
    }
    //Couldn't find it.
    return -1;
}

char * str_replace(
    char const * const original,
    char const * const pattern,
    char const * const replacement
) {
  size_t const replen = strlen(replacement);
  size_t const patlen = strlen(pattern);
  size_t const orilen = strlen(original);

  size_t patcnt = 0;
  const char * oriptr;
  const char * patloc;

  // find how many times the pattern occurs in the original string
  for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
  {
    patcnt++;
  }

  {
    // allocate memory for the new string
    size_t const retlen = orilen + patcnt * (replen - patlen);
    char * const returned = (char *) malloc( sizeof(char) * (retlen + 1) );

    if (returned != NULL)
    {
      // copy the original string,
      // replacing all the instances of the pattern
      char * retptr = returned;
      for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
      {
        size_t const skplen = patloc - oriptr;
        // copy the section until the occurence of the pattern
        strncpy(retptr, oriptr, skplen);
        retptr += skplen;
        // copy the replacement
        strncpy(retptr, replacement, replen);
        retptr += replen;
      }
      // copy the rest of the string.
      strcpy(retptr, oriptr);
    }
    return returned;
  }
}
