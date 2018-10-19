#include <stdio.h>
#include <stdlib.h>
#include <math.h> //para a utilização da função floor para o arredondamento para baixo do numero de quadro de cada processo

#include "memvirt.h"

typedef struct pagina{
    int numPag;
    char bitReferencia;
    struct pagina *prox,*ant;
}Pagina;

typedef struct fila{
   int tamanhoFila, numElem;
   Pagina *primeiro,*ultimo;
}filaCircular;

typedef filaCircular *Processo;
typedef struct result Resultados;


Resultados *inicializaResultados(int numProcs);
filaCircular *inicializaFila(int tam);
int buscaPaginaNaFila(filaCircular *fila, int pagina);
void inserePaginaNaFila(filaCircular *fila, int pagina);
struct result * memvirt(int num_procs, uint32_t num_frames, char * filename);


struct result * memvirt(int num_procs, uint32_t num_frames, char * filename){
      FILE *arquivo;
      Resultados *estatisticas;
      Processo *processo;
      int i, pid = 0, pagAcessada = 0, contRef = 0;
      float contPageFault = 0.0;

      /* Retorna null caso o numero de processos ou de frame for zero ou caso o numero de processos for maior do que de frames*/
      if(num_procs == 0 || num_frames == 0 || ((uint32_t)num_procs > num_frames)){ 
    	   return NULL;
      } 

      processo = malloc(sizeof(Processo *) * num_procs);  //aloca espaco de memoria necessario para o processo
      if(processo==NULL){ 
    	   exit(1);
      }

      for(i = 0; i < num_procs; ++i){
   	    	processo[i] = inicializaFila(floor(num_frames/num_procs)); //cada processo eh inicializado com a mesma quantidade de frames, esta quantidade eh arredondada para baixo
      } 

      estatisticas = inicializaResultados(num_procs); //inicializa variaveis e aloca ponteiros da estrutura result

      arquivo = fopen(filename, "rt"); //abre arquivo texto para leitura
      if(arquivo == NULL){ // retorna null caso ocorra algum erro na abertura do arquivo 
   	     return NULL;
      }
 
      /* Realiza a leitura do arquivo de traço enquanto for diferente de final de arquivo */
      while((fscanf(arquivo, "%d %d", &pid,&pagAcessada))!=EOF){
   	     estatisticas->refs[pid]++; //incrementa o numero de referencia a pagina pelo processo
   	     if(buscaPaginaNaFila(processo[pid],pagAcessada)==1){ //verifica se a pagina nao foi encontrada na fila circular
            estatisticas->pfs[pid]++;  //incrementa o numero de falha de pagina do processo
            inserePaginaNaFila(processo[pid],pagAcessada); //realiza a inserção da pagina na fila
   		   }
      }

      fclose(arquivo); //fecha o arquivo
      
      for(i = 0; i < num_procs; ++i){
        /* calcula a taxa de page fault de cada processo, dividindo o numero de page fault do processo pelo numero de referencia a pagina obtido pelo processo*/
         estatisticas->pf_rate[i] = ((float)estatisticas->pfs[i] / estatisticas->refs[i]); 
         contPageFault = contPageFault + estatisticas->pfs[i];  //contabiliza a quantidade total de page faults
         contRef = contRef + estatisticas->refs[i];   //contabiliza a quantidade total de referencias a pagina
      }  
         /* calcula a taxa de page fault de toda simulação, dividindo o numero total de page faults de todos os processos pelo numero total de referencias */
         estatisticas->total_pf_rate = (contPageFault/contRef);

      free(processo); //libera o espaco de memoria alocado para o processo
      return estatisticas; //retorna estatisticas da simulacao
}


/*  
   Função que inicializa as variaveis e aloca espaço de memoria para os ponteiros da estrutura result. 
*/
Resultados *inicializaResultados(int numProcs){
	  Resultados *pagina;
    int i;

  	pagina = (Resultados *)malloc(sizeof(Resultados));
	  if(pagina==NULL){
		    exit(1);
	  }

	  pagina->refs = (uint32_t *) malloc(sizeof(uint32_t) * numProcs);
	  if(pagina->refs==NULL){
		  exit(1);
	  }

	  pagina->pfs = (uint32_t *) malloc(sizeof(uint32_t) * numProcs);
	  if(pagina->pfs==NULL){
		  exit(1);
	  }

	  pagina->pf_rate = (float *) malloc(sizeof(float) * numProcs);
	  if(pagina->pf_rate==NULL){
		  exit(1);
	  }

	 for(i = 0; i < numProcs; ++i){
	   	pagina->refs[i] = 0;
		  pagina->pfs[i] = 0;
		  pagina->pf_rate[i] = 0.0;
   }

     pagina->total_pf_rate = 0.0;

     return pagina;
}

