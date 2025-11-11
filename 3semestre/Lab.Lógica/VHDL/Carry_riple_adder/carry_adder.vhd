library IEEE; 
use IEEE.STD_LOGIC_1164.all;
entity carry_adder is
  port(a, b, c, d, e, f, g, h: in  STD_LOGIC;
       s1, s2, s3, s4, over:   out STD_LOGIC);
end;
architecture synth of carry_adder is
	component fulladder is
		port(a, b, cin: in  STD_LOGIC;
			s, cout:   out STD_LOGIC);
	end component;
  signal cout1, cout2, cout3: STD_LOGIC;
begin
	AD1: fulladder port map(
	a => a, b => b, cin => '0',
	s => s1, cout => cout1
	);
	
	AD2: fulladder port map(
	a => c, b => d, cin => cout1,
	s => s2, cout => cout2
	);
	
	AD3: fulladder port map(
	a => e, b => f, cin => cout2,
	s => s3, cout => cout3
	);
	
	AD4: fulladder port map(
	a => g, b => h, cin => cout3,
	s => s4, cout => over
	);
end synth;