/*
 ** dynamem.c
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
#include <string.h>
#include <stddef.h>
#include <ctype.h>

#include "dynamem.h"

size_t mstrcpy(char **str, const char *fmt, ...)
{
        size_t i;
        size_t space = strlen(fmt) + 1;
        char *n = NULL;
        va_list ap;

        while (1) {
                n = realloc(*str, space);
                if (n == NULL) {
			if (*str) free(*str);
			*str = NULL;
			return(0);
		}

		*str = n;

                va_start(ap, fmt);
                i = vsnprintf(*str, space, fmt, ap);
                va_end(ap);

                if (i < 0) {
			if (*str) free(*str);
			*str = NULL;
                        return(0);
                }

		if (i < space) return(i);

                space = i + 1;
        }
}

char *mstrdup(const char *fmt, ...)
{
        size_t i;
        size_t space = strlen(fmt) + 1;
        char *p = NULL;
        char *n = NULL;
        va_list ap;

        while (1) {
                n = realloc(p, space);
                if (n == NULL) {
                        if (p) free(p);
                        return(NULL);
                }

		p = n;

                va_start(ap, fmt);
                i = vsnprintf(p, space, fmt, ap);
                va_end(ap);

                if (i < 0) {
                        if (p) free(p);
                        return(NULL);
                }

                if (i < space) return(p);

                space = i + 1;
        }
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
	size_t i;

	if (x && (*x >= 0) && (*x < carr->len))
	{
		i = *x;
		*x = i + 1;
		return(carr->arr[i]);
	}
	return(NULL);
}

size_t len_carray(carray *carr)
{
	return(carr->len);
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
	size_t i;

	if (carr)
	{
		for (i = 0; i < carr->len; i++)
		{
			if (carr->arr[i]) free(carr->arr[i]);
		}
		close_carray(carr);
	}
	return;
}

char *mstrdup_replace(const char *str, const char *search, const char *replace)
{
	if (str == NULL) return(NULL);

	char *out = strdup(str);

	if ((replace == NULL) || (search == NULL) || !strlen(search)) return(out);

	mreplace(&out, search, replace);

	return(out);
}

size_t mreplace(char **str, const char *search, const char *replace)
{
        char *p, *seg;

        if ((*str == NULL) || (replace == NULL)) return(0);

        size_t slen = strlen(search);
        size_t rlen = strlen(replace);
	size_t size = strlen(*str);

	if ((search == NULL) || !strlen(search)) return(size);

	size_t oc = moccurence(*str, search);
	if (!oc) return(size);

	seg = *str;

	if (rlen > slen) {
		size = size + oc * (rlen - slen);
		seg = realloc(seg, size + 1);
		if (seg == NULL) {
			*str = NULL;
			return(0);
		}
		*str = seg;
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
                seg = realloc(seg, size + 1);
                if (seg == NULL) {
                        *str = NULL;
                        return(0);
                }
                *str = seg;
        }

	return(size);
}

void mstrcat(char **str, const char *add)
{
	if (str && add)
	{
		if (*str == NULL) {
			*str = strdup(add);
		} else {
			char *out = realloc(*str, strlen(*str) + strlen(add) + 1);
			if (out == NULL) {
				*str = NULL;
				return;
			}
			*str = out;
			strcat(*str, add);
		}
	}
}

size_t msubstring(char **str, size_t from, size_t to)
{
        char *seg;

        if (*str == NULL) return(0);

        size_t size = strlen(*str);

	if ((from < 0) || (to < 0)) return(size);

	if (from > size) from = size - 1;
	if ((to > size) || (to == 0)) to = size - 1;

	seg = *str;

	size = to - from + 1;

	memmove(seg, seg + from, size + 1);
	seg[size] = 0;

	seg = realloc(*str, size + 1);
	if (seg == NULL) {
		*str = NULL;
		return(0);
	}
	*str = seg;

        return(size);
}

size_t mstrlen(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	size_t size = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);

	return(size);
}

// good work, thanx!
