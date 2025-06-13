#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#define yellow(x) printf("\x1b[33m%s\x1b[0m", x)
#define blue(x) printf("\x1b[34m%s\x1b[0m", x)

const char* User = NULL;
const char* Host = NULL;

const char* getUser(){
	uid_t user_id = geteuid();
	struct passwd *user_info = getpwuid(user_id);
	if(user_info)
		return user_info->pw_name;
	return "user";
}

const char* getHost(){
	static char hostname[10001] = {0};
	if(!gethostname(hostname, 10001))
		return hostname;
	strcpy(hostname, "Linux");
	return hostname;
}

void head(){
	printf("\x1b[32m%s@%s\x1b[0m:", User, Host);
	printf("\x1b[34m~\x1b[0m$ ");
}


int main(){
	char input[10001] = {0};
	User = getUser(), Host = getHost();
	system("clear");
	// Intro
	printf("First of all, we'll talk about where the files be stored\n");
	printf("To show the files, we'll go back to home directory (~)\n");
	printf("The sources are located at \x1b[34m~/.zonda.dtk\x1b[0m and the tools are at ");
	blue("~/bin\n");
	printf("You can improve them if required after this tutorial\n");
	printf("\nMove to next page by 'Enter'...\n");
	getchar();
	system("clear");

	// +
	printf("Let's talk about the tool, '+'\n");
	printf("It's a tool for adding by templates with file type\n");
	printf("e.g. ");
	yellow("+ HelloWorld.c");
	printf(" will copy a template of file type 'c' from ");
	blue("~/.zonda.dtk/templates/c\n");
	printf("Let's try with ");
	yellow("+ HelloWorld.c");
	printf(":\n");
	head();
	fgets(input, 10001, stdin);
	input[strlen(input) - 1] = 0;
	while(strcmp(input, "+ HelloWorld.c") != 0){
		printf("Em, it's ");
		yellow("+ HelloWorld.c");
		printf(" instead of ");
		yellow(input);
		printf("\n\n");
		head();
		fgets(input, 10001, stdin);
		input[strlen(input) - 1] = 0;
	}
	printf("Then, try ");
	yellow("cat HelloWorld.c");
	printf(":\n");
	head();
	fgets(input, 10001, stdin);
	input[strlen(input) - 1] = 0;
	while(strcmp(input, "cat HelloWorld.c") != 0){
		printf("Em, it's ");
		yellow("cat HelloWorld.c");
		printf(" instead of ");
		yellow(input);
		printf("\n\n");
		head();
		fgets(input, 10001, stdin);
		input[strlen(input) - 1] = 0;
	}
	printf("#include <stdio.h>\n#include <stdlib.h>\n\nint main(int argc, char* argv[]){\n\n\treturn 0;\n}");
	head();
	printf("\n\nThen you see the template of file type 'c'!\n");
	printf("\nMove to next page by 'Enter'...\n");
	getchar();
	system("clear");

	// compile & run
	printf("It's time to step into our key, compile & run\n");
	printf("For the script languages, it requires ");
	yellow("'shebang'(start with #!)");
	printf(", you can + the template of file type .sh and .py after this tutorial and see its shebang\n");
	printf("For all the others, you'll need the compiler, like gcc, g++ for c/c++\n");
	printf("Make sure you have the makefile of the specific types, you can put it into \x1b[34m~/.zonda.dtk/makefiles\x1b[0m\n");
	printf("Initial one included c, c++ and their project.\n");
	printf("p.s. For c/c++ project, just named a directory like <name>.c and inside it are the programs of that project\n");
	printf("\nMove to next page by 'Enter'...\n");
	getchar();
	system("clear");
	
	printf("For ");
	yellow("compile");
	printf(", it's pretty simple\n");
	yellow("compile -<file_type>");
	printf("   : Compiling all that specific file type in current directory\n");
	yellow("compile file1 file2 ...");
	printf(": Compiling each file\n");
	printf("\x1b[31m[Please don't mix both of them together]\x1b[0m\n");
	printf("e.g. \x1b[33mcompile -c\x1b[0m, \x1b[33mcompile HelloWorld.c\x1b[0m, etc.\n");
	printf("For sure, some file type won't work until you add its makefile\n\n");
	printf("For \x1b[33mrun\x1b[0m, it's a little bit complicated\n");
	printf("You can directly run the specific type at current directory like the form of compile\n");
	printf("You can also run specific file like the form of compile\n");
	printf("Like compile, don't mix them together, like \x1b[33mrun -c HelloWorld.c\x1b[0m is invalid\n");
	printf("But please make sure, it is executable file, or the file contains shebang\n");
	printf("What's more, there's some flags you can used: ");
	yellow("-test -char_ins -valgrind\n");
	printf("These flags should be put after \x1b[33m-<file_type>\x1b[0m(if exists) and before files\n");
	printf("e.g. ");
	yellow("run -char_ins -valgrind HelloWorld.c");
	printf(", ");
	yellow("run -c -valgrind -char_ins");
	printf(", etc.\n");
	printf("We'll briefly introduce what these flags do\n");
	printf("\x1b[33m-test\x1b[0m     : For the <file_name>.test, it'll input the data insides .test for you\n");
	printf("\x1b[33m-char_ins\x1b[0m : It'll print all the ASCII characters that you can see clearly\n");
	printf("\x1b[33m-valgrind\x1b[0m : memory leaking inspector\n");
	printf("\nMove to next page by 'Enter'...\n");
	getchar();
	system("clear");
	
	printf("Let's give it a try!\nSince you've \x1b[33m+ HelloWorld.c\x1b[0m and I've edited it for you, let's compile it!\n");
	printf("Type ");
	yellow("compile HelloWorld.c\n");
	head();
	fgets(input, 10000, stdin);
	input[strlen(input) - 1] = 0;
	while(strcmp(input, "compile HelloWorld.c") != 0){
		printf("Em, it's \x1b[33mcompile HelloWorld.c\x1b[0m instead of");
		yellow(input);
		printf("\n\n");
		head();
		fgets(input, 10000, stdin);
		input[strlen(input) - 1] = 0;
	}
	printf("Awesome, then type ");
	yellow("run HelloWorld\n");
	head();
	fgets(input, 10000, stdin);
	input[strlen(input) - 1] = 0;
	while(strcmp(input, "run HelloWorld") != 0){
		printf("Em, it's \x1b[33mrun HelloWorld.c\x1b[0m instead of \x1b[33m%s\x1b[0m\n\n", input);
		head();
		fgets(input, 10000, stdin);
		input[strlen(input) - 1] = 0;
	}
	printf("\x1b[33;1m===== HelloWorld =====\x1b[0m\n");
	printf("Hello World!\n");
	head();
	printf("\n\nGreat!\n");
	printf("Move to next page by 'Enter'...\n");
	getchar();
	system("clear");
	
	// clean
	printf("Then we'll talk about clean\n");
	yellow("clean");
	printf(" will do the same things as make clean in makefile\n");
	printf("Take c/c++ for example, it'll delete object code and executable file for existing .c/.cpp\n");
	printf("You can clean a specific type by ");
	yellow("clean -<file_type>\n");
	printf("You can also clean all the things by only ");
	yellow("clean\n");
	printf("\nMove to next page by 'Enter'...\n");
	getchar();
	system("clear");
	printf("Since we have HelloWorld.c, HelloWorld.o and \x1b[32mHelloWorld\x1b[0m now and we don't need HelloWorld.o and \x1b[32mHelloWorld\x1b[0m now, type \x1b[33mclean -c\x1b[0m or \x1b[33mclean\x1b[0m\n");
	head();
	fgets(input, 10000, stdin);
	input[strlen(input) - 1] = 0;
	while(strcmp(input, "clean -c") != 0 && strcmp(input, "clean") != 0){
		printf("Em, it's \x1b[33mclean\x1b[0m or \x1b[33mclean -c\x1b[0m instead of '%s'\n\n", input);
		head();
		fgets(input, 10000, stdin);
		input[strlen(input) - 1] = 0;
	}
	printf("Superb!\n");
	printf("\nMove to next page by 'Enter'...\n");
	getchar();
	system("clear");
	
	// tfmanager
	printf("It'll be a mess if inside a directory have a lot of .test files, then tfmanager is a good tool to deal with that.\n");
	printf("You can merge all the .test files into single all.test by \x1b[33mtfmanager -merge\x1b[0m\n");
	printf("Once you need .test, you can divide it by \x1b[33mtfmanager -divide\x1b[0m\n");
	printf("\nMove to next page by 'Enter'...\n");
	getchar();
	system("clear");

	// char_ins
	printf("Remember the char_ins we talk about in 'run'? It's useful for you to find magical, invisible character that annoy you\n");
	printf("With a file, you can use \x1b[33mchar_ins < file\x1b[0m\n");
	printf("As for the outputs, you can use suffix \x1b[33m| char_ins\x1b[0m to get the same thing\n");
	printf("Also, if you need ASCII table, just type \x1b[33mchar_ins --ascii_table\x1b[0m\n");
	printf("\nMove to next page by 'Enter'...\n");
	getchar();
	system("clear");

	// settings
	printf("Finally, let's talk about the settings.\n");
	printf("You can reset the DTK if you delete some initial executable files, templates or makefiles by \x1b[33msettings -reset\x1b[0m\n");
	printf("However, if you delete the .c files inside \x1b[34m~/.zonda.dtk\x1b[0m, you can only to git clone them again...\n");
	printf("You can also uninstall this if you think this DTK sucks. Uninstall it by \x1b[33msettings -uninstall\x1b[0m\n");
	printf("For showing this tutorial again, please use \x1b[33msettings --help\x1b[0m\n");
	printf("\nMove to next page by 'Enter'...\n");
	getchar();
	system("clear");

	// final
	printf("For using this development tool kit, you can add some files\n");
	printf("makefile:\n\t<name>: ");
	yellow("makefile-[file_type](-prj), prj is for project that stored in directory\n");
	printf("\t<location>: ");
	blue("~/.zonda.dtk/makefiles\n\n");
	printf("templates:\n\t<name>:");
	yellow(" [file_type]\n");
	printf("\t<location>: ");
	blue("~/.zonda.dtk/templates\n\n");
	printf("\nThis is the end of the tutorial, leave by 'Enter'\n");
	getchar();
	system("clear");
	return 0;
}
