#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <curl/curl.h>

using namespace std;

int IndexOf(char *str, char *index, int start);
char *substr(char *str, int index);
char *substr_s(char *str, int index, int len);
int str_indexof(char *str, char *search);
int str_indexof(char *str, char *search, int start);
char * str_replace(
    char const * const original,
    char const * const pattern,
    char const * const replacement
);
void trim(char *str);
void error(char *msg);
string GET(char *url);
char *getSpotifySong();

#endif // MAIN_H
