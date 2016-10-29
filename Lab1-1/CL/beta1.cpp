#include<iostream>
#include<vector>
#include<string>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

using namespace std;

// lock and cond of creating thread:
pthread_mutex_t creating_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t creating_cond = PTHREAD_COND_INITIALIZER;

// identifier of 4 diresctions:
const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;

// identifier and lock of the next direction to go:
int next_to_go = -1;
pthread_mutex_t next_lock = PTHREAD_MUTEX_INITIALIZER;

// lock of 4 directions (when adding to counter):
/*
pthread_mutex_t north_plus = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t east_plus = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t south_plus = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t west_plus = PTHREAD_MUTEX_INITIALIZER;
*/
pthread_mutex_t cnt_lock[4] = {PTHREAD_MUTEX_INITIALIZER,
                               PTHREAD_MUTEX_INITIALIZER,
                               PTHREAD_MUTEX_INITIALIZER,
                               PTHREAD_MUTEX_INITIALIZER};

// mutex of 4 directions:
/*
pthread_mutex_t north_ready = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t east_ready = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t south_ready = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t west_ready = PTHREAD_MUTEX_INITIALIZER;
*/
pthread_mutex_t ready_lock[4] = {PTHREAD_MUTEX_INITIALIZER,
                               PTHREAD_MUTEX_INITIALIZER,
                               PTHREAD_MUTEX_INITIALIZER,
                               PTHREAD_MUTEX_INITIALIZER};


// cond var of 4 directions:
/*
pthread_cond_t north_first = PTHREAD_COND_INITIALIZER;
pthread_cond_t east_first = PTHREAD_COND_INITIALIZER;
pthread_cond_t south_first = PTHREAD_COND_INITIALIZER;
pthread_cond_t west_first = PTHREAD_COND_INITIALIZER;
*/
pthread_cond_t first_cond[4] = {PTHREAD_COND_INITIALIZER,
                                PTHREAD_COND_INITIALIZER,
                                PTHREAD_COND_INITIALIZER,
                                PTHREAD_COND_INITIALIZER};

// mutex for cond var:
pthread_mutex_t wait_lock[4] = {PTHREAD_MUTEX_INITIALIZER,
                                PTHREAD_MUTEX_INITIALIZER,
                                PTHREAD_MUTEX_INITIALIZER,
                                PTHREAD_MUTEX_INITIALIZER};

// mutex of runnning:
pthread_mutex_t running = PTHREAD_MUTEX_INITIALIZER;

// counter of 4 directions:
int cnt[4] = {0, 0, 0, 0};
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
    sleep(5);

    const bool DEADLOCK = true;
    const bool NO_DEADLOCK = false;
    bool flag = DEADLOCK;

    while(true){
        sleep(2);
        pthread_mutex_lock(&running);
        cout << "police lock running.\n";
/*        if(cnt[NORTH] > 0
           && cnt[EAST] > 0
           && cnt[SOUTH] > 0
           && cnt[WEST] > 0
           && next_to_go == -1){
*/
        if(next_to_go == -1){
            // cars in all 4 direction but no one is to go
            // deadlock detected
            cout << "DEADLOCK DETECTED!\n";
            cout << "police: North Go First\n";
            if(cnt[NORTH]){
                pthread_cond_signal(&first_cond[NORTH]);
                pthread_mutex_lock(&next_lock);
                next_to_go = NORTH;
                pthread_mutex_unlock(&next_lock);
            }
            else if(cnt[EAST]){
                pthread_cond_signal(&first_cond[EAST]);
                pthread_mutex_lock(&next_lock);
                next_to_go = EAST;
                pthread_mutex_unlock(&next_lock);
            }
            else if(cnt[SOUTH]){
                pthread_cond_signal(&first_cond[SOUTH]);
                pthread_mutex_lock(&next_lock);
                next_to_go = SOUTH;
                pthread_mutex_unlock(&next_lock);
            }
            else{
                pthread_cond_signal(&first_cond[WEST]);
                pthread_mutex_lock(&next_lock);
                next_to_go = WEST;
                pthread_mutex_unlock(&next_lock);
            }
        }
        else if(cnt[NORTH] == 0
                && cnt[EAST] == 0
                && cnt[SOUTH] == 0
                && cnt[WEST] == 0) {
            // all cars gone, reach the end of the program
            cout << "All cars gone, reach the end of the program.\n";
            cout << "Here is the leaving order of this case:\n";
            for(const int entry : result){
                const int DIRECTION = entry & 0x00000003;
                const int id = entry >> 2;
                cout << "\tCar " << id << " from ";
                switch (DIRECTION){
                    case NORTH:
                        cout << "north";
                        break;
                    case EAST:
                        cout << "east";
                        break;
                    case SOUTH:
                        cout << "south";
                        break;
                    case WEST:
                        cout << "west";
                        break;
                }
                cout << " leaves.\n";
            }
            break;
        }
        cout << "police unlock running.\n";
        pthread_mutex_unlock(&running);
    }

}

