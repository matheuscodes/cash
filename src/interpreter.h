/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * interpreter.h
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

#ifndef __CASH_INTERPRETER__

	#define __CASH_INTERPRETER__

	#include <stdio.h>
	#include <string.h>
	#include <unistd.h>
	#include <stdlib.h>

	#define STD_INPUT 			 0
	#define STD_OUTPUT 			 1
	#define TOKENLIST_LENGTH     30
	#define TRUE 				 1
	#define FALSE 				 0

	#define NO_REALLOCATION_ERR  2
	#define NO_ALLOCATION_ERR 	 3


	/* Prototypes */
	size_t gettokens(char *string, char ***tokenlist);
	char *clean_string(char *string);
	int destroy_tokenlist(char ***tokenlist, int length);

#endif

