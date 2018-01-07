## C string library with dynamic memory handling

**dynamem** is a library for C programming to handle strings easily.

Therefor **dynamem** implements the following functions:

**String building**

Functions which create new strings based on variable argument lists

char \*mstrdup(const char \*fmt, ...);  
char \*mstrdup_replace(const char \*str, const char\* search, const char\* replace);  
char \*msplitdup(char \*str, const char \*delimiter, char \*\*s);  

*mstrdup - creates a string using formats known by printf()  
mstrdup_replace - duplicates the string and replaces excerpts  
msplitdup - splits a string by a given delimiter, creates new strings*  

**String manipulation**

Functions which manipulate existing strings

size_t mstrcpy(char \*\*str, const char \*fmt, ...);  
size_t mreplace(char \*\*str, const char \*search, const char \*replace);  
void mstrcat(char \*\*str, const char \*add);  
size_t msubstring(char \*\*str, size_t from, size_t to);  

*mstrcpy - changes the contents of 'str' using formats known by printf()  
mreplace - replaces excerpts in 'str' (all occurrences)  
mstrcat - appends 'add' to 'str'  
msubstring - substring from the position'from' to the position 'to' in 'str'*  

**More helpful string functions**

size_t mstrlen(const char \*fmt, ...);  
size_t moccurence(char \*str, const char \*search);  

*mstrlen - returns the length of a string, the string could be formed using formats known by printf()  
moccurence - returns the number of 'search's in 'str'*

**String array functions**

Working with a dynamic array to collect strings

carray create_carray();  
size_t insert_carray(carray \*carr, char \*str);  
char \*select_carray(carray \*carr, size_t \*x);  
carray msplitarray(char \*str, const char \*delimiter);  
size_t len_carray(carray \*carr);  
void close_carray(carray \*carr);  
void free_carray(carray \*carr);  

*create_carray - creates an empty array of strings  
insert_carray - inserts a string into the array  
select_carray - reads a string in an array  
len_carray - returns the number of entries in the array  
close_carray - frees carray without freeing its content  
free_carray - frees carray including the linked content  
msplitarray - splits a string by a given delimiter, creates an array with the splitted strings*


Please take a look to **example.c** for example code.

The library was tested and checked by valgrind to prevent memory leaks.

Copyright by Thomas Heiny, Wolfsburg, Germany
