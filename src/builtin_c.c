/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * builtin_c.c
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

#include "builtin_c.h"

/* Global variables */
extern pid_t cash_pid;
extern linked_list *jobs_list;
extern int fd_out;


/* Cash built-in functions */
builtin_f cash_funcs[] = {
	{"jobs" , jobs       },
	{"fg"   , fg         },
	{"bg"   , bg         },
	{"cd"   , change_dir },
	{""     , NULL       }
};


/**
 * Handle the jobs
 *
 * @author Renê de Souza Pinto
 * @param args List of arguments
 * @return -1 on error, 0 otherwise
 */
int jobs(char **args) {

	linked_list *ltemp;
	job *jtemp;
	char c;
	char str_info[1024];
	char state[30];

	ltemp = jobs_list;
	while(ltemp != NULL) {

		jtemp = ltemp->element;
		if(jtemp != NULL) {
			if(ltemp->next == NULL) {
				c = '+';
			} else if(ltemp->next->next == NULL) {
				c = '-';
			} else {
				c = ' ';
			}

			switch(jtemp->state) {
				case JOB_RUNNING: strcpy(state, "Running");
								  break;

				case JOB_STOPPED: strcpy(state, "Stopped");
								  break;

				case JOB_KILLED:  strcpy(state, "Killed");
								  break;

				default:		  strcpy(state, "??????");
								  break;
			}

			if(args != NULL) {
				if(args[1] != NULL) {
					if(strcmp(args[1], "-p") == 0) {
						sprintf(str_info, "%d\n", jtemp->id);
					} else if(strcmp(args[1], "-l") == 0) {
						sprintf(str_info, "[%d]%c %d %.25s %s\n", jtemp->num, c, jtemp->id, state, jtemp->name);
					} else {
						sprintf(str_info, "[%d]%c %.25s %s\n", jtemp->num, c, state, jtemp->name);
					}
				} else {
					sprintf(str_info, "[%d]%c %.25s %s\n", jtemp->num, c, state, jtemp->name);
				}
			} else {
				sprintf(str_info, "[%d]%c %.25s %s\n", jtemp->num, c, state, jtemp->name);
			}
			write(fd_out, str_info, strlen(str_info));
		}

		ltemp = ltemp->next;
	}

}


/**
 * Put a job on foreground
 *
 * @author Renê de Souza Pinto
 * @param args List of arguments
 * @return -1 on error, 0 otherwise
 */
int fg(char **args) {

	return( bf_g(args, 1) );
}


/**
 * Put a job on background
 *
 * @author Renê de Souza Pinto
 * @param args List of arguments
 * @return -1 on error, 0 otherwise
 */
int bg(char **args) {

	return( bf_g(args, 0) );
}


/**
 * Put a job on foreground or background
 *
 * @author Renê de Souza Pinto
 * @param args List of arguments
 * @param where 1 - Foreground, 0 - Background
 * @return -1 on error, 0 otherwise
 */
int bf_g(char **args, int where) {

	linked_list *ltemp;
	job *jtemp;
	int num, st;
	char str_err[255];
	char founded = 0;
	siginfo_t sigi;
	unsigned long p;

	if(args != NULL)
		if(args[1] != NULL) {
			num = atoi(args[1]);
		} else {
			num = -1;
		}

	p     = 0;
	ltemp = jobs_list;
	while(ltemp != NULL) {
		jtemp = ltemp->element;

		if(jtemp != NULL) {
			if(jtemp->num == num) {
				founded = 1;
				break;
			} else {
				if(ltemp->next == NULL)
					break;
			}
		}

		ltemp = ltemp->next;
		p++;
	}

	if(num != -1 && !founded) {
		sprintf(str_err, "fg: %d: no such job\n", num);
		write(fd_out, str_err, strlen(str_err));
	} else if(num == -1 && !founded){
		if(where == 1) {
			sprintf(str_err, "fg: current: no such job\n");
		} else {
			sprintf(str_err, "bg: current: no such job\n");
		}
		write(fd_out, str_err, strlen(str_err));
	} else {
		llist_remove(&jobs_list, p);
		kill(jtemp->id, SIGCONT);

		if(where == 1) {
			//
		}
	}

}


/**
 * Change the current working directory
 *
 * @author Renê de Souza Pinto
 * @param args List of arguments
 * @return -1 on error, 0 otherwise
 */
int change_dir(char **args) {

	char *pathname;
	char *home;
	char c;
	int x;

	home = getenv("HOME");

	if(args != NULL) {
		if(args[1] != NULL) {

			char *newpath = args[1];

			if(newpath[0] == '~') {
				pathname = (char*)malloc( sizeof(char) * (strlen(newpath) + strlen(home)) );

				if(pathname != NULL) {
					strcpy(pathname, home);
					strcat(pathname, &newpath[1]);
					setenv("PWD", pathname, 1);
					free(pathname);
				}

			} else {
				setenv("PWD", newpath, 1);
			}

		} else {
			setenv("PWD", home, 1);
		}
	} else {
		setenv("PWD", home, 1);
	}
	return( kill(cash_pid, SIGUSR1) );

}

