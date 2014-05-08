/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * builtin_c.h
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

#ifndef __BUILTIN_COMMANDS__

	#define __BUILTIN_COMMANDS__

	#include <string.h>
	#include <unistd.h>
	#include "signals.h"

	#define NB_COMMANDS 4

	#define JOB_RUNNING 1
	#define JOB_STOPPED 2
	#define JOB_KILLED  3

	/* Built-in commands struct */
	typedef struct _builtin_f {
		char func_name[128];
		int (*func_addr)(char **);
	} builtin_f;

	/* Job struct */
	typedef struct _job {
		char name[1024];
		int  state;
		pid_t id;
		unsigned int num;
	} job;

	/* Prototypes */
	int jobs(char **args);
	int fg(char **args);
	int bg(char **args);
	int bf_g(char **args, int where);
	int change_dir(char **args);

#endif /* __BUILTIN_COMMANDS__ */
