#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void make_dir(){
	const char *home = getenv("HOME");
	char path[1001] = {0};
	snprintf(path, sizeof(path), "%s/.zonda.ide", home);
	if(mkdir(path, 0755) != 0){
		printf("Unable to make directory about infos!\n");
		exit(1);
	}
	snprintf(path, sizeof(path), "%s/.zonda.ide/makefiles", home);
	if(mkdir(path, 0755) != 0){
		printf("Unable to make directory about makefiles!\n");
		exit(1);
	}
}
void makefile_C(){
	const char *home = getenv("HOME");
	char path[1001] = {0};
	FILE *f;
	snprintf(path, sizeof(path), "%s/.zonda.ide/makefiles/makefile-c", home);
	f = fopen(path, "w");
	fprintf(f, ".SILENT:\n");
	fprintf(f, "CC = gcc\n");
	fprintf(f, "FLAGS = -std=c11 -Wall -Wextra -O2\n");
	fprintf(f, "LDFLAGS = \n");
	fprintf(f, "SRC1 = $(wildcard *.c)\n");
	fprintf(f, "SRC2 = $(wildcard *.c/)\n");
	fprintf(f, "PRJS = $(SRC2:/=)\n");
	fprintf(f, "TARGETS = $(basename $(filter-out $(PRJS), $(SRC1)))\n\n");
	fprintf(f, ".PHONY: all clean\n");
	fprintf(f, "all: $(TARGETS)\n");
	fprintf(f, "%%: %%.o\n");
	fprintf(f, "\t$(CC) $(FLAGS) $^ -o $@ $(LDFLAGS)\n");
	fprintf(f, "%%.o: %%.c\n");
	fprintf(f, "\t$(CC) $(FLAGS) -c $< -o $@\n\n");
	fprintf(f, "clean:\n");
	fprintf(f, "\trm -f $(TARGETS) *.o\n");
	fclose(f);
}
void makefile_C_prj(){
	const char *home = getenv("HOME");
	char path[1001] = {0};
	FILE *f;
	snprintf(path, sizeof(path), "%s/.zonda.ide/makefiles/makefile-c-prj", home);
	f = fopen(path, "w");
	fprintf(f, ".SILENT:\n");
	fprintf(f, "CC = gcc\n");
	fprintf(f, "FLAGS = -std=c11 -Wall -Wextra -g -O2\n");
	fprintf(f, "LDFLAGS =\n");
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
	const char *home = getenv("HOME");
	char path[1001] = {0};
	FILE *f;
	snprintf(path, sizeof(path), "%s/.zonda.ide/makefiles/makefile-cpp", home);
	f = fopen(path, "w");
	fprintf(f, ".SILENT:\n");
	fprintf(f, "CC = g++\n");
	fprintf(f, "FLAGS = -std=c++11 -Wall -Wextra -O2\n");
	fprintf(f, "LDFLAGS = \n");
	fprintf(f, "SRC1 = $(wildcard *.cpp)\n");
	fprintf(f, "SRC2 = $(wildcard *.cpp/)\n");
	fprintf(f, "PRJS = $(SRC2:/=)\n");
	fprintf(f, "TARGETS = $(basename $(filter-out $(PRJS), $(SRC1)))\n\n");
	fprintf(f, ".PHONY: all clean\n");
	fprintf(f, "all: $(TARGETS)\n");
	fprintf(f, "%%: %%.o\n");
	fprintf(f, "\t$(CC) $(FLAGS) $^ -o $@ $(LDFLAGS)\n");
	fprintf(f, "%%.o: %%.cpp\n");
	fprintf(f, "\t$(CC) $(FLAGS) -c $< -o $@\n\n");
	fprintf(f, "clean:\n");
	fprintf(f, "\trm -f $(TARGETS) *.o\n");
	fclose(f);
}
void makefile_Cpp_prj(){
	const char *home = getenv("HOME");
	char path[1001] = {0};
	FILE *f;
	snprintf(path, sizeof(path), "%s/.zonda.ide/makefiles/makefile-cpp-prj", home);
	f = fopen(path, "w");
	fprintf(f, ".SILENT:\n");
	fprintf(f, "CC = g++\n");
	fprintf(f, "FLAGS = -std=c++11 -Wall -Wextra -g -O2\n");
	fprintf(f, "LDFLAGS =\n");
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

int main(int argc, char* argv[]){
	const char *home = getenv("HOME");
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
		}
		else if(strcmp(argv[1], "-reset") == 0){
			system("rm -rf ~/.zonda.ide");
			make_dir();
			makefile_C();
			makefile_Cpp();
			makefile_C_prj();
			makefile_Cpp_prj();
		}
		else if(strcmp(argv[1], "-uninstall") == 0){
			char ch;
			printf("Second check for uninstalling? (Y/N) ");
			ch = getchar();
			getchar();
			if(ch == 'N' || ch == 'n'){
				printf("Stopped\n");
				return 0;
			}
			else if(ch == 'Y' || ch == 'y'){
				printf("Uninstalling...\n");
				system("rm -f ~/bin/compile ~/bin/run ~/bin/settings");
				printf("Thanks for using this.\nThis can be downloaded by \"git clone https://github.com/zonda-tsai/IDE-from-zonda\" if you want to.\n");
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
