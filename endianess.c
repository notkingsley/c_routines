#include <stdio.h>

typedef union unsignedInspect unsignedInspect;
union unsignedInspect{
	unsigned val;
	unsigned char bytes[sizeof(unsigned)];
};
int main(int argc, char const *argv[]){

	unsignedInspect twofold = {.val = 0xAAbbCCDD};

	printf("value is 0x%X\n", twofold.val);
	for(int i = 0; i < sizeof(twofold.bytes); i++){
		printf("byte[%d] : 0x%X\n", i, twofold.bytes[i]);
	}
	return 0;
}