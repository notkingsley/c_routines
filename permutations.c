#include <stdio.h>

//works fine, but there's a nasty stack corruption
//bug somewhere in next_permutation, which, I could
//swear wasn't there last time
void next_permutation(int perm[], int n){
	int p = n - 1;
	int g = p;
	int r = p;
	while (p > 0 && perm[p] <= perm[p- 1]){
		p--;
	}
	p--;
	//p is the first index from the right so that perm[p] < perm[p+1]
	while(perm[p] >= perm[g]){
		g--;
	}
	//g is so that perm[g] is the smallest no greater than perm[p]

	//switch perm[g] and perm[p]
	int z = perm[p];
	perm[p] = perm[g];
	perm[g] = z;

	//bubble sort
    for(int i =p +1; i < n; i++){
        for(int j =p +1; j < n - 1; j++){
            if(perm[j] > perm[j+1]){
                int hold = perm[j];
                perm[j] = perm[j+1];
                perm[j +1] = hold;
            }
        }
    }
}

void permute(int *permutation, int n, int f){
	//generates and prints all permutations
	for(int j = 0; j < f - 1; j ++){		
		for(int i = 0; i <n; i++){
			printf("%d", permutation[i]);
		}
		next_permutation(permutation, n);
		printf("\n");
	}
}

void derange(int* permutation, int n, int f){
	//generates and prints all derangements 
	//copy permuation into derr
	int derr[n];
	for(int i = 0; i < n; i ++){
		derr[i] = permutation[i];
	}
	for(int j = 0; j < f - 1; j ++){
		next_permutation(derr, n);
		int match = 0;
		for(int i = 0; i < n; i++){
			if(derr[i] == permutation[i]){
				match = 1;
				break;
			}
		}
		if(match) continue;
		//else, print
		for(int i = 0; i < n; i++){
			printf("%d", derr[i]);
		}
		printf("\n");
	}
}

int main(){
	// obtain an n 
	int n = 3;

	//generate initial permutation, f is the factorial
	int f = 1;
	int permutation[n];
	for (int m = 1; m <= n; m++){
		permutation[m-1] = m;
		f *= m;
	}
	//permutation[2] = 2;

	derange(permutation, n, f);
}