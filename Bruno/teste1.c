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
void pai_semaforo(int vet[10]);
void cria_semaforo(int ID);
void filho_semaforo(int pid,int vet[10]);
void destroi_semaforo(int id);



void p_sem2 ()
{
  operacao[0].sem_num = 0;
  operacao[0].sem_op = 0;
  operacao[0].sem_flg = 0;
  operacao[1].sem_num = 0;
  operacao[1].sem_op = 1;
  operacao[1].sem_flg = 0;
  if (semop (idsem, operacao, 2) < 0)
    printf ("erro P2 no p=%d\n", errno);
}

void v_sem2 ()
{
  operacao[0].sem_num = 0;
  operacao[0].sem_op = -1;
  operacao[0].sem_flg = 0;
  if (semop (idsem, operacao, 2) < 0)
    printf ("erro V2 no p=%d\n", errno);
}

void p_sem ()
{
  operacao[0].sem_num = 0;
  operacao[0].sem_op = 0;
  operacao[0].sem_flg = 0;
  operacao[1].sem_num = 0;
  operacao[1].sem_op = 1;
  operacao[1].sem_flg = 0;
  if (semop (idsem, operacao, 2) < 0)
    printf ("erro P no p=%d\n", errno);
   // printf("idsem: %d\n", idsem);
}

void v_sem ()
{
  operacao[0].sem_num = 0;
  operacao[0].sem_op = -1;
  operacao[0].sem_flg = 0;
  if (semop (idsem, operacao, 2) < 0)
    printf ("erro V no p=%d\n", errno);
}

int main ()
{

  int vet[10];
  int pid, estado;
  int *psem;

cria_semaforo(idsem);
filho_semaforo(pid,vet);
pai_semaforo(vet);


/*
if(Condição){
destroi_semaforo(idsem);
}
*/

exit (0);
}




void cria_semaforo(int ID){
  if ((idsem = semget (0x1111, 1, IPC_CREAT | 0x1ff)) < 0)
    {
      printf ("erro na criacao do semaforo\n");
      exit (1);
    }
}

void pai_semaforo(int vet[10]) {
  int a = 10;
  int i ;
  int estado = -1;

  for(i = 0 ; i < 10 ; i++){
  vet[i] = i + 1;
  printf ("pai - registrei um novo número\n");  
  v_sem();
  printf("escrevi %d no filho\n", vet[i]);
  printf ("pai - vou liberar o semaforo\n");
  wait(&estado);
  p_sem2();

  }
  

}

void filho_semaforo(int pid, int vet[10]){
   int i ;
  if ((pid = fork ()) == 0) {
      for(i = 0 ; i < 10 ; i++){
      p_sem ();
      printf ("filho - obtive o semaforo\n");
      printf("filho -  recebi do pai %d\n,", vet[i]);
      printf ("filho - vou liberar o semaforo\n");
      v_sem2 ();
      }
    }
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