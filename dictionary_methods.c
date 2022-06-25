#include <stdio.h>
#include <stdlib.h>

/*an attempt to reproduce the python
 *dictionary data structure and its 
 *various methods
 *it's okay, but NOT type safe.
*/

typedef struct dict dict__t;
typedef struct value value__t;
typedef struct key key__t;
typedef struct item item__t;

//a collection of pointers to items
struct dict{
	int* size;
	item__t** items;
	dict__t* ptr; //self reference ptr for free
};

//a structure containing a key and it's
//corresponding value
struct item{
	key__t* key;
	value__t* value;
};

//a void pointer and something to remember it by
struct key{
	int keytype;	//0 or 1 for int or char
	void* key;
};

//a void pointer and something to remember it by
struct value{
	int valuetype;	//0 to 5 for supported values
	void* value;
	int vlen; //length of arr if value is an array, else 0
};

//creates and return an empty dict
dict__t init_dict(void){
	dict__t* dd;
	dd = (dict__t*) malloc(sizeof(dict__t));
	dd->size = (int*) malloc(sizeof(int));
	dd->size[0] = 0;
	dd->items = (item__t**) malloc(sizeof(item__t*));
	dd->ptr = dd;
	return *dd;
}

//returns a vague view on d's keys
//the returned array is pointers to the key_t objects
//not pointer to key values
//need to call free after use
void** get_keys(dict__t d){
	void** ret = (void**) malloc(sizeof(void*) * *d.size);
	for(int i = 0; i < *d.size; i ++){
		ret[i] = d.items[i]->key;
	}
	return ret;
}

//adds a key key of type keytype to dict d, with the corresponding value
//keytype is 1 for int, 0 for char, anything else for void
//valuetype is 0, 1, 2, 3, 4, 5 for int, int*, char, char*, double, and double* respectively
//anything else for void. vlen is not neccesary
void dict_append(dict__t d, int keytype, void* key, int valuetype, void* value, int vlen){
	(*d.size)++;
	d.items = (item__t**) realloc(d.items, sizeof(item__t*) * *d.size);

	key__t* km = (key__t*) malloc(sizeof(key__t));
	km->keytype = keytype;
	if(keytype) km->key = (int*) key;
	else if(km->keytype == 0) km->key = (char*) key;
	else km->key = (void*) key;

	value__t* vm = (value__t*) malloc(sizeof(value__t));
	vm->valuetype = valuetype;
	vm->vlen = 0;
	switch(valuetype){
		case 0:
			vm->value = (int*) value;
			break;
		case 1:
			vm->value = (int**) value;
			vm->vlen = vlen;
			break;
		case 2:
			vm->value = (char*) value;
			break;
		case 3:
			vm->value = (char**) value;
			vm->vlen = vlen;
			break;
		case 4:
			vm->value = (double*) value;
			break;
		case 5:
			vm->value = (double**) value;
			vm->vlen = vlen;
			break;
		default:
			vm->value = (void*) value;
			break;
	}

	int pos = *d.size - 1;
	item__t* it = (item__t*) malloc(sizeof(item__t));
	it->key = km;
	it->value = vm;
	d.items[pos] = it;
}

//completely delete dictionary d
void dict_destroy(dict__t d){
	for(int i = 0; i < *d.size; i++){
		free(d.items[i]->key);
		free(d.items[i]->value);
		free(d.items[i]);
	}
	free(d.items);
	free(d.size);
	free(d.ptr);
}

//make and return a copy of a dictionary
dict__t dict_copy(dict__t old){
	dict__t new = init_dict();
	*new.size = *old.size;
	new.items = (item__t**) realloc(new.items, sizeof(item__t*) * *new.size);

	for(int i = 0; i < *new.size; i++){
		key__t* km = (key__t*) malloc(sizeof(key__t));
		km->keytype = old.items[i]->key->keytype;
		km->key = old.items[i]->key->key;

		value__t* vm = (value__t*) malloc(sizeof(value__t));
		vm->valuetype = old.items[i]->value->valuetype;
		vm->value = old.items[i]->value->value;

		item__t* it = (item__t*) malloc(sizeof(item__t));
		it->key = km;
		it->value = vm;

		new.items[i] = it;
	}
	return new;
}

//find and returns the void pointer to the
//value of k in d. NULL if key is not present
void* get_value(dict__t d, key__t* k){
	void** keys = get_keys(d);
	for(int i = 0; i < *d.size; i++){
		if(keys[i] == k){
			free(keys);
			return d.items[i]->value->value;
		}
	}
	return NULL;
}

//returns an array of d's items
//not intended for use
inline item__t** get_items(dict__t d){
	return d.items;
}

