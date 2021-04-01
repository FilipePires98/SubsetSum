//
// AED, October-November 2017
//
// João Alegria nmec.85048
// Filipe Pires nmec.85122
//
// Solution of the subset sum problems
//
// Compile with the command "cc -Wall -O2 -march=native subset_sum_problems.c -lm"
//



//
// Measure elapsed time (on GNU/Linux)
//


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>

#include "subset_sum_problems.h"
#define illegal_mask 0xFFFFFFFFFFFFFFFFull


typedef unsigned long long u64;


void u64tobin(int *bin, u64 numero, int tamanho_numero){
	int meu_indice=0;
	do{	
		if(((numero >> meu_indice) & 1) != 0){
			bin[meu_indice]=1;
		}
		else{
			bin[meu_indice]=0;
		}
		meu_indice++;
	}while(meu_indice < tamanho_numero);
}


int compara(const void* x1, const void* x2){
	u64* n1=*(u64**) x1;
	u64* n2=*(u64**) x2;
	if ( n1[0] < n2[0] ){
		return -1;
	}
	else if (n1[0] > n2[0]){
		return 1;
	}
	else{
		return 0;
	}
}

void all_sums(unsigned int n,u64 *a, u64 solucao, int *chave)
{ // 1 <= n <= 30
	unsigned int i,mask;
	double s;
	assert(n <= 60);
	mask = 0;
	do{
		s = 0.0;
		for(i = 0;i < n;i++){
			if(((mask >> i) & 1) != 0){
				s += a[i];
				chave[i]=1;
			}
			else{
				chave[i]=0;
			}
		}

		if(s==solucao){
			break;		
		}

		mask++;
	}while(mask < (1 << n));
}


u64 branch_and_bound(u64 goal,u64 mask, u64* a, int n)
{
	if(goal==0ull)return mask;
	if(n==0)return illegal_mask;
	u64 m=branch_and_bound(goal, mask, a, n-1);
	if(m!=illegal_mask)return m;
	if(goal>=a[n-1])
		return branch_and_bound(goal-a[n-1], mask|(1ull<<(n-1)), a, n-1);
	else
		return illegal_mask;
}

void pre_meet_in_the_middle( u64 tudo, u64 metade ,const u64* a, u64** parte1, u64 **parte2){
	for(u64 k=0; k<(u64)pow(2,(tudo-metade)); k++){
		u64 s = 0;
		u64 s1=0;
		u64 mask=0;
		for(u64 i = metade;i<tudo;i++){
			if(((k >> (i-metade)) & 1) == 1){
				s += a[i];
				s1+= a[i-metade];
				mask=mask|(1ull<<(i-metade));
			}
		}
		parte2[k][0]=s;
		parte2[k][1]=mask;
		if(k<(u64)pow(2, metade)){
			parte1[k][0]=s1;
			parte1[k][1]=mask;
		}
	}
}



u64 meet_in_the_middle(u64 tudo, u64 metade, u64 goal, u64 **parte1, u64 **parte2){
	u64 mask=0x0000000000000000;
	u64 e=0;
	u64 f=pow(2,(tudo-metade))-1;
	while(parte1[e][0]+parte2[f][0]!=goal){
		if(parte1[e][0]+parte2[f][0]>goal){
			f--;
		}
		else{
			e++;
		}
	}
	mask=parte1[e][1] | (parte2[f][1]<<metade);
	return mask;
}



double elapsed_time(void)
{
  static struct timespec last_time,current_time;

  last_time = current_time;
  if(clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&current_time) != 0)  // the first argument could also be CLOCK_REALTIME
    return -1.0; // clock_gettime() failed!!!
  return ((double)current_time.tv_sec - (double)last_time.tv_sec) +
         1.0e-9 * ((double)current_time.tv_nsec - (double)last_time.tv_nsec);
}


//
// solve all instances for a given n
//

void solve_using_xpto_strategy(subset_sum_problem_t *problem)
{
  int b[max_n]; // the solution
  double dt;
  int i,j;
  u64 sum;



  
  printf("solutions for n = %d\n",problem->n);
  //
  // pre-processing (optional)
  //
  




  //ativar caso se queira processar segundo a método meet in the middle




  /*
  (void)elapsed_time();
  

  u64 tudo=problem->n;
  u64 metade=round(problem->n/2);
  u64 metade2=tudo-metade;
  u64 **parte2=(u64**)malloc(pow(2,metade2)*sizeof(u64*));
  for(u64 init=0; init<pow(2,metade2); init++){
  	parte2[init]=(u64*)malloc(2*sizeof(u64));
  }
  u64 **parte1=(u64**)malloc(pow(2,metade)*sizeof(u64*));
  for(u64 init=0; init<pow(2,metade); init++){
  	parte1[init]=(u64*)malloc(2*sizeof(u64));
  }
  pre_meet_in_the_middle(tudo, metade, problem->a, parte1, parte2);
  qsort(parte2, (u64)pow(2, metade2), sizeof(u64*), compara);
  qsort(parte1, (u64)pow(2, metade), sizeof(u64*), compara);
  dt = elapsed_time();
  printf("  preprocessing done in %.03f\n",dt);
  //...
  
  */



  //
  // Solve all problems for the value of n; place the solution in the array b[i] 
  //
  for(i = 0;i < n_sums;i++)
  {
    (void)elapsed_time();
    


    //all sums
    all_sums(problem->n, problem->a, problem->sums[i], b);

    //branch and bound
    //for(int k=0; k<max_n; k++)b[k]=0;
    //u64 mask = branch_and_bound(problem->sums[i], 0x0000000000000000, problem->a, problem->n);
	//u64tobin(b, mask, problem->n);

	//meet_in_the_middle
	//for(u64 k=0; k<max_n; k++)b[k]=0;
    //u64 mask=meet_in_the_middle(tudo, metade, problem->sums[i], parte1, parte2);
    //u64tobin(b, mask, problem->n);



    dt = elapsed_time();
    sum = 0ull;
    for(j = 0;j < problem->n;j++)
      	if(b[j] != 0)
        	sum += problem->a[j];
    assert(sum == problem->sums[i]);
    printf("  solved [");
    fprintf(stderr,"%d \r",problem->n);
    for(j = problem->n - 1;j >= 0;j--)
      	printf("%d",b[j]);
    printf("] in %.03f\n",dt);
  }

  //
  // cleanup code (optional)
  //




  //ativar caso se queira processar usando o método meet in the middle


  //free(parte1);
  //free(parte2);



}


//
// main program
//

int main(void)
{
  int i;

  for(i = 0;i < n_problems;i++){
    solve_using_xpto_strategy(&subset_sum_problems[i]);
    printf("\n");
  }
  return 0;
}
