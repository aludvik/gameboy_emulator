CC = gcc
CFLAGS =

unit_test.exe: cpu.c mem.c unit_test.c cpu.h mem.h opcodes.h types.h emu.h
	$(CC) $(CFLAGS) -o unit_test.exe cpu.c mem.c unit_test.c

clean:
	rm unit_test.exe
