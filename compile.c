#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]){
	const char *home = getenv("HOME");
	if(home == NULL){
		printf("Environment Variable \"HOME\" haven't been defined!\n");
		exit(1);
	}
	char path[10001] = {0};
	int i;
	char *cmd = NULL, *temp;
	if(argc < 2){
		printf("Invalid input!\n");
		return 1;
	}
	if(argv[1][0] == '-'){
		snprintf(path, sizeof(path), "%s/.zonda.ide/makefiles/makefile%s", home, argv[1]);
		if(access(path, R_OK) != 0){
			printf("%s doesn't exists...\n", path);
			return 1;
		}
		cmd = malloc(strlen(path) + 9);
		if(cmd == NULL){
			printf("Failed on allocating memory...\n");
			return 1;
		}
		snprintf(cmd, strlen(path) + 9, "make -f %s", path);
		for(i = 2 ; i < argc ; i++){
			temp = realloc(cmd, strlen(cmd) + strlen(argv[i]) + 2);
			if(temp != NULL){
				cmd = temp;
				strcat(cmd, " ");
				strcat(cmd, argv[i]);
			}
			else{
				if(cmd != NULL)
					free(cmd);
				printf("Failed on allocating memory...\n");
				return 1;
			}
		}
		system(cmd);
	}
	else{
		printf("Incorrect form...\n");
		return 1;
	}
	free(cmd);
	return 0;
}
