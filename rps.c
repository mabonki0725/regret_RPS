/************************
  Regret Minimization 
  for Rock Paper Scisers
*************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define NUM_ACTION 3

#define ROCK    0
#define PAPER   1
#define SCISERS 2

int g_dbg=0;  /* dbg flag */
/**

**/
int getStrategy(num,regretSum,strategy,strategySum)
int num;
double regretSum[];
double strategy[]; /* out */
double strategySum[]; /* out */
{
  double normalizingSum=0;
  int a;

  for(a = 0;a < num;a++) {
    strategy[a] = regretSum[a] > 0 ? regretSum[a] : 0;
    normalizingSum += strategy[a];
  }
  for(a=0;a < num; a++) {
    if(normalizingSum > 0) {
      strategy[a] /= normalizingSum;
    }
    else {
      strategy[a] = 1.0 / num;
    }
    strategySum[a] += strategy[a];
  }
  return(0);
}
/**

**/
int getAction(num,strategy)
int num;
double strategy[];
{
  double r;
  int a=0;
  double cumulativeProbability = 0;

  r = (double) rand() / (double)RAND_MAX;
  while(a < num -1 ) {
    cumulativeProbability += strategy[a];
    if(r < cumulativeProbability) break;
    a++;
  }
  return(a);
}
/***

***/
int train(iteration,num,strategy,strategySum,oppStrategy,regretSum)
int num;
double strategy[];
double strategySum[];
double oppStrategy[];
int iteration;

double regretSum[]; /* out */
{
  double actionUtility[NUM_ACTION];
  int i;
  int myAction,otherAction;
  int a;
  char *view[3]={"ROCK    ","PAPER   ","SCISSERS"};

  for(i =0;i < iteration;i++) {
    /** Get regret-matched mixed-strategy action */
    getStrategy(num,regretSum,strategy,strategySum);
    myAction = getAction(num,strategy);
    otherAction = getAction(num,oppStrategy);
    /* compute action utilities */
    actionUtility[otherAction] = 0;
    actionUtility[otherAction == num -1 ? 0      : otherAction+1] = 1;
    actionUtility[otherAction == 0      ? num -1 : otherAction-1] = -1;
    /* Accumilate action regrets */
    for(a = 0;a < num;a++) {
      regretSum[a] += actionUtility[a] - actionUtility[myAction];
    }
    if(g_dbg) {
      fprintf(stderr,"iter=%3d my=%s other=%s regret=[%lf %lf %lf]\n"
      ,i,view[myAction],view[otherAction],regretSum[0],regretSum[1],regretSum[2]);
    }
  }
  return(0);
}
/***

***/
int getAverageStrategy(num,strategySum,avgStrategy)
int num;
double strategySum[];
double avgStrategy[];
{
  double normalizingSum=0;
  int a;

  for(a=0;a<num;a++) {
    normalizingSum += strategySum[a];
  }
  for(a=0;a<num;a++) {
    if(normalizingSum > 0) {
      avgStrategy[a] = strategySum[a] / normalizingSum;
    }
    else {
      avgStrategy[a] = 1.0 / normalizingSum;
    }
  }
  return(0);
}
/***

***/
int main(argc,argv)
int argc;
char *argv[];
{
   double regretSum[NUM_ACTION];
   double strategy[NUM_ACTION];
   double strategySum[NUM_ACTION];
   //double oppStrategy[NUM_ACTION];
   double oppStrategy[NUM_ACTION]={0.33,0.33,0.34};

   double avgStrategy[NUM_ACTION];
   int a,num;

   /* initialize */
   num = NUM_ACTION;
   for(a=0;a<num;a++) {
     regretSum[a]=0.0;
     strategy[a]=0.0;
     strategySum[a]=0.0;
     avgStrategy[a]=0.0;
   }
   /* trainning */
   train(1000,num,strategy,strategySum,oppStrategy,regretSum);

   /* result */
   getAverageStrategy(num,strategySum,avgStrategy);
   for(a =0; a< num;a++) {
     fprintf(stderr,"action=%d %lf\n",a,avgStrategy[a]);
   }

   return(0);

}