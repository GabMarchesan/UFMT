library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity RippleCarryAdder is
    Port ( A : in  STD_LOGIC_VECTOR(3 downto 0);  -- Entrada A de 4 bits
           B : in  STD_LOGIC_VECTOR(3 downto 0);  -- Entrada B de 4 bits
			  CLK : in  STD_LOGIC;       -- Clock input
           SUM : out  STD_LOGIC_VECTOR(3 downto 0); -- Saída da soma de 4 bits
           CARRY_OUT : out  STD_LOGIC);            -- Saída de carry-out
end RippleCarryAdder;

architecture synth of RippleCarryAdder is
    component FullAdder is  -- Componente Full Adder
        Port ( A : in  STD_LOGIC;
               B : in  STD_LOGIC;
               CIN : in  STD_LOGIC;
               SUM : out  STD_LOGIC;
               COUT : out  STD_LOGIC);
    end component;

    signal carry : STD_LOGIC_VECTOR(3 downto 0);  -- Vetor de carry interno
begin
    -- Instâncias do Full Adder para cada bit
    FA0: FullAdder port map (A(0), B(0), '0', SUM(0), carry(0)); -- O carry-in do primeiro bit é '0'
    FA1: FullAdder port map (A(1), B(1), carry(0), SUM(1), carry(1)); -- O carry-in do segundo bit é o carry-out do primeiro
    FA2: FullAdder port map (A(2), B(2), carry(1), SUM(2), carry(2)); -- O carry-in do terceiro bit é o carry-out do segundo
    FA3: FullAdder port map (A(3), B(3), carry(2), SUM(3), CARRY_OUT); -- O carry-in do quarto bit é o carry-out do terceiro
end synth;