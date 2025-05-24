#include <stdio.h>
#include <string.h>
#include <ctype.h>

void help(){
	printf("For stdout, please use pipeline, \'|\'\n");
	printf("For document, please use char_ins < \'file_name\'\n");
	printf("New-line, tab, and end of string will be represent by \e[34m\\n\e[0m, \e[34m\\t\e[0m and \e[34m\\0\e[0m\n");
	printf("Spaces will be represent by \'\e[100m \e[0m\'\n");
	printf("Control ASCII will be shown as \e[45m(Ctrl ASCII: )\e[0m\n");
	printf("For all the others, excluding digits, alphabets and punctuations, will be represent by \e[41m(ASCII: )\e[0m\n\n");
}

int main(int argc, char* argv[]){
	if(argc == 2 && strcmp(argv[1], "--help") == 0){
		help();
		return 0;
	}
	else if(argc != 1){
		printf("Invalid input!\n");
		return 1;
	}
	int i;
	unsigned long long n = 0, news = 0, tabs = 0, ctrls = 0, spaces = 0, unknows = 0, normals = 0;
	char temp[1000001] = {0};
	while(fgets(temp, 1000000, stdin) != NULL){
		for(i = 0 ; temp[i] != 0 ; i++){
			if(temp[i] == '\n'){
				news++;
				printf("\e[34m\\n\e[0m");
			}
			else if(temp[i] == '\t'){
				tabs++;
				printf("\e[34m\\t\e[0m");
			}
			else if(iscntrl(temp[i])){
				ctrls++;
				printf("\e[45m(Ctrl ASCII: %d)\e[0m", temp[i]);
			}
			else if(isspace(temp[i])){
				spaces++;
				printf("\e[100m%c\e[0m", temp[i]);
			}
			else if(!isalpha(temp[i]) && !isdigit(temp[i]) && !ispunct(temp[i])){
				unknows++;
				printf("\e[41m%c(ASCII: %d)\e[0m", temp[i], temp[i]);
			}
			else{
				normals++;
				putchar(temp[i]);
			}
		}
		n += i;
		printf("\e[34m\\0\e[0m\n");
	}
	printf("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("| Tabs                             : %-10llu\n", tabs);
	printf("| Spaces                           : %-10llu\n", spaces);
	printf("| Ctrls ASCIIs                     : %-10llu\n", ctrls);
	printf("| Alphabets, Numbers, Punctuations : %-10llu\n", normals);
	printf("| Unknows                          : %-10llu\n", unknows);
	printf("\e[33;1m| Total words                      : %-10llu\e[0m\n", n);
	printf("\e[33;1m| Lines                            : %-10llu\e[0m\n\n", news + 1);
	return 0;
}
