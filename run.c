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
	if(temp == NULL) return NULL;
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

char isDir(const char* file){
	struct stat st;
	if(stat(file, &st))
		return 0;
	if(S_ISDIR(st.st_mode))
		return 1;
	return 0;
}

char isExecutable(char* file){
	if(strcmp(file, ".") == 0 || strcmp(file, "..") == 0)
		return 0;
	struct stat st;
	if(stat(file, &st) != 0)
		return 0;
	if(S_ISDIR(st.st_mode))
		return 0;
	if(!access(file, X_OK))
		return 1;
	if(access(file, R_OK))
		return 0;
	FILE *f;
	f = fopen(file, "r");
	if(f == NULL)
		return 0;
	char shebang[1001] = {0};
	fgets(shebang, 1000, f);
	if(strlen(shebang) > 2 && strncmp(shebang, "#!", 2) == 0){
		fclose(f);
		return 1;
	}
	fclose(f);
	return 0;
}

char** dir_type_content(char* type){
	DIR *dir = opendir(".");
	if(dir == NULL){
		printf("Failed to open current dir.\n");
		return NULL;
	}
	struct dirent *entry;
	struct stat st;
	char **temp;
	int i = 0, n = 0;
	while((entry = readdir(dir)) != NULL){
		if(stat(entry->d_name, &st) != 0 || strcmp(file_type(entry->d_name), type) != 0)
			continue;
		n++;
	}
	if(n == 0){
		closedir(dir);
		printf("No .%s type file...\n", type);
		exit(1);
	}
	rewinddir(dir);
	temp = malloc((n + 1) * sizeof(char*));
	i = 0;
	if(temp == NULL) return NULL;
	while((entry = readdir(dir)) != NULL && i < n){
		if(stat(entry->d_name, &st) != 0 || strcmp(file_type(entry->d_name), type) != 0)
			continue;
		temp[i] = malloc(strlen(entry->d_name) + 1);
		if(temp[i] == NULL){
			free_content(temp);
			printf("Failed to allocate memory...\n");
			return NULL;
		}
		strcpy(temp[i++], entry->d_name);
	}
	temp[i] = NULL;
	sort(temp, i);
	return temp;
}

char** dir_exe_content(){
	DIR *dir = opendir(".");
	if(dir == NULL){
		printf("Failed to open current dir.\n");
		exit(1);
	}
	struct dirent *entry;
	char **temp;
	int i = 0, n = 0;
	while((entry = readdir(dir)) != NULL){
		if(!isExecutable(entry->d_name))
			continue;
		n++;
	}
	if(n == 0){
		closedir(dir);
		printf("No executable file...\n");
		exit(1);
	}
	temp = malloc((n + 1) * sizeof(char*));
	if(temp == NULL) return NULL;
	rewinddir(dir);
	while((entry = readdir(dir)) != NULL && i < n){
		if(!isExecutable(entry->d_name))
			continue;
		temp[i] = malloc(strlen(entry->d_name) + 1);
		if(temp[i] == NULL){
			closedir(dir);
			free_content(temp);
			printf("Failed to allocate memory...\n");
			return NULL;
		}
		strcpy(temp[i], entry->d_name);
		i++;
	}
	temp[i] = NULL;
	closedir(dir);
	sort(temp, i);
	return temp;
}

