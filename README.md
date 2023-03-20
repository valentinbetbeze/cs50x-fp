# Simple bash-like command-line interface.
#### Video demo: [link]([empty](https://www.youtube.com/watch?v=UXgAKzzFCYo))
<br>
<img src=screenshot.png width=320></img><br>
<br>

## TL;DR <hr>
>With this program, you can run basic commands to visualize, move around, and modify the directory structure of your system. It is also possible to compile individual C source files, and run their executables.

<br>

## What is this program doing? <hr>
The program is a command-line interface (or interpreter) that allows the user to enter command lines to operate various actions, such as listing the content of a directory, moving around the directory structure, modifying the directory structure and much more.<br>
<br>
To do so, the program first retrieves, parses, and stores the input into a linked list. It then goes through multiple conditionnal statement and checks the first argument, located in the head of the linked list. If valid, the program will call and execute the command along with other arguments present in the linked list.<br>
<br>
At the code source level, the program is split in 2 headers and 3 source codes, namely `cli.c`, `utils.c`, and `commands.c`.
* The main file `cli.c` contains the high-level structure of the code. Its responsability is to ensure that input retrieval, parsing, function calls, and error-handling modules are well implemented and working in tandem.<br>
<br>
* The `utils.c` file, along with its header `utils.h`, contains utility functions, constants as well as the important `Token` struct, the building block of the linked-list used for storing the parsed input.<br>
<br>
* Finally, the `commands.c` file contains all the command functions which are called in the `cli.c` code file. Each command function has its own error-handling, memory allocation and freeing mechanism. This way, all command functions are independant from each other, and new functions can be added safely to the program.<br>
<br>

## Getting started <hr>

1. **Compiling and executing the program** <br>
The current executable is compatible with Unix-like systems and can be executed by typing `./cli` in the command shell. If you are using another operative system, you must recompile the program and may have to modify the Makefile in the process.<br>

2. **Using the program** <br>
Once compiled and executed, a message stating how to exit the program should appear. Only then can the user start entering commands. The command-line interface uses the sign `£` to indicate the start of a prompt line. After writing the command line, the user shall hit the `enter` key to send the input.<br>

3. **Shuting down the program** <br>
To shut down the program, enter `exit` and hit the `enter` key.<br>
<br> 

## Available commands <hr>
For proper use, commands and options must be entered with the following format : `£ [command] [option1] [option2] [...]` using whitespaces between each argument. **The use of double quotation marks `" "` allows the presence of whitespaces within an argument.**<br>
<br>
The following commands and options are available for use:<br>
* `echo` :  Display the input argument as output
  
* `pwd` :  Display the current working directory
  
* `ls` : Display the content of the directory given as input. Available options:
  * `-l` : Provide additional details
  * `-a` : Show invisible files or directories
  
* `cd` : Change the current working directory to the directory given as input. The path can be both relative to the current working directory or absolute (from the root).

* `touch` : Create a file.

* `rm` : Remove a file (by default). Available options:
  * `-i` : Prompt a confirmation message
  * `-d` : Enable the deletion of an empty-only directory
  * `-r` : Enable the deletion of a directory and all of its content. Use of `-i` option is advised with this option. Use with caution.

* `mkdir` : Create a directory

* `rmdir` : Remove an empty-only directory

* `mv` : 2 possibilities:
  * Rename a file if 2 file names are given. Use the following format: `mv [oldname] [newname]`
  * Change the location of a file if a file name and a path are given as arguments. Use the following format: `mv [filename] [path]`

* `cat` : Display the content of a file in human-readable format.

* `make` : Compile a single C source code file and create its executable. Will not work if the C source code file has dependencies to other custom files.

* `./` : Execute a program.

* `exit` : Shut down the program.