#ifndef __PRTFMT_H__
#define __PRTFMT_H__

////// Define flags
#define FMT_FLAG_PLUS 			0x00000001
#define FMT_FLAG_MINUS 			0x00000002
#define FMT_FLAG_ZERO 			0x00000004
#define FMT_FLAG_TILDE 			0x00000008

// only available if type is b,d,x,X
#define FMT_MOD_SHORT 			0x00000010
#define FMT_MOD_LONG 			0x00000020
#define FMT_MOD_ALL 			0x00000030

#define FMT_TYPE_CHAR 			0x00000100 	// c
#define FMT_TYPE_STRING			0x00000200 	// s
#define FMT_TYPE_BINARY			0x00000400 	// b
#define FMT_TYPE_DECIMAL 		0x00000800 	// d
#define FMT_TYPE_HEX_LOWER 		0x00001000 	// x
#define FMT_TYPE_HEX_UPPER 		0x00002000 	// X
#define FMT_TYPE_IGNORE_ALL		0x00004000 	// %

#define FMT_TYPE_HEX 			0x00003000
#define FMT_TYPE_ALL			0x00007F00

#define FMT_TYPE_NUMERIC		(FMT_TYPE_BINARY | FMT_TYPE_DECIMAL | FMT_TYPE_HEX_LOWER | FMT_TYPE_HEX_UPPER)

#define FMT_HAS_WIDTH 			0x00100000 
#define FMT_HAS_PRECISION 		0x00200000 

// to save the result of parsing format
typedef struct fmt_s {
	unsigned int flags;
	int width;
	int precision;
} fmt_t;

typedef union value_u {
	long int li;
	short int si;
	int 	i;
} value_t;


#ifdef __cplusplus
extern "C"
{
#endif

// for debugging
extern char msg_buf[1024];
extern int  msg_idx;

// prototype
void mini_printf (const char* format, ...);


#ifdef __cplusplus
}
#endif
#endif // __PRTFMT_H__
