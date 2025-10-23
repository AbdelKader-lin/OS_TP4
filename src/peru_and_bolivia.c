#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define LOOPS 3

#include <semaphore.h>
sem_t sem ;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int count = 1; // Number of trains allowed in the shared track section;

void sem_initialise( int N ){
    sem_init( &sem , 0 , N);
}

void enter_shared_track_sem(void){ // Sem version
    sem_wait( &sem ) ;
}

void exit_shared_track_sem(void){ // Sem version
    sem_post( &sem ) ;
}

void enter_shared_track(void){ // No semaphore
    pthread_mutex_lock( &mutex );
    while ( count == 0 ){
        sleep( 1 );
    } 
    count-- ;
}

void exit_shared_track(void){ // No semaphore
    count++ ;
    pthread_mutex_unlock( &mutex ) ;
}

void using_shared_track_section(char* name){
    int trip_duration = rand() % 6 +1;

    enter_shared_track_sem();
    
    for(; trip_duration>0; trip_duration--){        
        printf("%c ",name[0]);
        fflush(stdout);
        usleep(1);
    }

    printf("\n");

    exit_shared_track_sem();
}


void* train(void* arg){
    char *country=(char*) arg;
    int i=0;
    
    for(i=0; i<LOOPS; i++){
        
        using_shared_track_section(country);

    }

    return NULL;
}


int main(void){
    pthread_t tids[2];
    int i=0;
    
    struct timespec tt;
    clock_gettime(CLOCK_MONOTONIC, &tt);
    /* seed for the random number generator */
    srand(tt.tv_sec);

    // On initialise avec 1 : 1 train only can enter the CS ( shared track in this case)
    sem_initialise(1);

    /* the peruvian train */
    if(pthread_create (&tids[0], NULL, train, (void*)"PERU") != 0){
        fprintf(stderr,"Failed to create the peruvian thread\n");
        return EXIT_FAILURE;
    }

    /* the bolivian train */
    if(pthread_create (&tids[1], NULL, train, (void*)"BOLIVIA") != 0){
        fprintf(stderr,"Failed to create the peruvian thread\n");
        return EXIT_FAILURE;
    }
    
    /* Wait until every thread ended */ 
    for (i = 0; i < 2; i++){
        pthread_join (tids[i], NULL) ;
    }  

    sem_destroy( &sem );
    
    return EXIT_SUCCESS;
}
