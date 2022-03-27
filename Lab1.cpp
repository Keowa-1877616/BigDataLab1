#include <syscall.h>

#include <functional>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define N 30

//Global variable
int A2[N][N], A3[N][N][N];
int B2[N][N], B3[N][N][N];
int C2[N][N], C3[N][N][N];

using namespace std;

void randomMatrix(){
    //3D matrices A3 and B3
    int MaxV = 20;  //Lessened from 20 to 10 to avoid runn time error thing like:
                    //https://www.youtube.com/watch?v=ynCc-v0K-do
    for (int l=0; l<N; l++){
        //2D matrices A2 and B2
        for (int i=0; i<N; i++){
            for (int j=0; j<N; j++){
                if(l == 0){
                    A2[i][j] = rand() % MaxV;
                    B2[i][j] = rand() % MaxV;
                }
                A3[l][i][j] = rand() % MaxV;
                B3[l][i][j] = rand() % MaxV;
            }
        }
    }

}

void* rank2TensorMultPThread(void* arg){

    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            for (int k=0; k<N; k++){
                C2[i][j] = C2[i][j] + (A2[i][k] * B2[k][j]);
            }
        }
    }
/*
    for (int i=0;i<N;i++){
      for(int j=0;j<N;j++){
        cout<<A2[i][j] << " ";
      }
        cout<< "\n";
    }
    cout<< "\n";
    for (int i=0;i<N;i++){
      for(int j=0;j<N;j++){
        cout<<B2[i][j] << " ";
      }
        cout<< "\n";
    }
    cout<< "\n";
    
    for (int i=0;i<N;i++){
      for(int j=0;j<N;j++){
        //cout<<C2[i][j] << " ";
        printf("%10d", C2[i][j]);
      }
        //cout<< "\n";
        printf("%d \n");
    }
    //cout<< "\n";
/**/
    pthread_exit(0);
};

void* rank3TensorMultPThread(void* arg){

    for (int l=0; l<N; l++){
        //Uses slices at point l to do 2-Dimensional matrix multiplication
        for (int i=0; i<N; i++){
            for (int j=0; j<N; j++){
                for (int k=0; k<N; k++){
                    C3[l][i][j] = C3[l][i][j] + (A3[l][i][k] * B3[l][k][j]);
                }
            }
        }
    }
    pthread_exit(0);
};
/*
void* rank2TensorMultOpenMP(){

    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            for (int k=0; k<N; k++){
                C2[i][j] = C2[i][j] + (A2[i][k] * B2[k][j]);
            }
        }
    }
};
/*
/*
void* rank3TensorMultOpenMP(){

    for (int l=0; l<N; l++){
        //Uses slices at point l to do 2-Dimensional matrix multiplication
        for (int i=0; i<N; i++){
            for (int j=0; j<N; j++){
                for (int k=0; k<N; k++){
                    C3[l][i][j] = C3[l][i][j] + (A3[l][i][k] * B3[l][k][j]);
                }
            }
        }
    }
};
*/

int main()
{
    int TestValue = 0;
    randomMatrix();

    //Test 2D pThread:
    TestValue = 1;
    //Thread ID
    pthread_t tId;
    //Thread attributes
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    //Create thread
        cpu_set_t cpuset;
        /* Set affinity mask to include CPUs 0 to 7. */
        
        CPU_ZERO(&cpuset);
        CPU_SET(0, &cpuset);
        pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
    pthread_create(&tId, &attr, rank2TensorMultPThread, &TestValue);
    printf("tid1 = %ld\n", tId);
    //printf("Hello There! thread %ld, pthread ID - %lu\n", tId, pthread_self());
    //wait until thread is done
    pthread_join(tId, NULL); //Null does nothing with return value
    //printf("Sysid = %ld\n", syscall(SYS_gettid));


    for (int i = 0; i<N; i++){
        for(int j = 0; j < N; j++){
            printf("%10d", C2[i][j]);
        }
        printf("\n");
    }

 /*   
    //Test 3D pThread:
    TestValue = 2;
    //Thread ID
    pthread_t tId2;
    //Thread attributes
    pthread_attr_t attr2;
    pthread_attr_init(&attr2);
    //Create thread
        cpu_set_t cpuset2;
        /* Set affinity mask to include CPUs 0 to 7. */
        /*
        CPU_ZERO(&cpuset2);
        CPU_SET(1, &cpuset2);
        pthread_attr_setaffinity_np(&attr2, sizeof(cpu_set_t), &cpuset2);
    pthread_create(&tId2, &attr2, rank3TensorMultPThread, &TestValue);
    printf("tid2 = %ld\n", tId2);
    //printf("Hello There! thread %ld, pthread ID - %lu\n", tId2, pthread_self());
    //wait until thread is done
    
    //pthread_join(tId, NULL); //Null does nothing with return value
    pthread_join(tId2, NULL); //Null does nothing with return value

/*
    for (int k = 0; k<N; k++){
        for (int i = 0; i<N; i++){
            for(int j = 0; j < N; j++){
                printf("%10d", C3[k][i][j]);
            }
            printf("\n");
        }
        printf("\n");
        printf("\n");
    }
*/

    return 0;
}
