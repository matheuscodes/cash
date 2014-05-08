/**
 * CASH - The C Again SHell
 *
 * Copyright (C) 2007 Matheus Borges Teixeira <matheus.bt at gmail.com>
 * Copyright (C) 2007 Renê de Souza Pinto <rene at renesp.com.br>
 * Copyright (C) 2007 Thiago Camargo Fernandes <thiagocf at grad.icmc.usp.br>
 *
 * about_execution.c
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


extern int line; /* Line counter */

/** 
 * Author's side note: 
 * Track - means one simple line of code.
 * Block - means one a group of lines of code delimited by {}
 */

/**
 * Processes a block of executable code.
 *
 * @author Matheus Borges Teixeira
 * @param from - opened file being read
 */
void get_block(FILE * from){
	char present = 0;
	if(feof(from)){
		interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while expecting a block to start.");
	}
	present = fgetc(from);
	if(present == '{'){
		/* This is a Block */
		clean_blank(from);
		if(feof(from)){
			interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while reading one block.");
		}
		present = fgetc(from);
		/* Recursively calling for another block */
		while(present != '}'){
			fseek(from,-sizeof(char),SEEK_CUR);
			clean_blank(from);
			get_block(from);
			present = fgetc(from);
		}
		clean_blank(from);
		return;
	}
	else{
		/* Not a block, but a Track */
		fseek(from,-sizeof(char),SEEK_CUR);
		get_track(from);
		clean_blank(from);
		return;
	}
}

/**
 * Processes a single line of executable code.
 *
 * @author Matheus Borges Teixeira
 * @param from - opened file being read
 */
void get_track(FILE * from){
	int code = 0;
	char present = 0;
	int counter = 0;
	data * temporary = NULL;

	fpos_t initial_position;
	clean_blank(from);
	
	code = get_identifier(from);
	switch(code){
		case VARIABLECODE: 
			fgetpos(from,&initial_position);
			present = fgetc(from);
			while(allowed_identifier_char(present)){
				if(feof(from)){
					interpreterError(UNEXPECTEDFILEEND,"File ended abruptly while confirming identifier.");
				}
				if(is_operator(from) != NOOPERATOR){
					/* Identifier ended and an operator appeared */
					break;
				}
				present = fgetc(from);
			}

			if(is_executable(from) != 1){
				/* The track is an expression */
				#ifdef __DEBUG__MODE__
				printf("Considered to be an expression\n");
				#endif
				
				fsetpos(from,&initial_position);
				temporary = calculate_expression(from,NOPRECEDENCE);
			}
			else{
				/* The track is a command */
				#ifdef __DEBUG__MODE__
				printf("Considered to be a command\n");
				#endif
				fsetpos(from,&initial_position);
				
				temporary = consume_identifier(COMMANDCODE,from);
			}
			present = fgetc(from);
			if(present != ';'){
				interpreterError(SYNTAXERROR,"All commands or expressions must end with ';'.");
			}
			break;

		case IFCODE:
			get_if(from);
			break;

		case FORCODE:
			get_for(from);
			break;

		case DOCODE:
			get_do(from);
			break;

		case WHILECODE:
			get_while(from);
			break;

		default:
			purge_variable(&temporary);
			interpreterError(UNKNOWN,"Illegal state: Unknown identifier in execution.");
	}
	purge_variable(&temporary);
}

/**
 * Processes an 'if' logic node.
 *
 * @author Matheus Borges Teixeira
 * @param from - opened file being read
 */
