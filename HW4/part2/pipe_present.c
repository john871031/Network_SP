/*
 *  pipe_present.c :  check for |
 */

#include <stdio.h>
#include "shell.h"

/*
 * Return index offset into argv of where "|" is,
 * -1 if in an illegal position (first or last index in the array),
 * or 0 if not present.
 */
int pipe_present(char ** myCurrentArgv) {
	int index = 0;
	int check = 0;
	int count = 0;
	int j = 0;
  	/* Search through myCurrentArgv for a match on "|". */

  	while(myCurrentArgv[j++] != NULL){			//計算參數有多少個
 	
  		count++;
	}

	while(myCurrentArgv[index] != NULL){
		if(*myCurrentArgv[index]=='|'){
			if((index==0) || (index==count-1)){		//判斷 "|" 是否位於第一或最後
				check = 1;
				break;
			}else if(index>=1 && index<count-1){		// "|" 位於中間
				check = 2;
				break;
			}
		}
		index++;
		
	}

  	if (check==1) /* At the beginning or at the end. */{
    	return -1;

  	} else if (check==0)/* Off the end. */ {
    	return 0;

  	} else {
    	/* In the middle. */
    	return index;
  	}
}
