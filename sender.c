/*
 * Author: Noreddine Kessa
 * Purpose:Opens a message queue and enqueues messages, these messages are to be received by a different
 * process.
 * Platform: tested on Ubuntu 16.04
 * Notes:to compile use  gcc -o sender sender.c  -lrt
 * 		 to run the program you need root privilege: sudo ./sender
 * */


#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include "./common.h"

int main(int argc , char * argv[]){

	//definition of a buffer,
	char buff[MESSAGE_MAX_SIZE+1]={0};

	//queue name
	char queue_name[]=QUEUE_NAME;

	//queue flags
	int queue_flags= O_CREAT|O_RDWR ; //automatically this is blocking queue

	//queue attributes
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = QUEUE_SIZE;
	attr.mq_msgsize = MESSAGE_MAX_SIZE;
	attr.mq_curmsgs = 0;

	//create if queue doesn't exist and open it for writing
	mqd_t message_queue =mq_open(queue_name,queue_flags ,0777 , &attr);

	//check if queue opened ok
	if (message_queue < 0){
		printf("Failed to open/create message queue %s, error %d\n" , queue_name , message_queue );
		return -1;
	}else{
		printf("Successfully opened/created message queue %s\n" , queue_name);

		//main loop variables definition
		int i=0;
		int err;

		//main loop, it runs forever
		while(1){

			//delay
			sleep(SEND_WAIT_SEC);

			//clear the buffer
			memset(buff,0,MESSAGE_MAX_SIZE+1);

			//compose the message to be queued
			sprintf(buff,"message#%d",i);

			//increment message number
			i++;

			//enqueue the message
			err = mq_send(message_queue,buff,strlen(buff)+1,1);

			//print the message that has been queued, show errors if any
			if (buff < 0){
				printf("Error Queuing\n %s", buff);
			}else {
				printf("Queuing:%s\n",buff);
			}


		}

		mq_close(message_queue);
	}

}



