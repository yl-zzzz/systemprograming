#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORTNUM 9004



int main(int argc,char *argv[]){
	char buf[50];
	char menu[] = "**************\n* 1. Buy\n* 2. order product \n* 3. Exit \n* 4. View ware List \n***************\n";
	char product_name[30];
	struct sockaddr_in sin;
	int flag = 1;
	int sd,i_count;
	char c_count[5];
	char check[3];
	int fd;
	caddr_t addr;
	struct stat statbuf;
	pthread_t recv_t;
	char c_num[5];
	char receipt[512];
	char online_client[40];
	
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
	
	if(stat("inventory.txt",&statbuf) == -1){
		perror("stat");
		exit(1);
	}

	if((fd = open("inventory.txt",O_RDWR)) == -1){
		perror("open");
		exit(1);
	}

	addr = mmap(NULL,statbuf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(off_t)0);
	if(addr == MAP_FAILED){
		perror("mmap");
		exit(1);
	}
	char list[strlen(addr)];
	close(fd);
	strcpy(online_client,"online");
	memset(receipt,0,sizeof(receipt));
	send(sd,online_client,strlen(online_client)+1,0);
	printf("****Welcome off-line_Client!!****\n");
	while(flag){
		int i_num;
		memset(c_num,0,sizeof(c_num));
		printf("%s\n",menu);
		printf("Choose\n");
		scanf("%d",&i_num);
		sprintf(c_num,"%d",i_num);
		if(send(sd,c_num,strlen(c_num)+1,0) == -1){
				perror("first send");
				exit(1);
		}
		switch(i_num){
			case 1:
				while(1){
					memset(product_name,'\0',sizeof(product_name));	
					printf("Keep going or Enter exit\n");
					printf("product name : \n");
					scanf("%s",product_name);

					if(send(sd,product_name,strlen(product_name)+1,0)==-1){
							perror("send");
							exit(1);
					}
					if(strcmp(product_name,"exit") == 0){
						break;
					}
					printf("count : \n");
					scanf("%d",&i_count);
					printf("p = %s\n",product_name);
					strcat(receipt,product_name);
					memset(c_count,0,sizeof(c_count));
					sprintf(c_count,"%d",i_count);
					printf("receipt = %s\n",receipt);
					printf("count = %s\n",c_count);
					if(send(sd,c_count,strlen(c_count)+1,0)==-1){
						perror("send");
						exit(1);
					}
					strcat(receipt,"=");
					strcat(receipt,c_count);
					printf("receipt = %s\n",receipt);
					strcat(receipt,"\n");
				}
				break;
			case 2:
				printf("<order> \n%s\n",receipt);
				break;
			case 3:
				printf("<Exit> \n");	
				flag=0;
				break;
			case 4:
				printf("<list>\n%s\n",addr);
				break;
			default:
				printf("Error : Choose 1 or 2 or 3 or 4\n");
				break;
		}
	}
	close(sd);
	return 0;
}
