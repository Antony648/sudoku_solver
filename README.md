Sudoku solver

this is a basic sudoku solver that uses constrain propagative logic  
current implementation can only solve beginner to slight intermediate problems
have added normal sudoku techniques(beginner) and naked pair(beginner -slight intermediate)

the solver has a record called top bar that for each cell provides the information 
regarding what values can reside in what blocks...

it has stablize block()
    tries to fill values not in current block that are  present in blocks that are left right top bottom of block
    
rowate()
    fill elements rowise
colate()
    fill elements column wise
blockate()
    fill elements block wise
we try to solve in iterations... 
if any iteration did not produce atleast a single change in value it is considered
unsolvable....
