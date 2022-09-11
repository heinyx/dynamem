/*
 ** dynamem.c
 **
 ** String library with dynamic memory handling
 ** https://github.com/heinyx/dynamem
 **
 ** Author: Dipl.-Ing. Thomas Heiny, Wolfsburg, Germany
 ** Version: 04.10.2020
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>

#include "dynamem.h"

int mstrcpy(char **str, const char *fmt, ...)
{
	if (str == NULL) return(0);

        va_list ap;

	va_start(ap, fmt);
	int space = 1 + vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);

	char *n = realloc(*str, space);
	if (n == NULL) {
		if (*str) free(*str);
		*str = NULL;
		return(0);
	}
	*str = n;

	va_start(ap, fmt);
	int i = vsnprintf(*str, space, fmt, ap);
	va_end(ap);

	if (i < 0) {
		if (*str) free(*str);
		*str = NULL;
		return(0);
	}

	return(i);
}

int hstrcpy(char **str, int *reserved, const char *fmt, ...)
{
	if (str == NULL) return(0);

	va_list ap;

	va_start(ap, fmt);
	int space = 1 + vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);

	if ((*str == NULL) || (*reserved < space)) {
		char *n = realloc(*str, space);
		if (n == NULL) {
			if (*str) free(*str);
			*str = NULL;
			*reserved = 0;
			return(0);
		}
		*str = n;
		*reserved = space;
	}

	va_start(ap, fmt);
	int i = vsnprintf(*str, *reserved, fmt, ap);
	va_end(ap);

	if (i < 0) {
		if (*str) free(*str);
		*str = NULL;
		*reserved = 0;
		return(0);
	}

	return(i);
}

char *mstrdup(const char *fmt, ...)
{
	if (fmt == NULL) return(NULL);

        va_list ap;

        va_start(ap, fmt);
        int space = 1 + vsnprintf(NULL, 0, fmt, ap);
        va_end(ap);

	char *n = malloc(space);
	if (n == NULL) {
		return(NULL);
	}

	va_start(ap, fmt);
	int i = vsnprintf(n, space, fmt, ap);
	va_end(ap);

	if (i < 0) {
		if (n) free(n);
		return(NULL);
	}
	return(n);
}

size_t moccurence(char *str, const char *search)
{
        char *p, *seg;

        if ((str == NULL) || (search == NULL)) return(0);

        size_t skip = strlen(search);
	size_t count = 0;

	seg = str;

        while ((p = strstr(seg, search)) != NULL) 
        {
		count++;
		seg = p + skip;
	}
	return(count);
}

char *msplitdup(char *str, const char *delimiter, char **s)
{
	char *p, *seg;

	if (delimiter == NULL) return(NULL);

	if (s && *s)
	{
		seg = *s;
	}
	else
	{
		seg = str;
	}
	
	if (seg && (p = strstr(seg, delimiter)))
	{
		if (s) *s = p + strlen(delimiter);
		return(strndup(seg, p - seg));
	}
	else if (seg)
	{
		if (s) *s = NULL;
		return(strdup(seg));
	}
	else
	{
		if (s) *s = NULL;
		return(NULL);
	}
}

carray create_carray()
{
	carray c;
        c.arr = NULL;
        c.len = 0;
	return(c);
}

size_t insert_carray(carray *carr, char *str)
{
	if ((carr == NULL) || (str == NULL)) return(0);
	char **p;

	p = (char**) realloc(carr->arr, (carr->len + 1) * sizeof(char*) );
	if (p != NULL) {
		carr->arr = p;
		carr->arr[carr->len] = str;
		carr->len++;
		return(carr->len);
	}
	return(0);
}

carray msplitarray(char *str, const char *delimiter)
{
        char *s, *b = NULL;

        carray c = create_carray();

        for (s = msplitdup(str, delimiter, &b); s != NULL; s = msplitdup(NULL, delimiter, &b))
        {
                insert_carray(&c, s);
        }
        return(c);
}

char *select_carray(carray *carr, size_t *x)
{
	if (carr && x && (*x < carr->len))
	{
		size_t i = *x;
		*x = i + 1;
		return(carr->arr[i]);
	}
	return(NULL);
}

size_t join_carray(carray *carr, char **join, const char *delimiter)
{
	if (carr && join && delimiter)
	{
		size_t i;
		for (i = 0; i < carr->len; i++)
		{
			if (i && (strcmp(delimiter, ""))) mstrcat(join, delimiter);
			mstrcat(join, carr->arr[i]);
		}
		if (*join) return(strlen(*join));
	}
	return(-1);
}

size_t joinr_carray(carray *carr, char **join, const char *delimiter)
{
        if (carr && join && delimiter)
        {
		size_t i = carr->len;
		while (i)
                {
			i--;
			if ((i != carr->len - 1) && (strcmp(delimiter, ""))) mstrcat(join, delimiter);
                        mstrcat(join, carr->arr[i]);
                }
                if (*join) return(strlen(*join));
        }
        return(-1);
}

size_t len_carray(carray *carr)
{
	if (carr)
	{
		return(carr->len);
	}
	return(0);
}

void close_carray(carray *carr)
{
	if (carr)
	{
		if (carr->arr) free(carr->arr);
		carr->len = 0;
	}
}

void free_carray(carray *carr)
{
	if (carr)
	{
		size_t i;
		for (i = 0; i < carr->len; i++)
		{
			if (carr->arr[i]) free(carr->arr[i]);
		}
		close_carray(carr);
	}
	return;
}

char *mstrdup_replace(char *str, const char *search, const char *replace)
{
	if (str == NULL) return(NULL);

	char *t = strdup(str);

	if (t == NULL) return(NULL);

	if ((replace == NULL) || (search == NULL) || !strlen(search)) return(t);

	mreplace(&t, search, replace);

	return(t);
}

size_t mreplace(char **str, const char *search, const char *replace)
{
        char *p, *t, *seg;

        if ((*str == NULL) || (replace == NULL)) return(0);

	size_t size = strlen(*str);

	if ((search == NULL) || !strlen(search)) return(size);

	size_t slen = strlen(search);
	size_t rlen = strlen(replace);

	size_t oc = moccurence(*str, search);
	if (!oc) return(size);

	seg = *str;

	if (rlen > slen) {
		size = size + oc * (rlen - slen);
		t = realloc(seg, size + 1);
		if (t == NULL) {
			if (*str) free(*str);
			*str = NULL;
			return(0);
		}
		*str = seg = t;
	}

	while ((p = strstr(seg, search)) != NULL)
	{
		seg = p + rlen;
		if (slen != rlen) memmove(p + rlen, p + slen, strlen(p + slen) + 1);
		memcpy(p, replace, rlen);
	}

	if (rlen < slen) {
		seg = *str;
                size = size + oc * (rlen - slen);
                t = realloc(seg, size + 1);
                if (t == NULL) {
			if (*str) free(*str);
                        *str = NULL;
                        return(0);
                }
                *str = seg = t;
        }

	return(size);
}

size_t hreplace(char **str, size_t *reserved, const char *search, const char *replace)
{
	char *p, *t, *seg;

	if ((*str == NULL) || (replace == NULL)) return(0);

	size_t size = strlen(*str);

	if ((search == NULL) || !strlen(search)) return(size);

	size_t slen = strlen(search);
	size_t rlen = strlen(replace);

	size_t oc = moccurence(*str, search);
	if (!oc) return(size);

	seg = *str;

	if (rlen > slen) {
		size = size + oc * (rlen - slen);
		if (*reserved < size + 1) {
			t = realloc(seg, size + 1);
			if (t == NULL) {
				if (*str) free(*str);
				*str = NULL;
				*reserved = 0;
				return(0);
			}
			*str = seg = t;
			*reserved = size + 1;
		}
	}

	while ((p = strstr(seg, search)) != NULL)
	{
		seg = p + rlen;
		if (slen != rlen) memmove(p + rlen, p + slen, strlen(p + slen) + 1);
		memcpy(p, replace, rlen);
	}
	return(strlen(*str));
}

void mstrcat(char **str, const char *add)
{
	if (str && add)
	{
		if (*str == NULL) {
			*str = strdup(add);
		} else {
			char *t = realloc(*str, strlen(*str) + strlen(add) + 1);
			if (t == NULL) {
				if (*str) free(*str);
				*str = NULL;
				return;
			}
			*str = t;
			strcat(*str, add);
		}
	}
}

void mstrcatf(char **str, const char *fmt, ...)
{
	if (str == NULL) return;
        va_list ap;

        va_start(ap, fmt);
        int space = 1 + vsnprintf(NULL, 0, fmt, ap);
        va_end(ap);

	char *n = malloc(space);
	if (n == NULL) return;
	va_start(ap, fmt);
	int i = vsnprintf(n, space, fmt, ap);
	va_end(ap);
	if (i > 0) mstrcat(str, n);
	free(n);
}

char *mstrdup_substring(char *str, int from, int to)
{
        if (str == NULL) return(NULL);

        char *t = strdup(str);

        if (t == NULL) return(NULL);

        msubstring(&t, from, to);

        return(t);
}

size_t msubstring(char **str, int from, int to)
{
        if ((str == NULL) || (*str == NULL)) return(0);

	char *seg;
        size_t size = strlen(*str);

	if (from < 0) from = size + from;
	if (to < 0) to = size + to;

	if (from > size) from = size - 1;
	if (to > size) to = size - 1;

	seg = *str;

	size = to - from + 1;

	memmove(seg, seg + from, size + 1);
	seg[size] = 0;

	seg = realloc(*str, size + 1);
	if (seg == NULL) {
		if (*str) free(*str);
		*str = NULL;
		return(0);
	}
	*str = seg;

        return(size);
}

size_t mstrlen(const char *fmt, ...)
{
	if (fmt == NULL) return(0);
	va_list ap;
	va_start(ap, fmt);
	int size = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);

	return(size);
}

// good work, thanx!
