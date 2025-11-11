library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity ula is
    port(
        A, B: in std_logic_vector(3 downto 0);  -- Entradas de dados
        F: in std_logic_vector(1 downto 0);  -- Seletores de operação
        Y: buffer std_logic_vector(3 downto 0);  -- Saída de dados
        Z: out std_logic  -- Saída de sinal de zero
    );
end ula;

architecture synth of ula is
    -- Declaração de componentes reutilizáveis
    component multiplexador_1 is
        port(
            A, B: in STD_LOGIC_VECTOR(3 downto 0);  -- Entradas do multiplexador
            S: in STD_LOGIC;  -- Sinal de seleção
            saida: out STD_LOGIC_VECTOR(3 downto 0)  -- Saída do multiplexador
        );
    end component;

    component carry_adder is
		  port( A, B: in STD_LOGIC_VECTOR(3 downto 0);
		         cin: in STD_LOGIC;
                 s: out STD_LOGIC_VECTOR(3 downto 0);
		        cout: out STD_LOGIC
		  );
    end component;

    component multiplexador_2 is
        port (d0, d1, d2, d3: in STD_LOGIC_VECTOR(3 downto 0);
	                       s : in STD_LOGIC_VECTOR(1 downto 0);
	                       y : out STD_LOGIC_VECTOR(3 downto 0));
    end component;

    -- Sinais internos
    signal o0, o1, o2, o3: STD_LOGIC_VECTOR(3 downto 0);
    signal COUT0: STD_LOGIC;
begin
    -- Mapeamento dos componentes principais
    G1: multiplexador_1 port map(B, NOT B, F(0), o0);
    G2: o1 <= A AND o0;
    G3: carry_adder port map(A, o0, F(0), o2, COUT0);
    G4: multiplexador_2 port map(o1, o0, o2, o2, F, Y);
    G5: o3 <= NOT Y;
    G6: Z <= o3(0) AND o3(1) AND o3(2) AND o3(3);
end synth;