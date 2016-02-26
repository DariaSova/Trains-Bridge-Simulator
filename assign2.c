#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "train.h"

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;
int turn_id=-1;
int e_count_max=0;
int e_count=0;
int w_count=0;

struct node {
  int id;
  struct node *next;
}*East, *West, *west_temp, *east_temp;

void ArriveBridge (TrainInfo *train);
void CrossBridge (TrainInfo *train);
void LeaveBridge (TrainInfo *train);

/*
 * This function is started for each thread created by the
 * main thread.  Each thread is given a TrainInfo structure
 * that specifies information about the train the individual 
 * thread is supposed to simulate.
 */
void * Train ( void *arguments )
{
  TrainInfo	*train = (TrainInfo *)arguments;

  /* Sleep to simulate different arrival times */
  usleep (train->length*SLEEP_MULTIPLE);

  ArriveBridge (train);
  CrossBridge  (train);
  LeaveBridge  (train); 

  /* I decided that the paramter structure would be malloc'd 
   * in the main thread, but the individual threads are responsible
   * for freeing the memory.
   *
   * This way I didn't have to keep an array of parameter pointers
   * in the main thread.
   */
  free (train);
  return NULL;
}

void ArriveBridge ( TrainInfo *train )
{
  //put all trains in queues
  pthread_mutex_lock(&m);

  struct node *current = (struct node *)malloc(1*sizeof(struct node));
  current->id = train->trainId;
  current->next = NULL;

  if(train->direction == 2) {
    if(e_count==0)
      East = current;
    else east_temp->next = current;
    east_temp = current;
    e_count++;
  }else {
    if(w_count==0)
      West = current;
    else west_temp->next = current;
    west_temp = current;
    w_count++;
  }

  if(turn_id==-1) {
    turn_id = train -> trainId;
  }

  pthread_mutex_unlock(&m);

  pthread_mutex_lock(&m);
  while(turn_id!=train->trainId){
    pthread_cond_wait(&c,&m);
  }
  pthread_mutex_unlock(&m);

}

/*
 * Simulate crossing the bridge
 */
void CrossBridge ( TrainInfo *train )
{
  /* 
   * This sleep statement simulates the time it takes to 
   * cross the bridge.  Longer trains take more time.
   */
  usleep (train->length*SLEEP_MULTIPLE);

  printf ("Train %2d is OFF the bridge(%s)\n", train->trainId, 
      (train->direction == DIRECTION_WEST ? "West" : "East"));
  fflush(stdout);
}

void LeaveBridge ( TrainInfo *train )
{
  pthread_mutex_lock(&m);

  //west train
  if(train->direction==1){
    if(West->next)
      West = West->next;
    w_count--;
    //reset the max east tran counter
    e_count_max=0;
  }else{
    if(East->next)
      East = East->next;
    e_count--;
    if(w_count != 0) e_count_max++;
  }

  if((e_count_max >=2 || e_count==0) && w_count!=0){
    turn_id = West->id;
  }else {
    turn_id = East->id;
  }

  pthread_cond_broadcast(&c);
  pthread_mutex_unlock(&m);

}

int main ( int argc, char *argv[] )
{
  int		trainCount = 0;
  char 		*filename = NULL;
  pthread_t	*tids;
  int		i;


  /* Parse the arguments */
  if ( argc < 2 )
  {
    printf ("Usage: part1 n {filename}\n\t\tn is number of trains\n");
    printf ("\t\tfilename is input file to use (optional)\n");
    exit(0);
  }

  if ( argc >= 2 )
  {
    trainCount = atoi(argv[1]);
  }
  if ( argc == 3 )
  {
    filename = argv[2];
  }	

  initTrain(filename);
  tids = (pthread_t *) malloc(sizeof(pthread_t)*trainCount);

  for (i=0;i<trainCount;i++)
  {
    TrainInfo *info = createTrain();

    if ( pthread_create (&tids[i],0, Train, (void *)info) != 0 )
    {
      printf ("Failed creation of Train.\n");
      exit(0);
    }
  }

  /*
   * This code waits for all train threads to terminate
   */
  for (i=0;i<trainCount;i++)
  {
    pthread_join (tids[i], NULL);
  }

  free(tids);
  free(East);
  free(West);
  return 0;
}
