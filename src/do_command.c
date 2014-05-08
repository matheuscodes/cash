/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * do_command.c
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

#include "do_command.h"

/* Global variables */
extern builtin_f cash_funcs[];
extern job current_job;

int pfd[2];
int fdo;

/**
 * Process a comand line (pipes, etc)
 *
 * @author Renê de Souza Pinto
 * @param command String to be processed
 * @param from Where the command are from (shell script or terminal line)
 * @return char* Return from command
 */
char *do_command(char *command, int from) {

	char **tokenlist;
	btree *exec_tree;
	char *res;
	size_t i, nt;
	int st;

	/* Get the tokens and create the binary tree */
	nt = gettokens(command, &tokenlist);
	btree_create(&exec_tree);

	for(i=0; i<nt; i++) {
		btree_insert(&exec_tree, tokenlist[i]);
	}

	/* Set old terminal settings */
	back_term();

	/* Execute the commands and pipes */
	pipe(pfd);
	if(vfork() == 0) {

		//fdo = open("ctmp", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);

		/* Reditrect the output *
		close(1);
		dup(fdo);*/

		process_command(exec_tree, from);

		//close(fdo);
		exit(0);
	} else {
		waitpid(-1, &st, WUNTRACED);
	}

	/* Restore cash terminal */
	cash_term();

	destroy_tokenlist(&tokenlist, nt);
	btree_destroy(&exec_tree);
	//TODO res never set!
	return(res);
}


/**
 * Process a command binary tree
 *
 * @author Renê de Souza Pinto
 * @param command The command binary tree
 * @param from Where the command are from (shell script or terminal line)
 * @return The output of execution //TODO WTF, return is void.
 */
