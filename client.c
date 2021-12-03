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
	char menu[] = "**************\n* 1. Calc \n* 2. Add inventory \n* 3. Exit \n* 4. View inventory List   \n***************\n";
	char product_name[30];
	struct sockaddr_in sin;
	int flag = 1;
	char client[40];
	int sd,i_count;
	char c_count[5];
	char check[3];
	int fd;
	caddr_t addr;
	struct stat statbuf;
	pthread_t recv_t;
	char c_num[5];
	
	strcpy(client,"client");
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
	send(sd,client,strlen(client)+1,0);
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
			
					memset(c_count,0,sizeof(c_count));
					sprintf(c_count,"%d",i_count);
					printf("count = %s\n",c_count);
					if(send(sd,c_count,strlen(c_count)+1,0)==-1){
						perror("send");
						exit(1);
					}
					memset(check,0,sizeof(check));
					recv(sd,check,sizeof(check),0);
					if(strcmp(check,"0") == 0){
							recv(sd,buf,sizeof(buf),0);
							printf("%s\n",buf);
					}
				}
				break;
			case 2:
				while(1){
					memset(product_name,'\0',sizeof(product_name));	
					printf("Keep going or Enter exit\n");
					printf("product name : \n");
					scanf("%s",product_name);

					send(sd,product_name,strlen(product_name)+1,0);
					if(strcmp(product_name,"exit") == 0){
						break;	
					}
					printf("count : \n");
					scanf("%d",&i_count);
					printf("p = %s\n",product_name);
			
					memset(c_count,0,sizeof(c_count));
					sprintf(c_count,"%d",i_count);
					printf("count = %s\n",c_count);
					if(send(sd,c_count,strlen(c_count)+1,0)==-1){
						perror("send");
						exit(1);
					}
				}
				break;
			case 3:
				printf("--EXIT--\n");
				flag = 0;
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

