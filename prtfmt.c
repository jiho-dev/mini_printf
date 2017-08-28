#include <stdarg.h> // for var_list
#include <unistd.h> // for write()
#include <stdlib.h> // for malloc() and free()

// for test
#ifdef __DEBUG__
#include <string.h>
#include <stdio.h>
#endif

#include "prtfmt.h"


static void print_string(const char *buf, size_t len, fmt_t* fmt);
//////////////////////////////////////////////

static void my_putchar(const char buf)
{
	write(0, (const void *)&buf, 1);
}

#if 0
static void my_putstring(const char *string)
{
	while (*string) {
		my_putchar((const char)*string);
		string ++;
	}
}
#endif

static void my_memset(char *buf, char c, unsigned int len)
{
	unsigned int i;

	for (i=0; i<len; i++) {
		buf[i] = c;
	}
}

static size_t my_strlen(const char *s) 
{
	size_t len = 0;

	while (*s) {
		len ++;
		s ++;
	}

	return len;
}

static void	my_memcpy(char *dst, const char *src, const char *end, 
					  size_t len, int is_reverse)
{
	size_t i = 0;

	while (i < len) {
		if (is_reverse) {
			dst[i] = *(end-i-1);
		}
		else {
			dst[i] = src[i];
		}

		i ++;
	}
}

static size_t get_integer_length(long int d, int base)
{
	size_t len = 0;

	while (d) {
		len ++;
		d /= base;
	}

	return len;
}

#if 0
static void my_int2str_old(int data, int base)
{
	int i;
	int len;
	int weight = 1;
	char digit[] = "0123456789abcdef";

	len = get_integer_length(data, base);

	for (i=0; i<len-1; i++)
		weight *= base;

	int c;
	while (data) {
		c = data / weight;
		my_putchar((const char)digit[c]);

		data -= (c * weight);
		weight /=base;
	}
}
#endif

static char* int2str(long int data, int base, fmt_t *fmt)
{
	char *base_string[] = { 
		"0123456789abcdef",
		"0123456789ABCEDF",
	};
	int len;
	char *buf;
	char *digit;
	
	if (fmt->flags & FMT_TYPE_HEX_UPPER) {
		digit = base_string[1];
	}
	else {
		digit = base_string[0];
	}

	len = (int)get_integer_length(data, base);

	if (len < 1)
		return NULL;

	if (fmt->flags & FMT_HAS_PRECISION && fmt->precision > len) {
		len = fmt->precision;
	}

	if (data < 0) {
		data *= -1;
	}

	buf = (char*)malloc(len+1);
	if (buf == NULL)
		return NULL;

	buf[len] = '\0';
	len --;

	while (data >= base) {
		int c = data % base;
		buf[len] = digit[c];
		data /= base;
		len --;
	}

	buf[len] = digit[data];
	len --;

	while (len >= 0) {
		buf[len] = '0';
		len --;
	}

	return buf;
}

static int parse_flags(const char *buf, fmt_t *fmt)
{
	char *start = (char*)buf;
	int cont = 1;

	while (cont && *buf) {
		switch (*buf) {
		case '+':
			fmt->flags |= FMT_FLAG_PLUS;
			buf ++;
			break;

		case '-':
			fmt->flags |= FMT_FLAG_MINUS;
			buf ++;
			break;

		case '0':
			fmt->flags |= FMT_FLAG_ZERO;
			buf ++;
			break;

		case '~':
			fmt->flags |= FMT_FLAG_TILDE;
			buf ++;

			break;
		default:
			cont = 0;
			break;
		}
	}

	return (int)(buf - start);
}

static int parse_decimal(const char *buf, int *val, int not_allow_first_zero)
{
	char *start = (char*)buf;
	int cont = 1, len;
	int weight = 1;

	*val = 0;

	while (cont && *buf) {
		switch (*buf) {
		case '0':
			if (not_allow_first_zero && buf == start) {
				// the cannot be zero
				return 0;
			}
			else {
				buf ++;
			}
			break;
		case '1' ... '9':
			buf ++;
			break;

		default:
			cont = 0;
		}
	}

	len = (int)(buf - start);

	if (len < 1) {
		return len;
	}

	// string to integer
	buf--;

	while (buf >= start) {
		*val += (((*buf) - '0') * weight);

		weight *= 10;
		buf --;
	}

	return len;
}

