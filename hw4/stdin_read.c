#include <unistd.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char** argv) {
    long ret;
    char ch;
    long len=1;
    printf("使用 'syscall' 呼叫system call\n");
    __asm__ volatile ( 
        "mov $0, %%rax\n"   //system call number
        "mov $0, %%rdi\n"   //stderr
        "mov %1, %%rsi\n"   //
        "mov %2, %%rdx\n"
        "syscall\n"
        "mov %%rax, %0\n"
        :  "=m"(ret)
        : "g" (&ch), "g" (len)
        : "rax", "rbx", "rcx", "rdx");
    printf("回傳值是：%ld\n", ret);
    printf("讀入的字元為\" %c \"\n",ch);
}
