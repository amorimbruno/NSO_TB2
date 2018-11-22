#include<errno.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<stdlib.h>

int main()
{
   int pid, idfila, fd[2], estado;
   struct mensagem
   {
      long pid;
      char msg[30];
   };
 
   struct mensagem mensagem_env, mensagem_rec; 

   /* cria */
   if ((idfila = msgget(0x1223, IPC_CREAT|0x1B6)) < 0)
   {
     printf("erro na criacao da fila\n");
     exit(1);
   }

   pid = fork();

   if (pid == 0)
   {
   mensagem_env.pid = getpid();
   strcpy(mensagem_env.msg, "teste de mensagem");
   msgsnd(idfila, &mensagem_env, sizeof(mensagem_env)-sizeof(long), 0);
   sleep(20);
   exit (0);
   }

   sleep(20);
   msgrcv(idfila, &mensagem_rec, sizeof(mensagem_rec)-sizeof(long), 0, 0);
   printf("mensagem recebida = %d %s\n", mensagem_rec.pid, mensagem_rec.msg);
   wait (&estado);
   exit(0);
}
     
//ipcs -a
//./progexms &
//ipcrm -q ID