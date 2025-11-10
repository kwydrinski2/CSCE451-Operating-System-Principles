#include <stdio.h>

// Reference:
// https://www.geeksforgeeks.org/c/basics-file-handling-c/

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("ERROR\n");
	}
	FILE *file = fopen(argv[1], "r");
	if(file == NULL) {
		printf("ERROR\n");
		return 0;
	}
	char ch;
	while ((ch = fgetc(file)) != EOF) {
		printf("%c", ch);
	}
	fclose(file);
	return 0;
}

