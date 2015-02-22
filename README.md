
# CASH - The C Again SHell

The initial goal for this project was to develop a shell which could process C language scripts, but more efficiently and powerfully than the known CSH. The compatibily to BASH has been kept where possible, for example with the keyboard input and the PS1 variable, for which the control characters are exactly the same as the ones in BASH. The following features are available in CASH:

## Main instructions

### Compiling CASH
```
$ autoreconf --install
$ ./configure
$ make
```
The executable `cash` is available inside the `src` folder.

### Testing CASH
```
$ cp src/cash tests/cash
$ bash tests/run_test.sh
```

### Executing CASH
```
$ ./cash
```

### Executing CASH scripts
```
$ ./cash <script.cash>
```

### Help and version
```
$ ./cash -hv
```

## Keyboard Input

CASH provides the user a friendly and functional terminal.
The following keys (and combinations) can be used:

- **Backspace** - removes the previous character
- **Home** - moves the cursor to the beginning of the line
- **End** - moves the cursor to the end of the line
- **Left and Right arrows** - moves the cursor between the characters in the line
- **Up and Down arrows** - selects and navigates the command history
- **Ctrl + K** - erases all characters till the end of the line
- **Ctrl + L** - cleans the entire screen
- **Ctrl + W** - erases the word closest to the cursor
- **Tab** - auto-complete

## PS1 Variable

CASH is totally compatible with BASH in relation to the PS1 variable. ALL control characters supported by BASH are also supported by CASH.

## Command History

User commands are stored in the file .cash\_profile. When CASH starts, this file is loaded and the history is stored in memory using a circular list and a hash table. The list is used for quick navigation with the keyword arrows and the hash table for reverse search, although it is currently not accessible via the terminal. The file is only updated when CASH successfully exits.

## Auto-Complete

CASH is equipped with a simple but extremely functional auto-complete, which can be used to finish file or directory names.

## Executing commands

Any command can be called normally via CASH terminals. The PATH environment variable is used for command search, which allows users to use simple command names, not needing a complete path. The command search is executed via the system call (syscall) `execvp`. CASH also allows redirecting in and output using the following:

```
command1 > output.txt
command1 >> output.txt

command2 < input.txt
command2 << input.txt
```

Using the `>` operand the file will be replaced by the output from `command1` meanwhile `>>` will only append the output to whatever content the destination file already had.

Pipes `|` are also supported by CASH:

```
command1 | command2
```

In the above given example, the `command1` output will be used as input for `command2`. It is important to note that CASH also tries to support multiple pipes:

```
command1 | command2 | command3 | ...
```

Internally a binary tree is built to correctly execute the commands.

## Job Control

CASH offers possiblity to control _Jobs_ through the following _built-in_ commands:

- `jobs` - Lists all current processes (stopped, killed, etc)
- `bg` - shifts a process to background
- `fg` - shifts a process to foreground

The sintax of those commands are the same as in BASH. The following signals can also be used in process:

- **Ctrl + Z** - Stops the current process
- **Ctrl + C** - Sends a SIGINT to kill the current process

## Script language support

