#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

uid_t userIdFromName(const char *name){
	
	struct passwd *pwd;
	uid_t u;
	char *endptr;
	if(name == NULL || *name == '\0'){
		return -1;
	}	
	u = strtol(name, &endptr, 10);	//字符串轉為長整數,10進制
	if(*endptr == '\0')
		return u;	
	
	pwd = getpwnam(name);		//搜索參數指定的帳號名稱以passwd結構傳回
	if(pwd == NULL){
		return -1;
	}
	return pwd->pw_uid;
}

int check(char *dirname, uid_t uid){		//比較當作命令行參數的用戶,尋找其中的uid,並比較其與自身的使用者id是否相同
	
	char filename[256], buf[256], proc_name[256], pid[256], uid_str[256];
	char *key, *val;
	FILE *file;
	int same = 0;
	sprintf(uid_str, "%d", uid);
	strcpy(filename, dirname);
	strcat(filename, "/status");
	file = fopen(filename, "r");
	if(file==NULL){
		return 1;
	}
	
	while(fgets(buf, sizeof(buf), file)!=NULL){
		key = strtok(buf, " :\t");
		val = strtok(NULL, " \n:\t");
		if(key != NULL && val != NULL){
			if((strcmp(key, "Uid")==0) && (strstr(val, uid_str)!=NULL)){
				same = 1;
			}
			if(strcmp(key, "Name")==0){
				strcpy(proc_name, val);
			}else if(strcmp(key, "Pid")==0){
				strcpy(pid, val);
			}
		}
	}
	if(same){
		printf("%s: %s\n", pid, proc_name); 
	}
	return 0;
}

int main(int argc, char **argv){
	uid_t uid;
	DIR *dir;
	struct dirent *dir_entry;
	char dirname[256];
	
	uid = userIdFromName(argv[1]);
	if((dir = opendir("/proc")) == NULL){
		perror("open: ");
		return 1;
	}
	
	while((dir_entry = readdir(dir))!=NULL){		//讀取dir開啟的目錄，並賦值給dir_entry
		if(dir_entry->d_type == DT_DIR) {			//讀取 開啟目錄的檔案型別 如果讀取的d_type型別為DT_DIR (=4 表示讀取的為目錄)
			strcpy(dirname,"/proc/");
			strcat(dirname,dir_entry->d_name);
			check(dirname,uid);
		}
	}
	
	return 0;
}
