#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORTNUM 9000
#define NAME_SIZE 30
#define BUF_SIZE 100
void *add_msg(void *arg);
void *cal_msg(void *arg);

char name[NAME_SIZE] = "[default]";
char msg[BUF_SIZE];


int main(void){
	char buf[256];
	char product_name[NAME_SIZE];
	struct sockaddr_in sin;
	int flag = 1;
	int sd,num,price;
	pthread_t recv_t;

	if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket");
		exit(1);
	}

	memset((char *)&sin,'\0',sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(connect(sd,(struct sockaddr *)&sin,sizeof(sin))){
		perror("connect");
		exit(1);
	}
	printf("****Welcome off-line_Client!!****\n");
	printf("1. Add inventory\n2. calculate\n3. QUIT");
	printf("***Choose :");
	scanf("%d",num);
	while(flag){
		switch(num){
			case 1:
				printf("product name :  , price :\n");
				scanf("%s %s",product_name,price);
				break;
			case 2:
				while(1){
					printf("product name : ");
					scanf("%s",produect_name);

				}
				break;

			case 3:
				flag = 0;
				break;
			default:
				printf("Error : Choose 1 or 2\n");
				break;
		}
	}

	pthread_create(&add_t,NULL,send_msg,(void *)&sd);
	pthread_create(&cal_t,NULL,recv_msg,(void *)&sd);
	pthread_join(add_t,NULL);
	pthread_join(cal_t,NULL);
	close(sd);
	return 0;
}

void *add_msg(void *arg){
	int sock = *((int *)arg);
	char add_msg[NAME_SIZE+BUF_SIZE];
	while(1){
		fgets(msg,BUF_SIZE,stdin);
		if(!strcmp(msg,"QUIT\n")){
			close(sock);
			exit(0);
		}
		sprintf(name_msg,"%s %s",name,msg);
		write(sock,name_msg,strlen(name_msg));
	}
	return NULL;
}

void *cal_msg(void *arg){
	int sock = *((int *)arg);
	char name_msg[NAME_SIZE+BUF_SIZE];
	int str_len;
	while(1){
		str_len = read(sock,name_msg,sizeof(name_msg)-1);
		if(str_len<0){
			return NULL;
		}
		name_msg[str_len] = 0;
		fputs(name_msg,stdout);
	}
	return NULL;
}

