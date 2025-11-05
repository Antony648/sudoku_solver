//jesus save us!
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#define MAX_ROW 3
#define MAX_COL 3
#define MAX_SIZE 81
#define MAX_ROW_COUNT 9
#define MAX_COL_COUNT 9

int matrix[MAX_ROW_COUNT][MAX_COL_COUNT];
uint16_t top_bar[MAX_SIZE];

void unset_value_gen(int row,int col,int val);
void init_top_bar();
int  init_setup();
bool is_block_full(int block_n);
bool is_row_full(int row);
bool is_col_full(int col);
void swap(int i,int j,int* array);
void insert_matrix(int row,int col,int val);
void is_possible(int row,int col,int val);
int get_block_array(int block_n,int array,int r,int c);
void stabilize_block(int block_n);

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
bool is_block_full(int block_n)
{
	//returns true if block is full else no....
	int count=0;
	int col=(block_n%3)*3;
	int row=(block_n/3)*3;
	int col_max=start_col+3;
	int row_max=start_row+3;
	for(;row<row_max;row++)
		for(;col<col_max;col++)
			if(matrix[row][col])
				count++;
	if(count==9)
		return true;
	return false;
}
bool is_row_full(int row)
{
	int count=0;
	for(int i=0;i<9;i++)
		if(matrix[row][i])
			count++;
	if(count==9)
		return true;
	retrun false;
}
bool is_col_full(int col)
{

	int count=0;
	for(int i=0;i<9;i++)
		if(matrix[i][col])
			count++;
	if(count==9)
		return true;
	retrun false;
}
void swap(int i,int j,int* array)
{
	int temp=array[i];
	array[i]=array[j];
	array[j]=temp;
	return;
}

void insert_matrix(int row,int col,int val)
{
	//is_possible(int row,int col,int val);
	unset_val_gen(int row,int col,int val);
	matrix[row][col];
	return;
}


void  get_blank_block(int block_n,int* array)
{
	//puts all block index value of 
	//blank entries in a block...
	for(int i=0;i<9;i++)
		array[i]=0;
	//fill array with zero
	int row=((int)block_n/3)*3;
	int col=((int)block_n%3)*3;

	//fill array with index numbers 
	//of empty block
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			if(!matrix[row+i][col+j])
			{
				*array=(i*3)+j;
				array++;
			}
	return;
}
bool is_possible(int row,int col,int val)
{
	//this function returns true if it is possible to place
	//a value in that block	
	for(int i=0;i<9;i++)
	{
		if(matrix[row][i]==val)
			return false;
		if(matrix[i][col]==val)
			return false;
	}
	int row=((int) row/3)*3;
	int col=((int)col/3)*3;
	row+=3;col+=3; //set value to upper bounds 
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			if(matrix[i][j]== val)
				return false;
	return true;
}
int get_block_array(int block_n,int array,int r,int c)
{
	//populates array passed as second param 
	//with the stable elements in 
	//a blocknumber passed as first param
	// and remaing free space with 0
	//and returns the count 
	
	// the row and col are additional params....
	// in some situvation we will not have the
	// block number so we can pass row and col...
	// but block number is the prefered ....
	
	int row,col,row_max,col_max,count;
	if(block_n)
	{
		row=((int)(block_n/3))*3;
		col=(block_n%3)*3;
	}
	else
	{
		row=r;col=c;	
	}
	row_max=row+3;
	col_max=col+3;
	for(int i=0;i<9;i++)
		array[i]=0;
	count=0;
	for(;row<row_max;row++)
		for(;col<col_max;col++)
			if(matrix[row][col])
			{
				array[count]=matrix[row][col];
				count++;
			}
	//sort upto count elements...
	for(int i=0;i<count;i++)
		for(int j=0;j<count;j++)
			if(array[i]> array[j])
				swap(i,j,array);

	return count;


	
}

