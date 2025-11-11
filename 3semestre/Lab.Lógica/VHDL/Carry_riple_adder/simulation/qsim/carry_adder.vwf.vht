-- Copyright (C) 2022  Intel Corporation. All rights reserved.
-- Your use of Intel Corporation's design tools, logic functions 
-- and other software and tools, and any partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Intel Program License 
-- Subscription Agreement, the Intel Quartus Prime License Agreement,
-- the Intel FPGA IP License Agreement, or other applicable license
-- agreement, including, without limitation, that your use is for
-- the sole purpose of programming logic devices manufactured by
-- Intel and sold by Intel or its authorized distributors.  Please
-- refer to the applicable agreement for further details, at
-- https://fpgasoftware.intel.com/eula.

-- *****************************************************************************
-- This file contains a Vhdl test bench with test vectors .The test vectors     
-- are exported from a vector file in the Quartus Waveform Editor and apply to  
-- the top level entity of the current Quartus project .The user can use this   
-- testbench to simulate his design using a third-party simulation tool .       
-- *****************************************************************************
-- Generated on "03/05/2024 14:18:47"
                                                             
-- Vhdl Test Bench(with test vectors) for design  :          carry_adder
-- 
-- Simulation tool : 3rd Party
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY carry_adder_vhd_vec_tst IS
END carry_adder_vhd_vec_tst;
ARCHITECTURE carry_adder_arch OF carry_adder_vhd_vec_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL a : STD_LOGIC;
SIGNAL b : STD_LOGIC;
SIGNAL c : STD_LOGIC;
SIGNAL d : STD_LOGIC;
SIGNAL e : STD_LOGIC;
SIGNAL f : STD_LOGIC;
SIGNAL g : STD_LOGIC;
SIGNAL h : STD_LOGIC;
SIGNAL over : STD_LOGIC;
SIGNAL s1 : STD_LOGIC;
SIGNAL s2 : STD_LOGIC;
SIGNAL s3 : STD_LOGIC;
SIGNAL s4 : STD_LOGIC;
COMPONENT carry_adder
	PORT (
	a : IN STD_LOGIC;
	b : IN STD_LOGIC;
	c : IN STD_LOGIC;
	d : IN STD_LOGIC;
	e : IN STD_LOGIC;
	f : IN STD_LOGIC;
	g : IN STD_LOGIC;
	h : IN STD_LOGIC;
	over : OUT STD_LOGIC;
	s1 : OUT STD_LOGIC;
	s2 : OUT STD_LOGIC;
	s3 : OUT STD_LOGIC;
	s4 : OUT STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : carry_adder
	PORT MAP (
-- list connections between master ports and signals
	a => a,
	b => b,
	c => c,
	d => d,
	e => e,
	f => f,
	g => g,
	h => h,
	over => over,
	s1 => s1,
	s2 => s2,
	s3 => s3,
	s4 => s4
	);

-- a
t_prcs_a: PROCESS
BEGIN
	a <= '1';
	WAIT FOR 200000 ps;
	FOR i IN 1 TO 2
	LOOP
		a <= '0';
		WAIT FOR 200000 ps;
		a <= '1';
		WAIT FOR 200000 ps;
	END LOOP;
WAIT;
END PROCESS t_prcs_a;

-- b
t_prcs_b: PROCESS
BEGIN
	b <= '1';
	WAIT FOR 200000 ps;
	FOR i IN 1 TO 2
	LOOP
		b <= '0';
		WAIT FOR 200000 ps;
		b <= '1';
		WAIT FOR 200000 ps;
	END LOOP;
WAIT;
END PROCESS t_prcs_b;

-- c
t_prcs_c: PROCESS
BEGIN
	FOR i IN 1 TO 3
	LOOP
		c <= '0';
		WAIT FOR 150000 ps;
		c <= '1';
		WAIT FOR 150000 ps;
	END LOOP;
	c <= '0';
WAIT;
END PROCESS t_prcs_c;

-- d
t_prcs_d: PROCESS
BEGIN
	FOR i IN 1 TO 3
	LOOP
		d <= '0';
		WAIT FOR 150000 ps;
		d <= '1';
		WAIT FOR 150000 ps;
	END LOOP;
	d <= '0';
WAIT;
END PROCESS t_prcs_d;

-- e
t_prcs_e: PROCESS
BEGIN
	FOR i IN 1 TO 3
	LOOP
		e <= '0';
		WAIT FOR 150000 ps;
		e <= '1';
		WAIT FOR 150000 ps;
	END LOOP;
	e <= '0';
WAIT;
END PROCESS t_prcs_e;

-- f
t_prcs_f: PROCESS
BEGIN
	FOR i IN 1 TO 3
	LOOP
		f <= '0';
		WAIT FOR 150000 ps;
		f <= '1';
		WAIT FOR 150000 ps;
	END LOOP;
	f <= '0';
WAIT;
END PROCESS t_prcs_f;

-- g
t_prcs_g: PROCESS
BEGIN
LOOP
	g <= '0';
	WAIT FOR 100000 ps;
	g <= '1';
	WAIT FOR 100000 ps;
	IF (NOW >= 1000000 ps) THEN WAIT; END IF;
END LOOP;
END PROCESS t_prcs_g;

-- h
t_prcs_h: PROCESS
BEGIN
LOOP
	h <= '0';
	WAIT FOR 100000 ps;
	h <= '1';
	WAIT FOR 100000 ps;
	IF (NOW >= 1000000 ps) THEN WAIT; END IF;
END LOOP;
END PROCESS t_prcs_h;
END carry_adder_arch;
