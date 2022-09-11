/**
 ** dynamem.h
 **
 ** String library with dynamic memory handling
 ** https://github.com/heinyx/dynamem
 **
 ** Author: Dipl.-Ing. Thomas Heiny, Wolfsburg, Germany
 ** Version: 04.10.2020
 **
 **/

#ifndef _DYNAMEM_H
#define _DYNAMEM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>

struct _carray
{
        size_t len;
        char **arr;
};

typedef struct _carray carray;

/**
 ** Duplicate strings
 ** (Functions which create new strings) 
 **
 ** mstrdup - creates a string using formats known by printf()
 ** mstrdup_replace - duplicates the string and replaces excerpts
 ** mstrdup_substring - duplicates the substring from the position'from' to the position 'to' in 'str'
 ** msplitdup - splits a string by a given delimiter, creates new strings
 **
 ** Return value: Pointer to the new string
 **
 **/

char *mstrdup(const char *fmt, ...);
char *mstrdup_replace(char *str, const char* search, const char* replace);
char *mstrdup_substring(char *str, int from, int to);
char *msplitdup(char *str, const char *delimiter, char **s);

/**
 ** String manipulation
 ** (Functions which manipulate existing strings)
 **
 ** mstrcpy - changes the contents of 'str' using formats known by printf()
 ** mreplace - replaces excerpts in 'str' (all occurrences)
 ** mstrcat - appends 'add' to 'str'
 ** mstrcatf - appends a formated string (like printf) to 'str'
 ** msubstring - substring from the position'from' to the position 'to' in 'str'
 **
 ** Return value: size of the manipulated string 'str' (without the terminating '\0')
 **
 **/

int mstrcpy(char **str, const char *fmt, ...);
size_t mreplace(char **str, const char *search, const char *replace);
void mstrcat(char **str, const char *add);
void mstrcatf(char **str, const char *fmt, ...);
size_t msubstring(char **str, int from, int to);

/** keep memory **/
int hstrcpy(char **str, int *reserved, const char *fmt, ...);
size_t hreplace(char **str, size_t *reserved, const char *search, const char *replace);

/**
 ** More helpful string functions
 **
 ** mstrlen - returns the length of a string, the string could be formed using formats known by printf()
 ** moccurence - returns the number of 'search's in 'str'
 **
 **/

size_t mstrlen(const char *fmt, ...);
size_t moccurence(char *str, const char *search);

/**
 ** String array functions
 **
 ** create_carray - creates an empty array of strings
 ** insert_carray - inserts a string into the array
 ** select_carray - reads a string in an array
 ** join_carray - joins the array elements to one string
 ** joinr_carray - joins the array elements to one string (reverse)
 ** len_carray - returns the number of entries in the array
 ** close_carray - frees carray without freeing its content
 ** free_carray - frees carray including the linked content
 ** msplitarray - splits a string by a given delimiter, creates an array with the splitted strings
 **
 **/

carray create_carray();
size_t insert_carray(carray *carr, char *str);
char *select_carray(carray *carr, size_t *x);
size_t join_carray(carray *carr, char **join, const char *delimiter);
size_t joinr_carray(carray *carr, char **join, const char *delimiter);
carray msplitarray(char *str, const char *delimiter);
size_t len_carray(carray *carr);
void close_carray(carray *carr);
void free_carray(carray *carr);

#endif
