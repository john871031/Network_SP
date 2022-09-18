/*
 * convert.c : take a file in the form 
 *  word1
 *  multiline definition of word1
 *  stretching over several lines, 
 * followed by a blank line
 * word2....etc
 * convert into a file of fixed-length records
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "dict.h"
#define BIGLINE 512

int main(int argc, char **argv) {
	FILE *in;
	FILE *out;        /* defaults */
	char line[BIGLINE];
	static Dictrec dr, blank;
	char buf[BIGLINE];
	/* If args are supplied, argv[1] is for input, argv[2] for output */
	if (argc==3) {
		if ((in =fopen(argv[1],"r")) == NULL){DIE(argv[1]);}
		if ((out =fopen(argv[2],"w")) == NULL){DIE(argv[2]);}	
	}
	else{
		printf("Usage: convert [input file] [output file].\n");
		return -1;
	}

	/* Main reading loop : read word first, then definition into dr */

	/* Loop through the whole file. */
	while (fgets(line, BIGLINE, in)) {
		
		/* Create and fill in a new blank record.
		 * First get a word and put it in the word field, then get the definition
		 * and put it in the text field at the right offset.  Pad the unused chars
		 * in both fields with nulls.
		 */
		memset(buf,'\0',BIGLINE);
		/* Read word and put in record.  Truncate at the end of the "word" field.
		 *
		 * Fill in code. */
		
		//printf("%s",line);
                
		/* Read definition, line by line, and put in record.
		 *
		 * Fill in code. */
		
		strcpy(dr.word,line);		//因為word都在第一行,所以先存入
		while(1){
			fgets(line,BIGLINE,in);	
			if(strcmp(line, "\n") == 0){
				break;
			}else{
				strcat(buf,line);	//判斷是否遇到換行,否就暫存入buf中
			}
		}
                strcpy(dr.text,buf);
		/* Write record out to file.
		 *
		 * Fill in code. */
		fwrite(&dr,sizeof(Dictrec),1,out);
	}

	fclose(in);
	fclose(out);
	return 0;
}
