#include <stdio.h>
#include <unistd.h>
#include "../src/solartimer.h"

int main(void)
{
    printf("Setup\n");
    solartimer_setup();

    printf("Loop\n");
    while (1) {
        solartimer_loop();
        printf("Sleep...\n");
        sleep(1);
    }
}
