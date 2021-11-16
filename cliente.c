
#include "comuns.h"
#include <signal.h>


int main(int argc, char **argv){

int b_fifo_fd;
int p_fifo_fd;
int m_fifo_fd;
utent_t utente;
balcao_t balcao;
medic_t  medic;
char p_fifo_fname[25];
char m_fifo_fname[25];
int read_res;
//int flag = 0;

 while(argc < 2){
   fprintf(stdout,"Faltam parametros!!!.\n Ex: ./utente <nome utente>\n");
   exit(EXIT_FAILURE);
 }

//Cria o FIFO do utente
strcpy(utente.nome,argv[1]);
utente.pid_utent = getpid();
sprintf(p_fifo_fname, getenv('CLIENT_FIFO'), utente.pid_utent);

if(mkfifo(p_fifo_fname, 0777) == -1){
perror("\nmkfifo do FIFO utente deu erro");
exit(EXIT_FAILURE);
}

fprintf(stderr,"\nFIFO do utente criado");

b_fifo_fd = open(getenv('BALC_FIFO'), O_WRONLY);
if(b_fifo_fd == -1){
 fprintf(stderr, "\nO Balcao não esta a correr\n");
 unlink(p_fifo_fname);
 exit(EXIT_FAILURE);
}
 fprintf(stderr,"\nFIFO do Balcao aberto WRITE / BLOCKING");
 //

 p_fifo_fd = open(p_fifo_fname, O_RDWR);
 if(p_fifo_fd == -1){
 perror("\nErro ao abrir o FIFO do utente");
 close(p_fifo_fd);
 unlink(p_fifo_fname);
 exit(EXIT_FAILURE);
 }


 fprintf(stderr, "\nFIFO do utente aberto para READ(+Write) Block");

 memset(utente.palavra, '\0', TAM_MAX);

 while(1){

printf("\nSr/a %s digite os sintomas: >",argv[1]);
scanf("%s",utente.palavra);
//fgets(utente.palavra,50,stdin);
if(!strcasecmp(utente.palavra,"fim") || !strcasecmp(utente.palavra,"fimb")){
write(b_fifo_fd, &utente, sizeof(utente));
break;
}
// B) Envia ao balcao
write(b_fifo_fd, &utente, sizeof(utente));

// c) recebe do balcao//////////////////////////////////

read_res = read(p_fifo_fd, &balcao, sizeof(balcao));
if(read_res == sizeof(balcao)){
  printf("\nNome -> %s Sintoma -> %s PID -> %d" , balcao.pnome, balcao.palavra,balcao.pid);
   //flag = 1;
}else{
  printf("\nSem resposta do balcao" "[bytes lidos: %d]", read_res);
}

};

   while(1){
    read_res = read(p_fifo_fd, &medic, sizeof(medic));

     if(!strcasecmp(utente.palavra,"adeus")){
    break;
    }

    if(read_res == sizeof(medic))
      printf("\nMensagem Dr/a %s %s %s: \n", medic.mnome, medic.espec, medic.pr);
    else
      printf("\nSem resposta do balcao");

    sprintf(m_fifo_fname, getenv('CLIENT_FIFO'), medic.pid_medic);

    m_fifo_fd = open(m_fifo_fname, O_WRONLY);
    if(m_fifo_fd == -1){
    perror("\nErro ao abrir o FIFO do medico");
    close(m_fifo_fd);
    unlink(m_fifo_fname);
    exit(EXIT_FAILURE);
  }
    scanf("%[^\n]",utente.palavra);
    write(m_fifo_fd, &utente, sizeof(utente));
    if(!strcasecmp(utente.palavra,"adeus")){
      break;
    }

  }

close(p_fifo_fd);
close(b_fifo_fd);
unlink(p_fifo_fname);
return 0;

}
