
1. 문제 정의:
http://online.antlabs.com/antlabs-kr/hiring/index.php

2. repository:
https://github.com/jhjgithub/mini_printf

3. 개발 환경
-. ubuntu-17.04
-. gcc version 6.3.0 20170406 (Ubuntu 6.3.0-12ubuntu2) 
-. binary dependency - No additional library
[jjh@ubuntu ~/src/mini_printf]$ ldd mini_printf 
        linux-vdso.so.1 =>  (0x00007ffd1d122000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f0233dc1000)
        /lib64/ld-linux-x86-64.so.2 (0x000056090dd4a000)

4. 빌드
-. make clean all
-. make run : test case 실행
-. 실행 결과 예: Test Result: testing case:52, fail:0 

