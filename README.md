# IDE-from-zonda
*This is an easy IDE so it's not that powerful.*  
**settings [arg]**  
&emsp;-initial  : Initial the IDE **(YOU DON'T NEED THIS! initial.sh have done that for you)**  
&emsp;-reset    : Reset the IDE  
&emsp;-uninstall: Uninstall this IDE  
**compile [args]**  
&emsp;[arg 1]: Types. Initial one only provide c and cpp (e.g. -c, -cpp, -c-prj, -cpp-prj (-prj means project))  
&emsp;&emsp;&emsp;&emsp;&emsp;**If you postfix (or suffix) " clean", it'll clean the executable file for that type.**  
&emsp;[arg n]: for n >= 2, you can assign specific files. To make sure the files you assigned are arg_1 type  
**run [args]**
&emsp;run      : run all. **It'll include all files in that directory**  
&emsp;run -test: run all with testfile. e.g. a =(find)=> a.test and ./a < a.test  
&emsp;run -type: run all the files with that type. e.g. run -c (-test)  