static int parse_width(const char *buf, fmt_t *fmt)
{
	int len;
	
	len = parse_decimal(buf, &fmt->width, 1);

	if (len > 0) {
		fmt->flags |= FMT_HAS_WIDTH;
	}

	return len;
}

static int parse_precision(const char *buf, fmt_t *fmt)
{
	if (*buf != '.') {
		return 0;
	}

	int len = parse_decimal(buf+1, &fmt->precision, 0);
	fmt->flags |= FMT_HAS_PRECISION;

	return len + 1;
}

static int parse_modifier(const char *buf, fmt_t *fmt)
{
	switch (*buf) {
	case 'h':
		fmt->flags |= FMT_MOD_SHORT;
		return 1;

	case 'l':
		fmt->flags |= FMT_MOD_LONG;
		return 1;

	default:
		break;
	}

	return 0;
}

static int parse_type(const char *buf, fmt_t *fmt)
{
	switch (*buf) {
	case 'c':
		fmt->flags |= FMT_TYPE_CHAR;
		return 1;

	case 's':
	//case 'S':
		fmt->flags |= FMT_TYPE_STRING;
		return 1;

	case 'b':
		fmt->flags |= FMT_TYPE_BINARY;
		return 1;

	case 'd':
		fmt->flags |= FMT_TYPE_DECIMAL;
		return 1;

	case 'x':
		fmt->flags |= FMT_TYPE_HEX_LOWER;
		return 1;

	case 'X':
		fmt->flags |= FMT_TYPE_HEX_UPPER;
		return 1;

	case '%':
		fmt->flags |= FMT_TYPE_IGNORE_ALL;
		return 1;

	default:
		break;
	}

	return 0;
}

static int parse_format(const char *buf, fmt_t *fmt)
{
	int len=0;
	char *start = (char*)buf;


	if (buf == NULL || fmt == NULL) {
		return 0;
	}

	buf ++;

	my_memset((char*)fmt, 0, sizeof(fmt_t));

	len = parse_flags(buf, fmt);
	buf += len;

	len = parse_width(buf, fmt);
	buf += len;

	len = parse_precision(buf, fmt);
	buf += len;

	len = parse_modifier(buf, fmt);
	buf += len;

	len = parse_type(buf, fmt);
	buf += len;

	len = (int)(buf - start);

	if (!(fmt->flags & FMT_TYPE_ALL)) {
		len *= -1;
	}

	return len;
}

static void print_integer(long int data, int base, fmt_t *fmt)
{
	char *string;
	size_t len;
	size_t tlen = 0;
	size_t plen = 0;
	size_t i;
	char *msg;
	char *p;
	char *end;

	string = int2str(data, base, fmt);
	if (string == NULL)
		return;

	len = my_strlen(string);
	end = string + len;
	
	if ((fmt->flags & FMT_HAS_WIDTH)) {
		tlen = fmt->width;
	}

	if (tlen < len){
		tlen = len;
	}

	if (tlen == 0) {
		tlen = len;
	}

	if (plen == 0) {
		plen = len;
	}

#if 0
	dprintf(2, "tlen=%lu, plen=%lu, len=%lu \n", tlen, plen, len);
	fflush(NULL);
#endif

	msg = (char*)malloc(tlen);
	if (msg == NULL) {
		return;
	}

	my_memset(msg, ' ', tlen);

	if (fmt->flags & FMT_FLAG_MINUS) {
		p = msg;
	}
	else {
		p = msg + (tlen - plen);
	}

	my_memcpy(p, string, end, len, fmt->flags & FMT_FLAG_TILDE);

	for (i=0; i<tlen; i++) {
		my_putchar((const char)msg[i]);
	}

	free(msg);
	free(string);
}

