#include "solartimer.h"

int main(void)
{
    solartimer_setup();

    while (1) {
        solartimer_loop();
    }
}
