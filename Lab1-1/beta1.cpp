#include<iostream>
#include<vector>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<windows.h>

using namespace std;

// mutex of 4 directions (when adding to counter):
pthread_mutex_t north_plus = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t east_plus = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t south_plus = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t west_plus = PTHREAD_MUTEX_INITIALIZER;

// mutex of 4 directions:
pthread_mutex_t north_ready = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t east_ready = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t south_ready = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t west_ready = PTHREAD_MUTEX_INITIALIZER;

// cond var of 4 directions:
pthread_cond_t north_first = PTHREAD_COND_INITIALIZER;
pthread_cond_t east_first = PTHREAD_COND_INITIALIZER;
pthread_cond_t south_first = PTHREAD_COND_INITIALIZER;
pthread_cond_t west_first = PTHREAD_COND_INITIALIZER;

// mutex for cond var:
pthread_cond_t waiting[4] = {PTHREAD_MUTEX_INITIALIZER, 
							 PTHREAD_MUTEX_INITIALIZER,
							 PTHREAD_MUTEX_INITIALIZER,
							 PTHREAD_MUTEX_INITIALIZER};

// mutex of runnning:
pthread_mutex_t running = PTHREAD_MUTEX_INITIALIZER;

// counter of 4 directions:
int cnt[4] = {0, 0, 0, 0};
const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;
/*
int north_cnt = 0;
int east_cnt = 0;
int south_cnt = 0;
int west_cnt = 0;
*/

// result vector:
vector<int> result;

// police thread:
void* police_proc(void* in){
	//HERE!!
}

// car thread:
void* north_car_proc(void* in){
	int id = *reinterpreted_cast<int*>(in);	

	// when this thread is created, add 1 to counter
	pthread_mutex_lock(&north_plus);
	cnt[NORTH]++;
	pthread_mutex_unlock_lock(&north_plus);
	
	// wait until this car is ready
	pthread_mutex_lock(&north_ready);
	pthread_mutex_lock(&waiting[NORTH]);
	cout << "Car " << id << "arrives from north.\n";
	sleep(1); // when ready, sleep for 1 second

	// wait for "first" signal
	// HERE!!
	pthread_cond_wait(&north_first, &waiting[NORTH]);// here we should not use north_ready

	// go
	// wait for "running" lock	
	pthread_mutex_lock(&running);
	// process of going	
	result.push_back(id);
	cnt[NORTH]--;
	int tmp = (NORTH + 1) % 4;
	// find the first non-empty direction
	while(!cnt[tmp]){tmp = (tmp + 1) % 4;}
	switch(tmp){
		case EAST:
			pthread_cond_signal(&east_first);
			break;
		case SOUTH:
			pthread_cond_signal(&south_first);
			break;
		case WEST:
			pthread_cond_signal(&west_first);
			break;
		default:
			pthread_cond_signal(&north_first);
			break;	
	}
	cout << "Car " << id << " is leaving!\n";
	pthread_mutex_unlock(&running);
	// HERE!!
	pthread_mutex_unlock(&waiting[NORTH]);// here we should not use north_ready
	pthread_mutex_unlock(&north_ready);
	
}

int main(){

	return 0;
}
