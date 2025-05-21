#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		printf("Invalid form...\n");
		return 1;
	}
	if(access(argv[1], R_OK)){
		printf("Unreadable file...\n");
		return 1;
	}
	FILE *f;
	f = fopen(argv[1], "r");
	if(f == NULL){
		printf("Failed to open file!\n");
		return 1;
	}
	int i;
	char temp[1000001] = {0};
	while(fgets(temp, 1000000, f) != NULL){
		for(i = 0 ; temp[i] != 0 ; i++){
			if(temp[i] == '\n')
				printf("\e[34m\\n\e[0m");
			else if(isspace(temp[i]))
				printf("\e[100m%c\e[0m", temp[i]);
			else if(!isalpha(temp[i]) && !isspace(temp[i]) && !isdigit(temp[i]) && !ispunct(temp[i]))
				printf("\e[41m%c(ASCII: %d)\e[0m", temp[i], temp[i]);
			else
				putchar(temp[i]);
		}
		printf("\e[34m\\0\e[0m\n");
	}
	return 0;
}
