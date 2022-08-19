# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>

int arr[30];

void* Mul(void* arg){

    int answer = 1;
    for(int i=0;i<30;i++){
        answer *= arr[i];
    }

    pthread_exit ((void*)answer);

}


void* Add(void* arg){
    

    int answer = 0;
    for(int i=0;i<30;i++){
        answer += arr[i];
    }

    pthread_exit ((void*)answer);

}

int main(){

    for(int i=0;i<30;i++){
        arr[i] = i+1;
    }


    pthread_t thread_id1 , thread_id2;
    pthread_create (&thread_id1 , NULL, Add, NULL) ;
    pthread_create (&thread_id2 , NULL, Mul, NULL);
    
    void *addRes, *multRes;
    
    pthread_join(thread_id1 ,&addRes);
    pthread_join(thread_id2 ,&multRes);

    printf("%d\n",(int)addRes);
    printf("%d\n",(int)multRes);

}