char System(const char* file, int test, int valgrind, int char_ins){
	struct stat st;
	if(file == NULL || stat(file, &st)) return 0;
	char *cmd, *pure_name = file_name(file);
	if(pure_name == NULL){
		pure_name = malloc(strlen(file) + 1);
		if(pure_name == NULL){
			printf("Failed to allocate memory...\n");
			return 0;
		}
		strcpy(pure_name, file);
	}
	if(test){
		char *tst;
		tst = malloc(strlen(pure_name) + 6);
		if(tst == NULL){
			printf("Failed to allocate memory for %s.test\n", pure_name);
			test = 0;
		}
		else{
			snprintf(tst, strlen(pure_name) + 6, "%s.test", pure_name);
			if(access(tst, F_OK))
				test = 0;
			free(tst);
		}
	}
	if(!isDir(file) && !access(file, R_OK) && access(file, X_OK)){
		char shebang[1001] = {0};
		FILE *f;
		f = fopen(file, "r");
		if(f == NULL){
			printf("Failed to open file \"%s\"\n", file);
			free(pure_name);
			return 0;
		}
		fgets(shebang, 1000, f);
		if(strlen(shebang) > 2 && strncmp(shebang, "#!", 2) == 0){
			printf("\e[33;1m===== %s =====\e[0m\n", file);
			int len = strlen(shebang) + strlen(file) + test * (8 + strlen(pure_name)) + valgrind * 9 + char_ins * 11;
			cmd = malloc(len);
			if(cmd == NULL){
				free(pure_name);
				printf("Failed to allocate memory...\n");
				return 0;
			}
			if(strlen(shebang) > 1 && shebang[strlen(shebang) - 1] == '\n')
				shebang[strlen(shebang) - 1] = 0;
			strcpy(cmd, "");
			if(valgrind)
			strcat(cmd, "valgrind ");
		strcat(cmd, shebang + 2);
		strcat(cmd, " ");
		strcat(cmd, file);
		if(test){
			strcat(cmd, " < ");
			strcat(cmd, pure_name);
			strcat(cmd, ".test");
		}
		if(char_ins)
			strcat(cmd, " | char_ins");
		}
		else{
			printf("\"%s\" is not an executable file...\n", file);
			free(pure_name);
			return 0;
		}
	}
	else if(!isDir(file) && !access(file, X_OK)){
		printf("\e[33;1m===== %s =====\e[0m\n", file);
		int len = strlen(file) + 3 + test * (strlen(pure_name) + 8) + valgrind * 9 + char_ins * 11;
		cmd = malloc(len);
		if(cmd == NULL){
			free(pure_name);
			printf("Failed to allocate memory...\n");
			return 0;
		}
		strcpy(cmd, "");
		if(valgrind)
			strcat(cmd, "valgrind ");
		strcat(cmd, "./");
		strcat(cmd, file);
		if(test){
			strcat(cmd, " < ");
			strcat(cmd, pure_name);
			strcat(cmd, ".test");
		}
		if(char_ins)
			strcat(cmd, " | char_ins");
	}
	else if(!access(pure_name, X_OK)){
		printf("\e[33;1m===== %s =====\e[0m\n", pure_name);
		int len = strlen(pure_name) * (1 + test) + test * 8 + 3 + valgrind * 9 + char_ins * 11;
		cmd = malloc(len);
		if(cmd == NULL){
			free(pure_name);
			printf("Failed to allocate memory...\n");
			return 0;
		}
		strcpy(cmd, "");
		if(valgrind)
			strcat(cmd, "valgrind ");
		strcat(cmd, "./");
		strcat(cmd, pure_name);
		if(test){
			strcat(cmd, " < ");
			strcat(cmd, pure_name);
			strcat(cmd, ".test");
		}
		if(char_ins)
			strcat(cmd, " | char_ins");
	}
	else{
		printf("\"%s\" is not an executable file...\n", file);
		free(pure_name);
		return 0;
	}
	system(cmd);
	free(cmd);
	free(pure_name);
	return 1;
}

int args(int argc, char* argv[]){
	int i, test = 0, valgrind = 0, char_ins = 0, type = 0, tot, pos[4] = {0};
	for(i = 1 ; i < argc ; i++){
		if(strcmp("-test", argv[i]) == 0){
			test++;
			pos[2] = i;
		}
		else if(strcmp("-valgrind", argv[i]) == 0){
			valgrind++;
			pos[1] = i;
		}
		else if(strcmp("-char_ins", argv[i]) == 0){
			char_ins++;
			pos[3] = i;
		}
		else if(argv[i][0] == '-'){
			type++;
			pos[0] = i;
		}
		else break;
	}
	tot = type + test + valgrind + char_ins;
	if(type > 1 || test > 1 || valgrind > 1 || char_ins > 1)
		return -1;
	for(i = 0 ; i < 4 ; i++)
		if(pos[i] > tot)
			return -1;
	if(type && pos[0] != 1)
		return -1;
	return type * 8 + test * 4 + valgrind * 2 + char_ins * 1;
}

int main(int argc, char* argv[]){
	int result = args(argc, argv);
	if(result == -1){
		printf("Invalid form...\n");
		return 1;
	}
	int i = 0, test, valgrind, char_ins, typ, tot;
	typ = result / 8;
	result %= 8;
	test = result / 4;
	result %= 4;
	valgrind = result / 2;
	char_ins = result % 2;
	tot = typ + test + valgrind + char_ins;
	
	if(argc == tot + 1 && !typ){
		char **content = dir_exe_content();
		if(content == NULL) err_ptr();
		while(content[i] != NULL){
			if(!System(content[i], test, valgrind, char_ins)){
				printf("Stop\n");
				free_content(content);
				return 1;
			}
			i++;
		}
		free_content(content);
		return 0;
	}
	else if(argc == tot + 1){
		char type[strlen(argv[1])];
		strcpy(type, argv[1] + 1);
		char** content = dir_type_content(type);
		if(content == NULL)
			err_ptr();
		for(i = 0 ; content[i] != NULL ; i++){
			if(!System(content[i], test, valgrind, char_ins)){
				printf("Stop\n");
				free_content(content);
				return 1;
			}
		}
		return 0;
	}
	else if(argc > tot + 1 && !typ){
		for(i = tot + 1 ; i < argc ; i++){
			if(isExecutable(argv[i])){
				if(!System(argv[i], test, valgrind, char_ins)){
					printf("Stop\n");
					return 1;
				}
			}
			else
				printf("%s is not an executable file...\n", argv[i]);
		}
		return 0;
	}
	else{
		printf("Invalid input!\n");
		return 1;
	}
	return 0;
}
