/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * shell.h
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

#ifndef __CASH_SHELL__

	#define __CASH_SHELL__

	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <termios.h>
	#include <sys/types.h>

	#include "linkedl.h"
	#include "linkedcl.h"
	#include "signals.h"
	#include "do_command.h"
	#include "cash_readline.h"
	#include "interpreter.h"

	#define HISTORY_SIZE	500					/* Size of the command's history		*/
	#define HISTORY_FILE	".cash_history"		/* Command history file name			*/


	/* Proccess info */
	typedef struct _proccess_info {
		int state;			/* The state of proccess */
		pid_t id;			/* Proccess id 			 */
		char *command;		/* Command string		 */
	} proccess_info;


	/* Global variables */
	static struct termios old_term;		/* Old terminal attributes 			   			*/
	static struct termios terminal;		/* The curent terminal attributes				*/


	/* Prototypes */
	int init_shell(void);
	void back_term();
	void cash_term();
	int finalize_shell(void);
	int goto_shell(void);

#endif /* __CASH_SHELL__ */

