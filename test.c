
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

struct dt_s dt[] = {
#if 0
	// success
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
#endif
	// fail
	{ .fmt="[%+010.005ld]", "[    -00999]", -999l, NULL, '\0'},
#if 0
	// success
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
#endif

};

int main(void)
{
	//mini_printf("[%99% %T %hd, hex=%x, %X, %b]\n", 65534, 123, 123, 123);
	//mini_printf("short=%ld", -2);
	//mini_printf("[%9s]", "hello, world");
	//mini_printf("[%10.6s]\n", "hello, world");
	//mini_printf("[%4.6c]\n", "A");
	//mini_printf("[%4.0c]\n", "A");
	//mini_printf("[%-20c]\n", "A");
	
	//mini_printf("[%d %x %X %b]\n", 15, 15, 15, 15);

	mini_printf("### Numeric Text ##################\n");
	
	int i;
	int len = sizeof(dt) / sizeof(test_data_t);

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
	msg_idx = 0;
	fmt = "[%10d]";
	ret = "[       169]";
	//mini_printf("[%10d]\n", 169); //     [       169]
	mini_printf(fmt, 169); //     [       169]
	check_result(fmt, ret);

	mini_printf("[%10d]\n", 169); //     [       169]
	mini_printf("[%010d]\n", 169); //    [0000000169]
	mini_printf("[%+010d]\n", 169); //   [+000000169]
	mini_printf("[%10.5d]\n", 169); //   [     00169]
	mini_printf("[%+10.5d]\n", 169); //  [    +00169] 
	mini_printf("[%+010.5d]\n", 169); // [    +00169] 
	mini_printf("[%+010.05d]\n", 169); //[    +00169] 

	printf("##################\n"); fflush(NULL);
	mini_printf("[%10d]\n", -169); //     [      -169]
	mini_printf("[%010d]\n", -169); //    [-000000169]
	mini_printf("[%+010d]\n", -169); //   [-000000169]
	mini_printf("[%10.5d]\n", -169); //   [    -00169]
	mini_printf("[%+10.5d]\n", -169); //  [    -00169] 
	mini_printf("[%+010.5d]\n", -169); // [    -00169] 
	mini_printf("[%+010.05d]\n", -169); //[    -00169] 

	printf("##################\n"); fflush(NULL);

	mini_printf("[%-10d]\n", 169); //      [169       ]
	mini_printf("[%-010d]\n", 169); //     [169       ]
	mini_printf("[%+-010d]\n", 169); //    [+169      ]
	mini_printf("[%-10.5d]\n", 169); //    [00169     ]
	mini_printf("[%+-10.5d]\n", 169); //   [+00169    ]
	mini_printf("[%+-010.5d]\n", 169); //  [+00169    ]
	mini_printf("[%+-010.05d]\n", 169); // [+00169    ]

	printf("##################\n"); fflush(NULL);

	mini_printf("[%-10d]\n", -169); //     [-169      ]
	mini_printf("[%-010d]\n", -169); //    [-169      ]
	mini_printf("[%+-010d]\n", -169); //   [-169      ]
	mini_printf("[%-10.5d]\n", -169); //   [-00169    ]
	mini_printf("[%+-10.5d]\n", -169); //  [-00169    ]
	mini_printf("[%+-010.5d]\n", -169); // [-00169    ]
	mini_printf("[%+-010.05d]\n", -169); //[-00169    ]
#endif

#if 0

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

