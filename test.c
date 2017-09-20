
#include <stdio.h>
#include <string.h>
#include "prtfmt.h"

int test_cnt = 0;
int fail_cnt = 0;

void check_result(char *fmt, char *ret) {

	int len;
	int slen=0;
	int i;
	
	len = strlen(ret);
	test_cnt ++;

	slen = 30 - msg_idx;
	for (i=0; i<slen; i++) {
		putchar(' ');
	}

	printf(": fmt=%s:", fmt);

	if (msg_idx != len || strncmp(msg_buf, ret, len) != 0) {
		msg_buf[msg_idx] = 0;
		printf(" fail: expected='%s'\n", ret);
		fail_cnt ++;
	}
	else {
		printf(" success\n");
	}

	fflush(NULL);
}

typedef struct dt_s{
	char *fmt;
	char *result;
	int  int_data;
	char *str_data;
	char char_data;
} test_data_t;

// format, data, expected output을 정의한다.
// 실행 결과와 비교해서 성공/실패를 판단한다.

struct dt_s dt[] = {
	{ .fmt="[%10d]", 	"[       169]", 169, NULL, '\0'},
	{ .fmt="[%010d]", 	"[0000000169]", 169, NULL, '\0'},
	{ .fmt="[%+010d]", 	"[+000000169]", 169, NULL, '\0'},
	{ .fmt="[%10.5d]", 	"[     00169]", 169, NULL, '\0'},
	{ .fmt="[%+10.5d]", "[    +00169]", 169, NULL, '\0'},
	{ .fmt="[%+010.5d]","[    +00169]", 169, NULL, '\0'},
	{ .fmt="[%+010.05d]","[    +00169]", 169, NULL, '\0'},
	{ .fmt="[%10d]", "[      -169]", -169, NULL, '\0'},
	{ .fmt="[%010d]", "[-000000169]", -169, NULL, '\0'},
	{ .fmt="[%+010d]", "[-000000169]", -169, NULL, '\0'},
	{ .fmt="[%10.5d]", "[    -00169]", -169, NULL, '\0'},
	{ .fmt="[%+10.5d]", "[    -00169]", -169, NULL, '\0'},
	{ .fmt="[%+010.5d]", "[    -00169]", -169, NULL, '\0'},
	{ .fmt="[%+010.05d]", "[    -00169]", -169, NULL, '\0'},
	{ .fmt="[%-10d]", "[169       ]", 169, NULL, '\0'},
	{ .fmt="[%-010d]", "[169       ]", 169, NULL, '\0'},
	{ .fmt="[%+-010d]", "[+169      ]", 169, NULL, '\0'},
	{ .fmt="[%-10.5d]", "[00169     ]", 169, NULL, '\0'},
	{ .fmt="[%+-10.5d]", "[+00169    ]", 169, NULL, '\0'},
	{ .fmt="[%+-010.5d]", "[+00169    ]", 169, NULL, '\0'},
	{ .fmt="[%+-010.05d]", "[+00169    ]", 169, NULL, '\0'},
	{ .fmt="[%-10d]", "[-169      ]", -169, NULL, '\0'},
	{ .fmt="[%-010d]", "[-169      ]", -169, NULL, '\0'},
	{ .fmt="[%+-010d]", "[-169      ]", -169, NULL, '\0'},
	{ .fmt="[%-10.5d]", "[-00169    ]", -169, NULL, '\0'},
	{ .fmt="[%+-10.5d]", "[-00169    ]", -169, NULL, '\0'},
	{ .fmt="[%+-010.5d]", "[-00169    ]", -169, NULL, '\0'},
	{ .fmt="[%+-010.05d]", "[-00169    ]", -169, NULL, '\0'},
	{ .fmt="[%99% %T %d]", "[% %T 20]", 20, NULL, '\0'},
	{ .fmt="[%+010ld]", "[+000000999]", 999l, NULL, '\0'},
	{ .fmt="[%+010.005ld]", "[    -00999]", -999l, NULL, '\0'},

	{ .fmt="hello, world", "hello, world", 0, "hello, world", '\0'},
	{ .fmt="[%.6s]", "[hello,]", 0, "hello, world", '\0'},
	{ .fmt="[%10.6s]", "[    hello,]", 0, "hello, world", '\0'},
	{ .fmt="[%4.6s]", "[hello,]", 0, "hello, world", '\0'},
	{ .fmt="[%4.0s]", "[    ]", 0, "hello, world", '\0'},
	{ .fmt="[%9s]", "[hello, world]", 0, "hello, world", '\0'},
	{ .fmt="[%-20s]", "[hello, world        ]", 0, "hello, world", '\0'},
	{ .fmt="[%20s]", "[        hello, world]", 0, "hello, world", '\0'},
	{ .fmt="[%~s]", "[olleh]", 0, "hello", '\0'},
	{ .fmt="[%~20s]", "[               olleh]", 0, "hello", '\0'},
	{ .fmt="[%~-20s]", "[olleh               ]", 0, "hello", '\0'},
	{ .fmt="[%~-20.3s]", "[oll                 ]", 0, "hello", '\0'},

