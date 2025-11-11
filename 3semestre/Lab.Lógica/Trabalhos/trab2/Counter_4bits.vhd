library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Counter_4bits is -- Interface do componente, especificando as entradas e saídas
    Port ( CLK : in  STD_LOGIC;-- Sinal de clock que controla o funcionamento do contador
           RESET : in  STD_LOGIC; -- Sinal de reset para reiniciar o contador
           COUNT_OUT : out  STD_LOGIC_VECTOR(6 downto 0)); -- Saída do contador (número de 4 bits)
end Counter_4bits;


architecture synth of Counter_4bits is -- Descreve o comportamento interno do contador
    signal count_value : STD_LOGIC_VECTOR(3 downto 0);  -- Valor corrente do contador

    component RippleCarryAdder is -- Somador para incrementar o valor do contador
        Port ( A : in  STD_LOGIC_VECTOR(3 downto 0);
               B : in  STD_LOGIC_VECTOR(3 downto 0);
               CLK : in  STD_LOGIC;
               SUM : out  STD_LOGIC_VECTOR(3 downto 0);
               CARRY_OUT : out  STD_LOGIC);
    end component;

    component Register_4bits is -- - Registrador para armazenar o valor corrente do contador
        Port ( D : in  STD_LOGIC_VECTOR(3 downto 0);
               CLK : in  STD_LOGIC;
               Q : out  STD_LOGIC_VECTOR(6 downto 0));
    end component;
	 
	 component decodificador is
        port(
			    E : in std_logic_vector(3 downto 0);  -- Deixe a porta E como 4 bits
			    y : out std_logic_vector(6 downto 0));
    end component;

    signal incremented_value : STD_LOGIC_VECTOR(3 downto 0); -- Armazena o valor incrementado
	 signal decoded_value : std_logic_vector(6 downto 0);

begin
    -- Registrador para armazenar o valor corrente do contador
    REG: Register_4bits port map (count_value, CLK, COUNT_OUT);
	 -- Count_value = Dados a serem armazenados no registrador
	 -- CLK = Sinal de clock que controla o funcionamento do registrador
	 -- Count_out = Representa os dados armazenados no registrador. Estamos conectando este sinal a saída do contador

  
    ADD: RippleCarryAdder port map (count_value, "0001", CLK, incremented_value, open);
	 -- Count_Value = Representa os valores de entrada 'a' || valor atual do contador
	 -- "0001" = Representa o valor a ser somado no contador.
	 -- CLK = Controla o funcionamento do somador ripple carry
	 -- Incremented_value = Representa a saída do somador -> valor do contador pós incremento
	 -- Open = Indica que a porta Carry_out do somador não está sendo conectada a nenhum sinal nesta instância

	 -- Estamos instanciando o somador e mapeando os sinais de entrada e saída para realizar a operação de incremento do contador
	 DEC: decodificador port map (E => count_value(3 downto 0), y => decoded_value);

end synth;