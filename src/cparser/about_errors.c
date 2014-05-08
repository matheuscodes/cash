/**
 * CASH - The C Again SHell
 *
 * Copyright (C) 2007 Matheus Borges Teixeira <matheus.bt at gmail.com>
 * Copyright (C) 2007 Renê de Souza Pinto <rene at renesp.com.br>
 * Copyright (C) 2007 Thiago Camargo Fernandes <thiagocf at grad.icmc.usp.br>
 *
 * about_errors.c
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

#include "cparser.h"

/* Line counter */
extern int line;
/* Function to close the shell gracefully */
extern void finalize_shell();

/**
 * Finalizes the shell indicating an error.
 *
 * @author Matheus Borges Teixeira
 * @param which - code that indicates the class of the error
 * @param explanation - user friendly message for the error
 */
void interpreterError(int which,char * explanation){
	printf("cash: Error [%d] on line [%d]! %s\n",which,line,explanation);
	flush_variables();

	#ifndef __SKIP__SHELL__
	finalize_shell();
	#endif

	exit(1);
}
