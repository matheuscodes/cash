/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * history.h
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

#ifndef __CASH_HISTORY__

	#define __CASH_HISTORY__

	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>

	#include "linkedl.h"
	#include "linkedcl.h"

	#define TABLELENGTH 	16001

	/* Prototypes */
	int load_history_file(char *filename, linked_list ***hashtable, linked_clist **clist);
	int write_history_file(char *filename, linked_clist *clist);
	int string_position(char *string);
	int add_string(linked_list ***hashtable, char *string);
	int remove_string(linked_list ***hashtable, char *string);
	int create_hash_table(linked_list ***hashtable);
	int destroy_hash_table(linked_list*** hashtable);

#endif /* __CASH_HISTORY__ */

