#include <stdio.h>

/*so, this takes a number less than 1000
and returns the string representaion*/

int run(){
	printf("Enter a Number\n");
	int num;
	scanf("%d", &num);
	// num = 941;
	int h = num / 100;
	num %= 100;

	if(h){
		char* hh;
		switch(h){
			case 1:
				hh = "One"; 
				break;
			case 2:
				hh = "Two"; 
				break;
			case 3:
				hh = "Three"; 
				break;
			case 4:
				hh = "Four"; 
				break;
			case 5:
				hh = "Five"; 
				break;
			case 6:
				hh = "Six"; 
				break;
			case 7:
				hh = "Seven"; 
				break;
			case 8:
				hh = "Eight"; 
				break;
			case 9:
				hh = "Nine"; 
				break;
		}
		char *h1 = " hundred";


		printf("%s%s", hh, h1);
	}

	if(num){
		char* h1 = " and ";
		if(h) printf("%s", h1);

		char* rem;
		if(num >= 20){
			int rr = (num - 20) / 10 + 2;
			switch(rr){
				case 2:
					rem = "Twenty "; 
					break;
				case 3:
					rem = "Thirty "; 
					break;
				case 4:
					rem = "Forty "; 
					break;
				case 5:
					rem = "Fifty "; 
					break;
				case 6:
					rem = "Sixty "; 
					break;
				case 7:
					rem = "Seventy "; 
					break;
				case 8:
					rem = "Eighty "; 
					break;
				case 9:
					rem = "Ninety "; 
					break;
			}
			num %= 10;
		}else if(num >= 10){
			switch(num){
				case 10:
					rem = "Ten";
					break;
				case 11:
					rem = "Eleven"; 
					break;
				case 12:
					rem = "Twelve"; 
					break;
				case 13:
					rem = "Thirteen"; 
					break;
				case 14:
					rem = "Fourteen"; 
					break;
				case 15:
					rem = "Fifteen"; 
					break;
				case 16:
					rem = "Sixteen"; 
					break;
				case 17:
					rem = "Seventeen"; 
					break;
				case 18:
					rem = "Eighteen"; 
					break;
				case 19:
					rem = "Nineteen"; 
					break;
			}
			num = 0;
		}
		printf("%s", rem);
		if(num){
			char* nn;
			switch(num){
				case 1:
					nn = "One"; 
					break;
				case 2:
					nn = "Two"; 
					break;
				case 3:
					nn = "Three"; 
					break;
				case 4:
					nn = "Four"; 
					break;
				case 5:
					nn = "Five"; 
					break;
				case 6:
					nn = "Six"; 
					break;
				case 7:
					nn = "Seven"; 
					break;
				case 8:
					nn = "Eight"; 
					break;
				case 9:
					nn = "Nine"; 
					break;
			}
			printf("%s", nn);
		}
	}
	printf("\n");

	return 0;
}

int main(int argc, char const *argv[]){
	run();
	return 0;
}