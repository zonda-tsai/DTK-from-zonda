**IDE-from-zonda** is an easy IDE so it's not that powerful.  
You could download by "**git clone https://github.com/zonda-tsai/IDE-from-zonda**" and "**bash IDE-from-zonda/initial.sh**"  
# settings [arg]
&emsp;**-initial**   : Initial the IDE **(YOU DON'T NEED THIS! initial.sh have done that for you)**  
&emsp;**-reset**     : Reset the IDE  
&emsp;**-uninstall** : Uninstall this IDE  
# compile [args]
&emsp;**[arg 1]** : Types. Initial one only provide c and cpp (e.g. -c, -cpp)  
&emsp;&emsp;&emsp;&emsp;&emsp;**If you postfix (or suffix) " clean", it'll clean the executable file for that type.**  
&emsp;**[arg n]** : for n >= 1, you can assign specific files.  
# run [args]
&emsp;**run**                 : run all. **It'll include all files in that directory**  
&emsp;**run -char_ins**       : run with ascii inspector.  
&emsp;**run -valgrind**       : run with valgrind, memory inspector.  
&emsp;**run -test**           : run all with testfile. e.g. a =(find)=> a.test and ./a < a.test  
&emsp;**run -type**           : run all the files with that type. e.g. run -c (-test)  
&emsp;**run executable_file** : run all assigned executable_file (normal file with shebang is also available ~)  
# char_ins
&emsp;**--help** : show the usage of char_ins
# tfmanager [arg]
&emsp;**-merge**  : merge all .test file into all.test  
&emsp;**-divide** : divide all.test into original .test  