static long int get_integer_data(va_list *arg, unsigned int flags)
{
	long int d;

	if (flags & FMT_MOD_SHORT) {
		d = (short int)va_arg(*arg, int);
	}
	else if (flags & FMT_MOD_LONG) {
		d = (long int)va_arg(*arg, long int);
	}
	else {
		d = (long int)va_arg(*arg, int);
	}

	return d;
}

static void print_string(const char *buf, size_t len, fmt_t* fmt)
{
	size_t tlen = 0;
	size_t plen = 0;
	size_t i;
	char *msg;
	char *p;
	const char *end = buf + len;

	if ((fmt->flags & FMT_HAS_WIDTH)) {
		tlen = fmt->width;
	}

	if (fmt->flags & FMT_HAS_PRECISION) {
		plen = fmt->precision;

		if (fmt->flags & FMT_TYPE_CHAR) {
			// precision for char is always 1.
			plen = 1;
		}

		if (len > plen) {
			len = plen;
		}

		if (tlen < plen) {
			tlen = plen;
		}
	}
	else if (tlen < len){
		tlen = len;
	}

	if (tlen == 0) {
		tlen = len;
	}

	if (plen == 0) {
		plen = len;
	}

#if 0
	dprintf(2, "tlen=%lu, plen=%lu, len=%lu \n", tlen, plen, len);
	fflush(NULL);
#endif

	msg = (char*)malloc(tlen);
	if (msg == NULL) {
		return;
	}

	my_memset(msg, ' ', tlen);

	if (fmt->flags & FMT_FLAG_MINUS) {
		p = msg;
	}
	else {
		p = msg + (tlen - plen);
	}

	my_memcpy(p, buf, end, len, fmt->flags & FMT_FLAG_TILDE);

	for (i=0; i<tlen; i++) {
		my_putchar((const char)msg[i]);
	}

	free(msg);
}


static void print_argument(va_list *arg, fmt_t *fmt) 
{
	char *s;
	long int d;
	size_t len;

	switch (fmt->flags & FMT_TYPE_ALL) {
	case FMT_TYPE_IGNORE_ALL:
		my_putchar((const char)'%');
		break;

	case FMT_TYPE_CHAR:
		s = va_arg(*arg, char*);
		len = 1;
		print_string((const char*)s, len, fmt);
		//my_putchar((const char)*s);
		break;

	case FMT_TYPE_STRING:
		s = va_arg(*arg, char *);
		len = my_strlen((const char*)s);
		print_string((const char*)s, len, fmt);

		//for (s = va_arg(*arg, char *); *s; s++)
		//	my_putchar((const char)*s);
		break;

	case FMT_TYPE_BINARY:
		d = get_integer_data(arg, fmt->flags & FMT_MOD_ALL);
		print_integer(d, 2, fmt);
		break;

	case FMT_TYPE_DECIMAL:
		d = get_integer_data(arg, fmt->flags & FMT_MOD_ALL);
		print_integer(d, 10, fmt);
		break;

	case FMT_TYPE_HEX_LOWER:
		d = get_integer_data(arg, fmt->flags & FMT_MOD_ALL);
		print_integer(d, 16, fmt);
		break;

	case FMT_TYPE_HEX_UPPER:
		d = get_integer_data(arg, fmt->flags & FMT_MOD_ALL);
		print_integer(d, 16, fmt);
		break;

	default:
		//putchar(c);
		break;
	}
}

/////////////////////////////////////////////

void mini_printf (const char* format, ...) 
{
	va_list arg; 
	const char *p;
	fmt_t fmt;
	int len;

	va_start(arg, format);

	p = format;

	while (*p) {
		if (*p != '%') {
			my_putchar(*p);
			p ++;
			continue;
		}

		// negative: no valid format
		// zero : unknown error
		// positive: vaild format
		len = parse_format(p, &fmt);
		if (len == 0) {
			my_putchar(*p);
			p ++;
			continue;
		}
		else if (len < 0) {
			int i; 

			len *= -1;

			for (i=0; i<len; i++) {
				my_putchar(*p);
				p ++;
			}

			continue;
		}
		else {
			p += len;
		}

		print_argument(&arg, &fmt);
	}

	va_end(arg);
}
