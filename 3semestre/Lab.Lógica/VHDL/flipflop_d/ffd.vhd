library ieee;
use ieee.std_logic_1164.all;

entity ffd is
	port(clk, reset: in std_logic;
					  d: in std_logic_vector(3 downto 0);
					  q: out std_logic_vector(3 downto 0));   
end ffd;		 

architecture synth of ffd is
begin
	process(clk, reset)
	begin
		if reset = '1' then -- reset ativo em alto
			q <= "0000";
		elsif rising_edge(clk) then -- se acontecer borda de subida/descida, falling_edge(borda descida) rising_edge(borda de subida) 
			q <= d; --o process retem os sinais sem bloqueio, aí ele mantém os sinais 
		end if;
	end process;
end synth;