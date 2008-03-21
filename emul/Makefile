sources = $(wildcard GUI/*.c)
objects = $(sources:.c=.o)

xemul: $(objects) xemul.o
	gcc -o xemul $^ -L/usr/X11R6/lib -lX11

emul_w32: $(objects) emul_w32.o
	gcc -o emul_w32 $^ -lwin32k -lgdi32

%.o: %.c
	gcc -c -o $@ $<
	
clean:
	rm -rf $(objects) *.o xemul emul_w32.exe
