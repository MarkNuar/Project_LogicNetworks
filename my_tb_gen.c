// test bench generator, Marco Giuseppe Caruso, 2019

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void generate_vhdl(FILE*,int);

int main(){
  FILE* fp=NULL;
  int numTest;
  char fileName[100];

  printf("Insert the number of test cases\n");
  do
    scanf("%d",&numTest);
  while(numTest>1000 || numTest<1);
  for(int i=0; i<numTest;i++){
    sprintf(fileName,"test_%d",i+1);
    if(fp=fopen(fileName, "w")){
        generate_vhdl(fp,i+1);
      fclose(fp);
    }else
      printf("Error creating file\n");
  }
  return 0;
}


void generate_vhdl(FILE* fp, int numTest){

  int dontCareMask[8];
  int points[2][9]; //righe e poi colonne, il primo è il centro
  int result[8];
  int minDistance = 510;
  int curDistance;

  for(int i=0; i<8; i++)
    result[i]=0;
  for(int i=0; i<8; i++)
    dontCareMask[i]=(rand()%2);

  for(int i=0; i<2; i++)
    for(int j=0; j<9; j++)
      points[i][j] = (rand()%256);

  for(int j=1; j<9; j++){
    if(dontCareMask[j-1]==1){
      curDistance=abs(points[0][0]-points[0][j])+abs(points[1][0]-points[1][j]);
      if(curDistance<minDistance){
        for(int k=0; k<8; k++)
	  result[k]=0;
        result[j-1]=1;
      }
      else if(curDistance=minDistance){
        result[j-1]=1;
      }
    }
  }

  fprintf(fp, "library ieee;\n");
  fprintf(fp, "use ieee.std_logic_1164.all;\n");
  fprintf(fp, "use ieee.numeric_std.all;\n");
  fprintf(fp, "use ieee.std_logic_unsigned.all;\n");
  fprintf(fp, "entity test_%d is\n",numTest);
  fprintf(fp, "end test_%d;\n",numTest);
  fprintf(fp, "architecture bhv of test_%d is\n",numTest);
  fprintf(fp, "	constant c_CLOCK_PERIOD        : time := 100 ns;\n");
  fprintf(fp, "	signal   tb_done               : std_logic;\n");
  fprintf(fp, "	signal   mem_address           : std_logic_vector (15 downto 0) := (others => '0');\n");
  fprintf(fp, "	signal   tb_rst                : std_logic := '0';\n");
  fprintf(fp, "	signal   tb_start              : std_logic := '0';\n");
  fprintf(fp, "	signal   tb_clk                : std_logic := '0';\n");
  fprintf(fp, "	signal   mem_o_data,mem_i_data : std_logic_vector (7 downto 0);\n");
  fprintf(fp, "	signal   enable_wire           : std_logic;\n");
  fprintf(fp, "	signal   mem_we                : std_logic;\n");
  fprintf(fp, "	type ram_type is array (65535 downto 0) of std_logic_vector(7 downto 0);\n");
  fprintf(fp, "	signal RAM: ram_type := (\n");
  fprintf(fp, "0 =>\"");
  for(int i=0;i<8;i++)
    fprintf(fp,"%d", dontCareMask[i]);
  fprintf(fp, "\",\n");
  int j=1;
  for(int i=0;i<9;i++){
      fprintf(fp, "%d => std_logic_vector(to_unsigned(%d,8)),\n",j,points[0][i]);
      j++;
      fprintf(fp, "%d => std_logic_vector(to_unsigned(%d,8)),\n",j,points[1][i]);
      j++;
  }
  fprintf(fp, "		others => (others =>'0'));\n");

  fprintf(fp, "	constant EXPECTED_OUTPUT : std_logic_vector(7 downto 0) := \"");
  for(int i=0;i<8;i++)
    fprintf(fp, "%d", result[i]);
  fprintf(fp, "\";\n");

  fprintf(fp, "	component project_reti_logiche is\n");
  fprintf(fp, "		port (\n");
  fprintf(fp, "			i_clk         : in  std_logic;\n");
  fprintf(fp, "			i_start       : in  std_logic;\n");
  fprintf(fp, "			i_rst         : in  std_logic;\n");
  fprintf(fp, "			i_data        : in  std_logic_vector(7 downto 0);\n");
  fprintf(fp, "			o_address     : out std_logic_vector(15 downto 0);\n");
  fprintf(fp, "			o_done        : out std_logic;\n");
  fprintf(fp, "			o_en          : out std_logic;\n");
  fprintf(fp, "			o_we          : out std_logic;\n");
  fprintf(fp, "			o_data        : out std_logic_vector (7 downto 0)\n");
  fprintf(fp, "	);\n");
  fprintf(fp, "	end component project_reti_logiche;\n");
  fprintf(fp, "begin\n");
  fprintf(fp, "UUT:\n");
  fprintf(fp, "	project_reti_logiche port map (\n");
  fprintf(fp, "		i_clk          => tb_clk,\n");
  fprintf(fp, "		i_start        => tb_start,\n");
  fprintf(fp, "		i_rst          => tb_rst,\n");
  fprintf(fp, "		i_data         => mem_o_data,\n");
  fprintf(fp, "		o_address      => mem_address,\n");
  fprintf(fp, "		o_done         => tb_done,\n");
  fprintf(fp, "		o_en           => enable_wire,\n");
  fprintf(fp, "		o_we           => mem_we,\n");
  fprintf(fp, "		o_data         => mem_i_data\n");
  fprintf(fp, "	);\n");
  fprintf(fp, "p_CLK_GEN:\n");
  fprintf(fp, "	process is\n");
  fprintf(fp, "	begin\n");
  fprintf(fp, "		wait for c_CLOCK_PERIOD/2;\n");
  fprintf(fp, "		tb_clk <= not tb_clk;\n");
  fprintf(fp, "	end process p_CLK_GEN;\n");
  fprintf(fp, "MEM:\n");
  fprintf(fp, "	process(tb_clk)\n");
  fprintf(fp, "	begin\n");
  fprintf(fp, "		if tb_clk'event and tb_clk = '1' then\n");
  fprintf(fp, "			if enable_wire = '1' then\n");
  fprintf(fp, "				if mem_we = '1' then\n");
  fprintf(fp, "					RAM(conv_integer(mem_address)) <= mem_i_data;\n");
  fprintf(fp, "					mem_o_data                     <= mem_i_data after 2 ns;\n");
  fprintf(fp, "				else\n");
  fprintf(fp, "					mem_o_data <= RAM(conv_integer(mem_address)) after 2 ns;\n");
  fprintf(fp, "				end if;\n");
  fprintf(fp, "			end if;\n");
  fprintf(fp, "		end if;\n");
  fprintf(fp, "	end process;\n");
  fprintf(fp, "test:\n");
  fprintf(fp, "	process is\n");
  fprintf(fp, "	begin\n");
  fprintf(fp, "		wait for 100 ns;\n");
  fprintf(fp, "		wait for c_CLOCK_PERIOD;\n");
  fprintf(fp, "		tb_rst <= '1';\n");
  fprintf(fp, "		wait for c_CLOCK_PERIOD;\n");
  fprintf(fp, "		tb_rst <= '0';\n");
  fprintf(fp, "		wait for c_CLOCK_PERIOD;\n");
  fprintf(fp, "		tb_start <= '1';\n");
  fprintf(fp, "		wait for c_CLOCK_PERIOD;\n");
  fprintf(fp, "		wait until tb_done = '1';\n");
  fprintf(fp, "		wait for c_CLOCK_PERIOD;\n");
  fprintf(fp, "		tb_start <= '0';\n");
  fprintf(fp, "		wait until tb_done = '0';\n");
  fprintf(fp, "		assert RAM(19) = EXPECTED_OUTPUT report \"TEST FALLITO\" severity failure;\n");
  fprintf(fp, "		assert false report \"TEST %d OK\" severity failure;\n",numTest);
  fprintf(fp, "	end process test;\n");
  fprintf(fp, "end bhv;\n");

}