	{ .fmt="[%4.6c]", "[   A]", 0, NULL, 'A'},
	{ .fmt="[%4.0c]", "[   A]", 0, NULL, 'A'},
	{ .fmt="[%-20c]", "[A                   ]", 0, NULL, 'A'},

	{ .fmt="[%10.6b]", "[    010000]", 16, NULL, 0},
	{ .fmt="[%d]", "[15]", 15, NULL, 0},
	{ .fmt="[%x]", "[f]", 15, NULL, 0},
	{ .fmt="[%X]", "[F]", 15, NULL, 0},
	{ .fmt="[%b]", "[1111]", 15, NULL, 0},
	{ .fmt="[%+10x]", "[  ffffffff]", -1, NULL, 0},

};

int main(void)
{
	
	int i;
	int len = sizeof(dt) / sizeof(test_data_t);

	printf("### Start Test ##################\n"); fflush(NULL);

	for (i=0; i<len; i++) {
		test_data_t *d = &dt[i];
		msg_idx = 0;
		printf("%02d: mini_printf: ", i+1); fflush(NULL);

		if (d->int_data != 0)
			mini_printf(d->fmt, d->int_data);
		else if (d->str_data != NULL)
			mini_printf(d->fmt, d->str_data);
		else if (d->char_data != 0)
			mini_printf(d->fmt, d->char_data);
		else  {
			printf("Unknown data type !!! \n");
			fflush(NULL);
			continue;
		}

		check_result(d->fmt, d->result);
	}

	printf("\nTest Result: testing case:%d, fail:%d \n", test_cnt, fail_cnt);
	fflush(NULL);

#if 0
	// for referencing output
	printf("##################\n"); fflush(NULL);
	printf("##################\n"); fflush(NULL);
	printf("[%10d]\n", 169); //      [       169]
	printf("[%010d]\n", 169); //     [0000000169]
	printf("[%+010d]\n", 169); //    [+000000169]
	printf("[%10.5d]\n", 169); //    [     00169]
	printf("[%+10.5d]\n", 169); //   [    +00169]
	printf("[%+010.5d]\n", 169); //  [    +00169]
	printf("[%+010.05d]\n", 169); // [    +00169]

	printf("##################\n"); fflush(NULL);

	printf("[%10d]\n", -169); //     [      -169]
	printf("[%010d]\n", -169); //    [-000000169]
	printf("[%+010d]\n", -169); //   [-000000169]
	printf("[%10.5d]\n", -169); //   [    -00169]
	printf("[%+10.5d]\n", -169); //  [    -00169]
	printf("[%+010.5d]\n", -169); // [    -00169]
	printf("[%+010.05d]\n", -169); //[    -00169]

	printf("##################\n"); fflush(NULL);

	printf("[%-10d]\n", 169); //      [169       ]
	printf("[%-010d]\n", 169); //     [169       ]
	printf("[%+-010d]\n", 169); //    [+169      ]
	printf("[%-10.5d]\n", 169); //    [00169     ]
	printf("[%+-10.5d]\n", 169); //   [+00169    ]
	printf("[%+-010.5d]\n", 169); //  [+00169    ]
	printf("[%+-010.05d]\n", 169); // [+00169    ]

	printf("##################\n"); fflush(NULL);

	printf("[%-10d]\n", -169); //     [-169      ]
	printf("[%-010d]\n", -169); //    [-169      ]
	printf("[%+-010d]\n", -169); //   [-169      ]
	printf("[%-10.5d]\n", -169); //   [-00169    ]
	printf("[%+-10.5d]\n", -169); //  [-00169    ]
	printf("[%+-010.5d]\n", -169); // [-00169    ]
	printf("[%+-010.05d]\n", -169); //[-00169    ]
#endif

	return 0;
}

/*
 * antlabs feedback
 *
 * Your solution contains at least the following issues:
 * Not able to  handle a 64-bit number on a 64-bit platform for wrong types used in some functions. (e.g. int in get_integer_data)
 * Residue code injected for the testing purpose causes crash for an input that produces long output (e.g. msg_buf)
 * Prohibited/unnecessary use of library functions (e.g. malloc.) 
 * Naive use of the output function (e.g. write of a single character)
 *
 */
