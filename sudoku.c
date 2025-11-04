#include <stdio.h>
#include <stdint.h>
#define MAX_ROW 3
#define MAX_COL 3
#define MAX_SIZE 81
#define MAX_ROW_COUNT 9
#define MAX_COL_COUNT 9

int matrix[MAX_ROW_COUNT][MAX_COL_COUNT];
uint16_t top_bar[MAX_SIZE];

void unset_value_gen(int row,int col,int val)
{
	//this function unsets all the values in top bar in all 
	//neccessary locations on element entry
	//the params are the rows and columns in which
	//we added values and the third param is the 
	//value...
	
	uint16_t syndrome=0x0001,value_tb;
	syndrome<<=(val-1);
	syndrome^=0xffff;
	int row_gen=((int) row/MAX_ROW)*MAX_ROW;//get start row
	int col_gen=((int) col/MAX_COL)*MAX_COL; //get start col
	int row_max=row_gen+MAX_ROW;
	int col_max=col_gen+MAX_COL;
	
	for(;row_gen<row_max;row_gen++)
		for(;col_gen<col_max;col_gen++)
		{
			value_tb=top_bar[(row_gen*MAX_ROW_COUNT)+col_gen];
			if(value_tb)
				continue;
			value_tb&=syndrome;
			top_bar[(row_gen*MAX_ROW_COUNT)+col_gen]=value_tb;
		}
	//for rows	
	row_gen=row;
	col_gen=0;
	col_max=MAX_COL_COUNT;
	int col_sub=((int)col/MAX_COL)*MAX_COL;//first column  in the block that was aldready 
					       //modified
	for(;col_gen<col_max;col_gen++)
	{
		if(col_gen ==col_sub)
		{			//if we encounter the the aldready modified
					//column... so we jump 3 columns
			col_gen+=3; 
			continue;
		}
		value_tb=top_bar[(row_gen*MAX_ROW_COUNT)+col_gen];
		if(value_tb)
			continue;
		value_tb&=syndrome;
		top_bar[(row_gen*MAX_ROW_COUNT)+col_gen]=value_tb;
	}
	//for cols
	row_gen=0;
	row_max=MAX_ROW_COUNT;
	col_sub=((int)row/MAX_ROW)*MAX_ROW;//first row in the block was aldready modified
	for(;row_gen<row_max;row_gen++)
	{
		if(row_gen==row_sub)
		{
			row_gen+=3;
			continue;
		}
		value_tb=top_bar[(row_gen*MAX_ROW_COUNT)+col_gen];
		if(value_tb)
			continue;
		value_tb&=syndrome;
		top_bar[(row_gen*MAX_ROW_COUNT)+col_gen]=value_tb;
	}		


}
void init_top_bar()
{
	//sets all the blocks to all possible values
	//should be called only once during init phase
	for(int i=0;i<81;i++)
		top_bar[i]=0x01ff;
}
int  init_setup()
{
	// this is the initial function it returns the 
	// count of stable values......
	// on receiving sudoku we expect some values
	// based on those values we can directly set values.....
	
	int count=0;
	init_top_bar();

	for(int i=0;i<MAX_ROW_COUNT;i++)
		for(int j=0;j<MAX_COL_COUNT;j++)
			if(matrix[i][j])
			{
				top_bar[(i*9)+j]=0x00;
				unset_value_gen(i,j,matrix[i][j]);
				count++;
			}
	return count;

		
}
void stabilize_block(int block_no)
{
	//the params are the first row and col in that blocks....
	
} 
void print_success()
{
	printf("puzzle solved!");
	for(int i=0;i<MAX_ROW_COUNT;i++)
	{
		printf("_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ ");
		for(int j=0;j<MAX_COL_COUNT;j++)
			printf("%d|",matrix[i][j]);
	}
	printf("_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ ");
	return;
}
void main()
{
	printf("welcome to sudoku...");
	int count,count_prev;
	count=init_setup();
	count_perv=count;
	//loop is designed to terminate if all blocks are full or if a 
	//single itreation did not contribute even a single value to
	//sudoku....
	do{
	
		count_prev=count;
		if(count==81)
		{
			print_success();
			return;
		}
		
	}while(count>count_prev);
	printf("the program has failed to solve sudoku...");
}
