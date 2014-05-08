/**
 * CASH - The C Again SHell
 *
 * Copyright (C) 2007 Matheus Borges Teixeira <matheus.bt at gmail.com>
 * Copyright (C) 2007 Renê de Souza Pinto <rene at renesp.com.br>
 * Copyright (C) 2007 Thiago Camargo Fernandes <thiagocf at grad.icmc.usp.br>
 *
 * about_operations.c
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

int given_results_count = 0; /* Counter for result names */
int given_commands_count = 0; /* Counter for command result names */

/**
 * Verifies whether there is an operator without moving the file pointer.
 *
 * @author Matheus Borges Teixeira
 * @param from - opened file being read
 * @return a code for the operator or NOOPERATOR if none
 */
int is_operator(FILE * from){
	char present;
	if(feof(from)){
		return NOOPERATOR;
	}

	present = fgetc(from);

	if(present == '='){
		if(feof(from)){
			fseek(from,-sizeof(char),SEEK_CUR);
			return NOOPERATOR;
		}
		present = fgetc(from);
		fseek(from,-sizeof(char)*2,SEEK_CUR);
		if(present == '='){
			return EQUAL;
		}
		else{
			return ASSIGNMENT;
		}
	}
	if(present == '<'){
		if(feof(from)){
			fseek(from,-sizeof(char),SEEK_CUR);
			return NOOPERATOR;
		}
		present = fgetc(from);
		fseek(from,-sizeof(char)*2,SEEK_CUR);
		if(present == '='){
			return LESSEQUAL;
		}
		else{
			return LESS;
		}
	}
	
	if(present == '>'){
		if(feof(from)){
			fseek(from,-sizeof(char),SEEK_CUR);
			return NOOPERATOR;
		}
		present = fgetc(from);
		fseek(from,-sizeof(char)*2,SEEK_CUR);
		if(present == '='){
			return GREATEREQUAL;
		}
		else{
			return GREATER;
		}
	}
	
	if(present == '+'){
		if(feof(from)){
			fseek(from,-sizeof(char),SEEK_CUR);
			return NOOPERATOR;
		}
		present = fgetc(from);
		fseek(from,-sizeof(char)*2,SEEK_CUR);
		if(present == '+'){
			return PLUSPLUS;
		}
		else if(present == '='){
			return PLUSEQUAL;
		}
		else {
			return PLUS;
		}
	}
	
	if(present == '-'){
		if(feof(from)){
			fseek(from,-sizeof(char),SEEK_CUR);
			return NOOPERATOR;
		}
		present = fgetc(from);
		fseek(from,-sizeof(char)*2,SEEK_CUR);
		if(present == '-'){
			return MINUSMINUS;
		}
		else if(present == '='){
			return MINUSEQUAL;
		}
		else {
			return MINUS;
		}
	}

	if(present == '!'){
		if(feof(from)){
			fseek(from,-sizeof(char),SEEK_CUR);
			return NOOPERATOR;
		}
		present = fgetc(from);
		fseek(from,-sizeof(char)*2,SEEK_CUR);
		if(present == '='){
			return NOTEQUAL;
		}
		else{
			return NOT;
		}
	}

	if(present == '*'){
		if(feof(from)){
			fseek(from,-sizeof(char),SEEK_CUR);
			return NOOPERATOR;
		}
		present = fgetc(from);
		fseek(from,-sizeof(char)*2,SEEK_CUR);
		if(present == '='){
			return TIMESEQUAL;
		}
		else{
			return TIMES;
		}
	}

	if(present == '/'){
		if(feof(from)){
			fseek(from,-sizeof(char),SEEK_CUR);
			return NOOPERATOR;
		}
		present = fgetc(from);
		fseek(from,-sizeof(char)*2,SEEK_CUR);
		if(present == '='){
			return DIVISIONEQUAL;
		}
		else{
			return DIVISION;
		}
	}
	
	if(present == '%'){
		fseek(from,-sizeof(char),SEEK_CUR);
		return RESTDIVISION;
	}

	if(present == '&'){
		fseek(from,-sizeof(char),SEEK_CUR);
		return AND;
	}

	if(present == '|'){
		fseek(from,-sizeof(char),SEEK_CUR);
		return OR;
	}

	if(present == ','){
		fseek(from,-sizeof(char),SEEK_CUR);
		return COMMA;
	}

	fseek(from,-sizeof(char),SEEK_CUR);
	return NOOPERATOR;
}

/**
 * Verifies if there is an operator which makes sense to be executed alone.
 * This does not move the file pointer.
 *
 * @author Matheus Borges Teixeira
 * @param from - opened file being read
 * @return 1 for yes or 0 for no
 */
