library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Register_4bits is
    Port ( D : in  STD_LOGIC_VECTOR(3 downto 0);  -- Entrada de dados de 4 bits
           CLK : in  STD_LOGIC;                   -- Sinal de clock
           Q : out  STD_LOGIC_VECTOR(6 downto 0));-- Saída de dados de 4 bits
end Register_4bits;

architecture synth of Register_4bits is
    component FlipFlop_D is
        Port( D : in  STD_LOGIC;      
              CLK : in  STD_LOGIC;    
              Q : out  STD_LOGIC);   
    end component;

    signal Q_temp : STD_LOGIC_VECTOR(6 downto 0);  -- Sinal temporário para armazenar os valores do registrador
    
begin
    -- Instância dos Flip-Flops D
-- O sinal de entrada 'D' é conectado correspondente da entrada de dados
-- O sinal de clock é fornecido para todos os FFD
-- Os sinais de saída 'Q' sao conectados aos bits correspondetes do sinal temporario	 
    FF0: FlipFlop_D port map (D(0), CLK, Q_temp(0));
    FF1: FlipFlop_D port map (D(1), CLK, Q_temp(1));
    FF2: FlipFlop_D port map (D(2), CLK, Q_temp(2));
    FF3: FlipFlop_D port map (D(3), CLK, Q_temp(3));
    
    -- Processo sensível à borda de subida do clock para atualizar o valor do registrador
    process(CLK)
    begin
        if rising_edge(CLK) then
            Q <= Q_temp;  -- Atualiza a saída Q com o valor armazenado no sinal temporário Q_temp
        end if;
    end process;
end synth;