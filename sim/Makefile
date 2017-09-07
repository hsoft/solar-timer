OBJS = main.o
OBJS += $(addprefix ../src/, solartimer.o)
OBJS += $(addprefix ../common/, sim_pin.o sim_timer.o sim_adc.o)
PROGNAME = solartimer

INCS = `pkg-config --cflags python-3.4`
LIBS = `pkg-config --libs python-3.4`

EXTRACFLAGS ?= 
CFLAGS = $(INCS) -DSIMULATION -Wall $(EXTRACFLAGS) -c
LDFLAGS = $(LIBS)

# Patterns

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Rules

.PHONY: clean

all: $(PROGNAME)
	@echo Done!

clean:
	rm -f $(OBJS) $(PROGNAME)

$(PROGNAME): $(OBJS)
	$(CC) $(LDFLAGS) $+ -o $@

