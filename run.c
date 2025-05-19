#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void err_ptr(){
	printf("Failed to allocate memory...\n");
	exit(1);
}

const char* file_type(const char *file){
	if(file == NULL || file[0] == '\0') return "";
	int i;
	for(i = strlen(file) - 1 ; i >= 0 && file[i] != '.' ; i--);
	if(i == -1 || i == (int)strlen(file) - 1) return "";
	return file + i + 1;
}

char* file_name(const char *file){
	if(file == NULL || file[0] == '\0') return NULL;
	int i;
	char *temp;
	for(i = strlen(file) - 1 ; i >= 0 && file[i] != '.'; i--);
	if(i == -1 || i == (int)strlen(file) - 1) return NULL;
	temp = malloc(i + 1);
	if(temp == NULL) err_ptr();
	strncpy(temp, file, i);
	temp[i] = 0;
	return temp;
}

int cmp(const void *t1, const void *t2){
	const char *a = *(const char**)t1;
	const char *b = *(const char**)t2;
	return (strcmp(file_type(a), file_type(b)) == 0) * strcmp(a, b) + strcmp(file_type(a), file_type(b));
}

void sort(char **arr, int n){
	qsort(arr, n, sizeof(char*), cmp);
}

char** dir_content(){
	DIR *dir = opendir(".");
	if(dir == NULL){
		printf("Failed to open current dir.\n");
		exit(1);
	}
	struct dirent *entry;
	struct stat st;
	char **temp;
	int i = 0, n = 0;
	while((entry = readdir(dir)) != NULL){
		if((strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) || stat(entry->d_name, &st) != 0 || (S_ISDIR(st.st_mode) && strcmp(file_type(entry->d_name), "prj") != 0))
			continue;
		n++;
	}
	if(n == 0){
		closedir(dir);
		return NULL;
	}
	temp = malloc((n + 1) * sizeof(char*));
	if(temp == NULL) err_ptr();
	rewinddir(dir);
	while((entry = readdir(dir)) != NULL && i < n){
		if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || stat(entry->d_name, &st) != 0 || (S_ISDIR(st.st_mode) && strcmp(file_type(entry->d_name), "prj") != 0))
			continue;
		temp[i] = malloc(strlen(entry->d_name) + 1);
		if(temp[i] == NULL){
			closedir(dir);
			i--;
			while(i--) free(temp[i]);
			free(temp);
			err_ptr();
		}
		strcpy(temp[i], entry->d_name);
		i++;
	}
	temp[i] = NULL;
	closedir(dir);
	sort(temp, i);
	return temp;
}

void System(const char* file, int test){
	if(file == NULL || access(file, F_OK)) return;
	printf("\e[33;1m=====%s=====\e[0m\n", file);
	char *cmd, *pure_name = file_name(file);
	if(pure_name == NULL){
		pure_name = malloc(strlen(file) + 1);
		if(pure_name == NULL) err_ptr();
		strcpy(pure_name, file);
	}
	if(!access(file, R_OK)){
		FILE *f;
		f = fopen(file, "r");
		if(f == NULL){
			printf("Cannot find %s\n", file);
			return;
		}
		char shebang[10001] = {0};
		fgets(shebang, 10000, f);
		if(strlen(shebang) > 0 && shebang[strlen(shebang) - 1] == '\n')
			shebang[strlen(shebang) - 1] = 0;
		if(strlen(shebang) > 2 && shebang[0] == '#' && shebang[1] == '!'){
			cmd = malloc(strlen(shebang) + strlen(file));
			if(cmd == NULL){
				if(f != NULL)
					fclose(f);
				err_ptr();
			}
			snprintf(cmd, strlen(shebang) + strlen(file), "%s %s", shebang + 2, file);
		}
		else if(!access(pure_name, X_OK)){
			fclose(f);
			cmd = malloc(strlen(pure_name) + 3);
			if(cmd == NULL) err_ptr();
			snprintf(cmd, strlen(pure_name) + 3, "./%s", pure_name);
		}
		else{
			if(pure_name != NULL) free(pure_name);
			printf("**\"%s\" is not an executable file**\n", file);
			fclose(f);
			return;
		}
		if(f != NULL)
			fclose(f);
		if(test){
			char *ins;
			if(pure_name == NULL){
				if(cmd != NULL)
					free(cmd);
				exit(1);
			}
			ins = realloc(pure_name, strlen(pure_name) + 6);
			if(ins == NULL){
				if(cmd != NULL)
					free(cmd);
				free(pure_name);
				err_ptr();
			}
			pure_name = ins;
			strcat(pure_name, ".test");
			if(!access(pure_name, R_OK)){
				ins = realloc(cmd, strlen(cmd) + strlen(pure_name) + 4);
				if(ins == NULL){
					if(cmd != NULL)
						free(cmd);
					free(pure_name);
					err_ptr();
				}
				cmd = ins;
				strcat(cmd, " < ");
				strcat(cmd, pure_name);
			}
		}
		system(cmd);
		free(cmd);
	}
	else
		printf("File cannot be executed <try chmod +x %s> or its not executable file\n", file);
	free(pure_name);
}

void free_content(char **content){
	int i = 0;
	if(content != NULL){
		while(content[i] != NULL){
			free(content[i]);
			i++;
		}
		free(content);
	}
}

int main(int argc, char* argv[]){
	int i = 0;
	char **content = dir_content();
	if(content == NULL) err_ptr();
	if(argc == 1 || (argc == 2 && strcmp(argv[1], "-test") == 0)){
		while(content[i] != NULL){
			System(content[i], argc - 1);
			i++;
		}
		free_content(content);
		return 0;
	}
	if(strcmp(argv[1], "-test") == 0 || argv[1][0] != '-'){
		for(i = 1 + (strcmp(argv[1], "-test") == 0) ; i < argc ; i++)
			System(argv[i], strcmp(argv[1], "-test") == 0);
		free_content(content);
		return 0;
	}
	char type[strlen(argv[1])];
	strcpy(type, argv[1] + 1);
	if(argc == 2 || (argc == 3 && strcmp(argv[2], "-test") == 0)){
		i = 0;
		while(content[i] != NULL){
			if(strcmp(type, file_type(content[i])) == 0)
				System(content[i], argc - 2);
			i++;
		}
		free_content(content);
		return 0;
	}
	if(argc > 2 && strcmp(argv[2], "-test") != 0)
		for(i = 2 ; i < argc ; i++)
			System(argv[i], 0);
	else if(argc > 3 && strcmp(argv[2], "-test") == 0)
		for(i = 3 ; i < argc ; i++)
			System(argv[i], 1);
	else{
		printf("Invalid input!\n");
		free_content(content);
		return 1;
	}
	free_content(content);
	return 0;
}
