#include <regex.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <unistd.h>

int main(void)
{
    //准备表达式
	char *regstr="<a href=\"\\([^\"]\\+\\?\\)\">\\([^<]\\+\\?\\)</a>";
	//准备正则
	regex_t reg;
	regcomp(&reg,regstr,0);

	//关于数据源
	int fd;
	fd=open("URL.txt",O_RDWR);
	int size;
	size=lseek(fd,0,SEEK_END);

	char * mmap_data=NULL;
	mmap_data=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);
	close(fd);

	printf("%s\n",mmap_data);


	//遍历查找
	int regnum=3;
	regmatch_t match[regnum];
	char link[1024];
	char title[1024];
	while(regexec(&reg,mmap_data,regnum,match,0)==0){
	//提取数据
	bzero(link,sizeof(link));
	bzero(title,sizeof(title));
	snprintf(link,match[1].rm_eo-match[1].rm_so+1,"%s",mmap_data+match[1].rm_so);
	snprintf(title,match[2].rm_eo-match[2].rm_so+1,"%s",mmap_data+match[2].rm_so);
	mmap_data+=match[0].rm_eo;
	printf("匹配结果，title=%s  link=%s\n",title,link);
	}
	regfree(&reg);
	return 0;

}
