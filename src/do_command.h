/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * do_command.h
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

#ifndef __DO_COMMAND__

	#define __DO_COMMAND__

	#include "interpreter.h"
	#include "btree.h"
	#include "linkedl.h"
	#include "shell.h"
	#include "builtin_c.h"
	#include <sys/mman.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/wait.h>
	#include <signal.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <errno.h>

	#define MSG(a) a, strlen(a)

	#define COMES_FROM_SHELL	0
	#define COMES_FROM_TERMINAL 1

	/* Prototypes */
	char *do_command(char *command, int from);
	void process_command(btree *exec_tree, int from);
	int  execute_command(char *command, int from);
	char *buff_read(int fd);
	void show_error(char *msg, int from);

	extern char *comm_to_exec;
	extern char **environ;

#endif /* __DO_COMMAND__ */
