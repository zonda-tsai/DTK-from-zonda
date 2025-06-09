#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define ERR_PTR(x)\
	if(x == NULL){\
		printf("Failed to write the file...\n");\
		exit(1);\
	}
const char* home = NULL;
void make_dir(){
	char path[1001] = {0};
	struct stat st;
	snprintf(path, sizeof(path), "%s/.zonda.due", home);
	if(stat(path, &st))
		if(mkdir(path, 0755) != 0){
			printf("Unable to make directory about infos!\n");
			exit(1);
		}
	snprintf(path, sizeof(path), "%s/.zonda.due/makefiles", home);
	if(stat(path, &st))
		if(mkdir(path, 0755) != 0){
			printf("Unable to make directory about makefiles!\n");
			exit(1);
		}
	snprintf(path, sizeof(path), "%s/.zonda.due/templates", home);
	if(stat(path, &st))
		if(mkdir(path, 0755) != 0){
			printf("Unable to make directory about templates!\n");
			exit(1);
		}
}

void makefile_C(){
	char path[1001] = {0};
	FILE *f;
	snprintf(path, sizeof(path), "%s/.zonda.due/makefiles/makefile-c", home);
	f = fopen(path, "w");
	ERR_PTR(f);
	fprintf(f, ".SILENT:\n");
	fprintf(f, "CC = gcc\n");
	fprintf(f, "FLAGS = -std=c11 -Wall -Wextra -O2\n");
	fprintf(f, "LDFLAGS = -lm\n");
	fprintf(f, "SRC1 = $(wildcard *.c)\n");
	fprintf(f, "SRC2 = $(wildcard *.c/)\n");
	fprintf(f, "PRJS = $(SRC2:/=)\n");
	fprintf(f, "TARGETS = $(basename $(filter-out $(PRJS), $(SRC1)))\n\n");
	fprintf(f, ".PHONY: all clean\n");
	fprintf(f, "all: $(TARGETS)\n");
	fprintf(f, "$(TARGETS): %% : %%.o\n");
	fprintf(f, "\t$(CC) $(FLAGS) $< -o $@ $(LDFLAGS)\n");
	fprintf(f, "%%.o: %%.c\n");
	fprintf(f, "\t$(CC) $(FLAGS) -c $< -o $@\n\n");
	fprintf(f, "clean:\n");
	fprintf(f, "\trm -f $(TARGETS) *.o\n");
	fclose(f);
}

void makefile_C_prj(){
	char path[1001] = {0};
	FILE *f;
	snprintf(path, sizeof(path), "%s/.zonda.due/makefiles/makefile-c-prj", home);
	f = fopen(path, "w");
	ERR_PTR(f);
	fprintf(f, ".SILENT:\n");
	fprintf(f, "CC = gcc\n");
	fprintf(f, "FLAGS = -std=c11 -Wall -Wextra -g -O2\n");
	fprintf(f, "LDFLAGS = -lm\n");
	fprintf(f, "ALL_DEPS =\n\n");
	fprintf(f, "DIR = $(sort $(patsubst %%/,%%,$(wildcard *.c/)))\n");
	fprintf(f, "TARGET = $(notdir $(basename $(DIR)))\n\n");
	fprintf(f, "define TEMPLATE\n");
	fprintf(f, "_SOURCES = $(wildcard $(1).c/*.c)\n");
	fprintf(f, "_OBJECTS = $$(_SOURCES:.c=.o)\n");
	fprintf(f, "_DEPENDS = $$(_SOURCES:.c=.d)\n\n");
	fprintf(f, "$(1): $$(_OBJECTS)\n");
	fprintf(f, "\t$$(CC) $$(FLAGS) $$^ -o $$@ $$(LDFLAGS)\n\n");
	fprintf(f, "$(1).c/%%.o: $(1).c/%%.c\n");
	fprintf(f, "\t$$(CC) -I$(1).c $$(FLAGS) -MMD -MP -MF $$(@:.o=.d) -c $$< -o $$@\n\n");
	fprintf(f, "ALL_DEPS += $$(_DEPENDS)\n");
	fprintf(f, "endef\n\n");
	fprintf(f, "$(foreach proj_dir,$(TARGET),$(eval $(call TEMPLATE,$(proj_dir))))\n\n");
	fprintf(f, ".PHONY: all clean\n");
	fprintf(f, ".DEFAULT_GOAL = all\n\n");
	fprintf(f, "all: $(TARGET)\n\n");
	fprintf(f, "-include $(ALL_DEPS)\n\n");
	fprintf(f, "clean:\n");
	fprintf(f, "\trm -f $(TARGET)\n");
	fprintf(f, "\t$(foreach dir,$(DIR), rm -f $(wildcard $(dir)/*.o) $(wildcard $(dir)/*.d);)\n");
	fclose(f);
}

