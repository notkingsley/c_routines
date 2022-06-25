#include <stdio.h>

/*
 *c program to sort an array of strings
*/

//print an array of strings to stdout
void print(char** arr, int len){
	for(int c = 0; c < len; c++){
		printf("%s ", arr[c]);
	}
	printf("\n");
}

//returns a < b by lexicographic comparison
//you might want to use strcmp instead
int lexicographic_sort(char *a, char *b){
	int i =0;
	while(a[i] == b[i]) i++;
	return a[i] < b[i];
}

//bubble sort an array of strings lexicographically
void sort(char **arr, int len){
	for(int i = 0; i < len; i ++){
		for(int j = 0; j < len -1; j++){
			if(!lexicographic_sort(arr[j], arr[j +1])){
				char *temp = arr[j];
				arr[j] = arr[j +1];
				arr[j + 1] = temp;
			}
		}
	}
}

int main(){	
	char* arr[] = {"me", "you", "jason", "all", "of", "us"};
	int len = sizeof(arr)/sizeof(char*);
	sort(arr, len);
	print(arr, len);

	return 0;
}