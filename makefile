run: main
	./main.sh 
	
valgrind: main.o ip_lib.o bmp.o
	gcc main.o ip_lib.o bmp.o -omain -Wall --ansi --pedantic -ggdb -lm -g -O1 
	valgrind -v --leak-check=full --track-origins=yes ./main
	
main: main.o ip_lib.o bmp.o
	gcc main.o ip_lib.o bmp.o -omain -Wall --ansi --pedantic -lm -g3 -O3 -fsanitize=address -fsanitize=undefined -std=gnu89 -Wextra 
	
main.o: main.c ip_lib.h
	gcc main.c -omain.o -c -Wall --ansi --pedantic -ggdb -g3 -O3 -fsanitize=address -fsanitize=undefined -std=gnu89 -Wextra
	
ip_lib.o: ip_lib.c ip_lib.h
	gcc ip_lib.c -oip_lib.o -c -Wall --ansi --pedantic -ggdb -g3 -O3 -fsanitize=address -fsanitize=undefined -std=gnu89 -Wextra

test_bmp: testbmp
	./testbmp
	
testbmp: test_bmp.o bmp.o
	gcc test_bmp.o bmp.o -otestbmp -Wall -lm
	
test_bmp.o: test_bmp.c
	gcc test_bmp.c -otest_bmp.o -Wall -c
	
bmp.o: bmp.c bmp.h
	gcc bmp.c -obmp.o -Wall -c
	
clean:
	rm *.o main
	rm -R BMP
	clear
