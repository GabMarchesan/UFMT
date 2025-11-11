library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity FlipFlop_D is
    Port ( D : in  STD_LOGIC;       -- Data input
           CLK : in  STD_LOGIC;     -- Clock input
           Q : out  STD_LOGIC);    -- Output
end FlipFlop_D;

architecture synth of FlipFlop_D is
begin
    process(CLK)
    begin
        if rising_edge(CLK) then
            Q <= D;  -- Q takes on the value of D on the rising edge of the clock
        end if;
    end process;
end synth; 