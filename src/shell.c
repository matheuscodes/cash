/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * shell.c
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

#include "shell.h"


/* Global Variables */
pid_t cash_pid;						/* ID of cash (main shell)							 */
job current_job;					/* Info about current process 						 */
unsigned int n_jobs;				/* The number of jobs currently managed by the shell */
linked_list  *jobs_list;			/* List of jobs 									 */
linked_clist *c_history;			/* Command history list								 */
linked_list **c_hashtable;			/* Hash table to command history					 */
char *term_name;					/* The basename of the shell's terminal device name  */
int fd_out;							/* File descriptor for standard output				 */

/* Extern global variables */
extern char *name_of_cash;			/* Cash executable file name					*/
extern char *VERSION;				/* Version of CASH								*/
extern char *RELEASE;				/* Release of CASH								*/


/**
 * Initialize the Shell environment
 * 
 * @author Renê de Souza Pinto
 * @return int 1 on success, on error 0 is returned
 */
int init_shell(void) {

	/* Initialize the console */
	fd_out = STDIN_FILENO;

	/* Backup the current configuration */
	if(tcgetattr(fd_out, &old_term) == -1)
			return(0);

	/* Set the new configuration for console */
	terminal = old_term;
	terminal.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	if(tcsetattr(fd_out, TCSANOW, &terminal) == -1)
			return(0);

	/* Get the terminal pathname */
	term_name = ttyname(fd_out);

	/* Initialize job's list */
	if(!llist_create(&jobs_list))
			return(0);

	/* Initialize command's history */
	if(!clist_create(&c_history))
			return(0);
	if(!create_hash_table(&c_hashtable))
			return(0);

	/* Read history from file */
	load_history_file(HISTORY_FILE, &c_hashtable, &c_history);

	/* Get the proccess ID */
	cash_pid = getpid();

	/* Initialize the signals */
	start_signals();

	/* Return */
	return(1);
}


/**
 * Set default settings to terminal
 */
void back_term() {
	tcsetattr(fd_out, TCSANOW, &old_term);
}


/**
 * Set cash settings to terminal
 */
void cash_term() {
	tcsetattr(fd_out, TCSANOW, &terminal);
}


/**
 * Finalize the Shell environment
 *
 * @author Renê de Souza Pinto
 */
int finalize_shell(void) {

	linked_clist *htemp;
	linked_list *jtemp;
	unsigned long int x;


	/* Restore the terminal settings */
	if(tcsetattr(fd_out, TCSANOW, &old_term) == -1) {
			return(0);
	}

	/* Save history file */
	write_history_file(HISTORY_FILE, c_history);

	/* Free memory */
	htemp = c_history;
	x     = 0;
	while(x < clist_get_size(c_history) && c_history != NULL) {
			char *str = (char*)htemp->element;
			if(str != NULL)
					free(str);
			htemp = htemp->next;
			x++;
	}
	if(!destroy_hash_table(&c_hashtable))
			return(0);
	if(!clist_destroy(&c_history))
			return(0);

	jtemp = jobs_list;
	x = 0;
	while(x < llist_get_size(jobs_list) && jtemp != NULL) {
		if(jtemp->element != NULL)
			free(jtemp->element);
		jtemp = jtemp->next;
		x++;
	}
	if(!llist_destroy(&jobs_list))
			return(0);

	if(name_of_cash != NULL)
			free(name_of_cash);

	return(1);
}


/**
 * Start the Shell
 *
 * @author Renê de Souza Pinto
 * @return int EXIT_FAILURE on error, EXIT_SUCCESS otherwise.
 */
int goto_shell(void) {

	char *command;

	/* Init the Sell */
	if( !init_shell() ) {
		fprintf(stderr, "\nThe Shell could not be initialized. Aborted.\n");
		finalize_shell();
		return;
	}

	/* CASH Shell :) */
	while(1) {
		command = readline();
		if(strcmp(command, "exit") != 0) {
			/* Process command */
			do_command(command, COMES_FROM_TERMINAL);
		} else {
			/* Exit from Shell */
			write(fd_out, "\n", 1);
			break;
		}
	}

	/* Finalize the shell */
	finalize_shell();
	return(EXIT_SUCCESS);
}

