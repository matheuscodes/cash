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

int line = 1; /* Line counter */

/**
 * Opens the script file and executes it.
 *
 * @author Matheus Borges Teixeira
 * @param filename - name of the file to be opened
 */
void interpret(char * filename){
	char * error = NULL;
	FILE * source = fopen(filename,"r");
	#ifdef __DEBUG__MODE__
	printf("Opening script file\n");
	#endif
	if(source == NULL){
		error = (char*)malloc((strlen("Cannot open file ''.")+strlen(filename)+1)*sizeof(char));
		sprintf(error,"Cannot open file '%s'.",filename);
		interpreterError(CANTOPENFILE,error);
	}
	rewind(source);
	#ifdef __DEBUG__MODE__
	printf("Starting the execution\n");
	#endif
	while(!feof(source)){
		clean_blank(source);
		get_block(source);
	}
	flush_variables();
	fclose(source);
	#ifdef __DEBUG__MODE__
	printf("Ended the execution\n");
	#endif
	return;
}
