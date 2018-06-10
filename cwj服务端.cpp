#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#pragma comment(lib, "WS2_32.lib")          //add ws2_32.lib

#define MAXLINE 4096
#define BUFFER_SIZE 1024

int main()
{

    WORD    wVersionRequested;
    WSADATA wsaData;
    int     err,iLen;
    SOCKET sockSrv;
    wVersionRequested   =   MAKEWORD(2,2);

    char filename[100];
    char filepath[100];

    char *buffer;//file buffer
    int fileTrans;
    int lenfilepath;
    FILE *fp;
    int writelength;

    buffer = (char *)malloc(sizeof(char)*BUFFER_SIZE);
    //bzero(buffer,BUFFER_SIZE);
    memset(buffer,0,sizeof(buffer));

    if(WSAStartup(wVersionRequested,&wsaData)!=0)
    {
        printf("WSAStartup error!\n");
        return -1;
    }

    if(( sockSrv =socket(AF_INET,SOCK_STREAM,0))<0){
        printf("socket error!\n");
        return -2;
    }
    SOCKADDR_IN addrSrv;
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_addr.s_addr = htonl(INADDR_ANY);
    addrSrv.sin_port = htons(9000);

    if(bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR))!=0)
    {

        printf("bind error!\n");
        return -3;
    }

    if(listen(sockSrv,5)!=0)
    {

        printf("listen error!\n");
        return -4;
    }
    printf("Sever build finished ...\n");

    while(1)
    {
        SOCKET temp;
        if((temp = accept(sockSrv,(SOCKADDR*)NULL,NULL))>0)
        {
            //printf("client IP:   %d\n",ntohl(client.sin_addr.s_addr));
            //printf("client PORT: %d\n",ntohs(client.sin_port));

            memset(filename,'\0',sizeof(filename));
            memset(filepath,'\0',sizeof(filepath));

            lenfilepath = recv(temp,filepath,100,0);
            printf("filepath :%s\n",filepath);
            if(lenfilepath<0)
            {
                printf("recv error!\n");
            }
            else
            {
                int i=0,k=0;
                for(i=strlen(filepath);i>=0;i--)
                {
                    if(filepath[i]!='/')
                    {
                        k++;
                    }
                    else
                        break;
                }
                strcpy(filename,filepath+(strlen(filepath)-k)+1);
            }
            printf("filename :%s\n",filename);
            fp = fopen(filename,"w");
            if(fp!=NULL)
            {
                while(fileTrans =recv(temp,buffer,BUFFER_SIZE,0))
                {
                    if(fileTrans<0)
                    {
                        printf("recv error!\n");
                        break;
                    }
                    writelength = fwrite(buffer,sizeof(char),fileTrans,fp);
                    if(writelength <fileTrans)
                    {
                        printf("write error!\n");
                        break;
                    }
                    //bzero(buffer,BUFFER_SIZE);
                    memset(buffer,'\0',sizeof(buffer));
                }
                printf("recv finished!\n");
                fclose(fp);
            }
            else
            {
                printf("filename is null!\n");

            }
            closesocket(temp);


        }
        else
        {
            printf("accept error!\n");
            closesocket(temp);
        }
    }
    closesocket(sockSrv);
    WSACleanup();
    return 0;

}