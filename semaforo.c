#include<errno.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>

struct sembuf operacao[2];
int idsem;
int p_sem()
{
operacao[0].sem_num = 0;
operacao[0].sem_op = 0;
operacao[0].sem_flg = 0;
operacao[1].sem_num = 0;
operacao[1].sem_op = 1;
operacao[1].sem_flg = 0;
if ( semop(idsem, operacao, 2) < 0)
printf("erro no p=%d\n", errno);
}
int v_sem()
{
operacao[0].sem_num = 0;
operacao[0].sem_op = -1;
operacao[0].sem_flg = 0;
if ( semop(idsem, operacao, 2) < 0)
printf("erro no p=%d\n", errno);
}

int main()
{
int pid, estado;
int *psem;
if ((idsem = semget(0x1223, 1, IPC_CREAT|0x1ff)) < 0)
{ printf("erro na criacao do semaforo\n"); exit(1);}
if ((pid = fork()) == 0)
{ /* codigo do filho */
p_sem();
printf("filho - obtive o semaforo\n");
printf("filho – vou liberar o semaforo\n");
v_sem();
semctl(idsem, 1 , IPC_RMID, 0);
exit(0);
}
/* codigo do pai */
p_sem();
printf("pai - obtive o semaforo\n");
printf("pai – vou liberar o semaforo\n");
v_sem();
//wait(&estado);
exit (0);
}