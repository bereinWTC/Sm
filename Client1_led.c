#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <wiringPi.h>
#include <softPwm.h>

#define uchar unsigned char

#define LedPinRed    0
#define LedPinGreen  1
#define LedPinBlue   2

void ledInit(void)
{
	softPwmCreate(LedPinRed,  0, 100);
	softPwmCreate(LedPinGreen,0, 100);
	softPwmCreate(LedPinBlue, 0, 100);
}

void ledColorSet(uchar r_val, uchar g_val, uchar b_val)
{
	softPwmWrite(LedPinRed,   r_val);
	softPwmWrite(LedPinGreen, g_val);
	softPwmWrite(LedPinBlue,  b_val);
}

char* IPadd= "192.168.1.198";
const char* argv;
argv = IPadd;


int main(void *arg)
{

    int i;

    if(wiringPiSetup() == -1)
    { //when initialize wiring failed,print messageto screen
	printf("setup wiringPi failed !");
	return 1; 
    }
    //printf("linker LedPin : GPIO %d(wiringPi pin)\n",LedPin); //when initialize wiring successfully,print message to screen

    ledInit();

    int sockfd = 0, n = 0;
    int listenfd = 0;
    char recvBuff[1024];
    //char sendBuff[1025];
    //char MesSend[1025];
    struct sockaddr_in serv_addr; 
    while(1)
    {
        memset(recvBuff, 0,sizeof(recvBuff));
        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("\n Error : Could not create socket \n");
            return 1;
        } 

        memset(&serv_addr, '0', sizeof(serv_addr)); 

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(5000); 

        if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
        {
            printf("\n inet_pton error occured\n");
            return 1;
        } 

        if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
           printf("\n Error : Connect Failed \n");
           return 1;
        } 
        printf("connected %s \n", argv);

        n=recv(sockfd, recvBuff, sizeof(recvBuff)-1, 0);
        printf("receive message%s\n", recvBuff);
        if(recvBuff==0){
	    ledColorSet(0xff,0x00,0x00);//red
        }
        else if (recvBuff==1){
	    ledColorSet(0x00,0xff,0x00);//Green
        }
        else if (recvBuff==2){
	    ledColorSet(0xb4,0xb4,0);//yellow
        }
	else if (recvBuff==3){
	    ledColorSet(0x28,0x28,0xff);//blue
	}
        else{
	    //do nothing
        }
        delay(500);
        //scanf("%[^\n]", &MesSend);
        //getchar();
        //snprintf(sendBuff, sizeof(sendBuff), "%s", MesSend);
        //printf("sendBuff %s\n", sendBuff);
 
        //send(sockfd, sendBuff, sizeof(sendBuff), 0);
        //memset(MesSend, 0 ,sizeof(MesSend));
        sleep(5);

    }
    return 0;
}
