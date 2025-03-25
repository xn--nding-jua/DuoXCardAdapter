library ieee;
use ieee.std_logic_1164.all;

entity clk_connector is
	port (
		clk_i 		: in std_logic;
		clk_o 		: out std_logic
	  );
end clk_connector;

architecture rtl of clk_connector is
begin
	clk_o <= clk_i;
end rtl;