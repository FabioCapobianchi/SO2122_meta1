#export BALC_FIFO='balc_fifo'
#export CLIENT_	FIFO='clinte_%d_fifo'

all:
	gcc -g -Wall -o balcao balcao.c
	gcc -g -Wall -o cliente cliente.c
	gcc -g -Wall -o medico medico.c
balcao:
	gcc -c balcao.c
cliente:
	gcc -c cliente.c
medico:
	gcc -c medico.c
comuns:
	gcc -c comuns.c
clean:
	$(RM)balcao balcao.o
	$(RM)cliente cliente.o
	$(RM)medico medico.o
	$(RM)comuns comuns.o