void gen_row_col_block(int*row,int*col,int index,int block_n)
{
	//this function takes the index of a cell within a block
	//and generates corresponding row and col as
	//we cannot pass 2 values out.. need to fill values
	//to pointers passed as param one and param 2
	int row_b=((int) block_n/3)*3;
	int col_b=((int) block_n%3)*3;
	row_b+=((int) index/3);
	col_b+=((int) index%3);
	*row=row_b;
	*col=col_b;
	return;
}
void stabilize_block(int block_n)
{
	//the idea is that if we want to add any number to a block 
	//the number will be generally present in multiple blocks
	//that come as horizontal or vertical neighbours....
	if(is_block_full(int block_n))
		return;
	int n1[9],n2[9],n3[9],n4[9];
	int * blob[4]={n1,n2,n3,n4};
	int jazz=0,col;
	int row=block_n/3;
	int max=row+3;
	int ans[]={0,0,0,0,0,0,0,0,0,0};
	int min=999,prev_min,rep;
	int tmp;int* tmp_ptr;

	//horizontal
	for(int i=row;i<max;i++)
	{
		if(i==block_n)
			continue;
		get_block_array(i,blob[jazz]);
		jazz++;
	}
	//vertical
	col=block_n%3;
	max=col+7;
	for(int i=col;i<max;i+=3)
	{
		if(i==block_n)
			continue;
		get_block_array(i,blob[jazz]);
		jazz++;
	}
	//at this point we expect all contents of blob to be 
	//poplated with corresponding elems from blocks....
	
	jazz=0;

	//as each elemet in blob contains the 
	//start of the array and as array is 
	//non decreasing order... we can compare
	//its first elements to get the common...
	//on each iteration it selects the smallest 
	//item... if  an item occurs muliple times we select it to ans
	//on the start of next loop the perv min is removed...so next
	//elem can take its place...
	while(true)
	{
		prev_min=min;
		min=999;rep=0;
		//preset condition...
		for(int j=0;j<4;j++)
		{
			tmp=*blob[j];
			if(tmp==0)
				continue;
			if(tmp==prev_min)
				blob[j]+=1;
			if(tmp==min)
			{
				rep=1;
				continue;
			}
			if(tmp<min)
			{
				rep=0;
				min=blob[j];
			}
		}
		if(min==999)
			break;
		if(rep==1)
		{
			ans[jazz]=min;
			jazz++;rep=0;
		}
	}
	//code below will get 
	//rid of common elements between the current block and other blocks
	get_block_array(block_n,n1);

	tmp_ptr=ans;

	while(*ans && *n1)
	{
		if(*ans ==*n1)
		{
			ans=999;
			ans++;n1++;
			continue;
		}
		if(*ans < *n1)
			ans++;
		else
			n1++;
	}

	//at this point we have all common elements removed,replaced by 999 
	//tmp_ptr points to the begining.... now we have to try to fill 
	//each of the elems to all free blocks in a block.....
	//insert_matrix(row,col,val)
	//gen_row_col_block(*row,*col,index,block_n)
	//bool is_possible(int row,int col,int val)
	get_blank_block(block_n,n2);
	int count=0,change=1,rtn;
	while(change)
	{
		change=0;
		for(int i=0;ans[i]!=0;i++)
		{
			count=0;	
			if(ans[i]==999)
				continue;
			//take a value from ans and try to fill it 
			//every single free cell of block
			for(int j=0;n1[j]!=0;j++)
			{
				
				if(n1[j]==999)
					continue;
				gen_row_col_block(&row,&col,n1[j],block_n);
				if(is_possible(row,col,ans[i]))
				{
					count++;rtn=j;
					if(count >1)
						break;
				}		
				
			}
			if(count==1)
			{
				//only one possible location ans[i] for value in 
				//the block and it is  in n1[rtn]
				gen_row_col(&row,&col,n1[rtn],block_n);
				insert_matrix(row,col,ans[i]);
				ans[i]=999;n1[rtn]=999;
				change=1;	//shows that atleast one value was added in this attempt
				
			}
		}

	}
	return;
	
} 
void colate(int col)
{
	//try to fill missing
	//elements based on column

}
void rowate(int row)
{
	//try to fill missing elements 
	//based on rowate

}
void blockate(int block_n)
{
	//try to fill missing elements 
	//based on missing values from 
	//row
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
		//important code should sit here...
		//call stablilze in spiral order 
		//call rowate ,colate,blockate
		for(int i=0;i<9;i++)
			if(!is_row_full(i))
				rowate(i);
		for(int i=0;i<9;i++)
			if(!is_col_full(i))
				colate(i);
		for(int i=0;i<9;i++)
			if(!is_block_full(i))
				blockate(i);	
		if(count==81)
		{
			print_success();
			return;
		}
		
	}while(count>count_prev);
	printf("the program has failed to solve sudoku...");
}
