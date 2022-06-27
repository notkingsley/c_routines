#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*my version of quicksort
quite literally, actually, the swap method
is off my head*/

void quicksort(int arr[], int start, int end){
	//sorts in place
	int n = start + 1;
	int m = end;
	if(n >= m) return;

	int ret = 1;
	for(int i = start + 1; i <= end; i++){
		if(arr[i] != arr[start]){
			ret = 0;
			break;
		}
	}
	if(ret) return;			//arr is homogenous

	while(n != m){
		//counting from the end
		while(arr[start] <= arr[m] && n < m) m--;
		//find next larger element than arr[start]
		while(arr[n] < arr[start] && n < m) n++;
		if(n == m){
			if(arr[start] > arr[n]){
				int hold = arr[start];
				arr[start] = arr[n];
				arr[n] = hold;
			}else n -= 1;
			
			break;
		}
		int hold = arr[n];
		arr[n] = arr[m];
		arr[m] = hold;
	}
	quicksort(arr, start, n);
	quicksort(arr, n + 1, end);
}

void test(int len){
	int ab[len];
	srand(time(0));
	for(int i = 0; i < len; i++){
		ab[i] = rand()%200;
	}

	int size_a = sizeof(ab)/sizeof(int);
	quicksort(ab, 0, size_a - 1);

	for(int pl = 0; pl < size_a; pl++){
		printf("%d ", ab[pl]);
	}
	printf("\n");
}

int main(int argc, char const *argv[])
{
	test(50);
	return 0;
}
