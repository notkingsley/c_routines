#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//my estimates say this is extremely time efficient, but we
//seem to use a lot of arrays, tho yet again, might still be
//the best solution

void merge(int *arr1, int *arr2, int *resarr, int size1, int size2){
	//merges two sorted arrays
	int c1 =0, c2 =0;
	for(int k = 0; k < size1 + size2; k++){
		if( c1 < size1 && c2 < size2){
			//doesnt enter if any array is exhausted
			if(arr1[c1] <= arr2[c2]) resarr[k] = arr1[c1++];
			else resarr[k] = arr2[c2++];
		}else if(c1 < size1){
			//when end of arr2 is reached
			//appends rest of arr1 to resarr
			for(int q = k; q <size1 + size2; q++) resarr[q] = arr1[c1++];
			break;
		}else if(c2 < size2){
			//if instead end of arr1 is reached
			//appends rest of arr2 to resarr
			for(int q = k; q < size1 + size2; q++) resarr[q] = arr2[c2++];
			break;
		}
	}
}

void mergesort(int *arr, int *retarr, int size){
	if(size > 1){
		//select a point of split
		int pos = size/2;
		int a1[pos], a2[size-pos];

		for(int i = 0; i < pos; i++){
			a1[i] = arr[i];
		}
		for(int i = pos; i < size; i++){
			a2[i - pos] = arr[i];
		}
		//a1 and a2 are arr split at index pos

		int a3[pos], a4[size -pos];
		mergesort(a1, a3, pos);
		mergesort(a2, a4, size- pos);
		merge(a3, a4, retarr, pos, size-pos);
		//merges in place, read retarr for result

	}else{
		retarr[0] = arr[0];
		//pretty straightforward
	}
}

void test2(int len){
	int ab[len];
	srand(time(0));
	for(int i = 0; i < len; i++){
		ab[i] = rand()%1000000;
	}
	int size_a = sizeof(ab)/sizeof(int);
	int ra[size_a];
	mergesort(ab, ra, size_a);
}

int main(){

	int sample_size = 10;

	double last_diff = 1;
	for(int i =0; i < 10; i ++){
		//perfomance measurement
		struct timespec t1, t2;
		timespec_get(&t1, TIME_UTC);

		test2(sample_size);

		timespec_get(&t2, TIME_UTC);
		double diff = t2.tv_nsec - t1.tv_nsec;
		diff += t2.tv_sec - t1.tv_sec;

		printf("%f \t\t%f \t\t%f\n", diff, diff/last_diff, last_diff);
		last_diff = diff;
		sample_size *= 3;
	}
	return 0;
}