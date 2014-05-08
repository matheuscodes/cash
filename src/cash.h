/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * cash.h
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

#ifndef __CASH__

	#define __CASH__

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "shell.h"
	#include "cparser/cparser.h"

	#define ARG_HELP		0x00000001
	#define ARG_VERSION		0x00000002
	#define ARG_INVALID		0x10000000


	/* Global variables */
	static unsigned int user_options;

	/* Global extern variables */
	extern pid_t parent_pid;

	/* Prototypes */
	void show_help(void);
	void show_version(void);

#endif /* __CASH__ */

