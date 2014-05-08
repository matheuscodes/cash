/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * signals.c
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

#include "signals.h"


extern pid_t cash_pid;
extern job current_job;
extern linked_list *jobs_list;
extern int fd_out;

/**
 * Start the CASH signals
 *
 */
void start_signals(void) {

	/* Ctrl-C SIGINT */
	signal(SIGINT, handle_sigint);

	/* Ctrl-Z SIGTSTP */
	signal(SIGTSTP, handle_sigstop);

	/* For cd command */
	signal(SIGUSR1, change_dir_sig);
}


/**
 * Handle the SIGINT signal
 *
 * @param sig Signal ID
 */
void handle_sigint(int id) {

	kill(current_job.id, SIGINT);
	write(fd_out, "\n", 1);
}


/**
 * Handle the SIGTSTP signal
 *
 * @param sig Signal ID
 */
void handle_sigstop(int id) {

	job *new_job;
	linked_list *ltemp;
	unsigned int pnum;
	char info_str[1024];

	new_job = (job*)malloc(sizeof(job));
	if(new_job == NULL) {
		show_error("Internal error! Cannot allocate memory for process information! =:0\n", COMES_FROM_TERMINAL);
		return;
	}

	strcpy(new_job->name, current_job.name);
	new_job->id    = current_job.id;
	new_job->state = JOB_STOPPED;

	/* Get a job number */
	pnum  = 1;
	ltemp = jobs_list;
	while(ltemp != NULL) {
		job *proc = ltemp->element;
		pnum      = proc->num + 1;
		ltemp     = ltemp->next;
	}
	new_job->num = pnum;

	/* Add to the job's list */
	llist_add(&jobs_list, new_job);

	/* Stop process, free terminal and show information */
	kill(new_job->id, SIGTSTP);

	sprintf(info_str, "[%d]+ Stopped                   %s\n", new_job->num, new_job->name);
	write(fd_out, info_str, strlen(info_str));
}


/**
 * Change the current working directory
 *
 * @param sig Signal ID
 */
void change_dir_sig(int sig) {

	char *path = getenv("PWD");
	char str_err[1024];

	if(path != NULL) {
		if( chdir(path) == -1 ) {
			sprintf(str_err, "cd: %s: No such file or directory\n", path);
			show_error(str_err, COMES_FROM_TERMINAL);
		}
	}
}

