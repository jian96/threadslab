#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
bool ready = false;
bool active = true;
int current_thread = 0;
int counter = 0;

struct normsparam{
    int tid;
};

void* normal(void* args){
    normsparam* idcon = static_cast<normsparam*>(args);
    int thread_num = idcon->tid;
    while (!ready){};
    while (active){
        pthread_mutex_lock(&mtx);
        cout << "thread #" << thread_num << " doing work\n";
        cout << "on turn #" << current_thread << "\n";
        usleep(30000);
        if (current_thread == thread_num) {
            for (int i = 0; i < 30; i++){
                if (thread_num == 1){
                    counter++;  
                } else {
                    counter--;
                } 
            }
            cout << "Thread " << thread_num << " is executing\n";
            current_thread = (current_thread + 1) % 2; // Round robin
            pthread_mutex_unlock(&mtx);
            continue;
        }

        cout << "counter: " << counter << endl;
        usleep(10000);
        pthread_mutex_unlock(&mtx);
    }
    pthread_exit(NULL);
}

void* special(void* args){

}



int main(){
    pthread_t spec;
    pthread_t norm[2];
    normsparam tids[2];

    pthread_create(&spec, NULL, special, (void *)NULL);
    for (int i = 0; i < 2; i++)
    {
        tids[i].tid = i;
        pthread_create(&norm[i], NULL, normal, (void *)&tids[i]);
    }

    ready = true;

    pthread_join(spec, NULL);
    for (int i = 0; i < (2); i++)
    {
        pthread_join(norm[i], NULL);
    }

    cout << "main finished";
    return 0;
}
