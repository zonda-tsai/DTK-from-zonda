#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

const char *home = NULL;

const char* file_type(const char* file){
	if(file == NULL)
		return "";
	int i;
	for(i = strlen(file) - 1 ; i >= 0 && file[i] != '.' ; i--);
	if(i == -1 || file[i + 1] == 0)
		return "";
	return (file + i + 1);
}

char allocate(char** a){
	if(*a != NULL)
		free(*a);
	*a = malloc(10001);
	if(*a == NULL){
		printf("Failed to allocate memory...\n");
		return 0;
	}
	return 1;
}

char copy(const char* file, const char* new_file){
	FILE *r, *w;
	r = fopen(file, "r");
	if(r == NULL)
		return 0;
	w = fopen(new_file, "w");
	if(w == NULL)
		return 0;
	char temp[100001] = {0};
	while(fgets(temp, 100000, r) != NULL)
		fputs(temp, w);
	fclose(r);
	fclose(w);
	return 1;
}

int main(int argc, char* argv[]){
	home = getenv("HOME");
	if(home == NULL){
		printf("Environmental Variables \"HOME\" Undefined!\n");
		return 1;
	}
	char *path = NULL, *file = NULL;
	if(!allocate(&path)) return 1;
	if(!allocate(&file)){free(path); return 1;}
	snprintf(path, 10001, "%s/.zonda.dtk/templates", home);
	struct stat st;
	if(stat(path, &st) || (!stat(path, &st) && !S_ISDIR(st.st_mode))){
		printf("Failed to open %s\n", path);
		free(path);
		return 1;
	}
	int i;
	const char* type;
	for(i = 1 ; i < argc ; i++){
		if(strcmp((type = file_type(argv[i])), "") == 0)
			printf("Type error for %s\n", argv[i]);
		else{
			snprintf(file, 10001, "%s/%s", path, type);
			if(access(file, R_OK)){
				printf("Failed to access file %s...\n", file);
				continue;
			}
			if(!copy(file, argv[i])){
				printf("Failed to + %s...\n", argv[i]);
			}
		}
	}
	free(path);
	free(file);
	return 0;
}
