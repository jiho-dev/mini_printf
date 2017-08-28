
#include <stdio.h>
#include <string.h>
#include "prtfmt.h"

int test_cnt = 0;
int fail_cnt = 0;

void check_result(char *fmt, char *ret) {

	int len;
	
	len = strlen(ret);
	test_cnt ++;

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
	} test_data_t;

	struct dt_s dt[2] = {
		{ .fmt="[%10d]", "[       169]", 169, NULL},
		{ .fmt="[%10d]", "[       169]", 169, NULL},

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
		printf("mini_printf: "); fflush(NULL);
		mini_printf(d->fmt, d->int_data);
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

