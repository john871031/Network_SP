#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

struct tree{
	char name[256];
	pid_t pid, ppid;
	struct tree *parent, *children[256], *next;
}*init;

int newnode(char *procname, pid_t pid, pid_t ppid){		//建立link list
	struct tree *node;
	node = (struct tree*)malloc(sizeof(struct tree));		//給每個node空間
	if (node == NULL){
		printf("node NULL\n");
		return 1;
	}
	strcpy(node->name, procname);
	node->pid = pid;
	node->ppid = ppid;
	node->children[0] = NULL;
	node->parent = NULL;
	node->next = init; 
	init = node;
	return 0;
}


int lib(char *dirname){
	char filename[256], buf[256], procname[256], pid[128],ppid[128];
	char *key,*val;
	FILE *file;
	
	strcpy(filename, dirname);
	strcat(filename, "/status");
	file = fopen(filename, "r");
	if(file == NULL){
		return 1;
	}
	while(fgets(buf, sizeof(buf), file) != NULL){
		key = strtok(buf, " :\t");
		val = strtok(NULL, " \n:\t");
		if(key != NULL && val != NULL){
			if(strcmp(key, "Name") == 0){
				strcpy(procname, val);
			}else if (strcmp(key, "Pid") == 0){
				strcpy(pid, val);
			}else if (strcmp(key, "PPid") == 0){
				strcpy(ppid, val);
			}
		}
	}
	return newnode(procname, atoi(pid), atoi(ppid));		//將切割好的建立link list
}


int build_tree(){
	int i;
	struct tree *node, *pnode;

	for(node = init; node != NULL; node = node->next) {
		i = 0;
		struct tree *tmp_node;
		for(tmp_node=init; tmp_node!=NULL; tmp_node = tmp_node->next){		//找parent 
			if(tmp_node->pid == node->ppid){		//若node的ppid等於link list中某個tmp_node的pid，則代表tmp_node是node的parent 
				pnode = tmp_node;
				break;			//若找到parent,跳出迴圈並回傳
			}else{			
				pnode = NULL;		//沒parent, 令為NULL
			}
		}
		
		if(pnode != NULL){
			node->parent = pnode;		//parent回傳值
			while (pnode->children[i++] != NULL);	//尋找可以填入child的位置
			pnode->children[i - 1] = node;
		}
	}
	return 0;
}

int output(struct tree * node, int level){		//確認自己在第幾層,並輸出
	int i;
	struct tree *tmp_node;
	
	for(i = 0; i < level; i++){			//根據層數輸出,為了表現出父子關係
		printf("------");
	}
	printf("%s pid: %d, ppid: %d\n", node->name, node->pid, node->ppid);
	while((tmp_node = node->children[i++]) != NULL){	//若還有child,則繼續往下尋找
		output(tmp_node, level + 1);		//通過遞迴了解自己的層數
	}
	return 0;
}

int main(int argc, char **argv){

	DIR *dir;
	struct dirent *direntry;
	char dirname[256];
	struct tree *node;
	if((dir = opendir("/proc")) == NULL){
		perror("open: ");
		return 1;
	}
	while((direntry = readdir(dir)) != NULL){		//讀取dir開啟的目錄，並賦值給dir_entry
		if(direntry->d_type == DT_DIR){		//讀取 開啟目錄的檔案型別 如果讀取的d_type型別為DT_DIR (=4 表示讀取的為目錄)
			strcpy(dirname, "/proc/");
			strcat(dirname, direntry->d_name);
			lib(dirname);
		}
		
	}
	build_tree();
	for(node = init; node != NULL; node = node->next){
		if(node->parent == NULL){		//先找出最頂端的parent,依序向下尋找
			output(node, 0);		//0代表level 0, 最頂層
		}
	}

	return 0;
}
