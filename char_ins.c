#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct{
	unsigned long long n;
	unsigned long long news;
	unsigned long long tabs;
	unsigned long long ctrls;
	unsigned long long spaces;
	unsigned long long unknows;
	unsigned long long normals;
	unsigned long long returns;
}chars;

void help(){
    printf("char_ins is a tool to inspect characters from a file or standard input.\n\n");
    printf("USAGE:\n");
    printf("  char_ins [FILE] [OPTIONS]\n");
    printf("  cat <file> | char_ins [OPTIONS]\n\n");
    printf("OPTIONS:\n");
    printf("  [FILE]           Specify an input file to analyze.\n");
    printf("                   If no file is given, reads from standard input (stdin).\n\n");
    printf("  --help           Show this help message and exit.\n");
    printf("  --ascii_table    Show the full ASCII table and exit.\n");
    printf("  --result         Only show the final statistics summary, without character details.\n\n");
    printf("EXAMPLES:\n");
    printf("  # Analyze a file and show character details\n");
    printf("  char_ins my_document.txt\n\n");
    printf("  # Analyze content from a pipe and show details\n");
    printf("  echo -e \"hello\\tworld\" | char_ins\n\n");
    printf("  # Get only the statistics for a file\n");
    printf("  char_ins my_document.txt --result\n\n");
    printf("  # Get only the statistics from a pipe\n");
    printf("  cat my_document.txt | char_ins --result\n\n");
	printf("OUTPUTS:\n[Control Characters]\n");
	printf("\tSpecials like new-line or tab will be shown by \e[34m\\n\e[0m, \e[34m\\t\e[0m\n");
	printf("\tOthers will be shown by \e[34m\\x<HEX>\e[0m\n[Spaces]\n");
	printf("\tSpaces will be shown by \'\e[100m \e[0m\'\n\n");
	printf("For all the others, excluding digits, alphabets and punctuations, will be represent by \e[31m\\x<HEX>\e[0m\n\n");
	printf("To show ascii-table by char_ins --ascii_table\n");
}

void ascii(){
	int i, j, cntrl = 0;
	unsigned char ctrl[33][5] = {"\\0", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "\\a", "\\b", "\\t", "\\n", "\\v", "\\f", "\\r", "SO", "SI", "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB", "CAN", "EM", "SUB", "ESC", "FS", "GS", "RS", "US", "DEL"};
	printf("    \e[33m");
	for(i = 0 ; i < 16 ; i++)
		printf("%-5x", i);
	printf("\e[0m\n\n");
	for(i = 0 ; i < 8 ; i++){
		printf("\e[33m%-4x\e[0m", i);
		for(j = 0 ; j < 16 ; j++){
			if(iscntrl((char)(i * 16 + j)))
				printf("%-5s", ctrl[cntrl++]);
			else
				printf("%-5c", (char)(16 * i + j));
		}
		printf("\n\n");
	}
}

chars output_detail(FILE* f){
	chars a = {0};
	char temp;
	while((temp = fgetc(f)) != EOF){
		if(iscntrl(temp)){
			a.ctrls++;
			switch(temp){
			case '\0':
				printf("\e[34m\\0\e[0m");
				break;
			case '\n':
				a.news++;
				printf("\e[34m\\n\e[0m\n");
				break;
			case '\t':
				a.tabs++;
				printf("\e[34m\\t\e[0m\t");
				break;
			case '\r':
				a.returns++;
				printf("\e[34m\\r\e[0m");
				break;
			case '\a':
				printf("\e[34m\\a\e[0m\a");
				break;
			case '\b':
				printf("\e[34m\\b\e[0m");
				break;
			case '\v':
				printf("\e[34m\\v\e[0m");
				break;
			default:
				printf("\e[34m\\x%x\e[0m", temp);
			}
		}
		else if(isspace(temp)){
			a.spaces++;
			printf("\e[100m%c\e[0m", temp);
		}
		else if(!isalpha(temp) && !isdigit(temp) && !ispunct(temp)){
			a.unknows++;
			printf("\e[31m\\x%x\e[0m", (unsigned char)temp);
		}
		else{
			a.normals++;
			putchar(temp);
		}
		a.n++;
	}
	return a;
}

chars calculate(FILE *f){
	chars a = {0};
	char temp;
	while((temp = fgetc(f)) != EOF){
		if(iscntrl(temp)){
			a.ctrls++;
			switch(temp){
			case '\n':
				a.news++;
				break;
			case '\t':
				a.tabs++;
				break;
			case '\r':
				a.returns++;
				break;
			}
		}
		else if(isspace(temp))
			a.spaces++;
		else if(!isalpha(temp) && !isdigit(temp) && !ispunct(temp))
			a.unknows++;
		else
			a.normals++;
		a.n++;
	}
	return a;
}

void output_result(const chars a){
	printf("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");	
	printf("| Tabs                             : %-10llu\n", a.tabs);
	printf("| Spaces                           : %-10llu\n", a.spaces);
	printf("| Returns                          : %-10llu\n", a.returns);
	printf("| Ctrl ASCIIs                      : %-10llu\n", a.ctrls);
	printf("| Alphabets, Numbers, Punctuations : %-10llu\n", a.normals);
	printf("| Unknows                          : %-10llu\n", a.unknows);
	printf("\e[33;1m| Total Characters                 : %-10llu\e[0m\n", a.n);
	printf("\e[33;1m| Lines                            : %-10llu\e[0m\n\n", a.news + 1);
}

int main(int argc, char* argv[]){
	FILE *f = NULL;
	if(argc == 2 && strcmp(argv[1], "--help") == 0){
		help();
		return 0;
	}
	else if(argc == 2 && strcmp(argv[1], "--ascii_table") == 0){
		ascii();
		return 0;
	}
	else if(argc == 2 && !strcmp(argv[1], "--result")){
		output_result(calculate(stdin));
	}
	else if(argc == 3 && !strcmp(argv[1], "--result")){
		f = fopen(argv[2], "r");
		if(f == NULL){
			printf("Failed to open FILE '%s'\n", argv[2]);
			return 1;
		}
		output_result(calculate(f));
		fclose(f);
	}
	else if(argc == 2){
		f = fopen(argv[1], "r");
		if(f == NULL){
			printf("Failed to open FILE '%s'\n", argv[1]);
			return 1;
		}
		output_result(output_detail(f));
		fclose(f);
	}
	else if(argc == 1)
		output_result(output_detail(stdin));
	else{
		printf("Invalid input!\n");
		return 1;
	}
	
	return 0;
}
