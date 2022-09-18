#include <sys/param.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main() {
	char *dir;
	//long pathmaxlen = pathconf(".", _PC_PATH_MAX);

	//dir = getcwd((char *)NULL, pathmaxlen + 1);
	dir = getcwd((char *)NULL, 1000);
	
	if(dir == NULL) {
		perror("getcwd");
		exit(1);
	}
	
	printf("%s\n", dir);

	return 0;
}
