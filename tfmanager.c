#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

const char* file_type(const char* file){
	if(file == NULL) return "";
	int i;
	for(i = strlen(file) - 1 ; i >= 0 && file[i] != '.' ; i--);
	if(i == (int)strlen(file) - 1 || i == -1 || file[i + 1] == '\0') return "";
	return file + i + 1;
}

int cmp(const void *t1, const void *t2){
    const char *a = *(const char**)t1;
    const char *b = *(const char**)t2;
    return strcmp(a, b);
}

void sort(char **arr, int n){
    qsort(arr, n, sizeof(char*), cmp);
}

void deleter(char** a){
	int i;
	for(i = 0 ; a[i] != NULL ; i++)
		free(a[i]);
	free(a);
}

char** get_test(){
	DIR *dir = opendir(".");
	if(dir == NULL){
		printf("Unable to open current directory...\n");
		exit(1);
	}
	struct stat st;
	struct dirent *entry;
	int i = 0, n = 0;
	while((entry = readdir(dir)) != NULL)
		if(strcmp(file_type(entry->d_name), "test") == 0 && !stat(entry->d_name, &st) && S_ISREG(st.st_mode))
			n++;
	if(n == 0){
		printf("No .test files...\n");
		exit(1);
	}
	rewinddir(dir);
	char **temp;
	temp = malloc((n + 1) * sizeof(char*));
	if(temp == NULL){
		printf("Failed to allocate memory...\n");
		exit(1);
	}
	while((entry = readdir(dir)) != NULL && i < n)
		if(strcmp(file_type(entry->d_name), "test") == 0 && !stat(entry->d_name, &st) && S_ISREG(st.st_mode)){
			temp[i] = malloc(strlen(entry->d_name) + 1);
			if(temp[i] == NULL){
				deleter(temp);
				printf("Failed to allocate memory...\n");
				exit(1);
			}
			strcpy(temp[i], entry->d_name);
			i++;
		}
	temp[i] = NULL;
	sort(temp, i);
	return temp;
}

char isTitle(const char *line){
	if(line == NULL)
		return 0;
	int len = strlen(line);
	if(len <= 12)
		return 0;
	if(!strncmp(line, "===== ", 6) && !strncmp(line + len - 6, " =====\n", 6))
		return 1;
	return 0;
}

char* getTitle(char *line, FILE *f){
	if(!isTitle(line)) return NULL;
	char *temp;
	temp = malloc(strlen(line) - 11);
	if(temp == NULL){
		printf("Failed to allocate memory...\n");
		if(f != NULL)
			fclose(f);
		exit(1);
	}
	strncpy(temp, line + 6, strlen(line) - 12);
	temp[strlen(line) - 12] = 0;
	return temp;
}

void merge(char **file){
	if(!access("all.test", F_OK)){
		printf("all.test exists, Stopped\n");
		if(file != NULL)
			deleter(file);
		exit(1);
	}
	FILE *f, *m;
	int i = 0;
	char temp[100001] = {0};
	m = fopen("all.test", "w");
	if(m == NULL){
		printf("Failed to write all .test into all.test...\n");
		deleter(file);
		exit(1);
	}
	while(file[i] != NULL){
		f = fopen(file[i], "r");
		if(f == NULL){
			printf("Failed to read .test file...\n");
			deleter(file);
			system("rm -f all.test");
			exit(1);
		}
		fprintf(m, "===== %s =====\n", file[i]);
		while(fgets(temp, 100000, f) != NULL)
			fputs(temp, m);
		fputc('\n', m);
		fclose(f);
		i++;
	}
	i = 0;
	while(file[i] != NULL){
		strcpy(temp, "rm -f ");
		strcat(temp, file[i]);
		system(temp);
		i++;
	}
}

void divide(){
	FILE *all, *f = NULL;
	all = fopen("all.test", "r");
	if(all == NULL){
		printf("Failed to open all.test...\n");
		exit(1);
	}
	int i = 0;
	char *title, temp[100001] = {0};
	while(fgets(temp, 100000, all) != NULL){
		if(temp[strlen(temp) - 1] == '\n')
			temp[strlen(temp) - 1] = 0;
		if((title = getTitle(temp, f)) != NULL){
			if(f != NULL)
				fclose(f);
			f = fopen(title, "w");
			free(title);
			if(f == NULL){
				printf("Unable to write in %s\n", title);
				exit(1);
			}
		}
		else if(f != NULL){
			fputs(temp, f);
			fputc('\n', f);
		}
		i++;
	}
	
	if(f != NULL)
		fclose(f);
	if(all != NULL)
		fclose(all);
	f = all = NULL;
	if(i == 0){
		printf("all.test is empty...\n");
		exit(1);
	}
	system("rm -f all.test");
}

int main(int argc, char* argv[]){
	if(argc != 2){
		printf("Invalid input...\n");
		return 1;
	}
	if(!strcmp(argv[1], "-merge")){
		char **file = get_test();
		if(file == NULL){
			printf("Failed to allocate memory...\n");
			return 1;
		}
		merge(file);
		deleter(file);
	}
	else if(!strcmp(argv[1], "-divide"))
		divide();

	return 0;
}
