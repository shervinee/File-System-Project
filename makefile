all: p5a p5b
p5a: p5a.o helperFunctions.o P5aHeaders.h
	gcc p5a.o helperFunctions.o -o p5a
p5b: p5b.o Funcs.o P5bHeaders.h
	gcc p5b.o Funcs.o -o p5b
helperFunctions.o: helperFunctions.c P5aHeaders.h
	gcc -c helperFunctions.c
Func.o: Funcs.c P5bHeaders.h
	gcc -c Funcs.c
clean: 
	rm -f p5a p5b *.o


