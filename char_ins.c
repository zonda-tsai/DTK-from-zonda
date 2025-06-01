#include <stdio.h>
#include <string.h>
#include <ctype.h>

void help(){
	printf("For stdout, please use pipeline, \'|\'\n");
	printf("For document, please use char_ins < \'file_name\'\n");
	printf("New-line, tab, return and end of string will be represent by \e[34m\\n\e[0m, \e[34m\\t\e[0m and \e[34m\\r\e[0m \e[34m\\0\e[0m\n");
	printf("Spaces will be represent by \'\e[100m \e[0m\'\n");
	printf("Control ASCII will be shown as \e[45m\\x<HEX>\e[0m\n");
	printf("For all the others, excluding digits, alphabets and punctuations, will be represent by \e[41m(\\x<HEX>)\e[0m\n\n");
	printf("To show ascii-table by char_ins --ascii_table\n");
}

void ascii(){
	int i, j, cntrl = 0;
	unsigned char ctrl[33][5] = {"\\0", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "\\a", "\\b", "\\t", "\\n", "\\v", "\\f", "\\r", "SO", "SI", "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB", "CAN", "EM", "SUB", "ESC", "FS", "GS", "RS", "US", "DEL"};
	printf("  \e[33m");
	for(i = 0 ; i < 16 ; i++)
		printf("%-5x", i);
	printf("\e[0m\n");
	for(i = 0 ; i < 8 ; i++){
		printf("\e[33m%-2x\e[0m", i);
		for(j = 0 ; j < 16 ; j++){
			if(iscntrl((char)(i * 16 + j)))
				printf("%-5s", ctrl[cntrl++]);
			else
				printf("%-5c", (char)(16 * i + j));
		}
		printf("\n");
	}
}

int main(int argc, char* argv[]){
	
	if(argc == 2 && strcmp(argv[1], "--help") == 0){
		help();
		return 0;
	}
	else if(argc == 2 && strcmp(argv[1], "--ascii_table") == 0){
		ascii();
		return 0;
	}
	else if(argc != 1){
		printf("Invalid input!\n");
		return 1;
	}
	int i;
	unsigned long long n = 0, news = 0, tabs = 0, ctrls = 0, spaces = 0, unknows = 0, normals = 0, returns = 0;
	unsigned char temp[1000001] = {0};
	while(fgets((char*)temp, 1000000, stdin) != NULL){
		for(i = 0 ; temp[i] != 0 && i < 1000000 ; i++){
			if(iscntrl(temp[i])){
				ctrls++;
				if(temp[i] == '\n'){
					news++;
					printf("\e[34m\\n\e[0m");
				}
				else if(temp[i] == '\t'){
					tabs++;
					printf("\e[34m\\t\e[0m");
				}
				else if(temp[i] == '\r'){
					returns++;
					printf("\e[34m\\r\e[0m");
				}
				else if(temp[i] == '\a')
					printf("\e[34m\\a\e[0m");
				else if(temp[i] == '\b')
					printf("\e[34m\\b\e[0m");
				else if(temp[i] == '\v')
					printf("\e[34m\\v\e[0m");
				else
					printf("\e[45m\\x%x\e[0m", temp[i]);
			}
			else if(isspace(temp[i])){
				spaces++;
				printf("\e[100m%c\e[0m", temp[i]);
			}
			else if(!isalpha(temp[i]) && !isdigit(temp[i]) && !ispunct(temp[i])){
				unknows++;
				printf("\e[41m\\x%x\e[0m", temp[i]);
			}
			else{
				normals++;
				putchar(temp[i]);
			}
		}
		n += i;
		if(temp[i] == 0)
			printf("\e[34m\\0\e[0m\n");
	}
	printf("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("| Tabs                             : %-10llu\n", tabs);
	printf("| Spaces                           : %-10llu\n", spaces);
	printf("| Returns                          : %-10llu\n", returns);
	printf("| Ctrls ASCIIs                     : %-10llu\n", ctrls);
	printf("| Alphabets, Numbers, Punctuations : %-10llu\n", normals);
	printf("| Unknows                          : %-10llu\n", unknows);
	printf("\e[33;1m| Total words                      : %-10llu\e[0m\n", n);
	printf("\e[33;1m| Lines                            : %-10llu\e[0m\n\n", news + 1);
	return 0;
}
