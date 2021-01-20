#include <stdlib.h> /* system */
#include <stdio.h> /*  perror, printf */
#include <unistd.h> /* fork, exec */
#include <sys/types.h> /* pid_t */
#include <assert.h> /* asssert */

static int TestMyServer(char *server, char *tcp, char *udp);
static int RunProgram(const char *prog);

int main(int argc, char *argv[])
{
    assert(3 < argc);
    return TestMyServer(argv[1], argv[2], argv[3]);
}

static int TestMyServer(char *server, char *tcp, char *udp)
{
    RunProgram(server);
    sleep(3);
    RunProgram(tcp);
    sleep(3);
    RunProgram(udp);
    sleep(3);
    return 0;
}

static int RunProgram(const char *prog)
{
    pid_t child = fork();
    switch (child)
    {
    case -1:
        perror(prog);
        break;
    case 0:
        if (-1 == execl(prog, prog, NULL))
        {
            perror(prog);
        }
        break;
    default:
        printf("%s started\n", prog);
        break;
    }
    return 0;
}