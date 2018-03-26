#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


typedef struct LogHead LogHead;//头部	24字节
struct LogHead{	
	int size;		//日志长度	
	char type[20];	//类型	
	char log[1024];	//日志
	};


int main(int argc, char *argv[])
{

	int num = 100;
	if(argc > 1)
	{
		num = atoi(argv[1]);
	}

	printf("send %d  data start!\n",num);

	LogHead logd;
	logd.size = sizeof("{\"log_type\":\"mobile_query\",\"status\":1000,\"date\":\"2015-02-11 13:00:01\",\"uid\":\"123456789\",\"search\":\"youhu\",\"time\":0.20535993576}");

	
        memset(logd.type , 0 , sizeof(logd.type ));
        memset(logd.log , 0 , sizeof(logd.log ));
	memcpy(logd.type,"query_pages",sizeof("query_pages"));
	memcpy(logd.log,"{\"log_type\":\"mobile_query\",\"status\":1000,\"date\":\"2015-02-11 13:00:01\",\"uid\":\"123456789\",\"search\":\"youhu\",\"time\":0.20535993576}",logd.size);
	
	char snd_buff[1024];
	memcpy(snd_buff,&logd,sizeof(logd));
	

	daemon(1,0);
	int sock;
	//sendto中使用的对方地址
	struct sockaddr_in toAddr;
	sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(sock < 0)
	{
 		printf("创建套接字失败了.\r\n");
 		exit(1);
	}


        int nRecvBuf=30*1024*1024;//30M
        setsockopt(sock, SOL_SOCKET, SO_SNDBUF,(const char*)&nRecvBuf,sizeof(int));



	memset(&toAddr,0,sizeof(toAddr));
	toAddr.sin_family=AF_INET;
	toAddr.sin_addr.s_addr=inet_addr("172.16.228.200");
//	toAddr.sin_addr.s_addr=inet_addr("172.16.54.31");
//	toAddr.sin_addr.s_addr=inet_addr("10.73.12.183");
	toAddr.sin_port = htons(16386);


	
	while(num--)
	{
	
		if(sendto(sock,snd_buff,sizeof(snd_buff),0,(struct sockaddr*)&toAddr,sizeof(toAddr)) < 0)
		{
 			printf("sendto() 函数使用失败了.\r\n");
 			close(sock);
 			exit(1);
		}
		
		int mi = 0;
		int i = 0;
		int j = 0;

		for( i ; i < 10000 ; i++ )
		{
			for(j ;j < 10000 ;j++)
			{
				mi = i * j;
			}
		}
		if(num%100 == 0)
		{
			sleep(1);
		}
	}
	close(sock);
	printf("done!!!\n");
}
