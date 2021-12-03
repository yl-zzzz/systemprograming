#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORTNUM 9004
#define MAX_CNT 256


int main(void){
	pid_t pid;
	struct sockaddr_in sin,cli;
	int sd,ns,clientlen = sizeof(cli);
	int i_menu_num,n;
	int idx;
	int flag=1;
	char c_menu_num[3];
	int fd;
	char server[40];
	char check[3];
	int i_count;
	char c_count[5];
	char buf[50];
	caddr_t addr;
	struct stat statbuf;
	char product_name[20];
	int check_client;

	if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket");
		exit(1);
	}
	
	memset((char *)&sin,'\0',sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	

	if(bind(sd,(struct sockaddr *)&sin,sizeof(sin))){
		perror("bind");
		exit(1);
	}

	if(listen(sd,5)){
		perror("listen");
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
	close(fd);
	while(flag){
		if((ns = accept(sd, (struct sockaddr *)&cli, &clientlen)) == -1){
			perror("accept");
			exit(1);
		}
		memset(server,0,sizeof(server));
		recv(ns,server,sizeof(server),0);

		if(strcmp(server,"client") == 0){
				printf("client\n");
				check_client=1;
		}else if(strcmp(server,"online") == 0){
				check_client=0;
				printf("online\n");
		}
		printf("connect\n");
		pid=fork();
		if(!pid){	
			while(flag){
				close(sd);
				n = recv(ns,c_menu_num,sizeof(c_menu_num),0);
				i_menu_num = atoi(c_menu_num);
				switch(i_menu_num){
					case 1:
						while(1){
							memset(product_name,'\0',sizeof(product_name));
							if(recv(ns,product_name,sizeof(product_name),0) == -1){
									perror("recv1");
									exit(1);
							}
							printf("product_name = %s\n",product_name);
							if(strcmp(product_name,"exit") == 0){
								printf("exit\n");
								break;
							}
							memset(c_count,0,sizeof(c_count));
							if(recv(ns,c_count,sizeof(c_count),0) == -1){
								perror("recv");
								exit(1);
							}
							printf("count = %s\n",c_count);
							i_count = atoi(c_count);
							idx=0;
							
							char ar[20];
							char p_name[strlen(product_name)];
							
							
							for(int i=0; i < strlen(addr);i++){
								memset(ar,0,sizeof(ar));	
								strncpy(ar,addr+i,strlen(product_name));	
								idx = i;
								if(strcmp(ar,product_name) == 0){
										break;
								}
							}
							int cc=0;
							idx = idx+strlen(product_name);
							int j = idx+1;
							char c_null[3];
							memset(c_null,0,sizeof(c_null));
							sprintf(c_null,"%c",addr[j]);		
							while(strcmp(c_null,"\n")){
								j++;
								sprintf(c_null,"%c",addr[j]);		
								cc++;
							}
							int ivt_i_count=0;
							char ivt_c_count[cc];
							strncpy(ivt_c_count,addr+idx+1,cc);
							ivt_i_count = atoi(ivt_c_count);
							
							ivt_i_count -= i_count;
							if(ivt_i_count < 0 ){
									printf("no stock\n");
									strcpy(buf," out of stock\n");
									strcpy(check,"0");
									send(ns,check,strlen(check)+1,0);
									send(ns,buf,strlen(buf)+1,0);
									continue;
							}else{
									strcpy(check,"1");
									send(ns,check,strlen(check)+1,0);
							}
							sprintf(ivt_c_count,"%d",ivt_i_count);	
							if(cc>strlen(ivt_c_count)){
								strncpy(addr+idx+1,ivt_c_count,strlen(ivt_c_count));
								strncpy(addr+idx+1+strlen(ivt_c_count)," ",cc-strlen(ivt_c_count));
							}
							strncpy(addr+idx+1,ivt_c_count,strlen(ivt_c_count));
							msync(addr,statbuf.st_size,MS_SYNC);
							printf("calc inventory\n");
						}
						break;
					case 2:
						while(check_client){
							memset(product_name,'\0',sizeof(product_name));
							if(recv(ns,product_name,sizeof(product_name),0) == -1){
									perror("recv1");
									exit(1);
							}
							printf("product_name = %s\n",product_name);
							if(strcmp(product_name,"exit") == 0){
									break;
							}

							memset(c_count,0,sizeof(c_count));
							if(recv(ns,c_count,sizeof(c_count),0) == -1){
								perror("recv");
								exit(1);
							}
							i_count = atoi(c_count);
							idx=0;
							
							char ar[20];
							char p_name[strlen(product_name)];
							
							for(int i=0; i < strlen(addr);i++){
								memset(ar,0,sizeof(ar));	
								strncpy(ar,addr+i,strlen(product_name));	
								idx = i;
								if(strcmp(ar,product_name) == 0){
										break;
								}
									
							}
							int cc=0;
							idx = idx+strlen(product_name);
							int j = idx+1;
							char c_null[3];
							memset(c_null,0,sizeof(c_null));
							sprintf(c_null,"%c",addr[j]);		
							while(strcmp(c_null,"\n")){
								j++;
								sprintf(c_null,"%c",addr[j]);		
								cc++;
							}
							int ivt_i_count=0;
							char ivt_c_count[cc];
							strncpy(ivt_c_count,addr+idx+1,cc);
							ivt_i_count = atoi(ivt_c_count);
							ivt_i_count += i_count;
							sprintf(ivt_c_count,"%d",ivt_i_count);	
							if(cc<strlen(ivt_c_count)){
								strncpy(addr+idx+1,ivt_c_count,strlen(ivt_c_count));
								strncpy(addr+idx+1+strlen(ivt_c_count),"\n",strlen(ivt_c_count)-cc);
							}
							strncpy(addr+idx+1,ivt_c_count,strlen(ivt_c_count));
							msync(addr,statbuf.st_size,MS_SYNC);
							printf("Add inventory\n");
						}
						break;
					case 3:
						printf("--EXIT--\n");
						flag = 0;
						pid = 1;
						break;
					case 4:
						printf("looking list\n");
						break;
					default:
						break;	
				}
			}
			close(ns);
			break;
		}else{
			close(ns);
		}
	}
	printf("EXIT\n");
	close(sd);
	return 0;
}
