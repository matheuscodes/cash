/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * cash.c
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

#include "cash.h"

/* Global variables */
const char *VERSION = "0.001";
const char *RELEASE = "0.001.0";

char *name_of_cash;


/**
 * The main function
 * 
 * @author Renê de Souza Pinto
 *
 * @param argc Number of passed parameters
 * @param argv Passed parameters
 *
 * @return int Return value
 */
int main(int argc, char **argv) {

	int i, p;
	char exit_main    = 0;
	char invalid_char = '\0';
	char *filename    = NULL;
	char *temp;
	char c;

	/* Handling arguments */
	if(argc > 1) {
		for(i=1; i<argc; i++) {
			char *arg_str = argv[i];

			/* Program option */
			if(arg_str[0] == '-') {
				for(p=1; p<strlen(arg_str); p++) {

					switch( tolower(arg_str[p]) ) {

							case 'h': /* Show the Help */
									  user_options |= ARG_HELP;
									  break;

							case 'v': /* Show the version */
									  user_options |= ARG_VERSION;
									  break;

							default:  /* Invalid option */
									  user_options |= ARG_INVALID;
									  invalid_char  = arg_str[p];
									  break;
					}

				}
			} else {
				/* File name */
				if(filename == NULL)
						filename = argv[i];
			}
		}
	}

	/* Check selected options */
	if( (user_options & ARG_INVALID) ) {
		show_help();
		fprintf(stdout, "  Invalid option: %c\n\n", invalid_char);
		exit(EXIT_FAILURE);
	}

	if( (user_options & ARG_HELP) ) {
		show_help();
		exit_main = 1;
	}

	if( (user_options & ARG_VERSION) ) {
		show_version();
		exit_main = 1;
	}

	if(exit_main)
			exit(EXIT_SUCCESS);


	/* Get the cash executable file name */
	temp = argv[0];
	i    = strlen(temp);
	while(i > 0) {
			i--;
			c = temp[i];
			if(c == '/')
					break;
	}
	if(i > 0) {
			name_of_cash = (char*)malloc(sizeof(char) * (strlen(temp) - i));
			if(name_of_cash != NULL)
					strcpy(name_of_cash, &temp[i+1]);
	}

	/* Check file to process */
	if(filename != NULL) {
		/* Init the Sell */
		#ifndef __SKIP__SHELL__
		if( !init_shell() ) {
			fprintf(stderr, "\nThe Shell could not be initialized. Aborted.\n");
			finalize_shell();
			return;
		}
		#endif
		interpret(filename);
		/* Finalize the shell */
		#ifndef __SKIP__SHELL__
		finalize_shell();
		#endif
		exit( EXIT_SUCCESS );
		
	} else {
		/* Entering in the Shell */
		exit( goto_shell() );
	}


	/* Exit */
	return(EXIT_SUCCESS);
}


/**
 * Show the help on default output
 * 
 * @author Renê de Souza Pinto
 */
void show_help(void) {

	fprintf(stdout, "\n  CASH - The C Again SHell\n");
 	fprintf(stdout, "\n  Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)");
 	fprintf(stdout, "\n  Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)");
	fprintf(stdout, "\n\n  Use:  cash [option] [file]");
	fprintf(stdout, "\n\n  Options avaliable:");
	fprintf(stdout, "\n          -h: Show the help");
	fprintf(stdout, "\n          -v: Show the version");
	fprintf(stdout, "\n\n  file parameter is the file to be processed by CASH.");
	fprintf(stdout, "\n\n");

	return;
}


/**
 * Show the version on default output
 * 
 * @author Renê de Souza Pinto
 */
void show_version(void) {

	fprintf(stdout, "\n  Version: %s\n", VERSION);
 	fprintf(stdout, "\n  CASH is free software; you can redistribute it and/or modify it");
	fprintf(stdout, "\n  under the terms of the GNU General Public License as published by");
 	fprintf(stdout, "\n  the Free Software Foundation; version 2.\n");
 	fprintf(stdout, "\n  CASH is distributed in the hope that it will be useful, but WITHOUT");
 	fprintf(stdout, "\n  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY");
 	fprintf(stdout, "\n  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public");
 	fprintf(stdout, "\n  License for more details.\n");
 	fprintf(stdout, "\n  You should have received a copy of the GNU General Public License");
 	fprintf(stdout, "\n  along with Bash; see the file COPYING.  If not, write to the Free");
 	fprintf(stdout, "\n  Software Foundation, 59 Temple Place, Suite 330, Boston, MA 02111 USA.\n\n");

	return;
}

