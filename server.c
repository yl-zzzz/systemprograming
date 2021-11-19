#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORTNUM 9000
#define MAX_CNT 256

void *start_client(void*arg);
void send_msg(char *name,int len);
struct Info{
	char name[50];
	
};

pthread_mutex_t mtx;
int cnt = 0;
int client_socks[MAX_CNT];

int main(void){
	char buf[256];
	struct sockaddr_in sin,cli;
	int sd,ns,clientlen = sizeof(cli);
	pthread_t pth;

	if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket");
		exit(1);
	}
	
	memset((char *)&sin,'\0',sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	pthread_mutex_init(&mtx,NULL);

	if(bind(sd,(struct sockaddr *)&sin,sizeof(sin))){
		perror("bind");
		exit(1);
	}

	if(listen(sd,5)){
		perror("listen");
		exit(1);
	}

	while(1){
		if((ns = accept(sd, (struct sockaddr *)&cli, &clientlen)) == -1){
			perror("accept");
			exit(1);
		}
		pthread_mutex_lock(&mtx);
		client_socks[cnt++] = ns;
		pthread_mutex_unlock(&mtx);
		
		pthread_create(&pth,NULL,start_client,(void *)&ns);

		pthread_detach(pth);
		printf("connect\n");

		close(sd);
	}
	return 0;
}
void *start_client(void *arg){
	int ns = *((int *)arg);
	int str_len = 0;
	char name[256];

	while(str_len = recv(ns,name,sizeof(name),0) != -1)
		send_msg(name,str_len);
	pthread_mutex_lock(&mtx);
	for(int i = 0; i < cnt; i++){
		if(ns == client_socks[i]){
			while(++i < cnt){
				client_socks[i-1] = client_socks[i];
			}
			break;
		}
	}
	cnt--;
	pthread_mutex_unlock(&mtx);
	close(ns);
	return NULL;
}
	
void send_msg(char *name,int len){
	pthread_mutex_lock(&mtx);
	for(int i = 0; i <cnt; i++){
		write(client_socks[i],name,len);
	}
	pthread_mutex_unlock(&mtx);
}