char is_executable(FILE * from){
	switch(is_operator(from)){
		case ASSIGNMENT:
		case PLUSEQUAL:
		case MINUSEQUAL:
		case TIMESEQUAL:
		case DIVISIONEQUAL:
		case PLUSPLUS:
		case MINUSMINUS:
			return 0;
		default:
			return 1;
	}

}

/**
 * Calls the shell for executing a command and fetches the result.
 *
 * @author Matheus Borges Teixeira
 * @param char - string with the command to be executed
 * @return variable with the content of the result
 */
data * run_command(char * command){
	char * temporary;
	data * result = NULL;
	result = (data*)malloc(sizeof(data));
	result -> name = (char*)malloc((NAMEOFTHINGS+1)*sizeof(char));
	sprintf(result -> name,"_command%d",given_commands_count++);
	result -> isExtern = 0;
	result -> type = TYPESTRING;
	/* Replaces all variable contents */
	temporary = replace_variables(command);

	#ifndef __SKIP__SHELL__
	do_command(temporary, 0);
	#else
	printf("TEST: %s\n",temporary);
	#endif

	free(temporary);

	/* TODO: Actually fetch execution result */
	result -> content = malloc(5*sizeof(char));
	strcpy(result -> content,"null");
	return result;
}

/**
 * Consumes the selected operator and handle errors.
 *
 * @author Matheus Borges Teixeira
 * @param what - string form of the operator to consume
 * @param from - opened file being read
 */
void consume_operator_string(char * what, FILE * from){
	char * error = "Error while consuming operator, expected '%s' and found '%s'.";
	char * errormessage = NULL;
	char * operatorname = (char*)malloc(strlen(what)+1);

	#ifdef __DEBUG__MODE__
	printf("Consuming '%s' in line %d\n",what,line);
	#endif


	fread(operatorname,sizeof(char),strlen(what),from);
	operatorname[strlen(what)] = '\0';

	if(strcmp(what,operatorname) != 0){
		errormessage = (char*)malloc((strlen(error)+strlen(what)+strlen(operatorname)+1)*sizeof(char));
		sprintf(errormessage,error,what,operatorname);
		free(operatorname);
		interpreterError(SYNTAXERROR,errormessage);
	}
	free(operatorname);
}

/**
 * Routes the consumption and expected value of the selected operator.
 *
 * @author Matheus Borges Teixeira
 * @param operatorcode - code for the selected operation
 * @param from - opened file being read
 */
void consume_operator(int operatorcode, FILE * from){
	switch(operatorcode){
		case NOOPERATOR:
			break;
		case ASSIGNMENT:
			consume_operator_string("=",from);
			break;
		case EQUAL:
			consume_operator_string("==",from);
			break;
		case LESS:
			consume_operator_string("<",from);
			break;
		case LESSEQUAL:
			consume_operator_string("<=",from);
			break;
		case GREATER:
			consume_operator_string(">",from);
			break;
		case GREATEREQUAL:
			consume_operator_string(">=",from);
			break;
		case PLUS:
			consume_operator_string("+",from);
			break;
		case PLUSPLUS:
			consume_operator_string("++",from);
			break;
		case PLUSEQUAL:
			consume_operator_string("+=",from);
			break;
		case MINUS:
			consume_operator_string("-",from);
			break;
		case MINUSMINUS:
			consume_operator_string("--",from);
			break;
		case MINUSEQUAL:
			consume_operator_string("-=",from);
			break;
		case TIMES:
			consume_operator_string("*",from);
			break;
		case TIMESEQUAL:
			consume_operator_string("*=",from);
			break;
		case DIVISION:
			consume_operator_string("/",from);
			break;
		case DIVISIONEQUAL:
			consume_operator_string("/=",from);
			break;
		case RESTDIVISION:
			consume_operator_string("%",from);
			break;
		case NOT:
			consume_operator_string("!",from);
			break;
		case NOTEQUAL:
			consume_operator_string("!=",from);
			break;
		case OR:
			consume_operator_string("||",from);
			break;
		case AND:
			consume_operator_string("&&",from);
			break;
		case COMMA:
			consume_operator_string(",",from);
			break;
		default:
			interpreterError(UNKNOWN,"Illegal state: consuming a non-existing operator.");
	} 
	return;
}

/**
 * Purges a variable if it is just created to be temporary in operations.
 *
 * @author Matheus Borges Teixeira
 * @param variable - variable which might be purged
 */
void release_helper(data ** variable){
	if(variable != NULL && (strncmp((*variable) -> name,"_zero",5) == 0 || strncmp((*variable) -> name,"_converted",10) == 0)){
		purge_variable(variable);
	}
}

