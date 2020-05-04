test_bmp: testbmp
	./testbmp
	
testbmp: test_bmp.o bmp.o
	gcc test_bmp.o bmp.o -otestbmp -Wall -lm
	
test_bmp.o: test_bmp.c
	gcc test_bmp.c -otest_bmp.o -Wall -c
	
bmp.o: bmp.c bmp.h
	gcc bmp.c -obmp.o -Wall -c
	
clean:
	rm *.o testbmp mandelbrot.bmp
