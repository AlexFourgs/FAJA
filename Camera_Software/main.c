#include "tracking.h"

// Ligne de compilation : gcc -o main main.c tracking.c `pkg-config --cflags opencv --libs opencv` -ldl -lm

int main (int argc, char* argv[]) {

	imageTreatement();

	return 0 ;
}
