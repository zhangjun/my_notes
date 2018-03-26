/*******************************************
   Author        : Jun Zhang
   Email         : ewalker.zj@gmail.com
   Last Modified : 2017-07-28 17:14
   Filename      : tail.c
   Description   : 
*******************************************/
#include<stdio.h>
#include<stdlib.h>

#define MAX_BUF_SIZE 10240

int main(int argc, char ** argv){
	char *filename = argv[1];
	FILE *fp = fopen(filename, "r");

	char buf[MAX_BUF_SIZE];
	while(1){
		fseek(fp, 0, SEEK_CUR);
		
		while(fgets(buf, MAX_BUF_SIZE, fp) != NULL){
			printf("%s", buf);
		}

		sleep(1);
		printf("continue\n");

	}

	return 0;
}




