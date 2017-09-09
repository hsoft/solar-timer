#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <Python.h>
#include "../src/solartimer.h"
#include "../common/sim.h"

static volatile sig_atomic_t should_stop = 0;

void handle_signal(int sig)
{
    should_stop = 1;
}

int main(void)
{
    PyGILState_STATE gilState;
    FILE *fp;

    signal(SIGINT, handle_signal);

    printf("Initializing...\n");
    Py_Initialize();
    gilState = PyGILState_Ensure();
    fp = fopen("circuit.py", "r");
    PyRun_SimpleFile(fp, "circuit.py");
    fclose(fp);
    PyGILState_Release(gilState);

    solartimer_setup();

    while (!should_stop) {
        if (timer0_interrupt_check()) {
            solartimer_timer0_interrupt();
        }
        if (timer1_interrupt_check()) {
            solartimer_timer1_interrupt();
        }
        solartimer_loop();
        _delay_ms(5);
    }

    sim_stop();
    Py_Finalize();
}