void get_if(FILE * from){
	char present = 0;
	data * result = NULL;

	consume_identifier(IFCODE,from);
	clean_blank(from);
	if(feof(from)){
		interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while opening 'if' expression.");
	}
	present = fgetc(from);
	if(present != '('){
		interpreterError(SYNTAXERROR,"The 'if' syntax is incorrect, missing '('.");
	}
	else{
		clean_blank(from);
		result = calculate_expression(from,NOPRECEDENCE);
		if(result == NULL){
			interpreterError(UNKNOWN,"Illegal State: The 'if' expression returned no value.");
		}
		if(feof(from)){
			interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while closing 'if' expression.");
		}
		present = fgetc(from);
		if(present != ')'){
			interpreterError(SYNTAXERROR,"The 'if' syntax is incorrect, missing ')'.");
		}

		#ifdef __DEBUG__MODE__
		printf("Executing 'if' on line %d\n",line);				
		#endif

		switch(result -> type){
			case TYPESTRING:
				if(strcmp((DECLARESTRING)(result -> content),"") != 0){
					clean_blank(from);
					get_block(from);
					if(get_identifier(from) == ELSECODE){
						consume_identifier(ELSECODE,from);
						skip_block(from);
					}
				}
				else{
					skip_block(from);
					clean_blank(from);
					if(get_identifier(from) == ELSECODE){
						consume_identifier(ELSECODE,from);
						clean_blank(from);
						get_block(from);
					}
				}							
				break;

			case TYPEINT:
				if(*((DECLAREINT*)(result -> content)) != 0){
					clean_blank(from);
					get_block(from);
					if(get_identifier(from) == ELSECODE){
						consume_identifier(ELSECODE,from);
						skip_block(from);
					}
				}
				else{
					skip_block(from);
					clean_blank(from);
					if(get_identifier(from) == ELSECODE){
						consume_identifier(ELSECODE,from);
						clean_blank(from);
						get_block(from);
					}
				}							
				break;

			case TYPEFLOAT:
				if(*((DECLAREFLOAT*)(result -> content)) != 0.0){
					clean_blank(from);
					get_block(from);
					if(get_identifier(from) == ELSECODE){
						consume_identifier(ELSECODE,from);
						skip_block(from);
					}
				}
				else{
					skip_block(from);
					clean_blank(from);
					if(get_identifier(from) == ELSECODE){
						consume_identifier(ELSECODE,from);
						clean_blank(from);
						get_block(from);
					}
				}							
				break;
			default:
				purge_variable(&result);
				interpreterError(UNKNOWN,"Illegal State: The 'if' expression result has no type.");
			
		}
	}

	#ifdef __DEBUG__MODE__
	printf("'if' ended in line %d\n",line);
	#endif
	purge_variable(&result);
	return;	
}

/**
 * Processes a 'for' loop node.
 *
 * @author Matheus Borges Teixeira
 * @param from - opened file being read
 */
