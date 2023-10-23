#
TARGET = SOC_PacMan
#
ALT_DEVICE_FAMILY ?= soc_cv_av
SOCEDS_ROOT ?= C:/intelFPGA/17.1/embedded
HWLIBS_ROOT = C:/intelFPGA/17.1/embedded/ip/altera/hps/altera_hps/hwlib
CROSS_COMPILE = arm-linux-gnueabihf-
CFLAGS = -g -Wall -D$(ALT_DEVICE_FAMILY) -I$(HWLIBS_ROOT)/include/$(ALT_DEVICE_FAMILY) -I$(HWLIBS_ROOT)/include/
LDFLAGS = -g -Wall -lm
CC = $(CROSS_COMPILE)gcc
ARCH= arm
REMOTE_USER = ubuntu
REMOTE_HOST = 169.254.110.50
REMOTE_DIR = /home/ubuntu

build: $(TARGET)
$(TARGET): main.o ADXL345.o VGA.o PacMan.o
	$(CC) $(LDFLAGS) $^ -o $@
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

# New rule to transfer the binary to the DE1-SOC board
flash: build
	scp $(TARGET) $(REMOTE_USER)@$(REMOTE_HOST):$(REMOTE_DIR)

.PHONY: clean
clean:
	C:/intelFPGA/17.1/embedded/host_tools/cygwin/bin/rm -f $(TARGET) *.a *.o *~
