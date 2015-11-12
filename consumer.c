#include <stdio.h>
#include <pthread.h>

#define TIMES 10
#define CAPACITY 3
#define ITEMSIZE 20

struct Buffer
{
  int item[ITEMSIZE];
  int size;
};

void *producer();
void *consumer();

pthread_mutex_t mutex;
pthread_cond_t  full_cond;
pthread_cond_t  empty_cond;
struct Buffer buffer;

int
main()
{ int i;
  pthread_t tid;

  for (i =0; i< ITEMSIZE; i++) buffer.item[i]=-1;
  buffer.size = 0;

  pthread_setconcurrency(2);

  pthread_create(&tid, NULL, (void *(*)(void *))producer, NULL  );
  pthread_create(&tid, NULL, (void *(*)(void *))consumer, NULL );
 

 pthread_exit(0);

}

void *producer()
{ int i,j;

 for (j = 0; j<TIMES; j++)
 {pthread_mutex_lock(&mutex);  
 if (buffer.size == CAPACITY)
    {printf("Buffer is full. Producer is waiting...\n");
     pthread_cond_wait(&full_cond, &mutex);
    }
 else
    {
     printf("Producer adds one more item ...\n");
     buffer.item[buffer.size++] = buffer.size;
     for (i =0;i<buffer.size; i++)
     printf("%d ", buffer.item[i]);
     printf("\n");
     pthread_cond_broadcast(&empty_cond);
     
    }
  pthread_mutex_unlock(&mutex);
}
}

void *consumer()
{ int i,j;

for (j=0; j<TIMES; j++)
{
  pthread_mutex_lock(&mutex);

  if (buffer.size == 0)
    {
      printf("Buffer is empty. Consumer is waiting...\n");
      pthread_cond_wait(&empty_cond, &mutex);
    }
  else
    {
      printf("Consumer consumes an item from buffer...\n");
      buffer.size--;
      for (i=0; i<buffer.size; i++)
      printf("%d ", buffer.item[i]);
      printf("\n");
      pthread_cond_broadcast(&full_cond);
    }

   pthread_mutex_unlock(&mutex);
 }
 
}


