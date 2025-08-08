#include "i8080.h"


int
main(int argc, char *argv[])
{
        i8080 cpu;
        init(&cpu, argv[1]);
        emulate(&cpu);

        return 0;
}