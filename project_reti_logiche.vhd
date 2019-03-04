----------------------------------------------------------------------------------
-- Progetto Reti Logiche - Ingegneria Informatica - Sez. Prof Fornaciari William
-- Marco Giuseppe Caruso (codice persona 10531943, matricola 866190)
----------------------------------------------------------------------------------

library IEEE;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.ALL;

entity project_reti_logiche is
    Port (
           i_clk : in std_logic;
           i_start : in std_logic;
           i_rst : in std_logic;
           i_data : in std_logic_vector(7 downto 0);
           o_address : out std_logic_vector(15 downto 0);
           o_done : out std_logic;
           o_en : out std_logic;
           o_we : out std_logic;
           o_data : out std_logic_vector(7 downto 0)
    );
end project_reti_logiche;

architecture Behavioral of project_reti_logiche is
    type state_type is (START_WAIT, SET_ADDRESS,
    WAIT_CLOCK_CICLE, STORE_DATA,
    CALC_DIST, UPDATE_DIST_1, UPDATE_DIST_2,  --me ne servono due per aggiornare due volte la maschera di uscita
    DONE_HIGH, DONE_LOW);
    signal cur_state, next_state : state_type; -- segnale per lo stato
    signal cur_address, next_address : std_logic_vector(15 downto 0);  --indirizzo dei punti incrementa
    signal cur_operation, next_operation : integer range 0 to 19;
    signal cur_x_bello, next_x_bello : integer range 0 to 255;
    signal cur_y_bello, next_y_bello : integer range 0 to 255;
    signal cur_x, next_x : integer range 0 to 255;
    signal cur_y, next_y : integer range 0 to 255;
    signal cur_min_distance, next_min_distance : integer range 0 to 510;
    signal cur_distance, next_distance : integer range 0 to 510;
    signal cur_out_mask, next_out_mask : std_logic_vector(7 downto 0);
    signal cur_point_number, next_point_number : integer range 0 to 7;
    signal cur_dont_care_mask, next_dont_care_mask : std_logic_vector (7 downto 0);

