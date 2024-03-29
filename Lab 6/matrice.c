#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>


int matrix1[3][3] = {
        {1,2,3},
        {4,5,6},
        {7,8,9}
};
int matrix2[3][3] = {
        {1,1,1},
        {2,2,2},
        {3,3,3}
};
int result_matrix[3][3];

struct pos{
    int i, j;
};

void* compute(void* position){
    struct pos * index = position;
    int i = index->i;
    int j = index->j;
    free(position);
    result_matrix[j][i] = 0;
    for(int k=0;k<3;++k)
        result_matrix[j][i] += matrix1[j][k]*matrix2[k][i];
    return NULL;
}

int main(){
    pthread_t threads[3*3];
    int thread_id = 0;
    for(int i=0;i<3;++i)
        for(int j=0;j<3;++j){
            struct pos* index = calloc(1, sizeof(struct pos));
            index->i = i;
            index->j = j;
            if(pthread_create(&threads[thread_id++], NULL, compute, index)){
                perror(NULL);
                return errno;
            }
        }

    for(int i=0;i<thread_id;++i){
        if(pthread_join(threads[i], NULL)){
            perror(NULL);
            return errno;
        }
        printf("Joined thread %d\n", i);
    }
     
    for(int i=0;i<3;++i){
        for(int j=0;j<3;++j)
            printf("%d ", result_matrix[i][j]);
        printf("\n");
    }
    return 0;
}