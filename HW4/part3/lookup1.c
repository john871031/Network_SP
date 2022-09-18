/*
 * lookup1 : straight linear search through a local file
 * 	         of fixed length records. The file name is passed
 *	         as resource.
 */
#include <string.h>
#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	Dictrec dr;
	static FILE * in;
	static int first_time = 1;
	char buf[512];
	
	if (first_time) { 
		first_time = 0;
		/* open up the file
		 *
		 * Fill in code. */
		 in = fopen(resource, "r");
	}

	/* read from top of file, looking for match
	 *
	 * Fill in code. */
	rewind(in);			//return start
	while(!feof(in)) {
		/* Fill in code. */
		
		fread(&dr, 512, 1, in);
		strcpy(buf, sought->word);		//要搜尋的word
		strcat(buf, "\n");
		
		if(strcmp(dr.word, buf)==0){		//比對是否在字典中
			strcpy(sought->text, dr.text);
			return FOUND;
		}
	}

	return NOTFOUND;
}
