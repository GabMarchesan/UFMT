library ieee;
use ieee.std_logic_1164.all;

entity flipflop_jk is 
    port(
        clk, reset, j, k: in std_logic;
        q: out std_logic_vector(3 downto 0)  
    );
end flipflop_jk;

architecture synth of flipflop_jk is
    --Declaração de componentes reutilizáveis
    component ffd is
        port(
            clk: in std_logic;
            d: in std_logic_vector(3 downto 0);
            q: out std_logic_vector(3 downto 0)
        ); 
    end component;
    
	    -- Sinal interno para armazenar o valor do flip-flop JK
    signal d_reg : std_logic_vector(3 downto 0) := (others => '0'); -- Inicializando com '0'

begin
    -- Instância do componente "ffd" e mapeamento das portas
    d_ff_inst : ffd port map (
        clk => clk,
        d => d_reg,
        q => q
    );

    -- Processo para atualizar o valor do flip-flop JK
    process (clk, reset)
    begin
        if reset = '1' then
            -- Reseta o valor do flip-flop quando o sinal de reset está em '1'
            d_reg <= (others => '0');
        elsif clk'event and clk = '1' then
            -- Condições para definir o valor do flip-flop com base nos sinais de entrada j e k
            if j = '1' and k = '1' then
                d_reg <= not d_reg;  -- Alternância entre 0 e 1
            elsif j = '1' and k = '0' then
                d_reg <= (others => '1');
            elsif j = '0' and k = '1' then
                d_reg <= (others => '0');
            elsif j = '0' and k = '0' then
                null; -- Nenhuma mudança, pode ser omitido
            end if;
        end if;
    end process;

end synth;