/**
 * Converts a variable into another temporary variable.
 * Used to make variable types uniform.
 * Preference is: string > float > int
 *
 * @author Matheus Borges Teixeira
 * @param first - one of the variables to be converted
 * @param second - one of the variables to be converted
 */
void normalize(data ** first,data ** second){
	data * deadend;
	/* Exclusive or used as if homogeneous no conversion is required */
	/* Check if string conversion needed, highest priority */
	if(((*first) -> type == TYPESTRING) ^ ((*second) -> type == TYPESTRING)){
		if((*first) -> type == TYPESTRING){
			deadend = *second;
			*second = convert_variable(*second,TYPESTRING);
			if(*second != deadend){
				release_helper(&deadend);
			}
		}
		else{
			deadend = *first;
			*first = convert_variable(*first,TYPESTRING);
			if(*first != deadend){
				release_helper(&deadend);
			}
		}
		return;
	}
	/* Second priority, none are string, so try float */
	if(((*first) -> type == TYPEFLOAT) ^ ((*second) -> type == TYPEFLOAT)){
		if((*first) -> type == TYPEFLOAT){
			deadend = *second;
			*second = convert_variable(*second,TYPEFLOAT);
			if(*second != deadend){
				release_helper(&deadend);
			}
		}
		else{
			deadend = *first;
			*first = convert_variable(*first,TYPEFLOAT);
			if(*first != deadend){
				release_helper(&deadend);
			}
		}
		return;
	}
	/* Last check for normalization is int */
	if(((*first) -> type == TYPEINT) ^ ((*second) -> type == TYPEINT)){
		if((*first) -> type == TYPEINT){
			deadend = *second;
			*second = convert_variable(*second,TYPEINT);
			if(*second != deadend){
				release_helper(&deadend);
			}
		}
		else{
			deadend = *first;
			*first = convert_variable(*first,TYPEINT);
			if(*first != deadend){
				release_helper(&deadend);
			}
		}
		return;
	}
	/* Reaching here, no conversion required */
	return;
}

/**
 * Operates two variables.
 *
 * @author Matheus Borges Teixeira
 * @param operatorcode - operation to be performed
 * @param one - first operand
 * @param two - second operand
 * @return variable with the result of the operation
 */
