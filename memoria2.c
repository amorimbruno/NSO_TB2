#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>

struct sembuf operacao[2];
int idsem;
int i;

void p_sem();
void v_sem();
void p_sem2();
void v_sem2();



void p_sem2 ()
{
  operacao[0].sem_num = 1;
  operacao[0].sem_op = 0;
  operacao[0].sem_flg = 0;
  operacao[1].sem_num = 1;
  operacao[1].sem_op = 1;
  operacao[1].sem_flg = 0;
  if (semop (idsem, operacao, 2) < 0)
    printf ("erro P2 no p=%d\n", errno);
}

void v_sem2 ()
{
  operacao[0].sem_num = 1;
  operacao[0].sem_op =  0;
  operacao[0].sem_flg = 1;
  if (semop (idsem, operacao, 2) < 0)
    printf ("erro V2 no p=%d\n", errno);
}

void p_sem1 ()
{
  operacao[0].sem_num = 1;
  operacao[0].sem_op = 0;
  operacao[0].sem_flg = 0;
  operacao[1].sem_num = 1;
  operacao[1].sem_op = 1;
  operacao[1].sem_flg = 0;
  if (semop (idsem, operacao, 2) < 0)
    printf ("erro P1 no p=%d\n", errno);
   // printf("idsem: %d\n", idsem);
}

void v_sem1 ()
{
  operacao[0].sem_num = 1;
  operacao[0].sem_op = -1;
  operacao[0].sem_flg = 0;
  if (semop (idsem, operacao, 2) < 0)
    printf ("erro V1 no p=%d\n", errno);
}

int main ()
{

  int pid, estado, idshm;
  struct shmid_ds buf;
  int *pshm;
  int *psem;

/* cria memoria */
if ((idshm = shmget(0x1223, sizeof(int),IPC_CREAT|0x1ff)) < 0)
  { 
    printf("erro na criação da fila\n"); 
    exit(1);
  }

/* cria semaforo */
if ((idsem = semget (0x1111, 1, IPC_CREAT | 0x1ff)) < 0)
  {
    printf ("erro na criacao do semaforo\n");
    exit (1);
  }

/* codigo do filho */
if ((pid = fork ()) == 0)
  {
    for(i = 0 ; i < 10 ; i++)
	{
      pshm = (int *) shmat(idshm, (char *)0, 0);
	  if (pshm == (int *)-1)
	  {
		printf("erro no attach\n"); 
        exit(1);
	  }
	  p_sem1 ();
      printf("filho - obtive o semaforo\n");
      printf("filho - recebi do pai %d\n", *pshm);
      //printf("filho - vou liberar o semaforo\n");
	  exit(1);
      v_sem2 ();
	  
    }
  }	

/* codigo do pai */
for(i = 0 ; i < 10 ; i++)
  {
	pshm = (int *) shmat(idshm, (char *)0, 0);  
	if (pshm == (int *) -1)
	{
      printf("erro no attach\n"); 
      exit(1);
	}	
    *pshm = 100 + i;
    printf("pai - registrei um novo número\n");  
    printf("escrevi %d para o filho\n", *pshm);
    //printf("pai - vou liberar o semaforo\n");
    v_sem1();
    wait(&estado);
    p_sem2();
  }


/*
if(Condição){
destroi_semaforo(idsem);
}
*/

exit (0);
}


void destroi_semaforo(int id){
  if (semctl(idsem,0,IPC_RMID,0)!=-1) {
  printf("O semaforo com semid %d foi destruido\n",idsem);
             }
      else {
        perror("Impossivel de destruir o semaforo");
             exit(1); 
             }
}