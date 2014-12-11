CC=gcc
CFLAGS+=-fopenmp -D__unix__

controller: AAAA_CONTROL_LOOP_IMPL.o AAAA_SHARED_IMPL.o AAAA_TEMPERATURE_IMPL.o AIBV_plant_IMPL.o Common.o MainEntry.o
	gcc AAAA_CONTROL_LOOP_IMPL.o AAAA_SHARED_IMPL.o AAAA_TEMPERATURE_IMPL.o AIBV_plant_IMPL.o Common.o MainEntry.o -lgomp -o controller

all: controller

clean:
	rm -rf *.o controller

