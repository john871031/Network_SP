/*
 * builtin.c : check for shell built-in commands
 * structure of file is
 * 1. definition of builtin functions
 * 2. lookup-table
 * 3. definition of is_builtin and do_builtin
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "shell.h"




/****************************************************************************/
/* builtin function definitions                                             */
/****************************************************************************/

/* "echo" command.  Does not print final <CR> if "-n" encountered. */
static void bi_echo(char **argv) {
  	/* Fill in code. */
  	int i;
  	if(argv[1] == NULL)		//檢查echo後有無值
  		return;
  		
  	if(!strcmp(argv[1],"-n")){	//if echo後為 -n 則輸出下一個位置所指定的位置的值
  		i = atoi(argv[2]);
  		printf("%s", argv[i+2]);
  	}else{				//非 -n 則輸出echo 後所有的值
  		int j = 1;
  		while(argv[j] != NULL){
  			printf("%s ",argv[j++]);
  		}
  	}
  	printf("\n");
}


/* Fill in code. */
static void ex_proc(char **argv){	//exit process,if exit quit logout bye
	exit(0);
}



/****************************************************************************/
/* lookup table                                                             */
/****************************************************************************/

static struct cmd {
	char * keyword;				/* When this field is argv[0] ... */
	void (* do_it)(char **);	/* ... this function is executed. */
} inbuilts[] = {

	/* Fill in code. */
	{ "quit", ex_proc },
	{ "exit", ex_proc },
	{ "logout", ex_proc },
	{ "bye", ex_proc },
	{ "echo", bi_echo },		/* When "echo" is typed, bi_echo() executes.  */
	{ NULL, NULL }				/* NULL terminated. */
};




/****************************************************************************/
/* is_builtin and do_builtin                                                */
/****************************************************************************/

static struct cmd * this; 		/* close coupling between is_builtin & do_builtin */

/* Check to see if command is in the inbuilts table above.
Hold handle to it if it is. */
int is_builtin(char *cmd) {
  	struct cmd *tableCommand;

  	for (tableCommand = inbuilts ; tableCommand->keyword != NULL; tableCommand++)
    	if (strcmp(tableCommand->keyword,cmd) == 0) {
			this = tableCommand;
			return 1;
		}
  	return 0;
}


/* Execute the function corresponding to the builtin cmd found by is_builtin. */
int do_builtin(char **argv) {
  	this->do_it(argv);
}
