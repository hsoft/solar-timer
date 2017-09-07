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

    printf("Initializing circuit\n");
    Py_Initialize();
    gilState = PyGILState_Ensure();
    fp = fopen("circuit.py", "r");
    PyRun_SimpleFile(fp, "circuit.py");
    fclose(fp);
    PyGILState_Release(gilState);

    printf("Setup\n");
    solartimer_setup();

    printf("Loop\n");
    while (!should_stop) {
        solartimer_loop();
        printf("Sleep...\n");
        _delay_ms(1000);
        sleep(1);
    }

    Py_Finalize();
}
