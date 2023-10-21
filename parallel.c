#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX 10240
#define NUM_THREADS  10


int n1,n2;
char *s1,*s2;
FILE *fp;
int countArray[NUM_THREADS]={0};
pthread_mutex_t lock= PTHREAD_MUTEX_INITIALIZER;

//read input file and generate string s2/s2 and length n1/n2
int readf(FILE *fp)
{
	if((fp=fopen("strings.txt", "r"))==NULL){
		printf("ERROR: can't open string.txt!\n");
		return 0;
	}
	s1=(char *)malloc(sizeof(char)*MAX);
	if(s1==NULL){
		printf("ERROR: Out of memory!\n");
		return -1;
	}
	s2=(char *)malloc(sizeof(char)*MAX);
	if(s1==NULL){
		printf("ERROR: Out of memory\n");
		return -1;
	}
	/*read s1 s2 from the file*/
	s1=fgets(s1, MAX, fp);
	s2=fgets(s2, MAX, fp);
	n1=strlen(s1);  /*length of s1*/
	n2=strlen(s2)-1; /*length of s2*/

	if(s1==NULL || s2==NULL || n1<n2)  /*when error exit*/
		return -1;
	return 0;
}

int num_substring(int t)
{
//add your logic here
//1, how to distribute different parts of string s1 into different threads
/*

for 1, we have 10 threads in total, so divide s1 into 10 different equally threads.
we take the length of s1, which is n1, and divide with the total num of threads, 10
--> int start = (n1/ NUM_THREADS) * tid to get the position where the search starts at
--> int end = (n1/NUM_THREADS) *(tid+1) at which string position the thread stop

follow the same logic for single pthread outer for loop example but substitute i for start and i <= end so it can
iterate through the divided threads

*/
int count;
int i, j, k;
//int end = (n1/NUM_THREADS) *(t+1);
//int start = (n1/ NUM_THREADS) * t;

int parts = n1/NUM_THREADS;
int start = 0;
char c[parts+1];
int counter;

for(i = 0; i < n1; i = i+parts){
    counter = 0;
    while(counter < parts){
        c[counter] = s1[i + counter];
        counter++;
    }
    c[counter] = '\0';
}

if res = strcmp(c[t], s2){
        if(res==0){
            count++;
        } else {
            break;
        }
    }

/*
for(i = start; i <= end;  i++){
    count  = 0;
    for(j = i, k = 0, k  <n2; j++; k++){ //search for the string with size of n2 == search for "is" in 10 threads
        if (*(s1+j) != *(s2+k)){
            break;
        } else{
            count++; //count after each thread
        }
        if(count==n2){
            count++;
        }
    }
}*/
return count;

/*
2, how to sum up the total number of substring from all threads
to sum up the total number but also ensure that the count of each thread got printed out, we must
place a lock to prevent the program to print out the total number after each thread
after all threads are run, the function will return the total number of substring from all the threads
*/

}

void *calSubStringThread(void *threadid){
    long  tid =(long) threadid;
    printf("This is thread %ld, ", tid);
    int num = num_substring(tid);
    printf("find num of is: %d\n", num);
    pthread_mutex_lock(&lock);
    totalNum += num;
    pthread_mutex_unlock(&lock);
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int t, rc;
    int totalNum = 0;

	readf(fp);

	for(t=0; t<NUM_THREADS; t++){
        rc = pthread_create(&threads[t], NULL, calSubStringThread, (void *) (size_t)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for(t=0; t<NUM_THREADS; t++){
        pthread_join(threads[t], NULL);
    }

 	printf("The number of substrings is: %d\n", totalNum);
	return 1;
}
