#include <stdio.h>

int main(int argc, char *argv[]) {

  if (argc - 1 <= 1 || argc - 1 > 2){
	printf("Error: incorrect number of arguments (%d specified).\n", argc -1);
	return 1;
  }
  return 0;
} 
