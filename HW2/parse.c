/*
 * parse.c : use whitespace to tokenise a line
 * Initialise a vector big enough
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

/* Parse a commandline string into an argv array. */
char ** parse(char *line) {

  	static char delim[] = " \t\n"; /* SPACE or TAB or NL */
  	int count = 0;
  	char * token;
  	char **newArgv;

  	/* Nothing entered. */
  	if (line == NULL) {
    		return NULL;
  	}

  	/* Init strtok with commandline, then get first token.
     * Return NULL if no tokens in line.
	 *
	 * Fill in code.
     */
	token = strtok(line, delim);
	if(token == NULL){
		return NULL;
	}
  	/* Create array with room for first token.
  	 *
	 * Fill in code.
	 */
	newArgv=(char**)malloc(sizeof(char*));
	newArgv[count] = token;
	count++;

  	/* While there are more tokens...
	 *
	 *  - Get next token.
	 *	- Resize array.
	 *  - Give token its own memory, then install it.
	 * 
  	 * Fill in code.
	 */
	

  	/* Null terminate the array and return it.
	 *
  	 * Fill in code.
	 */

	while(token != NULL){
		token = strtok(NULL, delim);
		newArgv[count] = token;
		count++;
	}
	
	for(int k = 0; k<count-1; k++){
		printf("[%d]:%s\n", k, newArgv[k]);
	}
	
  	return newArgv;
}


/*
 * Free memory associated with argv array passed in.
 * Argv array is assumed created with parse() above.
 */
void free_argv(char **oldArgv) {

	/* Free each string hanging off the array.
	 * Free the oldArgv array itself.
	 *
	 * Fill in code.
	 */
	 free(oldArgv);
}
