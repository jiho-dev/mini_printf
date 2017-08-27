http://online.antlabs.com/antlabs-kr/hiring/index.php

Hands-on coding test

You can skip this part if you submit the source code to your own project or program and move on to Login/Registration. If you don't have your own project or program that you can share with us, you can write and submit a small program to fullfil the requirement given in this hands-on coding test. No Korean translation is provided for the coding test in the hope that an applicant can read and understand the technical documents written in English although good verbal and written English communication skill is optional.

자신의 프로젝트나 프로그램의 소스코드를 제출하실 분은 이 부분을 건너뛰고 Login/Registration 으로 진행하면 됩니다. 저희와 공유할 수 있는 자신의 프로젝트나 프로그램이 없다면 이 코딩테스트에 주어진 요구사항을 만족시키는 작은 프로그램을 작성하여 제출하시면 됩니다. 코딩테스트는 한국어 번역을 제공하지 않습니다. 이는 지원자가 영어로 된 기술문서를 읽고 이해할 수준이 되었으면 좋겠다는 의미에서 그런것이지 영어로 말을 잘 하거나 문서를 작성하는 능력을 요구하는것은 아닙니다.

Write your own printf function in C.

Printf is a famous formatted output function provided by the standard C library. The standard printf function accepts a formatting string followed by zero or more variadic arguments to replace formatting specifiers in the formatting string. You are required to implement a subset printf function compliant to the following specification. The specification is mostly a subset of the standard printf function but includes two extensions that don't exist in the standard.

void mini_printf (const char* format, …);
The format string is a character string that may be composed of ordinary characters and format specifiers.

format specifier := %[flags][width][.precision][type-prefix]type
where flag := { + | - | 0 }
      flags := one or more flag
      width := a decimal number
      precision := a decimal number
      modifier := { h | l }
      type := { c | s | S | d | x | X | % }

[] indicates as optional.
An oridinary character is printed verbatim. A format specifier is replaced by a corresponding argument following the format string. A format specifier begins with a percent sign and ends when a valid type specifier is encountered. if no valid type specifier is encountered at the position where a type specifier is expected, the format specifier ends but the whole invalid format specifier segment is printed verbatim.

type

type specifies the type of the corresponding argument to a format specifier. It determines to print the argument as a character, a string, or a number.

c - take an argument and print it as a character
s - take an argument and print it as a character string
b - take an argument and print it as a binary number
d - take an argument and print it as a decimal number
x - take an argument and print it as a hexadecimal number in lower-case
X - take an argument and print it as a hexadecimal number in upper-case
% - print a single percent sign. takes no argument. ignores flags, width, precision, type-prefix.
flags

flags is an optional modifier that changes output style. Valid flag characters include +, -, 0, ~. flags can be composed of one or more flag characters appearing in any order. multiple occurrences of the same flag character are allowed. The flags part starts when a valid flag character follows the percent sign immediately and ends when an invalid flag character is encountered.

+ causes a positive number to be printed with the plus sign for decimal numeric output; - left-aligns the character, string, numeric output; 0 zero-pads the left side of numeric output if the actual output width is less than width specified in the format specifier. if both - and 0 appear, only - takes effect; ~ applies to string output only and causes a string to be printed reversely.

The b, x, X type specifiers treat the corresponding integer argument as an unsigned number and the + flag character doesn't apply to these type specifiers.

width

width is an optional decimal number that specifies the minimum number of characters to be printed for character, string, numeric output. It cannot begin with 0 as 0 is used as as flag character.

If the actual output width is less than width and no - flag is specified, the output is right-aligned.

precision

precision is an optional decimal number that is started by a period sign. it specifies the maximum number of charachters to be printed for string output and the minimum number of digits to be printed for numeric output. it doesn't apply to the character output. you can assume that precision is always 1 for character output.

type-prefix

type-prefix is an optional single character that specifies the size of the corresponding argument to the format specifier. For an integer argument, h takes it as short int and l takes it as long int. It is meaningful only if type is one of b, d, x, X. It is ignored for character and string output.

See these sample calls and outputs for clarification:

mini_printf ("hello, world\n");                      hello, world
mini_printf ("[%99% %T %d]\n", 20);                  [% %T 20]
mini_printf ("[%d %x %X %b]\n", 15, 15, 15, 15);     [15 f F 1111]
mini_printf ("[%+010ld]\n", 999l);                   [+000000999]
mini_printf ("[%+010.005ld]\n", -999l);              [    -00999]
mini_printf ("[%10.6b]\n", 16);                      [    010000]
mini_printf ("[%.6s]\n", "hello, world");            [hello,]
mini_printf ("[%10.6s]\n", "hello, world");          [    hello,]
mini_printf ("[%4.6s]\n", "hello, world");           [hello,]
mini_printf ("[%4.0s]\n", "hello, world");           [    ]
mini_printf ("[%9s]\n", "hello, world");             [hello, world]
mini_printf ("[%-20s]\n", "hello, world");           [hello, world        ]
mini_printf ("[%20s]\n", "hello, world");            [        hello, world]
mini_printf ("[%~s]\n", "hello");                    [olleh]
mini_printf ("[%~20s]\n", "hello");                  [               olleh]
mini_printf ("[%~-20s]\n", "hello");                 [olleh               ]
mini_printf ("[%~-20.3s]\n", "hello");               [oll                 ]
mini_printf ("[%4.6c]\n", 'A');                      [   A]
mini_printf ("[%4.0c]\n", 'A');                      [   A]
mini_printf ("[%-20c]\n", 'A');                      [A                   ]
mini_printf ("[%+10x]\n", -1);                       [  ffffffff]
The following restrictions apply:

Never use library functions except a system call to write output to the console. (e.g. write defined in unistd.h or WriteFile/WriteConsole on win32/win64)
Allowed to use macros and types defined in stdarg.h to handle variable arguments in the variadic function.
Write your own function if you need one. (e.g. my_strlen instead of strlen)
You may cross-check the output of mini_printf against the standard printf's output except the b type specifier and the ~ flag character.

Organize the files to submit this way.

prtfmt.c - put main implementation code of mini_printf
prtfmt.h - declare the prototype of mini_printf. make it usable for both C and C++.
test.c - put your test cases here. it's ok to use external libraries when writing test cases.
Makefile - put build procedure that produces a static library containing mini_printf and the test program that links against the library
Store these files into a single compressed archive file(.tar.gz or .zip) and upload it.

