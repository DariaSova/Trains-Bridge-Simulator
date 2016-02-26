#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "train.h"

/* A global to assign IDs to our trains */ 
int idNumber = 0;
int doRandom = 0;

/* The file to input train data from */
FILE *inputFile;
char ch[80];

#define MAXLINE		80

void	initTrain ( char *filename )
{
  doRandom = 0;


  /* If no filename is specified, generate randomly */
  if ( !filename )
  {
    doRandom = 1;
    srandom(getpid());
  }
  else
  {

    inputFile = fopen(filename, "r");

  }
}

/*
 * Allocate a new train structure with a new trainId, trainIds are
 * assigned consecutively, starting at 0
 *
 * Either randomly create the train structures or read them from a file
 *
 * This function malloc's space for the TrainInfo structure.  
 * The caller is responsible for freeing it.
 */
TrainInfo *createTrain ( void )
{
  TrainInfo *info = (TrainInfo *)malloc(sizeof(TrainInfo));

  /* I'm assigning the random values here in case
   * there is a problem with the input file.  Then
   * at least we know all the fields are initialized.
   */	 
  info->trainId = idNumber++;
  info->arrival = 0;
  info->direction = (random() % 2 + 1);
  info->length = (random() % MAX_LENGTH) + MIN_LENGTH;


  if (!doRandom)
  {
    int length;
    char *current;
    if ((fgets(ch, 82, inputFile) ) != NULL) {
      current = ch;
      current++;
      if(ch[0]=='W' || ch[0]=='w'){
        info->direction = 1;
      }
      else if(ch[0]=='E' || ch[0]=='e'){
        info->direction = 2;
      }
      length = atoi(current);
      info->length = length;
    }
  }
  return info;
}