data * operate(int operatorcode, data * one, data * two){	
	char * auxiliary = NULL;
	data * result = NULL; 
	data * temporary = NULL;
	data * first = one;
	data * second = two;

	if(one == NULL){
		first = create_zero();
	}

	if(two == NULL){
		second = create_zero();
	}

	if(operatorcode == ASSIGNMENT){
		#ifdef __DEBUG__MODE__
		printf("Operating %s = %s\n",first -> name, second -> name);
		#endif
		if(first -> name[0] == '_'){
			interpreterError(IRREGULARCHARACTER,"Wrong receiving operand, either invalid or starts with '_'");
		}
		switch(second -> type){
			case TYPESTRING:
				free(first -> content);
				first -> content = malloc(sizeof(char)*strlen((DECLARESTRING)(second -> content))+1);
				strcpy((DECLARESTRING)first -> content, (DECLARESTRING)second -> content);
				first -> type = TYPESTRING;
				if(first -> isExtern){
					update_extern(first);
				}
				break;
			case TYPEINT:
				free(first -> content);
				first -> content = malloc(sizeof(DECLAREINT));
				*((DECLAREINT*)first -> content) = *((DECLAREINT*)second -> content);
				first -> type = TYPEINT;
				if(first -> isExtern){
					update_extern(first);
				}
				break;							 
			case TYPEFLOAT:
				free(first -> content);
				first -> content = malloc(sizeof(DECLAREFLOAT));
				*((DECLAREFLOAT*)first -> content) = *((DECLAREFLOAT*)second -> content);
				first -> type = TYPEFLOAT;
				if(first -> isExtern){
					update_extern(first);
				}
				break;
			default:
				interpreterError(ILLEGALTYPE,"The type for assignment could not be resolved.");
		}
		result = first;
	}
	else{
		normalize(&first,&second);
		result = (data*)malloc(sizeof(data));
		auxiliary = (char*)malloc(sizeof(char)*NAMEOFTHINGS+1);
		sprintf(auxiliary,"_result%d",given_results_count++);
		result -> name = auxiliary;
		result -> isExtern = 0;

		switch(operatorcode){
			case NOOPERATOR:
				interpreterError(UNKNOWN,"Trying to operate something without a valid operator.");
				break;

			case EQUAL:
				if(one == NULL || two == NULL){
					interpreterError(SYNTAXERROR,"Missing a component for operating '=='.");
				}

				#ifdef __DEBUG__MODE__
				printf("Operating %s == %s\n",first -> name, second -> name);
				#endif

				result -> type = TYPEINT;
				result -> content = malloc(sizeof(DECLAREINT));
				switch(first -> type){
					case TYPESTRING:
						*(DECLAREINT*)(result -> content) = 
							(0 == strcmp((DECLARESTRING)(first -> content),(DECLARESTRING)(second -> content)));
						break;
					case TYPEINT:
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREINT*)(first -> content) == *(DECLAREINT*)(second -> content));
						 break;							 
					case TYPEFLOAT:
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREFLOAT*)(first -> content) == *(DECLAREFLOAT*)(second -> content));
						break;							 
					default:
						interpreterError(ILLEGALTYPE,"The type for comparison could not be resolved.");
				}
				break;

			case NOTEQUAL:
				if(one == NULL || two == NULL){
					interpreterError(SYNTAXERROR,"Missing a component for operating '!='.");
				}

				#ifdef __DEBUG__MODE__
				printf("Operating %s != %s\n",first -> name, second -> name);
				#endif

				result -> type = TYPEINT;
				result -> content = malloc(sizeof(DECLAREINT));
				switch(first -> type){
					case TYPESTRING:
						*(DECLAREINT*)(result -> content) = 
							(0 != strcmp((DECLARESTRING)(first -> content),(DECLARESTRING)(second -> content)));
						break;
					case TYPEINT:
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREINT*)(first -> content) != *(DECLAREINT*)(second -> content));
						 break;							 
					case TYPEFLOAT:
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREFLOAT*)(first -> content) != *(DECLAREFLOAT*)(second -> content));
						break;							 
					default:
						interpreterError(ILLEGALTYPE,"The type for comparison could not be resolved.");
				}
				break;

			case LESS:
				if(one == NULL || two == NULL){
					interpreterError(SYNTAXERROR,"Missing a component for operating '<'.");
				}

				#ifdef __DEBUG__MODE__
				printf("Operating %s < %s\n",first -> name, second -> name);
				#endif

				result -> type = TYPEINT;
				result -> content = malloc(sizeof(DECLAREINT));
				switch(first -> type){
					case TYPESTRING:
						*(DECLAREINT*)(result -> content) = 
							(0 > strcmp((DECLARESTRING)(first -> content),(DECLARESTRING)(second -> content)));
						break;
					case TYPEINT:
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREINT*)(first -> content) < *(DECLAREINT*)(second -> content));
						 break;							 
					case TYPEFLOAT:
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREFLOAT*)(first -> content) < *(DECLAREFLOAT*)(second -> content));
						break;
					default:
						interpreterError(ILLEGALTYPE,"The type for comparison could not be resolved.");
				}
				break;

			case LESSEQUAL:
				if(one == NULL || two == NULL){
					interpreterError(SYNTAXERROR,"Missing a component for operating '<='.");
				}

				#ifdef __DEBUG__MODE__
				printf("Operating %s <= %s\n",first -> name, second -> name);
				#endif

				result -> type = TYPEINT;
				result -> content = malloc(sizeof(DECLAREINT));
				switch(first -> type){
					case TYPESTRING: 
						*(DECLAREINT*)(result -> content) = 
							(0 >= strcmp((DECLARESTRING)(first -> content),(DECLARESTRING)(second -> content)));
						break;
					case TYPEINT:
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREINT*)(first -> content) <= *(DECLAREINT*)(second -> content));
						 break;							 
					case TYPEFLOAT:
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREFLOAT*)(first -> content) <= *(DECLAREFLOAT*)(second -> content));
						break;							 
					default:
						interpreterError(ILLEGALTYPE,"The type for comparison could not be resolved.");
				}
				break;

			case GREATER:
				if(one == NULL || two == NULL){
					interpreterError(SYNTAXERROR,"Missing a component for operating '>'.");
				}

				#ifdef __DEBUG__MODE__
				printf("Operating %s > %s\n",first -> name, second -> name);
				#endif

				result -> type = TYPEINT;
				result -> content = malloc(sizeof(DECLAREINT));
				switch(first -> type){
					case TYPESTRING:
						*(DECLAREINT*)(result -> content) = 
							(0 < strcmp((DECLARESTRING)(first -> content),(DECLARESTRING)(second -> content)));
						break;
					case TYPEINT:
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREINT*)(first -> content) > *(DECLAREINT*)(second -> content));
						 break;							 
					case TYPEFLOAT:
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREFLOAT*)(first -> content) > *(DECLAREFLOAT*)(second -> content));
						break;							 
					default:
						interpreterError(ILLEGALTYPE,"The type for comparison could not be resolved.");
				}
				break;

			case GREATEREQUAL:
				if(one == NULL || two == NULL){
					interpreterError(SYNTAXERROR,"Missing a component for operating '>='.");
				}

				#ifdef __DEBUG__MODE__
				printf("Operating %s >= %s\n",first -> name, second -> name);
				#endif

				result -> type = TYPEINT;
				result -> content = malloc(sizeof(DECLAREINT));
				switch(first -> type){
					case TYPESTRING:
						*(DECLAREINT*)(result -> content) = 
							(0 <= strcmp((DECLARESTRING)(first -> content),(DECLARESTRING)(second -> content)));
						break;
					case TYPEINT:
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREINT*)(first -> content) >= *(DECLAREINT*)(second -> content));
						 break;							 
					case TYPEFLOAT:
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREFLOAT*)(first -> content) >= *(DECLAREFLOAT*)(second -> content));
						break;							 
					default:
						interpreterError(ILLEGALTYPE,"The type for comparison could not be resolved.");
				}
				break;

			case PLUS:
				#ifdef __DEBUG__MODE__
				printf("Operating %s + %s\n",first -> name, second -> name);
				#endif

				switch(first -> type){
					case TYPESTRING:
						result -> type = TYPESTRING;
						result -> content = malloc(
							(strlen((DECLARESTRING)(first -> content))+strlen((DECLARESTRING)(second -> content))+1)*sizeof(char));
						((DECLARESTRING)(result -> content))[0] = '\0';
						strcat((DECLARESTRING)(result -> content),(DECLARESTRING)(first -> content));
						strcat((DECLARESTRING)(result -> content),(DECLARESTRING)(second -> content));
						break;
					case TYPEINT:
						result -> type = TYPEINT;
						result -> content = malloc(sizeof(DECLAREINT));
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREINT*)(first -> content) + *(DECLAREINT*)(second -> content));
						 break;							 
					case TYPEFLOAT:
						result -> type = TYPEFLOAT;
						result -> content = malloc(sizeof(DECLAREFLOAT));
						*(DECLAREFLOAT*)(result ->content) = 
							(*(DECLAREFLOAT*)(first -> content) + *(DECLAREFLOAT*)(second -> content));
						break;							 
					default:
						interpreterError(ILLEGALTYPE,"The type for sum could not be resolved.");
				}
				break;

			case PLUSPLUS:
				if(one == NULL){
					interpreterError(SYNTAXERROR,"Missing a component for operating '++'.");
				}

				#ifdef __DEBUG__MODE__
				printf("Operating %s++\n",first -> name);
				#endif
				
				switch(first -> type){
					case TYPESTRING:
						interpreterError(NOTIMPLEMENTED,"Sorry, auto increment of strings is not available.");
						break;
					case TYPEINT:
						result -> type = first -> type;
						result -> content = first -> content;
						first -> content = malloc(sizeof(DECLAREINT));
						*(DECLAREINT*)(first -> content) = (*(DECLAREINT*)(result -> content) + 1);
						break;							 
					case TYPEFLOAT:
						result -> type = first -> type;
						result -> content = first ->content;
						first -> content = malloc(sizeof(DECLAREFLOAT));
						*(DECLAREFLOAT*)(first ->content) = (*(DECLAREFLOAT*)(result -> content) + 1.0);
						break;							 
					default:
						interpreterError(ILLEGALTYPE,"The type for auto increment could not be resolved.");
				}

				if(first -> isExtern){
					update_extern(first);
				}
				break;

			case PLUSEQUAL:
				#ifdef __DEBUG__MODE__
				printf("Operating %s += %s\n",first -> name, second -> name);
				#endif

				free(result -> name);
				free(result);
				temporary = operate(PLUS,first,second);
				result = operate(ASSIGNMENT,first,temporary);
				purge_variable(&temporary);
				break;

			case MINUS:
				#ifdef __DEBUG__MODE__
				printf("Operating %s - %s\n",first -> name, second -> name);
				#endif

				switch(first -> type){
					case TYPESTRING:
						interpreterError(NOTIMPLEMENTED,"Sorry, subtraction of strings is not available.");
						break;
					case TYPEINT:
						result -> type = TYPEINT;
						result -> content = malloc(sizeof(DECLAREINT));
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREINT*)(first -> content) - *(DECLAREINT*)(second -> content));
						 break;							 
					case TYPEFLOAT:
						result -> type = TYPEFLOAT;
						result -> content = malloc(sizeof(DECLAREFLOAT));
						*(DECLAREFLOAT*)(result ->content) = 
							(*(DECLAREFLOAT*)(first -> content) - *(DECLAREFLOAT*)(second -> content));
						break;							 
					default:
						interpreterError(ILLEGALTYPE,"The type for subtraction could not be resolved.");
				}
				break;

			case MINUSMINUS:
				if(one == NULL){
					interpreterError(SYNTAXERROR,"Missing a component for operating '--'.");
				}

				#ifdef __DEBUG__MODE__
				printf("Operating %s--\n",first -> name);
				#endif

				switch(first -> type){
					case TYPESTRING:
						interpreterError(NOTIMPLEMENTED,"Sorry, auto decrement of strings is not available.");
						break;
					case TYPEINT:
						result -> type = first -> type;
						result -> content = first -> content;
						first -> content = malloc(sizeof(DECLAREINT));
						*(DECLAREINT*)(first -> content) = (*(DECLAREINT*)(result -> content) - 1);
						break;							 
					case TYPEFLOAT:
						result -> type = first -> type;
						result -> content = first ->content;
						first -> content = malloc(sizeof(DECLAREFLOAT));
						*(DECLAREFLOAT*)(first ->content) = (*(DECLAREFLOAT*)(result -> content) - 1.0);
						break;							 
					default:
						interpreterError(ILLEGALTYPE,"The type for auto decrement could not be resolved.");
				}

				if(first -> isExtern){
					update_extern(first);
				}
				break;

			case MINUSEQUAL:
				#ifdef __DEBUG__MODE__
				printf("Operating %s -= %s\n",first -> name, second -> name);
				#endif

				free(result -> name);
				free(result);
				temporary = operate(MINUS,first,second);
				result = operate(ASSIGNMENT,first,temporary);
				purge_variable(&temporary);
				break;

			case TIMES:
				if(one == NULL || two == NULL){
					interpreterError(SYNTAXERROR,"Missing a component for operating '*'.");
				}

				#ifdef __DEBUG__MODE__
				printf("Operating %s * %s\n",first -> name, second -> name);
				#endif

				switch(first -> type){
					case TYPESTRING:
						interpreterError(NOTIMPLEMENTED,"Sorry, multiplication of strings is not available.");
						break;
					case TYPEINT:
						result -> type = TYPEINT;
						result -> content = malloc(sizeof(DECLAREINT));
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREINT*)(first -> content) * *(DECLAREINT*)(second -> content));
						 break;							 
					case TYPEFLOAT:
						result -> type = TYPEFLOAT;
						result -> content = malloc(sizeof(DECLAREFLOAT));
						*(DECLAREFLOAT*)(result ->content) = 
							(*(DECLAREFLOAT*)(first -> content) * *(DECLAREFLOAT*)(second -> content));
						break;							 
					default:
						interpreterError(ILLEGALTYPE,"The type for multiplication could not be resolved.");
				}
				break;

			case TIMESEQUAL:
				#ifdef __DEBUG__MODE__
				printf("Operating %s *= %s\n",first -> name, second -> name);
				#endif

				free(result -> name);
				free(result);
				temporary = operate(TIMES,first,second);
				result = operate(ASSIGNMENT,first,temporary);
				purge_variable(&temporary);
				break;

			case RESTDIVISION:
				if(one == NULL || two == NULL){

					interpreterError(SYNTAXERROR,"Missing a component for operating '%'.");
				}

				#ifdef __DEBUG__MODE__
				printf("Operating %s %% %s\n",first -> name, second -> name);
				#endif

				switch(first -> type){
					case TYPESTRING:
						interpreterError(NOTIMPLEMENTED,"Sorry, rest of division for strings is not available.");
						break;
					case TYPEINT:
						result -> type = TYPEINT;
						result -> content = malloc(sizeof(DECLAREINT));
						if(*(DECLAREINT*)(second -> content) == 0){
							interpreterError(ILLEGALOPERATION,"Cannot divide by zero!");
						}
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREINT*)(first -> content) % *(DECLAREINT*)(second -> content));
						 break;							 
					case TYPEFLOAT:
						interpreterError(ILLEGALTYPE,"Sorry, rest of division for float is not available.");
						break;							 
					default:
						interpreterError(ILLEGALTYPE,"The type for rest of division could not be resolved.");
				}
				break;

			case DIVISION:
				if(one == NULL || two == NULL){
					interpreterError(SYNTAXERROR,"Missing a component for operating '/'.");
				}

				#ifdef __DEBUG__MODE__
				printf("Operating %s * %s\n",first -> name, second -> name);
				#endif

				switch(first -> type){
					case TYPESTRING:
						interpreterError(NOTIMPLEMENTED,"Sorry, division for strings is not available.");
						break;
					case TYPEINT:
						result -> type = TYPEINT;
						if(*(DECLAREINT*)(second -> content) == 0){
							interpreterError(ILLEGALOPERATION,"Cannot divide by zero!");
						}
						result -> content = malloc(sizeof(DECLAREINT));						
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREINT*)(first -> content) / *(DECLAREINT*)(second -> content));
						 break;							 
					case TYPEFLOAT:
						result -> type = TYPEFLOAT;
						if(*(DECLAREFLOAT*)(second -> content) == 0){
							interpreterError(ILLEGALOPERATION,"Cannot divide by zero!");
						}
						result -> content = malloc(sizeof(DECLAREFLOAT));
						*(DECLAREFLOAT*)(result ->content) = 
							(*(DECLAREFLOAT*)(first -> content) / *(DECLAREFLOAT*)(second -> content));
						break;							 
					default:
						interpreterError(ILLEGALTYPE,"The type for division could not be resolved.");
				}
				break;

			case DIVISIONEQUAL:
				#ifdef __DEBUG__MODE__
				printf("Operating %s += %s\n",first -> name, second -> name);
				#endif

				free(result -> name);
				free(result);
				temporary = operate(DIVISION,first,second);
				result = operate(ASSIGNMENT,first,temporary);
				purge_variable(&temporary);
				break;

			case OR:
				if(one == NULL || two == NULL){
					interpreterError(SYNTAXERROR,"Missing a component for operating '||'.");
				}

				#ifdef __DEBUG__MODE__
				printf("Operating %s || %s\n",first -> name, second -> name);
				#endif

				result -> type = TYPEINT;
				result -> content = malloc(sizeof(DECLAREINT));
				switch(first -> type){
					case TYPESTRING:
						*(DECLAREINT*)(result -> content) = 
							(strlen((DECLARESTRING)(first -> content)) || strlen((DECLARESTRING)(second -> content)));
						break;
					case TYPEINT:
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREINT*)(first -> content) || *(DECLAREINT*)(second -> content));
						 break;							 
					case TYPEFLOAT:
						*(DECLAREINT*)(result ->content) = 
							(*(DECLAREFLOAT*)(first -> content) || *(DECLAREFLOAT*)(second -> content));
						break;							 
					default:
						interpreterError(ILLEGALTYPE,"The type for comparison could not be resolved.");
				}
				break;
			case AND:
				if(one == NULL || two == NULL){
					interpreterError(SYNTAXERROR,"Missing a component for operating '&&'.");
				}
				
				#ifdef __DEBUG__MODE__
				printf("Operating %s && %s\n",first -> name, second -> name);
				#endif
				
				result -> type = TYPEINT;
				result -> content = malloc(sizeof(DECLAREINT));
				switch(first -> type){
					case TYPESTRING:
						*(DECLAREINT*)(result -> content) = (strlen((DECLARESTRING)(first -> content)) && strlen((DECLARESTRING)(second -> content)));
						break;
					case TYPEINT:
						*(DECLAREINT*)(result ->content) = (*(DECLAREINT*)(first -> content) && *(DECLAREINT*)(second -> content));
						 break;							 
					case TYPEFLOAT:
						*(DECLAREINT*)(result ->content) = (*(DECLAREFLOAT*)(first -> content) && *(DECLAREFLOAT*)(second -> content));
						break;							 
					default:
						interpreterError(ILLEGALTYPE,"The type for comparison could not be resolved.");
				}
				break;
			case NOT:
				if(two == NULL){
					interpreterError(SYNTAXERROR,"Missing a component for operating '!'.");
				}

				#ifdef __DEBUG__MODE__
				printf("Operating !%s\n",second -> name);
				#endif
				switch(second -> type){
					case TYPESTRING:
						interpreterError(NOTIMPLEMENTED,"Sorry, negation of strings is not available.");
						break;
					case TYPEINT:
						result -> content = malloc(sizeof(DECLAREINT));
						result -> type = TYPEINT;
						*(DECLAREINT*)(result -> content) = !(*(DECLAREINT*)(second -> content));
						 break;							 
					case TYPEFLOAT:
						result -> content = malloc(sizeof(DECLAREFLOAT));
						result -> type = TYPEFLOAT;
						*(DECLAREFLOAT*)(result ->content) = !(*(DECLAREFLOAT*)(second -> content));
						break;							 
					default:
						interpreterError(ILLEGALTYPE,"The type for negation could not be resolved.");
				}
				break;
			default:
				interpreterError(UNKNOWN,"Illegal state: trying to operate an unknown operation.");
		} 
	}
	
	release_helper(&first);
	release_helper(&second);

	if(result != NULL){
		return result;
	}
	else{
		interpreterError(UNKNOWN,"Illegal state: calculation failed!");
	}
}