//update the value of a key already in d, 
//does nothing if key is not present
void update_key(dict__t d, key__t* k, void* val){
	void* ptr = get_value(d, k);
	if(ptr){
		void** keys = get_keys(d);
		for(int i = 0; i < *d.size; i++){
			if(keys[i] == k){
				free(keys);
				d.items[i]->value->value = val;
				return;
			}
		}
		free(keys);
	}
}

//get and return the key structure associated with key in d
//return NULL if key is not found
//not reliable for void* keys
key__t* get_key_t(dict__t d, void* key, int keytype){
	for(int i = 0; i < *d.size; i++){
		if(keytype){
			int* a = (int*) d.items[i]->key->key;
			int* b = (int*) key;

			if(*a == *b){
				key__t* ret = (key__t*) d.items[i]->key;
				return ret;
			}
		}else{
			char* a = (char*) d.items[i]->key->key;
			char* b = (char*) key;

			if(*a == *b){
				key__t* ret = (key__t*) d.items[i]->key;
				return ret;
			}
		}
	}
	return NULL;
}

//return the item structure that k is contained in
//return NULL if isn't in d
item__t* get_item(dict__t d, key__t *k){
	void** keys = get_keys(d);
	for(int i = 0; i < *d.size; i++){
		if(keys[i] == k){
			free(keys);
			return d.items[i];
		}
	}
	free(keys);
	return NULL;
}

//removes and returns the item k is contained in
item__t* pop_item(dict__t d, key__t* k){
	void** keys = get_keys(d);
	for(int i = 0; i < *d.size; i++){
		if(keys[i] == k){
			free(keys);
			item__t* it = d.items[i];
			for(int j = i; j < *d.size - 1; j++){
				d.items[j] = d.items[j + 1];
			}
			(*d.size)--;
			d.items = (item__t**) realloc(d.items, sizeof(item__t*) * *d.size);
			return it;
		}
	}
}

//print the value of key k
//only valid if k isn't stored as void* wrt to keytype
void print_key(key__t k){
	if(k.keytype){
		int* kk = (int*) k.key;
		printf("%d", *kk); 
	}
	else{
		char* kk = (char*) k.key;
		printf("%c", *kk);
	}
}

//prints a value if the valuetype is supported (0 - 5)
void print_value(value__t v){
	switch(v.valuetype){
		case 0:
			int* vv0 = (int*) v.value;
			printf("%d ", *vv0);
			break;
		case 1:
			int** vv1 = (int**) v.value;
			for(int i = 0; i < v.vlen; i++){
				printf("%d ", *vv1[i]);
			}
			break;
		case 2:
			char* vv2 = (char*) v.value;
			printf("%c ", *vv2);
			break;
		case 3:
			char** vv3 = (char**) v.value;
			printf("%s", *vv3);
			break;
		case 4:
			double* vv4 = (double*) v.value;
			printf("%g ", *vv4);
			break;
		case 5:
			double* vv5 = (double*) v.value;
			for(int i = 0; i < v.vlen; i++){
				printf("%g ", vv5[i]);
			}
			break;
	}
}

//prints the contents of it using print_key and print_value
void print_item(item__t it){
	print_key(*it.key);
	printf(": ");
	print_value(*it.value);
	printf("\n");
}

//prints all items of d using print_item
void print_dict(dict__t d){
	for(int i = 0; i < *d.size; i++){
		print_item(*d.items[i]);
	}
}

//dev test of above functions. nothing you'll want to run
void test(){
	dict__t me = init_dict();

	int key1 = 5, key2 = 6;
	char char_value = 'f';
	char* str_value = "strong";

	char char_key = 'a', char_key2 = 'd';
	double arr1[] = {4.5, 2.3, 5.6, 76.9};
	double d2 = 3.142;


	dict_append(me, 1, &key1, 3, &str_value, 7);
	dict_append(me, 1, &key2, 2, &char_value, 0);
	//dict_append(me, 0, &char_key, 5, &arr1, 3);
	//printf("%p \n", me.items[2]->value->value);	
	dict_append(me, 0, &char_key2, 4, &d2, 0);

	// char* var_key = (char*) me.items[2]->key->key;
	// double* var_val = (double*) me.items[2]->value->value;
	// printf("%c %g\n", *var_key, var_val[1]);

	// int* var_key2 = (int*) me.items[1]->key->key;
	// char* var_val2 = (char*) me.items[1]->value->value;
	// printf("%d %c\n", *var_key2, *var_val2);

	int num = 6;
	printf("%p\n", me.items[1]);
	printf("%p\n", pop_item(me, get_key_t(me, &num, 1)));

	char* new = "neew";
	//update_key(me, get_key_t(me, &num, 1), &new);
	char* val_str = (char*) get_value(me, me.items[1]->key);
	if(val_str) printf("pass %c\n", *val_str);

	dict__t me2 = dict_copy(me);
	//print_dict(me);
	dict_destroy(me);
	print_dict(me2);
	dict_destroy(me2);
}

int main(int argc, char const *argv[]){
	// test();
	return 0;
}