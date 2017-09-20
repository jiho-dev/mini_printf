#include <stdarg.h> // for var_list
#include <unistd.h> // for write()
#include <stdlib.h> // for malloc() and free()

// for test
#ifdef __DEBUG__
#include <string.h>
#include <stdio.h>
#endif

#include "prtfmt.h"

#ifdef __DEBUG__
char msg_buf[1024];
int  msg_idx=0;
#endif


static void print_string(const char *buf, size_t len, fmt_t* fmt);

//////////////////////////////////////////////

static void my_putchar(const char buf)
{
#ifdef __DEBUG__
	// for debugging
	msg_buf[msg_idx] = (char)buf;
	
	if (msg_idx < 1024) {
		msg_idx ++;
	}
#endif

	write(0, (const void *)&buf, 1);
}

static void my_putstring(const char *msg, size_t len)
{
	size_t i;

	for (i=0; i<len; i++) {
		my_putchar((const char)msg[i]);
	}
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

// 숫자의 길이를 구한다.
static size_t get_integer_length(int d, int base)
{
	size_t len = 0;

	while (d) {
		len ++;
		d /= base;
	}

	return len;
}

static size_t get_uinteger_length(unsigned int d, int base)
{
	size_t len = 0;

	while (d) {
		len ++;
		d /= base;
	}

	return len;
}

// 숫자 데이터를 casting 한다.
static long int get_integer_data(va_list *arg, unsigned int flags)
{
	long int d;

	if (flags & FMT_MOD_SHORT) {
		d = (short int)va_arg(*arg, int);
	}
	else if (flags & FMT_MOD_LONG) {
		d = (long int)va_arg(*arg, long int);
	}
	else if (flags & FMT_TYPE_HEX) {
		d = (unsigned long int)va_arg(*arg, long int);
	}
	else {
		d = (int)va_arg(*arg, int);
	}

	return d;
}

// 숫자를 문자열로 변환 한다.
// 만일 precision을 지정된 경우 같이 처리 한다.
static char* int2str(va_list *arg, int base, fmt_t *fmt, char *sign)
{
	char *base_string[] = { 
		"0123456789abcdef",
		"0123456789ABCEDF",
	};
	int len;
	char *buf;
	char *digit;
	int data;
	
	if (fmt->flags & FMT_TYPE_HEX_UPPER) {
		digit = base_string[1];
	}
	else {
		digit = base_string[0];
	}

	data = get_integer_data(arg, fmt->flags & FMT_MOD_ALL);

	// 문자열로 변환 할때는 항상 양수형으로 연산 한 후에
	// sign 기호를 추가 한다.
	if (data == 0 || (fmt->flags & FMT_TYPE_HEX)) {
		*sign = ' ';
	}
	else if (data < 0) {
		*sign = '-';
		data *= -1;
	}
	else {
		*sign = '+';
	}

	if (fmt->flags & FMT_TYPE_HEX) {
		len = get_uinteger_length((unsigned int)data, base);
	}
	else {
		len = get_integer_length((int)data, base);
	}

	//printf("### %c %d, len=%d \n", *sign, data, len);
	if (len < 1)
		return NULL;

	if (fmt->flags & FMT_HAS_PRECISION && fmt->precision > len) {
		len = fmt->precision;
	}

	buf = (char*)malloc(len+1);
	if (buf == NULL)
		return NULL;

	buf[len] = '\0';
	len --;

	// 숫자를 문자로 만든다.
	if (fmt->flags & FMT_TYPE_HEX) {
		unsigned int li = (unsigned int)data;

		while (li >= base) {
			int c = li % base;
			buf[len] = digit[c];
			li /= base;
			len --;
		}

		buf[len] = digit[li];
		len --;
	}
	else {
		while (data >= base) {
			int c = data % base;
			buf[len] = digit[c];
			data /= base;
			len --;
		}

		buf[len] = digit[data];
		len --;
	}

	// precision이 지정 된 경우 zero로 채운다.
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
			// zero 허용 여부 적용: width 파싱시는 zero를 허용 안함
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

	// convert string to integer
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

static void print_integer(va_list *arg, int base, fmt_t *fmt)
{
	char *string;
	size_t len;
	size_t tlen = 0;
	size_t plen = 0;
	char *msg;
	char *p;
	char *end;
	char sign = 0;

	string = int2str(arg, base, fmt, &sign);
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

	plen = fmt->precision;

	if (tlen == 0) {
		tlen = len;
	}

	msg = (char*)malloc(tlen);
	if (msg == NULL) {
		return;
	}

	if (fmt->flags & FMT_FLAG_MINUS) {
		p = msg;

		my_memset(msg, ' ', tlen);
		
		if (((sign == '+') && (fmt->flags & FMT_FLAG_PLUS)) || 
			sign == '-') {
			*p = sign;
			p++;
		}
	}
	else {
		p = msg + (tlen - len);

		if (fmt->flags & (FMT_FLAG_PLUS | FMT_FLAG_ZERO)) {
			my_memset(msg, ' ', tlen);

			if (plen == 0 && fmt->flags & FMT_FLAG_ZERO) {
				char *s = msg;

				if (sign == '+' && (fmt->flags & FMT_FLAG_PLUS)) {
					*msg = sign;
					s ++;
				}
				else if (sign == '-') {
					*msg = sign;
					s ++;
				}

				my_memset(s, '0', p-s);
			}
			else if (!(fmt->flags & FMT_TYPE_HEX)) {
				*(msg + tlen - len - 1) = sign;
			}
		}
		else {
			my_memset(msg, ' ', tlen);

			if (sign == '-') {
				if (p == msg) {
					*p = sign;
					len --;
					p ++;
				}
				else {
					*(p-1) = sign;
				}
			}
		}
	}

	my_memcpy(p, string, end, len, fmt->flags & FMT_FLAG_TILDE);
	my_putstring(msg, tlen);

	free(msg);
	free(string);
}

static void print_string(const char *buf, size_t len, fmt_t* fmt)
{
	size_t tlen = 0;
	size_t plen = 0;
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
	my_putstring(msg, tlen);

	free(msg);
}

static void print_argument(va_list *arg, fmt_t *fmt) 
{
	char *s;
	//long int d;
	size_t len=0;
	char c;

	switch (fmt->flags & FMT_TYPE_ALL) {
	case FMT_TYPE_IGNORE_ALL:
		my_putchar((const char)'%');
		break;

	case FMT_TYPE_CHAR:
		c = (char)va_arg(*arg, int);
		len = 1;
		print_string((const char*)&c, len, fmt);
		break;

	case FMT_TYPE_STRING:
		s = va_arg(*arg, char *);
		len = my_strlen((const char*)s);
		print_string((const char*)s, len, fmt);
		break;

	case FMT_TYPE_BINARY:
		print_integer(arg, 2, fmt);
		break;

	case FMT_TYPE_DECIMAL:
		print_integer(arg, 10, fmt);
		break;

	case FMT_TYPE_HEX_LOWER:
		print_integer(arg, 16, fmt);
		break;

	case FMT_TYPE_HEX_UPPER:
		print_integer(arg, 16, fmt);
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
