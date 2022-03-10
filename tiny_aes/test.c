#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <wiringPi.h> 
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ECB 1

#include "aes.h"
#define TRACE_NUM 100000
#define TRIGGER_PIN 7
#define AES_BLOCK_SIZE 16


void record_plaintext(FILE *fp, unsigned char *plaintext){
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
	    fprintf(fp, "%02x", plaintext[i]);
    }
}	

void record_ciphertext(FILE *fp, unsigned char *ciphertext){
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
	    fprintf(fp, "%02x", ciphertext[i]);
    }
}


int main(void)
{
    uint8_t key[] = {0xe6, 0xfd, 0x68, 0x3f, 0x9e, 0x08, 0x62, 0xc8, 0xa5, 0x6e, 0x2b, 0x4e, 0x8f, 0x22, 0x4c, 0xbb};
    char *c_ipAddr = "10.168.1.182";
    int port = 8888;
    int sockfd, rec_len;
    struct sockaddr_in servaddr;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){  
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);  
        exit(0);  
    }  
    memset(&servaddr, 0, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, c_ipAddr, &servaddr.sin_addr);
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){  
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);  
        exit(0);  
    } 

    FILE *fp;
    fp = fopen("data/text", "w+");

    wiringPiSetup();
    pinMode(TRIGGER_PIN, OUTPUT);
    srand(time(NULL));
    digitalWrite(TRIGGER_PIN,LOW);

    for(int i = 0; i < TRACE_NUM; i++){
        printf("****%d trace****\n", i);
        uint8_t plaintext[AES_BLOCK_SIZE];
        uint8_t ciphertext[AES_BLOCK_SIZE];
        for(int i = 0; i < AES_BLOCK_SIZE; ++i){
            plaintext[i] = rand() % 256;
        }
        
        AES128_ECB_encrypt(plaintext, key, ciphertext);
        // sleep(1);
        digitalWrite(TRIGGER_PIN,LOW);
        //START
        char sendtext[] = "START";
        if(send(sockfd, sendtext, strlen(sendtext), 0) < 0){  
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
            exit(0);  
        } 
        char recvtext[10];
        if((rec_len = recv(sockfd, recvtext, 10, 0)) == -1){
            perror("recv error\n");  
            exit(1);
        }
        recvtext[rec_len] = '\0';
        if(!strncmp(recvtext, "DONE", 4)){
            printf("recv succcess\n");
        }
        else{
            printf("fail to recevive\n");
        }

        
        //printf("key:");
        //for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        //    printf("%02x", key[i]);
        //}
        //printf("\nplain:");
        //for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        //    printf("%02x", plaintext[i]);
        //}
        //printf("\ncipher:");
        //for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        //    printf("%02x", ciphertext[i]);
        //}
        //printf("\n\n");
        
        record_plaintext(fp, plaintext);
        fprintf(fp, " ");
        record_ciphertext(fp, ciphertext);
        fprintf(fp, "\n");
        
        // sleep(1);
    }
    fclose(fp);
    close(sockfd);
    return 0;
}
