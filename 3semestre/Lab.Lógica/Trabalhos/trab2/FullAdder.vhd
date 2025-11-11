library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity FullAdder is
    Port ( A : in  STD_LOGIC;       -- Bit de entrada A
           B : in  STD_LOGIC;       -- Bit de entrada B
           CIN : in  STD_LOGIC;     -- Carry-in
           SUM : out  STD_LOGIC;    -- Soma
           COUT : out  STD_LOGIC);  -- Carry-out
end FullAdder;

architecture synth of FullAdder is
begin
    process(A, B, CIN)
    begin
        SUM <= A xor B xor CIN;    -- Soma dos bits de entrada usando XOR
        COUT <= (A and B) or (B and CIN) or (A and CIN);  -- Cálculo do carry-out
    end process;
end synth;