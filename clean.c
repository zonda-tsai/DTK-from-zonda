#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define ARR_LEN 100001
#define SYSTEM(x)\
	if(system(x))\
		printf("Failed to make %s...\n", x);
const char *home = NULL;

int clean(char*);
int clean_all();

int main(int argc, char* argv[]){
	home = getenv("HOME");
	if(home == NULL){
		printf("Environment Variable \"HOME\" haven't been defined!\n");
		return 1;
	}
	if(argc == 1)
		return clean_all();
	else if(argc == 2 && argv[1][0] == '-'){
		if(strlen(argv[1]) >= 4 && strncmp(&argv[1][strlen(argv[1]) - 4], "-prj", 4) == 0){
			printf("Invalid type!\n");
			return 1;
		}
		return clean(argv[1]);
	}
	printf("Invalid input!\n");
	return 1;
}

int clean(char *type){
	char *path = NULL, *path2 = NULL, *cmd = NULL;
	path = malloc(ARR_LEN);
	if(path == NULL){
		printf("Failed to allocate memory...\n");
		return 1;
	}
	path2 = malloc(ARR_LEN);
	if(path2 == NULL){
		free(path);
		printf("Failed to allocate memory...\n");
		return 1;
	}
	cmd = malloc(ARR_LEN);
	if(cmd == NULL){
		free(path);
		free(path2);
		printf("Failed to allocate memory...\n");
		return 1;
	}
	snprintf(path, ARR_LEN - 1, "%s/.zonda.ide/makefiles/makefile%s", home, type);
	snprintf(path2, ARR_LEN - 1, "%s-prj", path);
	if(access(path, R_OK)){
		printf("Failed to get %s\n", path);
		free(path);
		free(path2);
		free(cmd);
		return 1;
	}
	snprintf(cmd, ARR_LEN - 1, "make -f %s clean", path);
	SYSTEM(cmd);
	if(!access(path2, R_OK)){
		snprintf(cmd, ARR_LEN - 1, "make -f %s clean", path2);
		SYSTEM(cmd);
	}
	free(path);
	free(path2);
	free(cmd);
	return 0;
}

int clean_all(){
	char *path = NULL, *makefile = NULL, *cmd = NULL;
	path = malloc(ARR_LEN);
	if(path == NULL){
		printf("Failed to allocate memory...\n");
		return 1;
	}
	makefile = malloc(ARR_LEN);
	if(makefile == NULL){
		free(path);
		printf("Failed to allocate memory...\n");
		return 1;
	}
	snprintf(path, ARR_LEN - 1, "%s/.zonda.ide/makefiles", home);
	DIR *dir = opendir(path);
	if(dir == NULL){
		free(path);
		free(makefile);
		printf("Failed to open ~/.zonda.ide/makefiles...\n");
		return 1;
	}
	struct dirent *entry;
	cmd = malloc(ARR_LEN);
	if(cmd == NULL){
		closedir(dir);
		free(path);
		free(makefile);
		printf("Failed to allocate memory...\n");
		return 1;
	}
	while((entry = readdir(dir)) != NULL){
		if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;
		snprintf(makefile, ARR_LEN - 1, "%s/%s", path, entry->d_name);
		snprintf(cmd, ARR_LEN - 1, "make -f %s clean", makefile);
		SYSTEM(cmd);
	}
	closedir(dir);
	free(path);
	free(makefile);
	free(cmd);
	return 0;
}