begin

    process(i_clk, i_rst)
    begin
        if(i_rst = '1') then
            cur_state <= START_WAIT;
        elsif(rising_edge(i_clk)) then
            cur_state <= next_state;
            cur_address <= next_address;
            cur_operation <= next_operation;
            cur_x_bello <= next_x_bello;
            cur_y_bello <= next_y_bello;
            cur_x <= next_x;
            cur_y <= next_y;
            cur_min_distance <= next_min_distance;
            cur_distance <= next_distance;
            cur_out_mask <= next_out_mask;
            cur_point_number <= next_point_number;
            cur_dont_care_mask <= next_dont_care_mask;
        end if;
    end process;

    process(i_start, i_data, i_start, cur_state, cur_address, cur_operation, cur_x_bello, cur_y_bello, cur_x, cur_y, cur_min_distance, cur_distance, cur_out_mask, cur_point_number, cur_dont_care_mask)
    begin

        next_state <= cur_state;
        next_address <= cur_address;
        next_operation <= cur_operation;
        next_x_bello <= cur_x_bello;
        next_y_bello <= cur_y_bello;
        next_x <= cur_x;
        next_y <= cur_y;
        next_min_distance <= cur_min_distance;
        next_distance <= cur_distance;
        next_out_mask <= cur_out_mask;
        next_point_number <= cur_point_number;
        next_dont_care_mask <= cur_dont_care_mask;

        case cur_state is
            when START_WAIT =>
                if(i_start = '1') then
                    next_x_bello <= 0;
                    next_y_bello <= 0;
                    next_x <= 0;
                    next_y <= 0;
                    next_address <= "0000000000000001";
                    next_operation <= 0;
                    next_min_distance <= 510;
                    next_distance <= 0;
                    next_out_mask <= "00000000";
                    next_point_number <= 0;
                    next_dont_care_mask <= "00000000";
                    next_state <= SET_ADDRESS;
                end if;

                 -- uscita per evitare i latch
                o_address <= "0000000000000000";
                o_done <= '0';
                o_en <= '0';
                o_we <= '0';
                o_data <= "00000000";

            when SET_ADDRESS =>
                if(cur_operation = 0) then
                    o_en <= '1';
                    o_we <= '0';
                    o_data <= "00000000";
                    o_address <= "0000000000000000";
                    next_state <= WAIT_CLOCK_CICLE;
                elsif(cur_operation = 1) then
                    o_en <= '1';
                    o_we <= '0';
                    o_data <= "00000000";
                    o_address <= "0000000000010001";
                    next_state <= WAIT_CLOCK_CICLE;
                elsif(cur_operation = 2) then
                    o_en <= '1';
                    o_we <= '0';
                    o_data <= "00000000";
                    o_address <= "0000000000010010";
                    next_state <= WAIT_CLOCK_CICLE;
                elsif(cur_operation > 2 and cur_operation < 19) then
                    if(cur_dont_care_mask(cur_point_number)='1') then
                        o_en <= '1';
                        o_we <= '0';
                        o_data <= "00000000";
                        o_address <= cur_address;
                        next_address <= cur_address + "0000000000000001";
                        next_state <= WAIT_CLOCK_CICLE;
                    else
                        o_en <= '0';
                        o_we <= '0';
                        o_data <= "00000000";
                        o_address <= "0000000000000000"; -- non mi interessa
                        next_operation <= cur_operation + 2;  --salto due operazioni
                        next_address <= cur_address + "0000000000000010"; -- incremento l'inidirizzo
                        next_point_number <= cur_point_number + 1; -- incremento il numero
                        next_state <= SET_ADDRESS;
                    end if;
                elsif(cur_operation = 19) then
                    o_en <= '1';
                    o_we <= '1';
                    o_address <= "0000000000010011";
                    o_data <= cur_out_mask;
                    next_state <= WAIT_CLOCK_CICLE;
                end if;

                -- uscita per evitare i latch
                o_done <= '0';


            when WAIT_CLOCK_CICLE =>
                if(cur_operation < 19) then
                    next_state <= STORE_DATA;
                elsif(cur_operation = 19) then
                    next_state <= DONE_HIGH;
                end if;

                -- uscita per evitare i latch
                o_address <= "0000000000000000";
                o_done <= '0';
                o_en <= '0';
                o_we <= '0';
                o_data <= "00000000";


            when STORE_DATA =>
                if(cur_operation = 0) then
                    next_dont_care_mask <= i_data;
                    next_state <= SET_ADDRESS;
                elsif(cur_operation = 1) then
                    next_x_bello <= conv_integer(i_data);
                    next_state <= SET_ADDRESS;
                elsif(cur_operation = 2) then
                    next_y_bello <= conv_integer(i_data);
                    next_state <= SET_ADDRESS;
                elsif(cur_operation > 2 and cur_operation < 19) then
                    if((cur_operation mod 2) = 1) then
                        next_x <= conv_integer(i_data);
                        next_state <= SET_ADDRESS;
                    else
                        next_y <= conv_integer(i_data);
                        next_state <= CALC_DIST;
                        next_point_number <= cur_point_number + 1;
                    end if;
                end if;
                next_operation <= cur_operation + 1;

                -- uscita per evitare i latch
                o_address <= "0000000000000000";
                o_done <= '0';
                o_en <= '0';
                o_we <= '0';
                o_data <= "00000000";


            when CALC_DIST =>
                next_distance <= (abs(cur_x_bello - cur_x) + abs(cur_y_bello - cur_y));
                next_state <= UPDATE_DIST_1;

                -- uscita per evitare i latch
                o_address <= "0000000000000000";
                o_done <= '0';
                o_en <= '0';
                o_we <= '0';
                o_data <= "00000000";


            when UPDATE_DIST_1 =>
                if(cur_distance < cur_min_distance) then
                    next_min_distance <= cur_distance;
                    next_out_mask <= "00000000";
                    next_state <= UPDATE_DIST_2;
                elsif(cur_distance = cur_min_distance) then
                    next_out_mask <= cur_out_mask;
                    next_state <= UPDATE_DIST_2;
                else
                    next_state <= SET_ADDRESS;  --non sempre aggiorno la maschera, solo se distanza min trovata
                end if;

                -- uscita per evitare i latch
                o_address <= "0000000000000000";
                o_done <= '0';
                o_en <= '0';
                o_we <= '0';
                o_data <= "00000000";

            when UPDATE_DIST_2 =>
                next_out_mask(cur_point_number - 1) <= '1';
                next_state <= SET_ADDRESS;

                -- uscita per evitare i latch
                o_address <= "0000000000000000";
                o_done <= '0';
                o_en <= '0';
                o_we <= '0';
                o_data <= "00000000";

            when DONE_HIGH =>
                o_en <= '0';
                o_we <= '0';
                o_done <= '1';
                if(i_start = '0') then
                    next_state <= DONE_LOW;
                else
                    next_state <= DONE_HIGH;
                end if;

                -- uscita per evitare i latch
                o_address <= "0000000000000000";
                o_data <= "00000000";


            when DONE_LOW =>
                o_done <= '0';
                next_state <= START_WAIT;

                -- uscita per evitare i latch
                o_address <= "0000000000000000";
                o_en <= '0';
                o_we <= '0';
                o_data <= "00000000";


        end case;
    end process;
end Behavioral;
