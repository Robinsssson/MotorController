CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar
CFLAGS = -std=c89 -mcpu=cortex-m3 -mthumb

PID_STATIC = libPID.a
PID_OBJECT = PID.o
all: static
	@echo "CC is set to: $(CC)"
	@echo "AR is set to: $(AR)"

.c.o:
	$(CC) -c $(CFLAGS) $<

static: $(PID_STATIC)

$(PID_STATIC): $(PID_OBJECT)
	$(AR) rcs $@ $<
