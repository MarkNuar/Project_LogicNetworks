// test bench generator, Marco Giuseppe Caruso, 2019

#include <math.h>
#include <stdio.h>

void generate_vhdl(FILE*,int);

int main(){
  FILE* fp=null;
  int numTest;
  char[100] fileName;

  printf("Insert the number of test cases\n");
  do
    scanf("%d",&numTest);
  while(numTest>1000 || numTest<1);
  for(int i=0; i<numTest;i++){
    sprintf(fileName,"test_%d",i);
    if(fp=fopen(fileName, "w"){
        generate_vhdl(fp,i);
      fclose(fp);
    }else
      printf("Error creating file\n");
  }
  return 0;
}


void generate_vhdl(FILE* fp, int numTest){

  int[8] dontCareMask;
  int[2][9] points; //righe e poi colonne, il primo è il centro
  int[8] result;
  int minDistace = 510;

  for(int i=0; i<8; i++)
    dontCareMask[i]=(rand()%2);

  for(int i=0; i<2; i++)
    for(int j=0; j<8; j++)
      points[i][j] = (rand()%256);

  for(int i=0; i<8; i++){

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

}