void makefile_Cpp(){
	char path[1001] = {0};
	FILE *f;
	snprintf(path, sizeof(path), "%s/.zonda.due/makefiles/makefile-cpp", home);
	f = fopen(path, "w");
	ERR_PTR(f);
	fprintf(f, ".SILENT:\n");
	fprintf(f, "CC = g++\n");
	fprintf(f, "FLAGS = -std=c++11 -Wall -Wextra -O2\n");
	fprintf(f, "LDFLAGS = -lm\n");
	fprintf(f, "SRC1 = $(wildcard *.cpp)\n");
	fprintf(f, "SRC2 = $(wildcard *.cpp/)\n");
	fprintf(f, "PRJS = $(SRC2:/=)\n");
	fprintf(f, "TARGETS = $(basename $(filter-out $(PRJS), $(SRC1)))\n\n");
	fprintf(f, ".PHONY: all clean\n");
	fprintf(f, "all: $(TARGETS)\n");
	fprintf(f, "$(TARGETS): %% : %%.o\n");
	fprintf(f, "\t$(CC) $(FLAGS) $^ -o $@ $(LDFLAGS)\n");
	fprintf(f, "%%.o: %%.cpp\n");
	fprintf(f, "\t$(CC) $(FLAGS) -c $< -o $@\n\n");
	fprintf(f, "clean:\n");
	fprintf(f, "\trm -f $(TARGETS) *.o\n");
	fclose(f);
}

void makefile_Cpp_prj(){
	char path[1001] = {0};
	FILE *f;
	snprintf(path, sizeof(path), "%s/.zonda.due/makefiles/makefile-cpp-prj", home);
	f = fopen(path, "w");
	ERR_PTR(f);
	fprintf(f, ".SILENT:\n");
	fprintf(f, "CC = g++\n");
	fprintf(f, "FLAGS = -std=c++11 -Wall -Wextra -g -O2\n");
	fprintf(f, "LDFLAGS = -lm\n");
	fprintf(f, "ALL_DEPS =\n\n");
	fprintf(f, "DIR = $(sort $(patsubst %%/,%%,$(wildcard *.cpp/)))\n");
	fprintf(f, "TARGET = $(notdir $(basename $(DIR)))\n\n");
	fprintf(f, "define TEMPLATE\n");
	fprintf(f, "_SOURCES = $(wildcard $(1).cpp/*.cpp)\n");
	fprintf(f, "_OBJECTS = $$(_SOURCES:.cpp=.o)\n");
	fprintf(f, "_DEPENDS = $$(_SOURCES:.cpp=.d)\n\n");
	fprintf(f, "$(1): $$(_OBJECTS)\n");
	fprintf(f, "\t$$(CC) $$(FLAGS) $$^ -o $$@ $$(LDFLAGS)\n\n");
	fprintf(f, "$(1).cpp/%%.o: $(1).cpp/%%.cpp\n");
	fprintf(f, "\t$$(CC) -I$(1).cpp $$(FLAGS) -MMD -MP -MF $$(@:.o=.d) -c $$< -o $$@\n\n");
	fprintf(f, "ALL_DEPS += $$(_DEPENDS)\n");
	fprintf(f, "endef\n\n");
	fprintf(f, "$(foreach proj_dir,$(TARGET),$(eval $(call TEMPLATE,$(proj_dir))))\n\n");
	fprintf(f, ".PHONY: all clean\n");
	fprintf(f, ".DEFAULT_GOAL = all\n\n");
	fprintf(f, "all: $(TARGET)\n\n");
	fprintf(f, "-include $(ALL_DEPS)\n\n");
	fprintf(f, "clean:\n");
	fprintf(f, "\trm -f $(TARGET)\n");
	fprintf(f, "\t$(foreach dir,$(DIR), rm -f $(wildcard $(dir)/*.o) $(wildcard $(dir)/*.d);)\n");
	fclose(f);
}

