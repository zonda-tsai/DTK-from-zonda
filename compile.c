#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

const char *home = NULL;

const char* file_type(const char *a){
	if(a == NULL || a[0] == 0)
		return "";
	int i;
	for(i = strlen(a) - 1 ; i >= 0 ; i--)
		if(a[i] == '.')
			break;
	if(i == -1 || i == (int)strlen(a) - 1)
		return "";
	return a + i + 1;
}

char* file_name(const char *a){
	if(a == NULL || a[0] == 0)
		return NULL;
	int i;
	char *temp;
	for(i = strlen(a) - 1 ; i >= 0 ; i--)
		if(a[i] == '.')
			break;
	if(i == -1 || i == 0){
		temp = malloc(1);
		if(temp == NULL)
			return NULL;
		strcpy(temp, "");
		return temp;
	}
	temp = (char*)malloc(i + 1);
	if(temp == NULL)
		return NULL;
	strncpy(temp, a, i);
	temp[i] = 0;
	return temp;
} // Need to free()

char isDir(const char *a){
	struct stat st;
	if(stat(a, &st))
		return 0;
	if(S_ISDIR(st.st_mode))
		return 1;
	return 0;
}

char makefile(const char* type){
	if(home == NULL)
		return 0;
	char a, b;
	char base[strlen(home) + 50];
	snprintf(base, strlen(home) + 31, "%s/.zonda.dtk/makefiles/makefile", home);
	int len = strlen(base) + strlen(type);
	char makefile_normal[len + 1];
	char makefile_project[len + 5];
	snprintf(makefile_normal, len + 1, "%s%s", base, type);
	snprintf(makefile_project, len + 5, "%s%s-prj", base, type);
	a = (access(makefile_normal, F_OK) == 0);
	b = (access(makefile_project, F_OK) == 0);
	return a + 2 * b;
}

char make(const char* type, const char* name){
	int n = (name == NULL) ? 0 : strlen(name);
	char base[10001] = {0};
	snprintf(base, 10001, "make -f %s/.zonda.dtk/makefiles/makefile", home);
	char cmd[strlen(base) + strlen(type) + n + 2];
	if(n == 0)
		snprintf(cmd, strlen(base) + strlen(type) + 1, "%s%s", base, type);
	else
		snprintf(cmd, strlen(base) + strlen(type) + n + 2, "%s%s %s", base, type, name);
	return (system(cmd) == 0);
}

int make_all(){
	char *path;
	path = malloc(10001);
	if(path == NULL){
		printf("Failed to allocate memory...\n");
		return 1;
	}
	snprintf(path, 10001, "%s/.zonda.dtk/makefiles", home);
	DIR *dir = opendir(path);
	if(dir == NULL){
		printf("Failed to open %s...\n", path);
		free(path);
		return 1;
	}
	struct dirent *entry;
	char *cmd = NULL, *makefile = NULL;
	makefile = malloc(10001);
	if(makefile == NULL){
		printf("Failed to allocate memory...\n");
		free(path);
		closedir(dir);
		return 1;
	}
	cmd = malloc(10001);
	if(cmd == NULL){
		printf("Failed to allocate memory...\n");
		free(path);
		closedir(dir);
		free(makefile);
		return 1;
	}
	while((entry = readdir(dir)) != NULL){
		if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;
		snprintf(makefile, 10000, "%s/%s", path, entry->d_name);
		snprintf(cmd, 10000, "make -f %s", makefile);
		if(system(cmd))
			printf("Failed to make : %s\n", entry->d_name);
	}
	closedir(dir);
	free(path);
	free(makefile);
	free(cmd);
	return 0;
}

int main(int argc, char* argv[]){
	home = getenv("HOME");
	if(home == NULL){
		printf("Environment Variable \"HOME\" haven't been defined!\n");
		exit(1);
	}
	char ins;
	int i;
	for(i = 1 ; i < argc ; i++){
		if(!strcmp(argv[i], "clean") == 0)
			break;
		printf("Invalid keyword 'clean' included...\n");
		return 1;
	}
	if(argc == 1)
		return make_all();
	else if(argc == 2 && argv[1][0] == '-'){
		if(strlen(argv[1]) > 4 && strncmp(argv[1] + strlen(argv[1]) - 4, "-prj", 4) == 0){
			printf("Invalid form...\n");
			return 1;
		}
		ins = makefile(argv[1]);
		char temp[strlen(argv[1]) + 5];
		snprintf(temp, strlen(argv[1]) + 5, "%s-prj", argv[1]);
		switch(ins){
		case 0:
			printf("~/.zonda.dtk/makefiles/makefile%s doesn't exist...\n", argv[1]);
			printf("~/.zonda.dtk/makefiles/makefile%s-prj doesn't exist...\n", argv[1]);
			return 1;
		case 1:
			make(argv[1], NULL);
			break;
		case 2:
			make(temp, NULL);
			break;
		case 3:
			make(argv[1], NULL);
			make(temp, NULL);
			break;
		}
		return 0;
	}
	else if(argc > 1 && argv[1][0] != '-'){
		char *type, *type_prj, *name, ins;
		for(i = 1 ; i < argc ; i++){
			name = file_name(argv[i]);
			if(strcmp(name, "") == 0){
				printf("Unable to compile %s...\n", argv[i]);
				continue;
			}
			if(name == NULL){
				printf("Failed to allocate memory...\n");
				return 1;
			} 
			type = malloc(strlen(file_type(argv[i])) + 2);
			if(type == NULL){
				printf("Failed to allocate memory...\n");
				free(name);
				return 1;
			}
			type_prj = malloc(strlen(file_type(argv[i])) + 6);
			if(type_prj == NULL){
				printf("Failed to allocate memory...\n");
				free(type);
				free(name);
				return 1;
			}
			snprintf(type, strlen(argv[i]) + 2, "-%s", file_type(argv[i]));
			snprintf(type_prj, strlen(argv[i]) + 6, "-%s-prj", file_type(argv[i]));
			ins = makefile(type);
			if(isDir(argv[i]) && ins < 2)
				printf("~/.zonda.dtk/makefiles/makefile%s-prj doesn't exist...\n", type);
			else if(isDir(argv[i]))
				make(type_prj, name);
			else if(!(ins & 1))
				printf("~/.zonda.dtk/makefiles/makefile%s doesn't exist...\n", type);
			else
				make(type, name);
			free(type);
			free(type_prj);
			free(name);
		}
	}
	else{
		printf("Invalid input!\n");
		return 1;
	}
	return 0;
}
