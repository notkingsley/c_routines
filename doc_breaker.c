#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* so, this one breaks a given text string into char****,
 * delimited by \n, . and ' '. Actually twas from
 * hacckerank, breaking a doc into paragraphs and
 * those ones into sentences and those into words.
 * The problem is the resulting char**** holds no
 * info on how to access it's contents.
 */

//returns a sentece of char* words given char*
char** sent_breaker(char* sent){
	char del2 = ' ';
	int lp = strrchr(sent, del2) - sent;
	int p = strchr(sent, del2) - sent;
	int count = 1;
	int* poslist = (int*) malloc(count*sizeof(int));
	do{
		poslist = realloc(poslist, (count + 1)*sizeof(int));
		p = strchr(sent, del2) - sent;
		sent[p] = '!';
		poslist[count] = p;
		count++;
	}while(p < lp);
	poslist[0] = -1;
	poslist[count] = strlen(sent);

	char** warr = (char**) malloc((count - 1)*sizeof(char*));
	for(int i = 0; i < count; i++){
		int a = poslist[i];
		int b = poslist[i + 1];
		char* word = (char*) malloc((b -a)*sizeof(char));
		for(int c = 0; c < b - a - 1; c++){
			word[c] = sent[c + a + 1];
		}
		warr[i] = word;
	}

	return warr;
}

//returns a paragrah of char**s given a text char*
char*** par_breaker(char* par){
	char del2 = '.';
	int lp = strrchr(par, del2) - par;
	int p = strchr(par, del2) - par;
	int count = 1;
	int* poslist = (int*) malloc(count*sizeof(int));
	do{
		poslist = realloc(poslist, (count + 1)*sizeof(int));
		p = strchr(par, del2) - par;
		par[p] = '!';
		poslist[count] = p;
		count++;
	}while(p < lp);
	poslist[0] = -1;

	char*** sarr = (char***) malloc((count - 1)*sizeof(char**));
	for(int i = 0; i < count -1; i++){
		int a = poslist[i];
		int b = poslist[i + 1];
		char* sent = (char*) malloc((b -a)*sizeof(char));
		for(int c = 0; c < b - a - 1; c++){
			sent[c] = par[c + a + 1];
		}
		sarr[i] = sent_breaker(sent);
	}

	return sarr;

}

//start here
char**** get_document(char* text){
	char del2 = '\n';
	int lp = strrchr(text, del2) - text;
	int p = strchr(text, del2) - text;
	int count = 1;
	int* poslist = (int*) malloc(count*sizeof(int));
	do{
		poslist = realloc(poslist, (count + 1)*sizeof(int));
		p = strchr(text, del2) - text;
		text[p] = '!';
		poslist[count] = p;
		count++;
	}while(p < lp);
	poslist[0] = -1;
	poslist[count] = strlen(text);

	char**** parr = (char****) malloc((count - 1)*sizeof(char***));
	for(int i = 0; i < count; i++){
		int a = poslist[i];
		int b = poslist[i + 1];
		char* par = (char*) malloc((b -a)*sizeof(char));
		for(int c = 0; c < b - a - 1; c++){
			par[c] = text[c + a + 1];
		}
		parr[i] = par_breaker(par);
	}

	return parr;
}


int main(int argc, char const *argv[])
{
	char st[] = {"one two three four.five six seven eight."
	"nine ten eleven twelve.\nthirteen fourteen fifteen sixteen."
	"seventeen eighteen nineteen twenty.tone ttwo tthree tfour."};


	char**** st2 = get_document(st);
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 3; j++){
			for(int k = 0; k < 4; k++){
				printf("%s\n", st2[i][j][k]);
			}
		}
	}

	return 0;
}