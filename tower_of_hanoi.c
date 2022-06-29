#include <stdio.h>
#include <stdlib.h>

//solves the tower of hanoi puzzle and the four peg version
//mind the value you set to size: the solution is
//dangerously exponential!!
int size = 9;

void move_disk(int *pegs[], int from, int to){
	//does the actual moving
	//the topmost disk on from is moved to to
	printf("Removing a disk from peg %d to peg %d\n", from, to);
	
	from--; 
	to--;
	int top = 0, top2 = size -1;
	while(!pegs[from][top]){
		top++;
	}
	while(top2 >0 && pegs[to][top2]){
		top2--;
	}
	pegs[to][top2] = pegs[from][top];
	pegs[from][top] = 0;

	for(int p = 0; p < size; p++){
		for(int i = 0; i < 4; i++){
			if(pegs[i][p]) printf("%d ", pegs[i][p]);
			else printf("  ");
		}
		printf("\n");
	}
}

int print_pegs(int **pegs, int n){
	//prints the contents of the pegs array
	for(int p = 0; p < n; p++){
		for(int i = 0; i < 4; i++){
			printf("%d ", pegs[i][p]);
		}
		printf("\n");
	}
}

void solve(int *pegs[], int n, int from, int to, int freep){
	//solves the tower of hanoi puzzle with n disks
	//the n disks are initially on peg from.
	//their destination is peg to, and peg freep is free
	if(n == 1) move_disk(pegs, from, to);
	else{
		solve(pegs, n-1, from, freep, to);
		move_disk(pegs, from, to);
		solve(pegs, n-1, freep, to, from);
	}
}

void ssolve(int **pegs, int n, int from, int to, int freep, int other){
	//solves the reve's version of the puzzle with four pegs using the
	//frame-stewart algorithm. parameters as before, other is the 
	//peg the smallest n-k is moved to first
	if(n ==1) move_disk(pegs, from, to);
	else if(n == 2) solve(pegs, n, from, to, freep);
	else{
		//determine k
		/* k should be chosen to be the smallest integer
		such that n does not exceed tk = k(k + 1)/2, the kth triangu-
		lar number, that is, tk−1 < n <= tk */
		int tk = 1;
		int k = 1;
		while(n > tk){
			k++;
			tk += k;
		}
		ssolve(pegs, n-k, from, other, freep, to);
		solve(pegs, k, from, to, freep);
		ssolve(pegs, n-k, other, to, freep, from);
	}
}

int main(int argc, char const *argv[])
{
	int n = size;
	int from = 1, to = 2, freep = 3, other =4;

	//initialize 2d array of pegs
	int **pegs = (int**)malloc(4 * sizeof(int*));
	for(int i = 0; i <4; i++){
		pegs[i] = (int*)malloc(n * sizeof(int));
	}
	for(int i = 0; i < n; i++){
		pegs[from - 1][i] = i +1;
		pegs[to-1][i] = 0;
		pegs[freep - 1][i] = 0;
		pegs[other -1][i] = 0;
	}
	printf("Original peg state:\n");
	print_pegs(pegs, n);
	printf("\n");
	ssolve(pegs, n, 1, 2, 3, 4);

	for(int i = 0; i < 4; i++){
		free(pegs[i]);
	}
	free(pegs);
	return 0;
}