#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

int output(char *dirname){
	
	char filename[256], buf[256], procname[256], pid[256];
	char *key, *val;
	FILE *file;
	
	strcpy(filename, dirname);
	strcat(filename, "/status");
	file = fopen(filename, "r");
	if(file == NULL){
		return 1;
	}
	
	while(fgets(buf, sizeof(buf), file)!=NULL){
		key = strtok(buf, " :");
		val = strtok(NULL, "\n \t");
		if(key != NULL && val != NULL){
			if(strcmp(key, "Name")==0){
				strcpy(procname, val);
			}else if(strcmp(key, "Pid")==0){
				strcpy(pid, val);
			}
		}
		
	}
	printf("%s: %s\n", procname, pid);
	return 0;
}

int check(char *dirname, char *correspond){
	
	char path[256];
	char buf[512];
	DIR *dir;
	struct dirent *dir_entry;
	int len;
	
	strcpy(path, dirname);
	strcat(path, "/fd");
	if((dir = opendir(path)) == NULL){
		return 1;
	}
	
	while((dir_entry = readdir(dir))!=NULL){		//讀取dir開啟的目錄，並賦值給dir_entry
		if(dir_entry->d_type == DT_LNK){		//符號鏈接
			strcpy(path, dirname);
			strcat(path, "/fd/");
			strcat(path, dir_entry->d_name);
			len = readlink(path, buf, sizeof(buf));	//將path的符號鏈接內容存到buf的空間，返回字符數
			buf[len] = '\0';		//以NULL結尾
			if(strcmp(buf, correspond)==0){	//比對鏈結所指向的文件與查詢的是否相同
				return 0;
			} 
		}
	}
	return 1;
}

int main(int argc, char **argv){
	
	DIR *dir;
	struct dirent *dir_entry;
	char dirname[256];
	
	if((dir = opendir("/proc")) == NULL){
		perror("open: ");
		return 1;
	}
	
	while((dir_entry = readdir(dir))!=NULL){		//讀取dir開啟的目錄，並賦值給dir_entry
		if(dir_entry->d_type == DT_DIR){			//讀取 開啟目錄的檔案型別 如果讀取的d_type型別為DT_DIR (=4 表示讀取的為目錄)
			strcpy(dirname, "/proc/");
			strcat(dirname, dir_entry->d_name);
			if(!(check(dirname, argv[1]))){	//只輸出與fd相關的鏈結
				output(dirname);
			}
		}
	}
	return 0;	
} 