void process_command(btree *exec_tree, int from) {

	btree *child_l, *child_r;
	char *buffer, str_err[1024];
	int fdo, st1, st2;
	pid_t pid;

	if(exec_tree != NULL) {

		child_l = exec_tree->left;
		child_r = exec_tree->right;

		if( is_pipe(exec_tree->element) && child_l != NULL) {

			/* Do a pipe */
			if(strcmp(exec_tree->element, "|") == 0) {
				int fds[2];

				if(pipe(fds) < 0)
					return;

				if(strncmp(child_l->element, "|", 1) != 0) {

					if( is_pipe(child_l->element) || is_pipe(child_r->element) ) {
						sprintf(str_err, "syntax error near unexpected token `%s'\n", child_l->element);
						show_error(str_err, from);
						return;
					}

					if(fork() == 0) {
						/* Redirect the output */
						close(1);
						dup(fds[1]);
						close(fds[0]);
						close(fds[1]);

						if( execute_command(child_l->element, from) == -1)
							exit(1);
					} else {
						wait(&st1);

						/* Redirect the input */
						close(0);
						dup(fds[0]);
						close(fds[0]);
						close(fds[1]);

						if(fork() == 0) {
							if( execute_command(child_r->element, from) == -1)
								exit(1);
						} else {
							wait(&st2);
						}
					}
				} else {

					/*int fdi = open("ctmp", O_RDONLY);*/
					if(vfork() == 0) {
						/* Redirect the input *
						close(0);
						dup(fdi);*/

						if( execute_command(child_r->element, from) == -1)
							exit(1);
					} else {
						wait(&st1);
						//close(fdi);
					}
				}

			/* Redirect the standard output */
			} else if(strcmp(exec_tree->element, ">")  == 0 ||
					  strcmp(exec_tree->element, ">>") == 0) {

				int fdo;

				if( is_pipe(child_l->element) || is_pipe(child_r->element)) {
						sprintf(str_err, "syntax error near unexpected token `%s'\n", child_l->element);
						show_error(str_err, from);
						return;
				}

				/* Open/create the file output */
				if(strcmp(exec_tree->element, ">") == 0) {
					fdo = open(clean_string(child_r->element), O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
				} else {
					fdo = open(clean_string(child_r->element), O_APPEND | O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
				}

				if(vfork() == 0) {
					/* Redirect the output */
					close(1);
					dup(fdo);

					if( execute_command(child_l->element, from) == -1)
							exit(1);
				} else {
					close(fdo);
				}

			/* Redirect the standard input */
			} else if(strcmp(exec_tree->element, "<")  == 0 ||
			          strcmp(exec_tree->element, "<<") == 0) {

				int fdi;

				if( is_pipe(child_l->element) || is_pipe(child_r->element) ) {
						sprintf(str_err, "syntax error near unexpected token `%s'\n", child_l->element);
						show_error(str_err, from);
						return;
				}

				/* Open/create the file output */
				fdi = open(clean_string(child_r->element), O_RDWR);
				if(fdi < 0) {
					sprintf(str_err, "%s file not found\n", child_r->element);
					show_error(str_err, from);
				}

				if(vfork() == 0) {
					/* Redirect the input */
					close(0);
					dup(fdi);

					if( execute_command(child_l->element, from) == -1)
							exit(1);
				} else {
					wait(&st1);
					close(fdi);
				}


			}

		} else if( !is_pipe(exec_tree->element) && child_l == NULL && child_r == NULL) {

			/* Execute a simple command */

			if( is_pipe(exec_tree->element) ) {
				sprintf(str_err, "syntax error near unexpected token `%s'\n", child_l->element);
				show_error(str_err, from);
				return;
			}
			if( execute_command(exec_tree->element, from) == -1 )
				exit(1);

		} else {
			process_command(child_l, from);
			process_command(child_r, from);
		}
	}

}


/**
 * Execute a command (just a friendly interface for execvp syscall)
 *
 * @author Renê de Souza Pinto
 * @param command The command to be executed
 * @param from Where the command are from (shell script or terminal line)
 * @return -1 On error
 */
int execute_command(char *command, int from) {

	int status, i, j, k, n;
	linked_list *params, *temp;
	char *str, **v_par;
	char str_err[1024];

	/* Get command parameters */
	if( !llist_create(&params) )
		return(-1);

	str = clean_string(command);
	if(str == NULL)
		return(-1);
	k = strlen(str);

	j = n = i = 0;
	while(i <= k) {
		if((str[i] == ' ' && str[i-1] != '\\') || str[i] == '\0') {
				str[i] = '\0';
				if(strlen(&str[j]) > 0) {
						llist_add(&params, (char*)&str[j] );
						j = i + 1;
						n++;
				}
		}
		i++;
	}

	/* Pass parameters to a vector */
	v_par = (char**)calloc(sizeof(char*), sizeof(char*) * n);
	if(v_par == NULL)
		return(-1);

	temp = params;
	i    = 0;
	while(temp != NULL) {
		v_par[i] = (char*)temp->element;
		temp     = temp->next;
		i++;
	}

	/* Check for a built-in command */
	for(i=0; i<NB_COMMANDS; i++) {
		if(strcmp(str, cash_funcs[i].func_name) == 0) {
			(cash_funcs[i].func_addr)(v_par);
			exit(0);
		}
	}

	/* Call execvp */
	current_job.id     = getpid();
	current_job.state  = JOB_RUNNING;
	strcpy(current_job.name, command);
	execvp(str, v_par);

	if(from == COMES_FROM_TERMINAL) {
		sprintf(str_err, "%s: command not found\n", str);
		show_error(str_err, from);
	}

	return(-1);
}


/**
 * Read data from buffer
 *
 * @author Renê de Souza Pinto
 * @param fd File descriptor
 * @return char* Data readed
 */
char *buff_read(int fd) {

	char *buffer;
	struct stat finfo;

	/* Get file info */
	if(fstat(fd, &finfo) < 0)
		return(NULL);

	/* Allocate memory */
	buffer = (char*)malloc(sizeof(char) * finfo.st_size);
	if(buffer == NULL)
		return(NULL);

	/* Read the data */
	lseek(fd, 0, SEEK_SET);
	read(fd, buffer, finfo.st_size);

	return(buffer);
}


/**
 * Show a error message
 *
 * @author Renê de Souza Pinto
 * @param msg Error message string
 * @param from Where the command are from (shell script or terminal line)
 */
void show_error(char *msg, int from) {

	if(from == COMES_FROM_SHELL) {
			//TODO: show Line numbers, etc...
	} else {
			write(fd_out, "cash: ", 6);
			write(fd_out, MSG(msg));
	}
}

