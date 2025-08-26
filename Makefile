all:
	gcc -o main main.c engine.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
