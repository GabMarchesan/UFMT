ola:
	@echo "Olá mundo"

arquivo1: arquivo2
	@echo "Segunda regra a ser executada"
	@touch arquivo1

arquivo2:
	@echo "Primeira regra a ser executada"
	@touch arquivo2

secreto:
	touch secreto

arquivos := arquivo1 arquivo2

arq0: $(arquivos)
	@echo "Arquivos criados"
	@touch arq0

all: um dois tres

um: 
	touch um

dois:
	touch dois

tres:
	touch tres

f: f1 f2

f1 f2:
	@echo $@
#f1:
#	echo f1
#f2:
#	echo f2

olamundo:
	riscv64-linux-gnu-as olamundo.s -o olamundo.o
	riscv64-linux-gnu-ld olamundo.o -o olamundo.x

clean:
	rm -rf arquivo1 arquivo2 secreto