/*  
   Função que aloca espaço de memoria para a criação da fila circular
   e inicializa suas variaveis. 
*/
filaCircular* inicializaFila(int tam){
    filaCircular *fila = malloc(sizeof(filaCircular));
    if(fila==NULL){
   	  	return NULL;
    }

    fila->numElem = 0;
    fila->tamanhoFila = tam;
    fila->primeiro = NULL;
    fila->ultimo = NULL;

    return fila;
}


/*
   Função que busca uma pagina na fila circular e
   altera seu bit de referencia caso a tenha encontrado
*/
int buscaPaginaNaFila(filaCircular *fila, int pagina){
	 Pagina *aux;
	   if(fila->primeiro == NULL && fila->numElem == 0){ //verifica se a fila esta vazia 
		    return 1; 
	   }else{
       aux = fila->primeiro;
       /* Percorre a fila circular em busca da pagina enquanto 
       a fila não estiver novamente em seu elemento inicial */
  	   do{
         if(aux->numPag == pagina){ //verifica se a pagina foi encontrada, se for seta o bit de referencia da pagina para 1
            aux->bitReferencia = 1;
            return 0;
         }
         aux = aux->prox;
  	   }while(aux != fila->primeiro);      
     }
    return 1; // caso nao tenha encontrado a pagina
}

/*
   Função que realiza a inserção da pagina na fila circular
   e modifica seu bit de referencia, caso seja necessario.
*/
void inserePaginaNaFila(filaCircular *fila, int pagina){
     Pagina *novaPag;
     if(fila->primeiro==NULL){ //verifica se a fila estar vazia, caso esteja insere a primeira pagina na fila circular
        novaPag = (Pagina *)malloc(sizeof(Pagina)); //aloca espaco de memoria para a pagina
        if(novaPag==NULL){ 
        	exit(1);
        }
        novaPag->numPag = pagina;
        novaPag->bitReferencia = 1; //seta bit de referencia primeiro elemento inserido na fila para 1
        novaPag->prox = novaPag;
        novaPag->ant = novaPag;
        /* como eh uma fila circular realiza a insercao da primeira pagina na primeira e na ultima posicao da fila */
        fila->ultimo = novaPag;
        fila->primeiro = novaPag;
        fila->numElem++; //incrementa numero de elementos na fila
     }else{
        if(fila->numElem < fila->tamanhoFila){  //verifica se a quantidade de paginas na fila eh menor do que o numero de frames
        	novaPag = (Pagina *)malloc(sizeof(Pagina));
        	if(novaPag==NULL){
        		exit(1);
        	}
        	novaPag->numPag = pagina;
        	novaPag->bitReferencia = 1; //seta bit de referencia da pagina a ser inserida para 1
          if(fila->primeiro->prox==NULL){ //verifica se a segunda posicao da fila esta vazia
              novaPag->prox = novaPag; //insere a pagina 
          }else{
              novaPag->prox = fila->ultimo; 
          }
          if(fila->primeiro->ant==NULL){ //verifica se ultima posicao da fila esta vazia
              novaPag->ant = novaPag; //insere a pagina 
          }else{
              novaPag->ant = fila->ultimo->ant;
          }
          fila->ultimo->ant->prox = novaPag; 
          fila->ultimo->ant = novaPag;
        	fila->numElem++; //incrementa numero de elementos na fila
        }else{  //quantidade de paginas maior do que numero de quadros
        	while(fila->ultimo->bitReferencia == 1){   //Enquanto o bit de referencia da pagina for 1, eh dado uma segunda chance a pagina
        		fila->ultimo->bitReferencia = 0; //bit eh zerado quando pagina obtém uma segunda chance
        		fila->ultimo = fila->ultimo->prox;  //move ponteiro para proxima pagina
        	}
        	fila->ultimo->numPag = pagina; // insere a pagina na ultima posicao da fila circular 
        	fila->ultimo->bitReferencia = 1;  //seta bit de referencia da pagina inserida para 1
        	fila->ultimo = fila->ultimo->prox; //move ponteiro para proxima pagina
        }
     }
}

