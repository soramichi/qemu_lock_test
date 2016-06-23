#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

const int size = 1024 * 1024 * 1024;
const int n_threads = 4;
#if defined(MUTEX)
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
#else
pthread_spinlock_t lock;
#endif

void* f(void* _ptr){
  int index = 0, n = 0;
  int* ptr = (int*)_ptr;
  int block = 32;
  
  while(1){
    for(index = 0; index < size / (sizeof(int) * n_threads); index+=block){
      int j;

#if defined(MUTEX)
      pthread_mutex_lock(&lock);
#else
      pthread_spin_lock(&lock);
#endif
      
      for(j=0; j<block; j++){
	ptr[index+j] = (index+j) * n;
      }

#if defined(MUTEX)
      pthread_mutex_unlock(&lock);
#else      
      pthread_spin_unlock(&lock);
#endif
    }

    printf("%d\n", ptr[0] + ptr[1]);

    if(n == 10)
      break;

    n++;
  }
}

int main(){
  int* ptr = (int*)malloc(size);
  int t;
  pthread_t pid[n_threads];
  time_t start, end;

#if !defined(MUTEX)
  pthread_spin_init(&lock, n_threads);
#endif
  
  if(ptr == NULL){
    perror("malloc");
    return 1;
  }
   
  time(&start);

  for(t=0; t<n_threads; t++){
    pthread_create(&pid[t], NULL, f, (void*)ptr);
  }

  for(t=0; t<n_threads; t++){
    pthread_join(pid[t], NULL);
  }

  time(&end);

  printf("took %u seconds\n", end - start);
}
