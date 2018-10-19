#include "memvirt.h"
#include "simplegrade.h"

void liberaResultados(struct result *r);

void teste1(){
	     struct result *r;
     
       WHEN("Tenho 2 processos e 4 frames");
       r = memvirt(2, 4, "alunos/t01.txt");
              
       THEN("Processo 0 deve ser referenciado 7 vezes.");
       isEqual(r->refs[0],7,1);
       THEN("Processo 1 deve ser referenciado 8 vezes.");
       isEqual(r->refs[1],8,1);

   	   THEN("Processo 0 e processo 1 devem ter 3 page fault.");
   	   isEqual(r->pfs[0],3,1);
   	   isEqual(r->pfs[1],3,1);

       THEN("Processo 2 deve ter taxa de page fault de 43%.");
       isEqual(r->pf_rate[0],0.43,1);
       THEN("Processo 3 deve ter taxa de page fault de 38%.");
       isEqual(r->pf_rate[1],0.38,1);

       THEN("Total de falha de pagina deve ser 40%.");
   	   isEqual(r->total_pf_rate, 0.40, 1);

	     liberaResultados(r);
}


void teste2(){
      struct result *r;

      WHEN("Tenho 3 processos e 6 frames");
      r = memvirt(3,6,"alunos/t02.txt");
     
      THEN("Processo 0 deve ser referenciado 6 vezes.");
      isEqual(r->refs[0],6,1);
      THEN("Processo 1 deve ser referenciado 2 vezes.");
      isEqual(r->refs[1],2,1);
      THEN("Processo 2 deve ser referenciado 4 vezes.");
      isEqual(r->refs[2],4,1);
 
      THEN("Processo 0 deve ter 3 page fault.");
      isEqual(r->pfs[0],3,1);
      THEN("Processo 1 deve ter 2 page fault.");
      isEqual(r->pfs[1],2,1);
      THEN("Processo 2 deve ter 4 page fault.");
      isEqual(r->pfs[2],4,1);

      THEN("Processo 0 deve ter taxa de page fault de 50%.");
      isEqual(r->pf_rate[0],0.50,1);
      THEN("P1 e P2 devem ter taxa de page fault de 100%.");
      isEqual(r->pf_rate[1],1.00,1);
      isEqual(r->pf_rate[2],1.00,1);

      THEN("Total de falha de pagina deve ser 75%.");
      isEqual(r->total_pf_rate, 0.75, 1);

      liberaResultados(r);
}


void teste3(){
     struct result *r;

     WHEN("Tenho 4 processos e 12 frames.");
   
     r = memvirt(4,12,"alunos/t03.txt");    

     THEN("Processos devem ser referenciados 9 vezes, no total");
     isEqual(r->refs[0],1,1);
     isEqual(r->refs[1],3,1);
     isEqual(r->refs[2],3,1);
     isEqual(r->refs[3],2,1);

     THEN("P0 deve ter taxa de page fault de 100%.");
     isEqual(r->pf_rate[2],1.0,1);
     THEN("P1 deve ter page fault próximo de 67%.");
     isNear(r->pf_rate[1],0.67,2,1);
     THEN("P2 e P3 devem ter taxa de page fault de 100%.");
     isEqual(r->pf_rate[2],1.0,1);
     isEqual(r->pf_rate[3],1.0,1);

     THEN("Total de page fault deve ser próximo à 89%.");
     isNear(r->total_pf_rate,0.89,2,1);

     liberaResultados(r);
}

/* Função que libera memoria alocada dos ponteiros da estrutura result*/
void liberaResultados(struct result *r){
  free(r->refs);
  free(r->pfs);
  free(r->pf_rate);
  free(r);
}

int main(){
   
   DESCRIBE("Testes do trabalho 2: Simulador de gerenciador de memoria virtual");
 
   teste1();
   teste2();
   teste3();
  
   GRADEME();
   if(grade == maxgrade)
      return 0;
   else
      return grade;
}