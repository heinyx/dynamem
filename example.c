/**
 ** example.c
 **
 ** String library with dynamic memory handling
 ** https://github.com/heinyx/dynamem
 **
 ** Author: Dipl.-Ing. Thomas Heiny, Wolfsburg, Germany
 ** Version: 07.01.2018
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#include "dynamem.h"

int main(int argc, char **argv)
{
	/* Examples mstrdup and mstrdup_replace */

	char *s1, *s2;

	s1 = mstrdup("key=%s value=%d", "version", 1);

	s2 = mstrdup_replace(s1, "key", "k");

	printf("strlen(s1)=%ld s1=>%s<\n", strlen(s1), s1);
	printf("strlen(s2)=%ld s2=>%s<\n", strlen(s2), s2);

	if (s1) free(s1);
	if (s2) free(s2);

	/* Examples mstrcpy and mreplace */

	char *s3 = NULL;
	size_t i;

        i = mstrcpy(&s3, "dynamem library");
	printf("p=%p i=%ld s3=>%s<\n", s3, i, s3);

	i = mstrcpy(&s3, "dynamem library %s %d %s", "allocates memory", 4, "strings");
	printf("p=%p i=%ld s3=>%s<\n", s3, i, s3);

	i = mreplace(&s3, "4", "for");
	printf("p=%p i=%ld s3=>%s<\n", s3, i, s3);

	i = mreplace(&s3, "strings", "my strings");
	printf("p=%p i=%ld s3=>%s<\n", s3, i, s3);

	/* Example mstrcat */

	mstrcat(&s3, " automatically!");
	printf("p=%p s3=>%s<\n", s3, s3);

	/* Example moccurence */

	printf("number of 'string' in s3=%ld\n", moccurence(s3, "string"));

	/* Example msubstring */

	i = msubstring(&s3, 8, 31);
	printf("p=%p i=%ld s3=>%s<\n", s3, i, s3);

	/* Example mstrlen */

	i = mstrlen("1234%d", 5678);
	printf("strlen('12345678')=%ld\n", i);

	/* Example msplitdup */

	char *buff = NULL;
	char *word1 = msplitdup(s3, " ", &buff);
	char *word2 = msplitdup(NULL, " ", &buff);
	char *word3 = msplitdup(NULL, " ", &buff);

	printf("p=%p word1=>%s<\n", word1, word1);
	printf("p=%p word2=>%s<\n", word2, word2);
	printf("p=%p word3=>%s<\n", word3, word3);

	printf("p=%p s3=>%s<\n", s3, s3);

	if (word1) free(word1);
	if (word2) free(word2);
	if (word3) free(word3);

	if (s3) free(s3);

	/* Example msplitarray */

	carray split;
	split = msplitarray("splitted by the msplitarray function", " ");

	for (i = 0; i < split.len; i++) printf("p=%p split=>%s<\n", split.arr[i], split.arr[i]);

	free_carray(&split);

	/* Example carray */

	carray carr = create_carray();

	insert_carray(&carr, "first entry");
	insert_carray(&carr, "second entry");

	char *entry;
	i = 0;
	while ((entry = select_carray(&carr, &i)) != NULL) printf("carr entry=>%s<\n", entry);

	printf("length(carr) = %ld\n", len_carray(&carr));

	close_carray(&carr);

	return(0);
}
