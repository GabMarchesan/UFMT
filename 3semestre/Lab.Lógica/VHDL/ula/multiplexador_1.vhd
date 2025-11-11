-- Universidade Federal de Mato Grosso
-- Disciplina de Laboratorio de Logica Digital
-- Professor: Linder Candido ; Turma: CC1
-- Alunos: Raissa Cavalcanti e Gabriel Marchesan

-- Inicialização do programa, chamando a biblioteca principal
Library IEEE; 
Use IEEE.STD_LOGIC_1164.ALL;

-- Inicialização da interface do módulo 
entity multiplexador_1 is 
	port(A, B: in STD_LOGIC_VECTOR (3 downto 0);
		  S: in STD_LOGIC;
		  saida: out STD_LOGIC_VECTOR (3 downto 0));
end multiplexador_1; -- Finalização do Enti

-- Inicialização da Descrição Funcional do módulo	
architecture seletor of multiplexador_1 is
	begin
		saida <= -- A saída depende exclusivamente da entrada do seletor, logo: 
					A when S = '0' else 
					B when S = '1';
 end seletor; -- Finalizando a Descrição