
#include <wiringPi.h>
#include <stdio.h>
#include <sys/time.h>
#include <pcf8591.h>
#include <math.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <pthread.h>

#define HELLO_WORLD_SERVER_PORT    6666 
#define BUFFER_SIZE 1024
#define		PCF     120
#define		DOpin	0
#define Trig    3
#define Echo    4
#define TouchPin	0
#define Gpin		1
#define Rpin		2

#define NUM_THREADS 2
#define TouchPin2	5
pthread_t tid[NUM_THREADS];

//Touch module.........................................................................................
int tmp = 0;
int flagButton = 0;




void Print(int x){
	if (x != tmp){
		if (x == 0)
			printf("...ON\n");
		if (x == 1)
			printf("OFF..\n");
		tmp = x;
	}
}

//Photo module.........................................................................................

int PhotoModule(void)
{
	int analogVal;
	
	// Setup pcf8591 on base pin 120, and address 0x48
	

	
	analogVal = analogRead(PCF + 0);
	printf("Value: %d\n", analogVal);

	//delay (200);
	
	return analogVal;
}

//Ultrasonic module....................................................................................

void ultraInit(void)
{
	pinMode(Echo, INPUT);
	pinMode(Trig, OUTPUT);
}

float disMeasure(void)
{
	struct timeval tv1;
	struct timeval tv2;
	long time1, time2;
    float dis;

	digitalWrite(Trig, LOW);
	delayMicroseconds(2);

	digitalWrite(Trig, HIGH);
	delayMicroseconds(10);     
	digitalWrite(Trig, LOW);
								
	while(!(digitalRead(Echo) == 1));
	gettimeofday(&tv1, NULL);         

	while(!(digitalRead(Echo) == 0));
	gettimeofday(&tv2, NULL);        

	time1 = tv1.tv_sec * 1000000 + tv1.tv_usec;   
	time2  = tv2.tv_sec * 1000000 + tv2.tv_usec;

	dis = (float)(time2 - time1) / 1000000 * 34000 / 2; 

	return dis;
}

//ColorFlag.................................................................................................

int ColorFlag(void)
{
	int flag0 = 0;
	int flag1 = 0;
	int flag2 = 0;
	int flag3 = 0;
	int flag4 = 0;
	int flagGreen = 1;
	int PhotoResistVal = 0;
	float dist;
	int Dint;

	

	pinMode(TouchPin, INPUT);

		
	Print(digitalRead(TouchPin));
	flag1 = tmp;
	if(flag1==1)
	{
		printf("someone on the seat\n");
	}
	else
	{
		printf("no one on the seat\n");
	}
	PhotoResistVal = PhotoModule();
	if(PhotoResistVal > 160)
	{
		flag2 = 1;
		printf("something on the table\n");
	}
	else
	{
		flag2 = 0;
		printf("nothing on the table\n");
	}
	printf("ultrasonic begins\n");
	ultraInit();
	dist = disMeasure();
	Dint = (int)dist;
	printf("Distance: %d\n",Dint);
	if(Dint < 40)
	{
		flag3 = 1;
			printf("something on the seat\n");
	}
	else
	{
		printf("nothing on the chair\n");
		flag3 = 0;
	}

	flag4 = flagButton;
	
	if(flag1==1&&flag3==1&&flag4==0)
	{
		printf("RED\n");
		flagGreen = 0;
		flag0 = 0;
	}
	if(flag1==1&&flag3==1&&flag4==1)
	{
		printf("Blue\n");
		flagGreen = 0;
		flag0 = 3;
	}
	if(flag1==0&&flag2==1)
	{
		printf("YELLOW\n");
		flagGreen = 0;
		flag0 = 2;
	}
	if(flag1==0&&flag2==0&&flag3==1)
	{
		printf("YELLOW\n");
		flagGreen = 0;
		flag0 = 2;
	}
	if(flagGreen==1)
	{
		printf("GREEN\n");
		flag0 = 1;
		flagButton = 0;
	}
	//delay(3000);
	

	return flag0;
}

//Socket-Client...............................................................................................
int Client_send(char *IPadd, char *message)
{
    
 

    struct sockaddr_in client_addr;
    bzero(&client_addr,sizeof(client_addr)); 
    client_addr.sin_family = AF_INET;    
    client_addr.sin_addr.s_addr = htons(INADDR_ANY);
    client_addr.sin_port = htons(0);    
  
    int client_socket = socket(AF_INET,SOCK_STREAM,0);
    if( client_socket < 0)
    {
        printf("Create Socket Failed!\n");
        exit(1);
    }
   
    if( bind(client_socket,(struct sockaddr*)&client_addr,sizeof(client_addr)))
    {
        printf("Client Bind Port Failed!\n"); 
        exit(1);
    }
 
    
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if(inet_aton(IPadd,&server_addr.sin_addr) == 0) 
    {
        printf("Server IP Address Error!\n");
        exit(1);
    }
    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);
    socklen_t server_addr_length = sizeof(server_addr);
 
    if(connect(client_socket,(struct sockaddr*)&server_addr, server_addr_length) < 0)
    {
        printf("Can Not Connect To %s!\n",IPadd);
        exit(1);
    }
 
	char buffer[BUFFER_SIZE];
		
	bzero(buffer,BUFFER_SIZE);
	snprintf(buffer, sizeof(buffer), "%s", message);

	send(client_socket,buffer,BUFFER_SIZE,0);

  
    close(client_socket);
    return 0;
}





void* thread_part_1(void)
{
	


	int flagColor;
	char seatID[5] = "0001";
	char IPaddress[14] = "192.168.1.197";
	char sendBuff[1024];

    pcf8591Setup(PCF, 0x48);



    


    


    while(1)
    {
	bzero(sendBuff,sizeof(sendBuff));
	flagColor = ColorFlag();
	snprintf(sendBuff, sizeof(sendBuff), "%s %d", seatID, flagColor);
		
	printf("%s\n",seatID);	

	printf("%s\n",sendBuff);
	Client_send(IPaddress, sendBuff);
	
    sleep(3); 
  
    }
}

void* Button(void *arg)
{
	int flagB;
	
	while(1)
	{
		flagB = digitalRead(TouchPin2);
		if(flagB==1)
		{
			flagButton = 1;
			printf("Button is pressed\n");
		}
	}
	return 0;
}

int main()
{


	int err1;
	int err2;
	int err3;
	int err4;
	if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
		printf("setup wiringPi failed !");
		return 1; 
	}
// Create threads here. Calls each function on the top
// If you need to create new threads, just add them here following the rules.
	err1 = pthread_create(&(tid[0]), NULL, thread_part_1, NULL);
	if(err1!=0)  
    {  
        printf("Create pthread error!\n");  
		return -1;
	}
	err2 = pthread_create(&(tid[1]), NULL, Button, NULL);
	if(err2!=0)  
    {  
        printf("Create pthread error!\n");  
		return -1;
	}
	err3 = pthread_join(tid[0],NULL);
	if(err3!=0)  
    {  
        printf("Thread join error!\n");  
		return -1;
	}
	err4 = pthread_join(tid[1],NULL);
	if(err4!=0)  
    {  
        printf("Thread join error!\n");  
		return -1;
	}
	
	return 0;
}

