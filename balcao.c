#include "comuns.h"
#include <sys/wait.h>


int main(int argc, char **argv){

  int bal_to_cla[2];
  int cla_to_bal[2];
  pipe(bal_to_cla);
  pipe(cla_to_bal);
  int id;
  char frase[50];

  printf("digite os sintomas:\n");
  //scanf("%[^\n]",frase);

  id=fork();

  if(id == 0){
    dup(cla_to_bal[1]);
    close(cla_to_bal[1]);
    close(cla_to_bal[0]);

    dup(bal_to_cla[0]);
    close(bal_to_cla[0]);
    close(bal_to_cla[1]);

    execl("classificador", "classificador",NULL);

  }else{

  dup(cla_to_bal[0]);
  close(cla_to_bal[0]);
  close(cla_to_bal[1]);

  dup(bal_to_cla[1]);
  close(bal_to_cla[1]);
  close(bal_to_cla[0]);

  wait(NULL);
}
  

return 0;
}
