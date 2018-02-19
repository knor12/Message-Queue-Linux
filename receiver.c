/*
 * Author: Noreddine Kessa
 * Purpose:Opens a message queue and dequeues messages sent by another
 * process, then displays them to the terminal
 *
 * Platform: tested on Ubuntu 16.04
 * Notes:to compile use  gcc -o receiver receiver.c  -lrt
 * 		 to run the program you need root privilege: sudo ./receiver
 * */


#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include "./common.h"


int main(int argc , char * argv[]){

	//buffer to temp message processing
	char buff[MESSAGE_MAX_SIZE+1]={0};

	//this null terminated string holds the message queue name
	char queue_name[]=QUEUE_NAME;

	//write/read/create flags of the queue
	int queue_flags=O_RDWR ; //automatically this is blocking queue

	//queue attributes structure
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = MESSAGE_MAX_SIZE;
	attr.mq_msgsize = QUEUE_SIZE;
	attr.mq_curmsgs = 0;

	//open the queue
	mqd_t message_queue =mq_open(queue_name,queue_flags ,0777 , &attr);

	//check if the queue was opened OK
	if (message_queue < 0){
		printf("failed to open/create message queue %s, error %d\n" , queue_name , message_queue );
	}else{
		printf("Successfully opened/created message queue %s\n" , queue_name);

		//declaration of a few variables
		int i=0;
		int message_size;
		buff[0]=0;
		unsigned int * message_priority;

		//forever loop
		while(1){

			//clear the buffer
			memset(buff,0,MESSAGE_MAX_SIZE+1);

			//delay
			sleep(RECEIVE_WAIT_SEC);

			//get a message if any, otherwise block until a message is available
			message_size = mq_receive(message_queue,buff,MESSAGE_MAX_SIZE,message_priority);

			//print the message that you have received
			printf("Dequeuing: %s\n",buff);
		}

		//this point will never be reach.
		//I added this for future interrupt signal
		//handler implementation, when  ctr-c is pressed
		mq_close(message_queue);
	}

}