```java
#!/bin/cash

# This is a comment
/* This is also a comment */
// This too, is a comment

echo "CASH can read C-like scripts!";

var1 = 12+1;

echo "One can use \$ to recover variable data, like this: var1=$var1";
echo "Also a more C-Like approach with (\*variable) is available: var1=(*var1)";
echo "See below a couple of operation results:";

var2 = 2 + 3;
echo "2+3 = (*var2)";
var2 = 2 - 3;
echo "2-3 = (*var2)";
var2 = 2 * -3;
echo "2*-3 = (*var2)";
var2 = 6 / 3;
echo "6/3 = (*var2)";
var2 = 7 % 4;
echo "7%4 = (*var2)";

var2 = 1;
var2++;
echo "CASH can also have unary operators. Starting from 1 with +1: (*var2)";
var2--;
echo "Then going back to 1 with -1: (*var2)";

echo "Lazy while coding? No problem, CASH supports operation + assigment contractions:";
var2 = 1;
echo "From 1: (*var2)";
var2 += 2;
echo "Plus 2: (*var2)";
var2 *= 3;
echo "Times 3: (*var2)";
var2 -= 1;
echo "Minus 1: (*var2)";
var2 /= 5;
echo "Divided by 5: (*var2)";

echo "Note the last operation was an integer division. CASH also support floats:";

var2 = 2.0 + 3E2;
echo "2.0+3E2 = (*var2)";
var2 = 2e-1 - 3.2;
echo "2e-1-3.2 = (*var2)";
var2 = 2 * -3E+1;
echo "2*-3E+1 = (*var2)";
var2 = 8.0 / 3.0;
echo "8.0/3.0 = (*var2)";

var2 = 1.2;
var2++;
echo "Floats can also have unary operators. Starting from 1.2 with ++: (*var2)";
var2--;
echo "Then going back to 1.2 with --: (*var2)";

echo "Floats are also in contractions:";
var2 = 1e-0;
echo "From 1e-0: (*var2)";
var2 += 2.0;
echo "Plus 2.0: (*var2)";
var2 *= 3E+0;
echo "Times 3E+0: (*var2)";
var2 -= 1.0;
echo "Minus 1.0: (*var2)";
var2 /= 5e0;
echo "Divided by 5e0: (*var2)";

var2 = "Hello";
echo "CASH also understants strings: (*var2)";
var2 += " World" + "!";
echo "The only supported operations are concatenations (involving +): (*var2)";

var2 = "a float "+3.2E2+" with an int "+2;
echo "From dynamic types, it is also expected to mix and match: (*var2)";

echo "Do you want to know if it works with the right precedence?";
f = 10.0;
var2 = 3+17e-1*(42%13+4-(f+=2e1/2*7+17%5))-23.0;
echo "Here: f = 10.0 and 3+17e-1*(42%13+4-(f+=2e1/2*7+17%5))-23.0 = (*var2)";
/**
 * The variable f started with 10.0, so the value should be -147.5.
 * Make and compile a C code if you are skeptical
 */

echo "Beware: the assignment in the middle of the expression might not work.";
echo "This is due to some limitations and orders in conversion of types.";
#One can doubt that is actually ever a must in an expression.

echo "These are the totally unsuported operators:";
echo "'[]' '->' '.' '~' '(type)' '&' 'sizeof' '<<' '>>' '^' '|' '?:'";

echo "Now moving on to logic...";

var3 = /*Are you ready?*/ "yes";

if(var3 == "yes"){
	echo "...so let's proceed.";
} else {
	//echo "never";
}

echo "Logical expressions always return integers, but 'ifs' support all types:";

if("" /*means false*/){
	echo "This will never be executed.";
}
else if(1.0){
	echo "This will be executed.";
}

echo "CASH also support loops, starting with 'for's.";
echo "See bellow how to calculate common multiples.";

var3 = "0";
for(i = 0, a = 2, b = 3, k = 1; i < 10; k++){
	/* Important: be careful with infinite loops, it will lead CASH to CrASH */
	if((a % 3 == 0) && (b % 2 == 0)){
		var3 += ", "+a;
		i++;
	}
	if(a > b){
		b += 3;
	}
	else {
		a += 2;
	}
}

echo "The after (*k) attempts, found first 10 common multiples of 2 and 3:";
echo "$var3";

echo "'while's and 'do-while's are also supported.";
echo "See bellow all primes until 50.";

i = 1, j = 0;
while(i <= 50){
	j = i-1;
	prime_flag = 1;
	do{
		//Note for the parenthesis on the -- operator.
		//It works a bit differently (more strict, always "post")
		if((j--) > 2){
			if((i % j) == 0){
				prime_flag = 0;
			}
		}
		//This creepy logic is just to show in this script the remaining operators.
	} while((j >= 2) && !(prime_flag != 1));

#Not sure if 1 is a prime... just for testing operators.
	if(prime_flag == 1 || i == 1){
		echo "The number (*i) is a prime!";
	}
	i += 1;
}

echo "Unfortunately for loops CASH does not support 'break' or 'continue'";
echo "Here is the end of the tutorial/manual/success_coverage_test!";

/* THE END */
```

## Developers

Matheus Borges Teixeira, matheus.bt at gmail.com

Renê de Souza Pinto, rene at renesp.com.br

Thiago Camargo Fernandes, thiagocf at grad.icmc.usp.br

## License 

CASH is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

CASH is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You don't need ca$h to use CASH! Have fun! :)


