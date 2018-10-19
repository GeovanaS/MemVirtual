# MemVirtual

Programa em C que simula o funcionamento do gerenciamento de memória virtual em um sistema operacional.

Cada processo começa com o mesmo número de quadros (total de quadros dividido número de
processos arredondado para baixo).

O algoritmo para escolha de página vítima é o algoritmo de segunda chance (ou do relógio). Para cada
processo, o algoritmo começa sempre pela primeira página acessada pelo processo. Novas páginas que
não estavam em memória são incluídas na lista de páginas sempre antes da próxima página a ser
avaliada.

    ​Não esqueçam de manter um bit de referência para cada página.

A entrada do simulador consistirá de:

    ● Número de processos
    ● Número total de quadros (​frames​)
    ● Um arquivo de traço com as páginas que são referenciadas por cada processo no formato "PID PAGE\n" (uma referência por linha)
    
Um exemplo de arquivo de traço é:
      
     1 1
     1 5
     2 4
     3 1
     
Onde o processo ​1​ acessa as páginas ​1​ e ​5​, o processo ​2​ acessa a página ​4​ e o processo ​3​ acessa sua
página ​1​. O PID começa em ​0​ e termina no número de processos ​N-1​ especificado na entrada.


A saída do simulador conterá:

    ● Número de referências por processo, em ordem crescente de PID
    ● Número de ​page faults​ por processo, em ordem crescente de PID
    ● Taxa de ​page faults​ por processo, em ordem crescente de PID
    ● Taxa de ​page faults​ de toda a simulação
    
# Detalhamento do algoritmo do relógio

Cada nova página inserida na tabela de páginas de um processo vai sempre ser colocada na lista do
algoritmo de vítima na última posição do ​buffer​ circular, com o bit de referência ligado.
    
    
