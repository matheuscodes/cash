/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * readline.h
 *
 * This file is part of CASH, CASH is The C Again SHell.
 *
 * CASH is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CASH is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CASH_READLINE__

	#define __CASH_READLINE__

	#include <stdio.h>
	#include <string.h>
	#include <unistd.h>
	#include <sys/types.h>
	#include <pwd.h>
	#include <time.h>
	#include <ctype.h>
	#include <dirent.h>
	#include "shell.h"
	#include "linkedcl.h"
	#include "autocomplete.h"


	/* Keyboard keys */
	#define ENTER_KEY		0x0A
	#define ESC_KEY			0x1B
	#define TAB_KEY			0x09
	#define BACKSPACE_KEY	0x7F

	#define CTRL_R			0x12
	#define CTRL_D			0x04
	#define CTRL_C			0x03
	#define CTRL_Z			0x1A
	#define CTRL_K			0x0B
	#define CTRL_L			0x0C
	#define CTRL_W			0x17

	#define UP_ARROW		"\033[A"
	#define DOWN_ARROW		"\033[B"
	#define RIGHT_ARROW		"\033[C"
	#define LEFT_ARROW		"\033[D"
	#define HOME_KEY		"\033[1"
	#define DEL_KEY			"\033[3"
	#define END_KEY			"\033[4"
	#define PAGE_UP			"\033[5"
	#define PAGE_DOWN		"\033[6"

	#define HOME_KEY2		"\033[H"
	#define END_KEY2		"\033[F"

	/* Default buffers size */
	#define DEFAULT_BSIZE		  1024
	#define MAX_PATH_STRING_SIZE  2048
	#define MAX_STRING_SIZE  	   200

	/* Terminal control sequences */
	#define TERM_CLEAR_SCREEN	  "\033[2J\033[H"
	#define TERM_CLEAR_LINE_END	  "\033[K"
	#define TERM_SCROLL			  "\033[\%d;\%dr"
	#define TERM_MOVE_CURSOR	  "\033[\%d;\%dH"
	#define TERM_HILITE_ON		  "\033[m"
	#define TERM_HILITE_OFF		  "\033[7m"

	#define DEFAULT_PS1		"cash> "

	/* Buffer */
	typedef struct _LINE_BUFFER {
		char *line;					/* Pointer to line 		*/
		size_t max_size;			/* Max length of line 	*/
		unsigned long size;			/* Length of line  		*/
		unsigned long cur_pos;		/* Cursor position 		*/
	} LINE_BUFFER;


	/* Global variables */
	static LINE_BUFFER *old_buff      = NULL; /* The last line readed by readline function		 	 */
	static unsigned int comm_n        = 1;    /* Command number (how many times readline was called) */
	static unsigned long int comm_hn  = 0;	  /* The history number of command 						 */


	/* Extern global variables */
	extern int fd_out;				/* File descriptor for standard output				 */
	extern unsigned int n_jobs;		/* The number of jobs currently managed by the shell */
	extern char *term_name;			/* The basename of the shell's terminal device name  */


	/* Prototypes */
	char *readline(void);
	char *read_ps_string(char *ps_str, size_t *u_size);
	char *strrcat(char *dest, char *src, size_t dest_size);
	void write_prompt(void);
	int oct2dec(char *octn);

#endif /* __CASH_READLINE__ */

