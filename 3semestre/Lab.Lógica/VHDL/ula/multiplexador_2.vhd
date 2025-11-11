-- Inicialização do programa, chamando a biblioteca principal
library ieee; 
use IEEE.STD_LOGIC_1164.all;

entity multiplexador_2 is
	port (d0, d1, d2, d3: in STD_LOGIC_VECTOR(3 downto 0);
	                  s : in STD_LOGIC_VECTOR(1 downto 0);
	                  y : out STD_LOGIC_VECTOR(3 downto 0));
end multiplexador_2;

architecture seletor of multiplexador_2 is
	begin
   y <= d0 when s = "00" else
   d1 when s = "01" else
   d2 when s = "10" else
   d3;
end seletor;