library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity decodificador is
    port(
        E : in std_logic_vector(3 downto 0);
        y : out std_logic_vector(6 downto 0)
    );
end decodificador;

architecture synth of decodificador is
begin
    process(E)
    begin
        case E is
            when "0000" => y <= "1000000"; -- '0'
				when "0001" => y <= "1111001"; -- '1'
				when "0010" => y <= "0100100"; -- '2'
				when "0011" => y <= "0110000"; -- '3'
				when "0100" => y <= "0011001"; -- '4'
				when "0101" => y <= "0010010"; -- '5'
				when "0110" => y <= "0000010"; -- '6'
				when "0111" => y <= "1111000"; -- '7'
				when "1000" => y <= "0000000"; -- '8'
				when "1001" => y <= "0010000"; -- '9'
				when "1010" => y <= "0001000"; -- 'A'
				when "1011" => y <= "0000011"; -- 'B'
				when "1100" => y <= "1000110"; -- 'C'
				when "1101" => y <= "0100001"; -- 'D'
				when "1110" => y <= "0000110"; -- 'E'
				when "1111" => y <= "0001110"; -- 'F'
				when others => y <= "XXXXXXX"; -- quando for outro valor
        end case;
    end process;
end synth;