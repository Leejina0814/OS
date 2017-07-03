#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
typedef int buffer_item;
#define BUFFER_SIZE 10

buffer_item *buffer;
int rear = 0;
int front = 0;
pthread_mutex_t mutex;
pthread_t ip, ic, im;
sem_t empty;
sem_t full;
int chk =0;
int sum = 0;
int m_c[2] = {0,0};

int insert_item(buffer_item item){
	
	if(chk>=BUFFER_SIZE)
		return -1;
	buffer[rear] = item;
	rear=(rear+1)&BUFFER_SIZE;
	chk++;
	sum++;

	return 0;
}

int remove_item(buffer_item item){
	
	if(chk<=0)
		return -1;
	item = buffer[front];
	buffer[front] = -1;
	front = (front+1)%BUFFER_SIZE;
	chk--;
	sum++;

	return 0;
}

void *producer(void *param){

	buffer_item item;
	while(1){
		int r = rand()%5;
		sleep(r);
		item = rand();

		sem_wait(&empty);
		pthread_mutex_lock(&mutex);

		while(m_c[0] != m_c[1]);
		if(insert_item(item))
			fprintf(stderr, "report error condition" );
		else{
			printf("insert : %d\n", chk );
		}
		m_c[1] = sum;
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
		
		
	}
}
void *consumer(void *param){
	buffer_item item;
	while(1){
		int r = rand()%5;
		sleep(r);
		
		sem_wait(&full);
		pthread_mutex_lock(&mutex);

		while(m_c[0] != m_c[1]);

		if(remove_item(item)){
			fprintf(stderr, "report error condition");
		}
		else{
			printf("remove : %d\n", chk );
		}
		m_c[1] = sum;
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
		
	}
}

void *monitor(void *param){
	while(1){
		while(m_c[0] == m_c[1]);
		printf("Acknowledge no : %d -> count == : %d\n",sum, chk);
		m_c[0] = m_c[1];
	}
}

int main(int argc, char *argv[]){
	int sp = atoi(argv[1]);
	int np = atoi(argv[2]);
	int nc = atoi(argv[3]);
	buffer = (buffer_item*)malloc(sizeof(buffer_item)*BUFFER_SIZE);
	for(int i =0; i<BUFFER_SIZE; i++)
		buffer[i] = -1;

	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full,0,0);
	pthread_mutex_init(&mutex, NULL);

	pthread_create(&im, NULL, monitor, NULL);

	for(int i =0; i<np; i++)
		pthread_create(&ip,NULL,producer,NULL);

	for(int i =0; i<nc;i++)
		pthread_create(&ic,NULL,consumer,NULL);

	sleep(sp);
	return 0;
}