void get_for(FILE * from){
	fpos_t condition;
	fpos_t ending;
	fpos_t execution;
	fpos_t finally;
	char present = 0;
	data * expressionresult = NULL;
	data * temporary = NULL;
	int keepgoing = 1;
	int start_line = 0;
	int end_line = 0;
	
	consume_identifier(FORCODE,from);

	clean_blank(from);
	if(feof(from)){
		interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while opening 'for' expression.");
	}
	present = fgetc(from);
	if(present != '('){
		interpreterError(SYNTAXERROR,"The 'for' syntax is incorrect, missing '(' before expression.");
	}
	else{
		clean_blank(from);
		purge_variable(&expressionresult);
		expressionresult = calculate_expression(from,NOPRECEDENCE);
		clean_blank(from);

		if(feof(from)){
			interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file after executing 'for' initial expression.");
		}
		present = fgetc(from);
		if(present != ';'){
			interpreterError(SYNTAXERROR,"The 'for' syntax is incorrect, missing ';' after 'for' initial expression.");
		}

		clean_blank(from);
		fgetpos(from,&condition);
		purge_variable(&expressionresult);
		expressionresult = calculate_expression(from,NOPRECEDENCE);
		clean_blank(from);

		if(feof(from)){
			interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file after executing 'for' condition expression.");
		}
		present = fgetc(from);
		if(present != ';'){
			interpreterError(SYNTAXERROR,"The 'for' syntax is incorrect, missing ';' after 'for' condition expression.");
		}
		clean_blank(from);
		if(feof(from)){
			interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while skipping last 'for' expression.");
		}
		fgetpos(from,&ending);
		
		skip_expression(from);

		if(feof(from)){
			interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file after skipping last 'for' expression.");
		}
		present = fgetc(from);
		if(present != ')'){
			interpreterError(SYNTAXERROR,"The 'for' syntax is incorrect, missing ')' after last 'for' expression.");
		}

		clean_blank(from);
		fgetpos(from,&execution);
		start_line = line;

		#ifdef __DEBUG__MODE__
		printf("Executing 'for' in line %d\n",line);
		#endif
		while(keepgoing){
			switch(expressionresult -> type){
				case TYPESTRING:
					if(strcmp((DECLARESTRING)(expressionresult -> content),"") != 0){
						fsetpos(from, &execution);
						line = start_line;
						get_block(from);
						fgetpos(from,&finally);
						end_line = line;
						fsetpos(from, &ending);
						purge_variable(&temporary);
						temporary = calculate_expression(from,NOPRECEDENCE);
						purge_variable(&temporary);
						fsetpos(from, &condition);
						purge_variable(&expressionresult);
						expressionresult = calculate_expression(from,NOPRECEDENCE);
					}
					else{
						 keepgoing = 0;
					}		
					break;
					
				case TYPEINT:
					if(*((DECLAREINT*)(expressionresult -> content)) != 0){
						fsetpos(from, &execution);
						line = start_line;
						get_block(from);
						fgetpos(from,&finally);
						end_line = line;
						fsetpos(from, &ending);
						purge_variable(&temporary);
						temporary = calculate_expression(from,NOPRECEDENCE);
						purge_variable(&temporary);
						fsetpos(from, &condition);
						purge_variable(&expressionresult);
						expressionresult = calculate_expression(from,NOPRECEDENCE);
					}
					else{
						 keepgoing = 0;
					}							
					break;

				case TYPEFLOAT:
					if(*((DECLAREFLOAT*)(expressionresult -> content)) != 0){
						fsetpos(from, &execution);
						line = start_line;
						get_block(from);
						fgetpos(from,&finally);
						end_line = line;
						fsetpos(from, &ending);
						purge_variable(&temporary);
						temporary = calculate_expression(from,NOPRECEDENCE);
						purge_variable(&temporary);
						fsetpos(from, &condition);
						purge_variable(&expressionresult);
						expressionresult = calculate_expression(from,NOPRECEDENCE);
					}
					else{
						 keepgoing = 0;
					}							
					break;
			}
		}
	}

	#ifdef __DEBUG__MODE__
	printf("'for' ended in line %d\n",line);
	#endif

	fsetpos(from,&finally);
	line = end_line;
	purge_variable(&expressionresult);
	purge_variable(&temporary);
	return;
}

/**
 * Processes a 'while' loop node.
 *
 * @author Matheus Borges Teixeira
 * @param from - opened file being read
 */
