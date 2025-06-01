# IDE-from-zonda

***IDE-from-zonda** is an easy and light-weight IDE so it's not that powerful.*  

## Initialization

Download and Initialize by 
```sh  
git clone https://github.com/zonda-tsai/IDE-from-zonda
bash IDE-from-zonda/initial.sh
```
  
## settings

   - `-reset`     : Reset the IDE  
   - `-uninstall` : Uninstall this IDE
   - `--help`     : Show this README.md

## compile

   - `[arg 1]` : Types. Initial one only provide c and cpp (e.g. -c, -cpp)  
       + **Postfix (or suffix) " clean" can clean the executable file for that type.**  
   - `[arg n]` : for n >= 1, you can assign specific files.

## run
   - `-type`, `-char_ins`, `-valgrind` and `-test` can be suffixed at the same time.  
&emsp;// ***Type should always be the first argument after run***  
   - ``run``                 : run all. **It'll include all files in that directory**  
   - ``run -char_ins``       : run with ascii inspector.  
   - ``run -valgrind``       : run with valgrind, memory inspector.  
   - ``run -test``           : run all with testfile. e.g. `./a < a.test`  
   - ``run -type``           : run all the files with that type. e.g. run -c (-test)  
   - ``run executable_file`` : run all assigned executable_file and files prefixed shebang

## char_ins
   - ``--help``        : show the usage of char_ins
   - ``--ascii_table`` : show the ascii table (in HEX)  

## tfmanager
   - ``-merge``  : merge all .test file into all.test  
   - ``-divide`` : divide all.test into original .test files  
