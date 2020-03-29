#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_mutex_t mutex, WriterReaderTime;
int sharedkey, rcount = 0;

void *writer(void *arg){
        pthread_mutex_lock(&WriterReaderTime);
        int n = rand() % 10;
        int d = ((int)arg);
        printf("=====================================\n");
        printf("W%d Waiting for %d sec ........\n", d, n);
        sleep(n);
        printf("Enter the Time W%d want to write:\n", d);
        int t;
        scanf("%d", &t);
        printf("W%d is writing now ...\n", d);
        int j;
        for(j=0; j<t; j++){
                printf("Enter the %dth INTEGER value to write:\n", (j+1));
                int u;
                scanf("%d", &u);
                sharedkey = sharedkey + u;
        }
        printf("Shared variable updated to = %d \n", sharedkey);
        printf("=================================================\n");
        pthread_mutex_unlock(&WriterReaderTime);
}

void *reader(void *arg){
        //Entry Part
        pthread_mutex_lock(&mutex);
        rcount++;
        if(rcount==1){
                pthread_mutex_lock(&WriterReaderTime);
        }
        pthread_mutex_unlock(&mutex);
        int n = rand() % 10;
        int d = ((int)arg);
        printf("R%d Waiting for %d sec ........\n", d, n);
        sleep(n);
        printf("Enter time R%d want to read:\n", d);
        int t;
        scanf("%d", &t);
        printf("Now R%d is reading....\n", d);
        int j;
        for(j=0; j<t; j++){
                printf("R%d Shared value reading  = %d\n", d, sharedkey);
        }
        printf("Number of Readers present = %d\n", rcount);
        pthread_mutex_lock(&mutex);
        rcount--;
        if(rcount==0){
                pthread_mutex_unlock(&WriterReaderTime);
        }
        pthread_mutex_unlock(&mutex);
}

void main(){
        printf("Enter the Initial value (INTEGER) of share variable: \n");
        scanf("%d", &sharedkey);
        printf("=============================================\n");
        int numberReader, numberWriter, i;
        printf("no. of Readers?:\n");
        scanf("%d", &numberReader);
        for(i=0; i<numberReader; i++){
                printf("R%d\n", i);
        }
        printf("=============================================\n");
        printf("no. of Writer:?\n");
        scanf("%d", &numberWriter);
        for(i=0; i<numberWriter; i++){
                printf("W%d\n", i);
        }
        printf("=============================================\n");
        if(numberReader<0 || numberWriter<0){
                printf("READER or WRITER should not be negative...\n");
                printf("exiting ........\n");
                return;
        }else if(numberReader == 0){
                printf("No readers ??no threads\n");
        }else if(numberWriter == 0){
                printf("no writers ?? no threads\n");
        }else{
                printf("Creating thread...\n");
        }
        printf("=================================================\n");
        pthread_t rdrs[numberReader], wtrs[numberWriter];
        pthread_mutex_init(&WriterReaderTime, NULL);
        pthread_mutex_init(&mutex, NULL);
        

        if(numberWriter==numberReader){
                for(i=0; i<numberWriter; i++){
                        pthread_create(&wtrs[i], NULL, &writer, (int *)i);
                        pthread_create(&rdrs[i], NULL, &reader, (int *)i);
                }
                for(i=0; i<numberWriter; i++){
                        pthread_join(wtrs[i], NULL);
                        pthread_join(rdrs[i], NULL);
                }
        }else if(numberWriter>numberReader){
                for(i=0; i<numberReader; i++){
                        pthread_create(&wtrs[i], NULL, &writer, (int *)i);
                        pthread_create(&rdrs[i], NULL, &reader, (int *)i);
                }
                for(i=numberReader; i<numberWriter; i++){
                        pthread_create(&wtrs[i], NULL, &writer, (int *)i);
                }
                for(i=0; i<numberReader; i++){
                        pthread_join(wtrs[i], NULL);
                        pthread_join(rdrs[i], NULL);
                }
                for(i=numberReader; i<numberWriter; i++){
                        pthread_join(wtrs[i], NULL);
                }
        }else{
                for(i=0; i<numberWriter; i++){
                        pthread_create(&wtrs[i], NULL, &writer, (int *)i);
                        pthread_create(&rdrs[i], NULL, &reader, (int *)i);
                }
                for(i=numberWriter; i<numberReader; i++){
                        pthread_create(&rdrs[i], NULL, &reader, (int *)i);
                }
                for(i=0; i<numberWriter; i++){
                        pthread_join(wtrs[i], NULL);
                        pthread_join(rdrs[i], NULL);
                }
                for(i=numberWriter; i<numberReader; i++){
                        pthread_join(rdrs[i], NULL);
                }
        }
        printf("Thread Joined and the ");
        printf("Final value of Shared variable = %d\n", sharedkey);
}