/**
 * Prepares the operations to be executed.
 * Used to consume the tokens and purge in-between calculations.
 *
 * @author Matheus Borges Teixeira
 * @param from - opened file being read
 * @param operation - operation to be performed
 * @param ongoing - variable with the current result of the expression
 * @param precedence - current precedence level of the expression
 * @param expected - precedence level of the operation called
 * @return variable with the result of the operation
 */
data * pre_operate(FILE * from, int operation, data ** ongoing, int precedence, int expected){
	data * temporary = NULL;
	data * auxiliary = NULL;
	if(precedence > expected || *ongoing == NULL){
		consume_operator(operation,from);
		auxiliary = calculate_expression(from,expected);
		temporary = operate(operation,*ongoing,auxiliary);
		purge_variable(&auxiliary);
		purge_variable(ongoing);
		return temporary;
	}
	else{
		return *ongoing;
	}
	
}

/**
 * Loops over the expression executing all operations.
 *
 * @author Matheus Borges Teixeira
 * @param from - opened file being read
 * @param expected - precedence level of the expression
 * @return variable with the result of the expression
 */
data * calculate_expression(FILE * from,int level){
	char present = 0;
	int code = 0;
	int operation = 0;
	int precedence = level;
	data * ongoing = NULL;

	fpos_t position;
	clean_blank(from);
	if(feof(from)){
		interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while an expression was expected.");
	}
	present = fgetc(from);
	if(present == '('){
		ongoing = calculate_expression(from,NOPRECEDENCE);
		if(feof(from)){
			purge_variable(&ongoing);
			interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while ')' was expected.");
		}
		present = fgetc(from);
		if(present != ')'){
			purge_variable(&ongoing);
			interpreterError(SYNCHRONIZECHARACTER,"Missing ')' in the expression.");
		}
	}
	else{
		fseek(from,-sizeof(char),SEEK_CUR);
		code = get_identifier(from);
		if(code != OPERATORCODE){
			ongoing = consume_identifier(code,from);
		}
	}
	clean_blank(from);
	operation = is_operator(from);
	/* C Specifications, in priority order */
	while(operation != NOOPERATOR){
		switch(operation){
			/* Level 0 - '()' '[]' '->' '.' from left to right */
			/* No implemented operators here */
			/* Level 1 - '!' '~' '++' '--' '-' '(type)' '*' '&' 'sizeof'	 from right to left */
			case NOT:
			case PLUSPLUS:
			case MINUSMINUS:
				ongoing = pre_operate(from,operation,&ongoing,precedence,1);
				precedence = 2;
				break;
			/* Level 2 - '*' '/' '%' from left to right */
			case TIMES:
			case DIVISION:
			case RESTDIVISION:
				ongoing = pre_operate(from,operation,&ongoing,precedence,2);
				precedence = 3;
				break;
			/* Level 3 - '+' '-' from left to right */
			case PLUS:
			case MINUS:
				ongoing = pre_operate(from,operation,&ongoing,precedence,3);
				precedence = 4;
				break;
			/* Level 4 - '<<' '>>' from left to right */
			/* No implemented operators here */
			/* Level 5 - '<' '<=' '>' '>=' from left to right */
			case LESS:
			case LESSEQUAL:
			case GREATER:
			case GREATEREQUAL:
				ongoing = pre_operate(from,operation,&ongoing,precedence,5);
				precedence = 5;
				break;
			/* Level 6 - '==' '!=' from left to right */
			case EQUAL:
			case NOTEQUAL:
				ongoing = pre_operate(from,operation,&ongoing,precedence,6);
				precedence = 7;
				break;
			/* Level 7 - '&' from left to right */
			/* No implemented operators here */
			/* Level 8 - '^' from left to right */
			/* No implemented operators here */
			/* Level 9 - '|' from left to right */
			/* No implemented operators here */
			/* Level 10 - '&&' from left to right */
			case AND:
				ongoing = pre_operate(from,operation,&ongoing,precedence,10);
				precedence = 11;
				break;
			/* Level 11 - '||' from left to right */
			case OR:
				ongoing = pre_operate(from,operation,&ongoing,precedence,11);
				precedence = 12;
				break;
			/* Level 12 - '?:' from right to left */
			/* No implemented operators here */
			/* Level 13 - '=' '+=' '-=' '*=' (...etc...) from right to left */
			case ASSIGNMENT:
			case PLUSEQUAL:
			case MINUSEQUAL:
			case TIMESEQUAL:
			case DIVISIONEQUAL:
				ongoing = pre_operate(from,operation,&ongoing,precedence,13);
				precedence = 14;
				break;
			/* Level 14 - ',' from left to right */
			case COMMA:
				consume_operator(operation,from);
				calculate_expression(from,NOPRECEDENCE);
				return ongoing;
			default:
				interpreterError(NOTIMPLEMENTED,"Sorry, requested operation not supported.");
			
		}
		clean_blank(from);
		operation = is_operator(from);
		if(precedence > level){
			break;
		}
	}
	return ongoing;
}

