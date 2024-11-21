
# SCHED-PROFILER

## Descrição do Programa

Este programa é uma aplicação multithread em C que demonstra o uso de mutexes, barreiras e políticas de escalonamento em threads e tem como objetivo compreender as características de escalonamento de processos no Linux e avaliar o impacto de diferentes políticas de escalonamento no desempenho da aplicação. Ele utiliza múltiplas threads para preencher um buffer global compartilhado com caracteres específicos atribuídos a cada thread. O comportamento das threads é controlado por diferentes políticas de escalonamento definidas pelo usuário.

Após preencher o buffer, o programa realiza a exibição do conteúdo do buffer, tanto na forma original quanto em uma versão pós-processada que destaca caracteres únicos e suas contagens. Além disso, também apresenta a contagem de vezes que cada thread foi escalonado para escrita no buffer.
Estrutura do Programa
## Entrada de Parâmetros

O programa requer três parâmetros para execução:

- buffer_size: Tamanho do buffer global que será preenchido.

- num_threads: Número de threads que preencherão o buffer.

- policy: Política de escalonamento a ser aplicada nas threads. 

As opções 
suportadas são:
    SCHED_LOW_IDLE
    SCHED_IDLE
    SCHED_FIFO
    SCHED_RR

## Componentes Principais

Buffer Global: Um vetor de caracteres compartilhado por todos as threads, protegido por um mutex.
Políticas de Escalonamento: O programa ajusta as prioridades de escalonamento das threads com base na política especificada.
Barreira: Sincroniza o início das operações de todos as threads.
Post-Processing: Realiza uma análise do conteúdo do buffer, exibindo caracteres únicos e suas contagens.

## Execs no QEMU
O programa foi cross compiled para ser executado no qemu e aqui seguem os resultados para 2, 4 e 8 threads para cada policy.

```bash
# sched_profiler 100000 2 SCHED_LOW_IDLE
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_LOW_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_LOW_IDLE PRI_MIN: 0 PRI_MAX: 0
Buffer without post-processing:
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBA

Buffer with post-processing:
Unique characters in buffer:
AB
A = 1
B = 1

Thread writings counts:
A: 32319 
B: 67681 
-------------

# sched_profiler 100000 4 SCHED_LOW_IDLE
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_LOW_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_LOW_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_LOW_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_LOW_IDLE PRI_MIN: 0 PRI_MAX: 0
Buffer without post-processing:
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAD

Buffer with post-processing:
Unique characters in buffer:
ABD
A = 1
B = 1
D = 1

Thread writings counts:
A: 41841 
B: 47715 
C: 0 
D: 10444 
-------
# sched_profiler 100000 8 SCHED_LOW_IDLE
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_LOW_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_LOW_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_LOW_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_LOW_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_LOW_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_LOW_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_LOW_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_LOW_IDLE PRI_MIN: 0 PRI_MAX: 0
Buffer without post-processing:
HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHF

Buffer with post-processing:
Unique characters in buffer:
CFH
C = 1
F = 1
H = 1

Thread writings counts:
A: 0 
B: 0 
C: 13758 
D: 0 
E: 0 
F: 77279 
G: 0 
H: 8963 
```

```bash
# sched_profiler 100000 2 SCHED_IDLE
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_IDLE PRI_MIN: 0 PRI_MAX: 0
Buffer without post-processing:
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBA

Buffer with post-processing:
Unique characters in buffer:
AB
A = 1
B = 1

Thread writings counts:
A: 14982 
B: 85018 
-----------

# sched_profiler 100000 4 SCHED_IDLE
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_IDLE PRI_MIN: 0 PRI_MAX: 0
Buffer without post-processing:
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBD

Buffer with post-processing:
Unique characters in buffer:
BD
B = 1
D = 1

Thread writings counts:
A: 0 
B: 59941 
C: 0 
D: 40059 
--------------------

# sched_profiler 100000 8 SCHED_IDLE
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_IDLE PRI_MIN: 0 PRI_MAX: 0
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_IDLE PRI_MIN: 0 PRI_MAX: 0
Buffer without post-processing:
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDC

Buffer with post-processing:
Unique characters in buffer:
CD
C = 1
D = 1

Thread writings counts:
A: 0 
B: 0 
C: 85631 
D: 14369 
E: 0 
F: 0 
G: 0 
H: 0 

```

```bash
# sched_profiler 100000 2 SCHED_FIFO
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_FIFO PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_FIFO PRI_MIN: 1 PRI_MAX: 99
Buffer without post-processing:
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Buffer with post-processing:
Unique characters in buffer:
A
A = 1

Thread writings counts:
A: 100000 
B: 0 
# 
----------

# sched_profiler 100000 4 SCHED_FIFO
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_FIFO PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_FIFO PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_FIFO PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_FIFO PRI_MIN: 1 PRI_MAX: 99
Buffer without post-processing:
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Buffer with post-processing:
Unique characters in buffer:
A
A = 1

Thread writings counts:
A: 100000 
B: 0 
C: 0 
D: 0 
--------
# sched_profiler 100000 8 SCHED_FIFO
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_FIFO PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_FIFO PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_FIFO PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_FIFO PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_FIFO PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_FIFO PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_FIFO PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_FIFO PRI_MIN: 1 PRI_MAX: 99
Buffer without post-processing:
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Buffer with post-processing:
Unique characters in buffer:
A
A = 1

Thread writings counts:
A: 100000 
B: 0 
C: 0 
D: 0 
E: 0 
F: 0 
G: 0 
H: 0 
----------
```

```bash
# sched_profiler 100000 2 SCHED_RR
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_RR PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_RR PRI_MIN: 1 PRI_MAX: 99
Buffer without post-processing:
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Buffer with post-processing:
Unique characters in buffer:
A
A = 1

Thread writings counts:
A: 100000 
B: 0 
# 
-------
# sched_profiler 100000 4 SCHED_RR
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_RR PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_RR PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_RR PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_RR PRI_MIN: 1 PRI_MAX: 99
Buffer without post-processing:
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Buffer with post-processing:
Unique characters in buffer:
A
A = 1

Thread writings counts:
A: 100000 
B: 0 
C: 0 
D: 0 
# 
----------

# sched_profiler 100000 8 SCHED_RR
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_RR PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_RR PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_RR PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_RR PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_RR PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_RR PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_RR PRI_MIN: 1 PRI_MAX: 99
current: unknown
 PRI_MIN: 0 PRI_MAX: 0
new: SCHED_RR PRI_MIN: 1 PRI_MAX: 99
Buffer without post-processing:
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Buffer with post-processing:
Unique characters in buffer:
A
A = 1

Thread writings counts:
A: 100000 
B: 0 
C: 0 
D: 0 
E: 0 
F: 0 
G: 0 
H: 0 

``` 
