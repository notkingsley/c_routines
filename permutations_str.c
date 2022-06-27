#include <stdio.h>
#include <string.h>


//find all permutations of a given array of strings
int next_permutation(int n, char** s){
	//rise till you fall, then switch and sort
	int no_next = 1;
	for(int i = 0; i <n - 1; i++){
		if(strcmp(s[i], s[i +1]) < 0){
			no_next = 0;
			break;
		}
	}
	if(no_next) return 0;

	int p = n - 1;
	int g = p;
	int r = p;
	while (strcmp(s[p], s[p-1]) <= 0){ 		//rising...
		p--;
	}										//till you fall
	p--;
	//p is the first index from the right so that perm[p] < perm[p+1]
	while((strcmp(s[p], s[g]) >= 0)){			//finding switch spot
		g--;
	}
	//g is so that perm[g] is the smallest no greater than perm[p]

	//switch perm[g] and perm[p]
	char* z = s[p];
	s[p] = s[g];
	s[g] = z;								//switching...

	//bubble sort
    for(int i =p +1; i < n; i++){
        for(int j =p +1; j < n - 1; j++){
            if(strcmp(s[j], s[j +1]) > 0){
                char *hold = s[j];
                s[j] = s[j+1];
                s[j +1] = hold;
            }
        }
    }
	return 1;
}

void str_test(){
	char* s[] = {"abc", "def", "def", "jkl"};
	int n = 4;

	do{
		for (int i = 0; i < n; i++)
			printf("%s%c", s[i], i == n - 1 ? '\n' : ' ');
	} while (next_permutation(n, s));

}


int main(){
	str_test();

	return 0;
}