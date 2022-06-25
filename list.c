#include <stdio.h>
#include <stdlib.h>

/*again, a copy of python's list
unlike c's arrays, the lists pretty much 
take care of themselves
the list can only take pointers, and stores 
them void*, so the user has to track the types
memcheck  clean ;)*/

typedef struct list list__t;
struct list{
	int max_size[1];
	int size[1];
	void** contents;
};

list__t* list_init(){
	//creates and returns an empty list
	//use init then extend to initialize list 
	//with array elemnents
	list__t* ret = malloc(sizeof(list__t));
	*ret->max_size = 1;
	*ret->size = 0;
	ret->contents = malloc(sizeof(void*));
	return ret;
}

void append(list__t* list, void* object){
	//append object to end of list
	if(*list->size >= *list->max_size){
		*list->max_size *= 2;
		list->contents = realloc(list->contents, *list->max_size * sizeof(void*));
	}
	list->contents[(*list->size)++] = object;
}

void append_list(list__t* list, list__t* source){
	//appends source to the end of list
	while(*list->size + *source->size >= *list->max_size){
		*list->max_size *= 2;
		list->contents = realloc(list->contents, *list->max_size * sizeof(void*));
	}
	for(int i = 0; i < *source->size; i++){
		list->contents[(*list->size)++] = source->contents[i];
	}
	// list->contents[*list->size - 1] = list->contents[0];
}

void clear(list__t* list){
	//removes everything from list
	//list remains intact
	for(int i = 0; i < *list->size; i++){
		list->contents[i] = NULL;
	}
	*list->size = 0;
	*list->max_size = 1;
	list->contents = realloc(list->contents, *list->max_size * sizeof(void*));
}

list__t* copy(list__t* list){
	//returns a copy of list
	//the copy is only a different object
	//pointing to the same objects
	list__t* copy = list_init();
	*copy->max_size = *list->max_size;
	*copy->size = *list->size;
	copy->contents = realloc(copy->contents, *copy->max_size * sizeof(void*));
	for(int i = 0; i < *copy->size; i++){
		copy->contents[i] = list->contents[i];
	}
	return copy;
}

int count(list__t* list, void* value){
	//returns the number of occurences of value in list
	//as void pointers can't be dereferenced without further
	//info, different pointers pointing to same object
	//get counted differently
	int ret = 0;
	for(int i = 0; i < *list->size; i++){
		if(list->contents[i] == value) ret++;
	}
	return ret;
}

void extend(list__t* list, void** arr, int len){
	//appends arr to end of list, returns (new) list
	while(*list->size + len >= *list->max_size){
		*list->max_size *= 2;
		list->contents = realloc(list->contents, *list->max_size * sizeof(void*));
	}
	for(int i = 0; i < len; i++){
		list->contents[(*list->size)++] = arr[i];
	}
}

void* get(list__t* list, int index){
	//returns the item at position index
	//NULL if index is out of range
	if(index < 0 || index >= *list->size) return NULL;
	return list->contents[index];
}

int get_index(list__t* list, void* value){
	//returns position of the first occurence of value in list
	// -1 is returned if value isn't in list
	for(int i = 0; i < *list->size; i++){
		if(list->contents[i] == value) return i;
	}
	return -1;
}

void insert(list__t* list, int index, void* object){
	//insert object at position index in list
	//everything at and after index is shifted left
	if(index >= *list->size){
		append(list, object);
		return;
	}
	if(index < 0) return;
	if(*list->size >= *list->max_size){
		*list->max_size *= 2;
		list->contents = realloc(list->contents, *list->max_size * sizeof(void*));
	}
	for(int i = *list->size; i > index; i--){
		list->contents[i] = list->contents[i - 1];
	}
	list->contents[index] = object;
	(*list->size)++;
}

void* pop(list__t* list, int index){
	//removes and returns the element at position index
	//NULL is returned if parameter error
	if(index < 0 || index >= *list->size) return NULL;
	void* hold = list->contents[index];
	for(int i = index; i < *list->size; i++){
		list->contents[i] = list->contents[i + 1];
	}
	(*list->size)--;
	if(*list->size <= *list->max_size/2){
		*list->max_size /= 2;
		list->contents = realloc(list->contents, *list->max_size * sizeof(void*));
	}
	return hold;
}

void remove_object(list__t* list, void* value){
	//removes the first occurence of value in list
	pop(list, get_index(list, value));
}

void reverse(list__t* list){
	//reverses in place
	// void** hold = malloc(*list->size * sizeof(void*));
	void* hold[*list->size];
	for(int i = 0; i < *list->size; i++){
		hold[i] = list->contents[i];
	}
	for(int i = 0; i < *list->size; i++){
		list->contents[i] = hold[*list->size - i - 1];
	}
}

void set(list__t* list, int index, void* object){
	//replaces value at position index with object
	//appends if index is too big, does nothing if < 0
	if(index >= *list->size){
		append(list, object);
		return;
	}
	if(index < 0) return;
	list->contents[index] = object;
}

list__t* slice(list__t* list, int start, int end){
	//returns a slice of list including start but not end
	//same as copy if start = 0 and end = *list->size
	if(end > *list->max_size) end = *list->max_size;
	if(start < 0) start = 0;

	list__t* ret = list_init();
	*ret->size = end - start;
	while(*ret->size >= *ret->max_size) *ret->max_size *= 2;
	ret->contents = realloc(ret->contents, *ret->max_size * sizeof(void*));

	for(int i = start; i < end; i++){
		ret->contents[i - start] = list->contents[i];
	}
	return ret;
}

void printstrlist(list__t* list){
	//test function
	for(int i = 0; i < *list->size; i++){
		char* str = list->contents[i];
		printf("%s\n", str);
	}
	printf("\n");
}

void test(){
	list__t* me = list_init();
	list__t* me2 = list_init();
	char* s1 = "string1";
	char* s2 = "string2";
	char* s3 = "string3";
	char s4[] = "string4";
	char* s5 = "strang5";
	char* s6 = "string6";

	char* sarr[] = {s1, s2, s3, s4, s5, s6};

	append(me, s1);
	append(me, s4);
	append(me, s6);

	append(me2, s2);
	append(me2, s3);
	append(me2, s4);
	append(me2, s5);
	append(me2, s6);

	list__t* me3 = copy(me2);

	append_list(me, me3);
	extend(me, (void**)sarr, 4);
	s4[4] = 'k';
	insert(me, 8, s4);
	printf("%s\n", (char*) pop(me, 11));
	remove_object(me, s4);
	reverse(me);
	set(me, 2, s6);
	printstrlist(me);
	printf("%d %d\n", count(me, s4), get_index(me, s4));
	list__t* slc = slice(me, 3, 9);
	printstrlist(slc);
	clear(me);
	clear(me2);
	clear(me3);

	free(slc->contents);
	free(slc);
	free(me->contents);
	free(me);
	free(me2->contents);
	free(me2);
	free(me3->contents);
	free(me3);
}

int main(int argc, char const *argv[]){
	// test();
	return 0;
}