// car thread:
void* car_proc(void* in){
	const int input = *reinterpret_cast<int*>(in);
    const int DIRECTION = input & 0x00000003;
    const int id = input >> 2;
    cout << "Car " << id << " of direction " << DIRECTION << " created. Now sleep(1).\n";
    pthread_cond_signal(&creating_cond);
    sleep(1);
	// when this thread is created, add 1 to counter
	pthread_mutex_lock(&cnt_lock[DIRECTION]);
	cnt[DIRECTION]++;
	pthread_mutex_unlock(&cnt_lock[DIRECTION]);

	// wait ready_lock
    // once gained, this car is ready to go
	pthread_mutex_lock(&ready_lock[DIRECTION]);
	pthread_mutex_lock(&wait_lock[DIRECTION]); // wait_lock only used to bind to first_cond
	cout << "Car " << id << " arrives from ";
    switch (DIRECTION){
        case NORTH:
            cout << "north";
            break;
        case EAST:
            cout << "east";
            break;
        case SOUTH:
            cout << "south";
            break;
        case WEST:
            cout << "west";
            break;
    }
    cout << ".\n";
	sleep(1); // when ready, sleep for 1 second

	// wait for "first" signal
	// HERE!!
    cout << "Car " << id << " is waiting for cond.\n";
	//pthread_cond_wait(&first_cond[DIRECTION], &wait_lock[DIRECTION]);
    while(true){
        sleep(1);
        pthread_mutex_lock(&next_lock);
        if(next_to_go == DIRECTION){
            pthread_mutex_unlock(&next_lock);
            break;
        }
        else
            pthread_mutex_unlock(&next_lock);
    }
    cout << "Car " << id << " gets cond.\n";
        // here we should not use ready_lock
        // otherwise the following cars will compete for ready_lock

	// go
	// wait for "running" lock
	pthread_mutex_lock(&running);
	// process of going
	result.push_back(input);
	cnt[DIRECTION]--;
	int tmp = (DIRECTION + 1) % 4;
	// find the first non-empty direction on the left
	while(tmp != DIRECTION && !cnt[tmp]){
        cout << "Seeking for next.\n";
        tmp = (tmp + 1) % 4;
    }
    /*
    if(tmp == DIRECTION && cnt[DIRECTION] == 0){
        // all car gone
        pthread_mutex_lock(&next_lock);
        next_to_go = -1;
        cout << "All car gone!\n"
             << "set next_to_go = " << next_to_go << "\n";
        pthread_mutex_unlock(&next_lock);
    }
    */
    /*
    do{
        pthread_mutex_lock(&cnt_lock[tmp]);
        if(cnt[tmp]){
            // cnt[tmp] != 0 means direction "tmp" is non-empty
            break;
        }
        else{
            // direction "tmp" is empty
            pthread_mutex_unlock(&cnt_lock[tmp]);
            tmp = (tmp + 1) % 4;
        }
    }while(true);
    */

    // give signal to the first non-empty direction on the left
    switch(tmp){
		case EAST:
			pthread_cond_signal(&first_cond[EAST]);
            next_to_go = EAST;
			break;
		case SOUTH:
			pthread_cond_signal(&first_cond[SOUTH]);
            next_to_go = SOUTH;
			break;
		case WEST:
			pthread_cond_signal(&first_cond[WEST]);
            next_to_go = WEST;
			break;
		default:
			pthread_cond_signal(&first_cond[NORTH]);
            next_to_go = NORTH;
			break;
	}
	cout << "Car " << id << " is leaving!\n";
    cout << "Set next_to_go = " << next_to_go << "\n";
    //pthread_mutex_unlock(&cnt_lock[tmp]);
	pthread_mutex_unlock(&running);
	// HERE!!
	pthread_mutex_unlock(&wait_lock[DIRECTION]);// here we should not use ready_lock
	pthread_mutex_unlock(&ready_lock[DIRECTION]);

}

int main(){
    string input;
    cout << "Please input the start order:\n"
         << "e.g.: input neewsw means cars appear in order\n"
         << "north, east, east, west, south, west\n";
    cin >> input;

    pthread_t thread_cars[100];
    pthread_t thread_police;

    for(int i = 0; i < input.length(); i++){
        char c = input[i];
        int rc = 0;
        pthread_mutex_lock(&creating_lock);
        int passin = i << 2;
        switch(c){
            case 'n':
                cout << "input n\n";
                passin += NORTH;
                //rc = pthread_create(&thread_cars[i], NULL, north_car_proc, &i);
                break;
            case 'e':
                passin += EAST;
                cout << "input e\n";
                break;
            case 's':
                passin += SOUTH;
                cout << "input s\n";
                break;
            case 'w':
                passin += WEST;
                cout << "input w\n";
                break;
            default:
                break;

        }
        rc = pthread_create(&thread_cars[i], NULL, car_proc, &passin);
        if(rc){
            cout << "ERROR: creating the " << i << "-th car thread error!\n"
                 << "your input is \'" << c << "\'\n"
                 << "Program interrupted.\n";
            break;
        }
        else{
            cout << "creating the " << i << "-th car thread succeeded.\n";
        }
        pthread_cond_wait(&creating_cond, &creating_lock);
        pthread_mutex_unlock(&creating_lock);
    }

    // create police thread
    int rc = pthread_create(&thread_police, NULL, police_proc, nullptr);
    if(rc){
        cout << "ERROR: creating the police thread error!\n"
             << "Program interrupted.\n";
        cout << "Program terminates.\n";
        return 0;
    }

    void* status;
    pthread_join(thread_police, nullptr);
    cout << "Program terminates.\n";
	return 0;
}
