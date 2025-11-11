library ieee;
use ieee.std_logic_1164.all;

entity dec_2_4 is
	port(A: in std_logic_vector(1 downto 0);
		  F: out std_logic_vector(3 downto 0));
end dec_2_4;

architecture synth of dec_2_4 is
begin
	process(A) --all usa todas as entradas, ou pode por só A
	begin
		case A is
			when "00" => F <= "0001";
			when "01" => F <= "0010";
			when "10" => F <= "0100";
			when "11" => F <= "1000";
		end case;
	end process;
end synth;