void Template(){
	char path[1001] = {0};
	FILE *f;
	snprintf(path, 1000, "%s/.zonda.due/templates/c", home);
	f = fopen(path, "w");
	ERR_PTR(f);
	fprintf(f, "#include <stdio.h>\n");
	fprintf(f, "#include <stdlib.h>\n\n");
	fprintf(f, "int main(int argc, char* argv[]){\n\n\treturn 0;\n}");
	fclose(f);
	snprintf(path, 1000, "%s/.zonda.due/templates/cpp", home);
	f = fopen(path, "w");
	ERR_PTR(f);
	fprintf(f, "#include <iostream>\n");
	fprintf(f, "using namespace std;\n\n");
	fprintf(f, "int main(int argc, char* argv[]){\n\n\treturn 0;\n}");
	fclose(f);
	snprintf(path, 1000, "%s/.zonda.due/templates/h", home);
	f = fopen(path, "w");
	ERR_PTR(f);
	fprintf(f, "#ifndef _H\n");
	fprintf(f, "#define _H\n");
	fprintf(f, "#endif");
	fclose(f);
	snprintf(path, 1000, "%s/.zonda.due/templates/hpp", home);
	f = fopen(path, "w");
	ERR_PTR(f);
	fprintf(f, "#ifndef _HPP\n");
	fprintf(f, "#define _HPP\n");
	fprintf(f, "#endif");
	fclose(f);
	snprintf(path, 1000, "%s/.zonda.due/templates/sh", home);
	f = fopen(path, "w");
	ERR_PTR(f);
	fprintf(f, "#!/bin/bash\n");
	fclose(f);
	snprintf(path, 1000, "%s/.zonda.due/templates/py", home);
	f = fopen(path, "w");
	ERR_PTR(f);
	fprintf(f, "#!/usr/bin/env python3\n");
	fclose(f);
}

int main(int argc, char* argv[]){
	home = getenv("HOME");
	if(home == NULL){
		printf("Environment Variables \"HOME\" Undefined!\n");
		exit(1);
	}
	if(argc == 2){
		if(strcmp(argv[1], "-initial") == 0){
			make_dir();
			makefile_C();
			makefile_C_prj();
			makefile_Cpp();
			makefile_Cpp_prj();
			Template();
		}
		else if(strcmp(argv[1], "-reset") == 0){
			char c;
			printf("This will initialize the makefiles into only C and C++\n");
			printf("Second check for reset (Y/N) ");
			c = getchar();
			if((c == 'n' || c == 'N') || (c != 'Y' && c != 'y' && c != '\n')){
				printf("Stopped\n");
				return 0;
			}
			char *cmd = NULL;
			cmd = malloc(10001);
			if(cmd == NULL){
				printf("Failed to allocate memory...\n");
				return 1;
			}
			snprintf(cmd, 10000, "rm -f %s/.zonda.due/makefiles/*", home);
			system(cmd);
			snprintf(cmd, 10000, "rm -f %s/.zonda.due/templates/*", home);
			system(cmd);
			free(cmd);
			make_dir();
			makefile_C();
			makefile_Cpp();
			makefile_C_prj();
			makefile_Cpp_prj();
			Template();
		}
		else if(strcmp(argv[1], "--help") == 0){
			char temp[strlen(home) + strlen("/.zonda.due/README.md") + 1];
			snprintf(temp, sizeof(temp), "%s/.zonda.due/README.md", home);
			if(!access(temp, R_OK)){
				char *cmd = malloc(10001);
				if(cmd == NULL){
					printf("Failed to allocate memory...\n");
					return 1;
				}
				snprintf(cmd, 10000, "glow %s", temp);
				system(cmd);
				free(cmd);
			}
			else{
				printf("FILE: README.md lost...\n");
				return 1;
			}
			return 0;
		}
		else if(strcmp(argv[1], "-uninstall") == 0){
			char ch;
			printf("Second check for uninstalling? (Y/N) ");
			ch = getchar();
			if(ch == 'N' || ch == 'n'){
				printf("Stopped\n");
				return 0;
			}
			else if(ch == '\n' || ch == 'Y' || ch == 'y'){
				printf("Uninstalling...\n");
				char *cmd;
				cmd = malloc(10001);
				if(cmd == NULL){
					printf("Failed to allocate memory...\n");
					return 1;
				}
				snprintf(cmd, 10000, "rm -f %s/bin/compile", home);
				system(cmd);
				snprintf(cmd, 10000, "rm -f %s/bin/run", home);
				system(cmd);
				snprintf(cmd, 10000, "rm -f %s/bin/settings", home);
				system(cmd);
				snprintf(cmd, 10000, "rm -f %s/bin/clean", home);
				system(cmd);
				snprintf(cmd, 10000, "rm -f %s/bin/+", home);
				system(cmd);
				snprintf(cmd, 10000, "rm -f %s/bin/char_ins", home);
				system(cmd);
				snprintf(cmd, 10000, "rm -f %s/bin/tfmanager", home);
				system(cmd);
				snprintf(cmd, 10000, "rm -rf %s/.zonda.ide", home);
				system(cmd);
				free(cmd);
				printf("Thanks for using this.\nThis can be downloaded by \"git clone https://github.com/zonda-tsai/DUE-from-zonda\" if you want to.\n");
				return 0;
			}
		}
		else{
			printf("Invalid input!\n");
			exit(1);
		}
	}
	else{
		printf("Invalid input!\n");
		exit(1);
	}
	return 0;
}
