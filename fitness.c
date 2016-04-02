
/* Nov 2015 . play with ga fitness algorithm */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int size = 4 , steps = 5 , seed = 0 ;
double pbase = 2 , crate = 0.75 , mrate = 0.3 ;
char *target = "to be or n";

int random_letter_or_space(void)
{
  int letter;
  letter = 'a';
  //printf("letter = %d\n" , letter);
  letter = 'z';
  //printf("letter = %d\n" , letter);
  letter = 'A';
  //printf("letter = %d\n" , letter);
  letter = 'Z';
  //printf("letter = %d\n" , letter);
  printf("rand = %d\n" , rand());
  letter = (rand() % ('z' - 'a' + 2)) + 'a';
  //printf("letter = %c\n" , letter);
  /* when using random() without seed , 
     it will present same random number-set each running .
  */
  if(letter > 'z')
    letter = ' ';
  return(letter);
}

void compute_fitness(char **pop , int *correct , double *fit)
{
  //printf("hello3 \n");
  int tlen ; 
  int i , j , count ;
  double sum ;
  //int *correct ;  
  //printf("hello4 \n");
  tlen = 10 ;
  sum = 0 ; 
  /*
  correct[0] = 2;
  correct[1] = 4;
  correct[2] = 7;
  correct[3] = 3;
  printf("size = %d\n" , size);
  */
  for(i = 0 ; i < size ; i++)
  {
    count = 0 ;    
    for(j = 0 ; j < tlen ; j++)
    {
      if(j == 0)
        printf(" pop = %c" , pop[i][j]);
      else if(j == tlen - 1)
        printf("%c\n" , pop[i][j]);
      else
        printf("%c" , pop[i][j]);

      if(pop[i][j] == target[j])
      {
        count++;
      }
      correct[i] = count;
    }
    
    //printf(" correct[i] = %d\n" , correct[i] );
    fit[i] = pow(pbase , correct[i] - tlen);
    //printf(" fit[i] = %f\n" , fit[i] );
    sum += fit[i];
  }
  //printf("sum = %f\n" , sum );  

  for(i = 0 ; i < size ; i++)
  {
    fit[i] /= sum;
    //printf("correct[i] = %d , fit2[i] = %f\n" , correct[i] , fit[i]);
  }
}

void dump_stats(int *correct , char **pop , int time , int tlen)
{
  int i , besti = -1 , sum = 0 ;
  double best = -1 , ave = 0 ;

  for(i = 0 ; i < size ; i++)
  {
    // whether to consider multiple high fitness objects .
    if(correct[i] > besti)
    {
      besti = correct[i];
    }
    sum += correct[i];
  }
  ave = sum / (size * tlen);

  printf("ntime = %d\n" , time);
  printf("average fitness = %f\n" , ave);
  printf("best for fitness = %f\n" , correct[besti] / (double) tlen );
  printf("best one = \"%s\"\n" , pop[besti] );
}

int select_one(double *normfit)
{
  int i;
  double x , sum;

  sum = 0;
  /*
  x = ((rand() % 10) / 10.0);
  printf("x = %f\n" , x);
  printf("rand = %d\n" , rand());
  printf("s2=%f\n" , rand() % 10.0);
  */
  x = rand();
  //printf("x1 = %f\n" , x);
  while(x >= 1)
  {
    x /= 10.0;
  }
  //printf("x2 = %f\n" , x);
  for(i = 0 ; i < size ; i++)
  {
    sum += normfit[i];
    //printf("loop i = %d , sum = %f\n" , i , sum);
    if(sum >= x)
    {
      //printf("selected i = %d\n" , i);
      return(i);
    }
  }
  return(size - 1);
}

void reproduce(char **oldpop , char **newpop , int pa , int pb , int tlen , int index)
{
  int i , j , k, cpoint ;
  double x , y , z;
  
  x = rand();
  //printf("reproduce_x = %f\n" , x);
  while(x >= 1)
  {
    x /= 10.0;
  }
  //printf("reproduce_x2 = %f\n" , x);
  cpoint = (x < crate) ? (rand() % (1 + tlen)) + 1 : tlen;
  //printf("cpoint = %d\n" , cpoint);
  for(i = 0 ; i < cpoint ; i++)
  {
    newpop[index][i] = oldpop[pa][i];
    newpop[index + 1][i] = oldpop[pb][i];
  }
  for(j = cpoint ; j < tlen ; j++)
  {
    newpop[index][j] = oldpop[pb][j];
    newpop[index + 1][j] = oldpop[pa][j];
  }

  for(k = 0 ; k < tlen ; k++)
  {
    y = rand();
    //printf("reproduce_y = %f\n" , y);
    while(y >= 1)
    {
      y /= 10.0;
    }
    if(y < mrate)
    {
      newpop[index][k] = random_letter_or_space();
    }
    
    z = rand();
    //printf("reproduce_z = %f\n" , z);
    while(z >= 1)
    {
      z /= 10.0;
    }
    if(z < mrate)
    {
      newpop[index + 1][k] = random_letter_or_space();
    }
  }
}

int main(int argc , char **argv)
{
  int i , j , t , targetlen , parent_a , parent_b ;
  int *numcorrect; 
  char **newpop , **oldpop , **swap;
  double *normfit;

  //srandom(seed);
  srand((unsigned)time(NULL));

  // whether to consider forcing the size to be even . 

  targetlen = 10;
  printf("hello! \n");
  newpop = malloc(sizeof(char *) * size);
  oldpop = malloc(sizeof(char *) * size);
  printf("hello 1-1 \n");
  numcorrect = malloc(sizeof(int) * size);
  normfit = malloc(sizeof(double) * size);
  printf("hello2 \n");
  for(i = 0 ; i < size ; i++)
  {
    newpop[i] = malloc(sizeof(char) * targetlen + 1);
    oldpop[i] = malloc(sizeof(char) * targetlen + 1);
    for(j = 0 ; j < targetlen ; j++)
    {
      oldpop[i][j] = random_letter_or_space();
    }
    newpop[i][targetlen] = 0;
    oldpop[i][targetlen] = 0;
  }

  for(t = 0 ; t < steps ; t++)
  {
    compute_fitness(oldpop , numcorrect , normfit);
    dump_stats(numcorrect , oldpop , t + 1 , targetlen);
    for(i = 0 ; i < size ; i += 2)
    {
      parent_a = select_one(normfit);
      parent_b = select_one(normfit);
      // whether to add parent_a can not be equal to parent_b .  
      //printf("p_a = %d , p_b = %d \n" , parent_a , parent_b);
      reproduce(oldpop , newpop , parent_a , parent_b , targetlen , i);
    }
    
    swap = oldpop;
    oldpop = newpop;
    newpop = swap; 
    /*
    swap = newpop;
    newpop = oldpop;
    oldpop = swap;
    */
  }
  exit(0);
}
