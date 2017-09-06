OBJS = main.o
OBJS += $(addprefix ../src/, solartimer.o)
OBJS += $(addprefix ../common/, sim_pin.o sim_timer.o sim_adc.o)
PROGNAME = solartimer

EXTRACFLAGS ?= 
CFLAGS = -DSIMULATION -Wall $(EXTRACFLAGS) -c

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