void get_while(FILE * from){
	fpos_t initial;
	char present = 0;
	int keepgoing = 1;
	int start_line = 0;
	data * expressionresult = NULL;
	
	fgetpos(from,&initial);
	start_line = line;
	#ifdef __DEBUG__MODE__
	printf("Executing 'while' on line %d\n",line);
	#endif
	while(keepgoing){
		fsetpos(from,&initial);
		line = start_line;
		consume_identifier(WHILECODE,from);
		clean_blank(from);
		if(feof(from)){
			interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while opening 'while' expression.");
		}
		present = fgetc(from);
		if(present != '('){
			interpreterError(SYNTAXERROR,"The 'while' syntax is incorrect, missing '(' before condition.");
		}
		purge_variable(&expressionresult);
		expressionresult = calculate_expression(from,NOPRECEDENCE);
		clean_blank(from);
		if(feof(from)){
			interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while finalizing 'while' expression.");
		}
		present = fgetc(from);
		if(present != ')'){
			interpreterError(SYNTAXERROR,"The 'while' syntax is incorrect, missing ')' after condition.");
		}
		switch(expressionresult -> type){
			case TYPESTRING:
				if(strcmp((DECLARESTRING)(expressionresult -> content),"") == 0){
					skip_block(from);
					keepgoing = 0;
				}
				else{
					clean_blank(from); 
					get_block(from);
				}
				break;
				
			case TYPEINT:
				if(*((DECLAREINT*)(expressionresult -> content)) == 0){
					skip_block(from);
					keepgoing = 0;
				}
				else{
					clean_blank(from);
					get_block(from);
				}
				break;

			case TYPEFLOAT:
				if(*((DECLAREFLOAT*)(expressionresult -> content)) == 0){
					skip_block(from);
					keepgoing = 0;
				}
				else{
					clean_blank(from);
					get_block(from);
				}
				break;
		}
	}
	#ifdef __DEBUG__MODE__
	printf("'while' ended in line %d\n",line);
	#endif
	purge_variable(&expressionresult);
	return;
}

/**
 * Processes a 'do-while' loop node.
 *
 * @author Matheus Borges Teixeira
 * @param from - opened file being read
 */
void get_do(FILE * from){
	char present;
	fpos_t blockaddress;
	fpos_t condition;
	int keepgoing = 1;
	int end_line = 0;
	int start_line = 0;
	data * expressionresult = NULL;
	
	consume_identifier(DOCODE,from);

	clean_blank(from);
	fgetpos(from,&blockaddress);
	start_line = line;
	skip_block(from);

	if(get_identifier(from) != WHILECODE){
		interpreterError(SYNTAXERROR,"The 'do-while' syntax is incorrect, expected 'while' at the end of execution.");
	}

	consume_identifier(WHILECODE,from);

	clean_blank(from);
	if(feof(from)){
		interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while opening 'do-while' expression.");
	}
	present = fgetc(from);
	if(present != '('){
		interpreterError(SYNTAXERROR,"The 'do-while' syntax is incorrect, missing '(' before 'do-while' condition.");
	}
	
	clean_blank(from);
	fgetpos(from,&condition);
	
	#ifdef __DEBUG__MODE__
	printf("Executing 'do-while' on line %d\n",line);
	#endif

	do{
		fsetpos(from,&blockaddress);
		line = start_line;
		clean_blank(from);
		get_block(from);
		fsetpos(from,&condition);
		purge_variable(&expressionresult);
		expressionresult = calculate_expression(from,NOPRECEDENCE);
		switch(expressionresult -> type){
			case TYPESTRING:
				if(strcmp((DECLARESTRING)(expressionresult -> content),"") == 0){
					 keepgoing = 0;
				}							
				break;
				
			case TYPEINT:
				if(*((DECLAREINT*)(expressionresult -> content)) == 0){
					 keepgoing = 0;
				}							
				break;

			case TYPEFLOAT:
				if(*((DECLAREFLOAT*)(expressionresult -> content)) == 0){
					 keepgoing = 0;
				}							
				break;
		}
	}while(keepgoing);

	if(feof(from)){
		interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file after 'do-while' condition.");
	}
	present = fgetc(from);
	if(present != ')'){
		interpreterError(SYNTAXERROR,"The 'do-while' syntax is incorrect, missing ')' after 'do-while' condition.");
	}
	clean_blank(from);
	if(feof(from)){
		interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while finalizing 'do-while'.");
	}
	present = fgetc(from);
	if(present != ';'){
		interpreterError(SYNTAXERROR,"The 'do-while' syntax is incorrect, missing ';' in the end.");
	}
	
	#ifdef __DEBUG__MODE__
	printf("'do-while' ended in line %d\n",line);
	#endif
	purge_variable(&expressionresult);
	return;
}


