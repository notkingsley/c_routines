#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*a supposed to be crude implementation
of graphs in c using structs and (likely)
lots of pointers. maybe find a way to render
later
update, it's not crude and yes pointers*/

typedef struct node node__t;
typedef struct edge edge__t;
typedef struct graph graph__t;
typedef struct partition par__t;
typedef struct path path__t;

//a fundamental unit representing
//a graph vertex
struct node{
	int* degree;
	char* name;
	node__t** neighbours;
	edge__t** edges;
};

//joins two nodes
struct edge{
	node__t* vertices[2];
	int multiplicity[1];
	int weight[1];
};

//a collection of nodes and edges
struct graph{
	int* size;
	int* edge_size;
	node__t** nodes;
	edge__t** edges;
};

//two disjoint sets of the same graph
struct partition{
	node__t** nodes[2];
	int s0[1];
	int s1[1];
};

//an ordered string of edges
struct path{
	edge__t** edges;
	int len[1];
	int weights[1];
};

//initialises and returns an empty graph
//to be freed using destroy_graph()
graph__t* graph_init(){
	graph__t* gp = malloc(sizeof(graph__t));
	gp->size = malloc(sizeof(int));
	gp->size[0] = 0;
	gp->edge_size = malloc(sizeof(int));
	gp->edge_size[0] = 0;
	gp->nodes = malloc(sizeof(node__t*));
	gp->edges = malloc(sizeof(edge__t*));
	return gp;
}

//initializes and returns a new node
//to be freed with destroy_node()
node__t* node_init(char* name){
	if(!name) name = "random name";

	node__t* np = malloc(sizeof(node__t));
	np->degree = malloc(sizeof(int));
	np->degree[0] = 0;
	np->name = name;
	np->neighbours = malloc(sizeof(node__t*));
	np->edges = malloc(sizeof(edge__t*));
	return np;
}

//changes the name of np to new
void set_name(node__t* np, char* new){
	if(new) np->name = new;
}

//finds and returns a common edge to n1 and n2
//returns NULL if none is found
edge__t* find_common_edge(node__t* n1, node__t* n2){
	for(int i = 0; i < *n1->degree; i++){
		for(int j = 0; j < *n2->degree; j ++){
			if(n1->edges[i] == n2->edges[j]){
				return n1->edges[i];
			}
		}
	}
	return NULL;
}

//joins two nodes n1 and n2 and returns the 
//linking newly created edge
edge__t* join_nodes(graph__t* gp, node__t* n1, node__t* n2){
	//create edge
	edge__t* ed = malloc(sizeof(edge__t));
	*ed->multiplicity = 1;
	ed->vertices[0]= n1;
	ed->vertices[1] = n2;
	*ed->weight = 1;

	//effect n1
	(*n1->degree)++;
	n1->neighbours = realloc(n1->neighbours, *n1->degree * sizeof(node__t*));
	n1->neighbours[*n1->degree - 1] = n2;
	n1->edges = realloc(n1->edges, *n1->degree * sizeof(edge__t*));
	n1->edges[*n1->degree - 1] = ed;

	//effect n2
	(*n2->degree)++;
	n2->neighbours = realloc(n2->neighbours, *n2->degree * sizeof(node__t*));
	n2->neighbours[*n2->degree - 1] = n1;
	n2->edges = realloc(n2->edges, *n2->degree * sizeof(edge__t*));
	n2->edges[*n2->degree - 1] = ed;

	//effect the common graph
	(*gp->edge_size)++;
	gp->edges = realloc(gp->edges, *gp->edge_size * sizeof(edge__t*));
	gp->edges[*gp->edge_size - 1] = ed;

	return ed; 
}

//finds and returns a node given its name
//if multiple nodes with the same name exist,
//the one created earliest is returned
//NULL is returned if none is found
node__t* find_node(graph__t* gp, char* name){
	for(int i = 0; i < *gp->size; i ++){
		if(!strcmp(gp->nodes[i]->name, name)) return gp->nodes[i];
	}
	return NULL;
}

//disconnects two nodes, does nothing if they weren't conected
void break_nodes(graph__t* gp, node__t* n1, node__t* n2){
	edge__t* ed = find_common_edge(n1, n2);
	if(!ed) return;

	//find n2's position in n1->neighbours
	int hold = *n1->degree;
	for(int i = 0; i < *n1->degree; i++){
		if(n1->neighbours[i] == n2){
			assert(n1->edges[i] == ed);
			hold = i;
			break;
		}
	}
	//shift everything after position hold one place left
	for(int j = hold; j < *n1->degree - 1; j++){
		n1->neighbours[j] = n1->neighbours[j + 1];
		n1->edges[j] = n1->edges[j +1];
	}
	(*n1->degree)--;
	n1->neighbours = realloc(n1->neighbours, *n1->degree * sizeof(node__t*));
	n1->edges = realloc(n1->edges, *n1->degree * sizeof(edge__t*));

	//find n1's position in n2->neighbours
	hold = *n2->degree;
	for(int i = 0; i < *n2->degree; i++){
		if(n2->neighbours[i] == n1){
			assert(n2->edges[i] == ed);
			hold = i;
			break;
		}
	}
	//shift everything after hold one place left
	for(int j = hold; j < *n2->degree - 1; j++){
		n2->neighbours[j] = n2->neighbours[j +1 ];
		n2->edges[j] = n2->edges[j +1];
	}
	(*n2->degree)--;
	n2->neighbours = realloc(n2->neighbours, *n2->degree * sizeof(node__t*));
	n2->edges = realloc(n2->edges, *n2->degree * sizeof(edge__t*));

	//find ed's position in gp->edges
	hold = *gp->edge_size;
	for(int i = 0; i < *gp->edge_size; i++){
		if(gp->edges[i] == ed){
			hold = i;
			break;
		}
	}
	//shift, again
	for(int j = hold; j < *gp->edge_size; j++){
		gp->edges[j] = gp->edges[j + 1];
	}
	(*gp->edge_size)--;
	gp->edges = realloc(gp->edges, *gp->edge_size * sizeof(edge__t*));

	free(ed);
}

//creates and adds a new node with the given name to gp
//name defaults to "random name" if NULL
//return the new node
node__t* add_new_node(graph__t* gp, char* name){
	node__t* np = node_init(name);
	(*gp->size)++;
	gp->nodes = realloc(gp->nodes, *gp->size * sizeof(node__t*));
	gp->nodes[*gp->size - 1] = np;
	return np;
}

//completely destroys a node n and all it's connections
//this includes all edges connected to n
void destroy_node(graph__t* gp, node__t* n){
	while(*n->degree){
		break_nodes(gp, n, n->neighbours[0]);
	}

	int hold = *gp->size;
	for(int i = 0; i < *gp->size; i++){
		if(gp->nodes[i] == n){
			hold = i;
			break;
		}
	}
	for(int j = hold; j < *gp->size - 1; j++){
		gp->nodes[j] = gp->nodes[j + 1];
	}
	(*gp->size)--;
	gp->nodes = realloc(gp->nodes, *gp->size * sizeof(node__t*));

	free(n->degree);
	free(n->edges);
	free(n->neighbours);
	free(n);
}

//completely destroys a graph and all it's internal nodes and edges
void destroy_graph(graph__t* gp){
	while (*gp->size){
		destroy_node(gp, gp->nodes[0]);
	}
	
	free(gp->size);
	free(gp->edges);
	free(gp->nodes);
	free(gp);
}

//creates and returns a new complete graph with size nodes
graph__t* complete_graph(int size){
	graph__t* gp = graph_init();
	for(int i = 0; i < size; i ++){
		node__t* np = add_new_node(gp, NULL);
		for(int j = 0; j < *gp->size -1; j++){
			join_nodes(gp, np, gp->nodes[j]);
		}
	}
	return gp;
}

//makes and returns a copy of a graph gp
//new graph has contents of gp after call
//and is independent henceforth
graph__t* copy_graph(graph__t* gp){
	graph__t* ret = graph_init();

	*ret->size = *gp->size;
	*ret->edge_size = 0;
	ret->nodes = realloc(ret->nodes, *ret->size * sizeof(node__t*));

	//initialize each node without setting up neighbours
	for(int i = 0; i < *ret->size; i++){
		char* nn = malloc(strlen(gp->nodes[i]->name) * sizeof(char));
		strcpy(nn, gp->nodes[i]->name);
		ret->nodes[i] = node_init(nn);
		*ret->nodes[i]->degree = 0;
	}

	//for each edge in gp, join vertices in ret
	for(int i = 0; i < *gp->edge_size; i++){
		//obtain corresponding locations in gp
		int h1, h2;
		for(int j = 0; j < *gp->size; j++){
			if(gp->nodes[j] == gp->edges[i]->vertices[0]){
				h1 = j;
				break;
			}
		}
		for(int j = 0; j < *gp->size; j++){
			if(gp->nodes[j] == gp->edges[i]->vertices[1]){
				h2 = j;
				break;
			}
		}
		edge__t* ed = join_nodes(ret, ret->nodes[h1], ret->nodes[h2]);
		*ed->weight = *gp->edges[i]->weight;
	}

	return ret;
}

//creates and returns a cycle with size nodes
//each node will be connected to the next and 
//the last back to the first
graph__t* cycle_graph(int size){
	graph__t* gp = graph_init();
	node__t* fnp = add_new_node(gp, NULL);
	node__t* lnp = fnp;
	for(int i = 0; i < size - 1; i++){
		node__t* np = add_new_node(gp, NULL);
		join_nodes(gp, lnp, np);
		lnp = np;
	}
	join_nodes(gp, fnp, lnp);
	return gp;
}

//creates and return a wheel graph
//containing size nodes
graph__t* wheel_graph(int size){
	graph__t* gp = cycle_graph(size -1);
	node__t* np = add_new_node(gp, NULL);
	for(int i = 0; i < *gp->size - 1; i++){
		join_nodes(gp, np, gp->nodes[i]);
	} 
	return gp;
}

//create and return a mesh graph
//the returned graph has size * size nodes 
graph__t* mesh_graph(int size){
	graph__t* gp = graph_init();
	gp->nodes = malloc(size * size * sizeof(node__t*));

	for(int i = 0; i < size * size; i++){
		add_new_node(gp, NULL);
	}
	for(int i = 0; i < size * size; i++){
		if(!(i%size == size - 1)){
			join_nodes(gp, gp->nodes[i], gp->nodes[i + 1]);
		}
		int hold = i / size;
		if(!(hold == size - 1)){
			join_nodes(gp, gp->nodes[i], gp->nodes[i + size]);
		}
	}
	return gp;
}

//construct a graph given it's adjacency matrix
//graph has size nodes and loops and multiple edges are 
//supported. only lower left of matrix is actually read
graph__t* construct(int size, int matrix[size][size], int weights[size][size]){
	graph__t* gp = graph_init();
	for(int i = 0; i < size; i++){
		node__t* np = add_new_node(gp, NULL);
		for(int j = 0; j <= i; j++){
			int count = matrix[i][j];
			while(count--){
				edge__t* ed = join_nodes(gp, gp->nodes[i], gp->nodes[j]);
				if(weights) ed->weight[0] = weights[i][j];
			}
		}
	}
	return gp;
}

//adds a copy of the contents of gp2 to gp1
//gp2 is unaffected. when done at least two
//connected components will be in gp1
void append(graph__t* gp1, graph__t* gp2){
	graph__t* cgp = copy_graph(gp2);

	gp1->nodes = realloc(gp1->nodes, 
		(*gp1->size + *cgp->size) * sizeof(node__t*));
	for(int i = *gp1->size; i < *gp1->size + *cgp->size; i++){
		gp1->nodes[i] = cgp->nodes[i - *gp1->size];
	}

	gp1->edges = realloc(gp1->edges, 
		(*gp1->edge_size + *cgp->edge_size) * sizeof(edge__t*));
	for(int i = *gp1->edge_size; i < *gp1->edge_size + *cgp->edge_size; i++){
		gp1->edges[i] = cgp->edges[i - *gp1->edge_size];
	}

	*gp1->size += *cgp->size;
	*gp1->edge_size += *cgp->edge_size;
}

//bipartition inner functions
int blue_add(graph__t* gp, graph__t* og, node__t** reds, node__t** blues, int* gpp, int* rdp, int* blp);
int red_add(graph__t* gp, graph__t* og, node__t** reds, node__t** blues, int* gpp, int* rdp, int* blp);

int red_add(graph__t* gp, graph__t* og, node__t** reds, node__t** blues, int* gpp, int* rdp, int* blp){
	//adds a node to reds. stops if already in red, throws 0 if already in blues
	for(int i = 0; i < *rdp; i++){
		if(reds[i] == gp->nodes[*gpp]) return 1;
	}
	for(int i = 0; i < *blp; i++){
		if(blues[i] == gp->nodes[*gpp]) return 0;
	}

	while(*gp->nodes[*gpp]->degree){
		//burning through the neighbours
		node__t* nn = gp->nodes[*gpp]->neighbours[0];
		while(find_common_edge(gp->nodes[*gpp], nn)){ //for multiplicity
			break_nodes(gp, gp->nodes[*gpp], nn);
		}
		reds[(*rdp)++] = og->nodes[*gpp];			//actual adding
		for(int i = 0; i < *gp->size; i++){
			if(nn == gp->nodes[i]){
				*gpp = i;							//adjust gpp
				break;
			}
		}
		if(!blue_add(gp, og, reds, blues, gpp, rdp, blp)) return 0;	//recursion
	}
	return 1;	//successful
}

int blue_add(graph__t* gp, graph__t* og, node__t** reds, node__t** blues, int* gpp, int* rdp, int* blp){
	//adds a node to blues. stops if already in blue, throws 0 if already in reds
	for(int i = 0; i < *rdp; i++){
		if(reds[i] == gp->nodes[*gpp]) return 0;
	}
	for(int i = 0; i < *blp; i++){
		if(blues[i] == gp->nodes[*gpp]) return 1;
	}

	while(*gp->nodes[*gpp]->degree){
		//burning through the neighbours
		node__t* nn = gp->nodes[*gpp]->neighbours[0];
		while(find_common_edge(gp->nodes[*gpp], nn)){ //for multiplicity
			break_nodes(gp, gp->nodes[*gpp], nn);
		}
		blues[(*blp)++] = og->nodes[*gpp];			//actual adding
		for(int i = 0; i < *gp->size; i++){
			if(nn == gp->nodes[i]){
				*gpp = i;							//adjust gpp
				break;
			}
		}
		if(!red_add(gp, og, reds, blues, gpp, rdp, blp)) return 0;	//recursion
	}
	return 1;	//successful
}

//seperates a graph gp into a bipartition
//->nodes[2] of par__t has the two partitions,
//*s0 and *s1 are the respective sizes. the two 
//are disjoint. returns NULL if
//there is no bipartiation
par__t* bipartition(graph__t* gp){
	node__t** reds = malloc(*gp->size * sizeof(node__t*));
	node__t** blues = malloc(*gp->size * sizeof(node__t*));
	graph__t* cgp = copy_graph(gp);
	int* rdp = malloc(sizeof(int));
	int* blp = malloc(sizeof(int));
	int* gpp = malloc(sizeof(int));
	*gpp = 0;
	*rdp = 0;
	*blp = 0;

	while(*cgp->size){
		if(!red_add(cgp, gp, reds, blues, gpp, rdp, blp)) return NULL;
		if(*rdp + *blp == *gp->size) break;		//all done

		//multiple components case
		for(int i = 0; i < *gp->size; i++){
			//find first missing node in gp
			int cont = 0;				//flow control
			for(int j = 0; j < *rdp; j++){
				if(reds[j] == gp->nodes[i]){
					cont = 1;
					break;
				}
			}
			if(cont) continue;
			for(int j = 0; j < *blp; j++){
				if(blues[j] == gp->nodes[i]){
					cont = 1;
					break;
				}
			}
			if(cont) continue;
			//i not found
			*gpp = i;
			break;
		}
	}
	par__t* ret;
	ret->nodes[0] = reds;
	ret->nodes[1] = blues;
	*ret->s0 = *rdp;
	*ret->s1 = *blp;

	free(cgp);
	free(gpp);
	free(rdp);
	free(blp);
	return ret;
}

//returns 1 if gp is bipartite, else 0
int isBipartite(graph__t* gp){
	return bipartition(gp) != NULL;
}

//splices a path p2 into p1 at position pos
//and returns the new merge
//p1->edges[pos] must correspond to the ends
//of p2, and p2 must be a circuit
path__t* splice_path(path__t* p1, path__t* p2, int pos){
	//working, but not integrated
	// assert(p1->edges[pos] == p2->edges[0]);
	// assert(p1->edges[pos] == p2->edges[*p2->len - 1]);

	p1->edges = realloc(p1->edges, (*p1->len + *p2->len - 1) * sizeof(edge__t*));
	for(int i = *p1->len - 1; i > pos; i--){
		p1->edges[i + *p2->len - 1] = p1->edges[i];
	}
	for(int i = 1; i < *p2->len; i ++){
		p1->edges[i + pos] = p2->edges[i];
	}
	*p1->len += *p2->len - 1;

	return p1;
}

//blindly makes a path through gp starting with np
//till a circuit is reached. particular path returned
//is dependent on internal structure of gp
//note: circuit is only guaranteed if euler 
//circuit exists in gp, else NULL is returned
//warning: this invalidates all existing external edge__t pointers
path__t* get_circuit(graph__t* gp, node__t* np){
	//working but not integrated
	path__t* pp = malloc(sizeof(path__t));
	pp->edges = malloc(sizeof(edge__t*));
	*pp->len = 0;

	graph__t* cgp = copy_graph(gp);

	node__t* lnp = np;
	do{
		if(!*lnp->degree) return NULL;
		// if(*lnp->degree % 2) return NULL;

		(*pp->len)++;
		pp->edges = realloc(pp->edges, *pp->len * sizeof(edge__t*));
		node__t* nn = lnp->neighbours[0];

		edge__t* ed = malloc(sizeof(edge__t*));
		edge__t* ed2 = find_common_edge(lnp, nn);
		*ed->multiplicity = *ed2->multiplicity;
		*ed->weight = *ed2->weight;
		ed->vertices[0] = ed2->vertices[0];
		ed->vertices[1] = ed2->vertices[1];
		pp->edges[*pp->len - 1] = ed;

		break_nodes(gp,lnp, nn);
		lnp = nn;
	}while(lnp != np);

	for(int i = 0; i < *pp->len; i++){
		join_nodes(gp, pp->edges[i]->vertices[0], pp->edges[i]->vertices[1]);
	}

	return pp;
}

//uses dijkstra's shortest path to find the shortest path
//between two nodes. will only work on weighted graphs
//nodes must be in gp
path__t* dijkstra(graph__t* gp, node__t* start, node__t* end){
	int absent1 = 1, absent2 = 1;
	for(int i = 0; i < *gp->size; i++){
		if(start == gp->nodes[i]) absent1 = 0;
		if(end == gp->nodes[i]) absent2 = 0;
	}
	if(absent1 || absent2) return NULL;

	//dummy node with edge
	node__t* dummy = node_init("dummy");
	edge__t* ed = join_nodes(gp, dummy, start);
	*ed->weight = 0;
	//order
	if(start == ed->vertices[0]){
		node__t* hold = ed->vertices[0];
		ed->vertices[0] = ed->vertices[1];
		ed->vertices[1] = hold;
		assert(start == ed->vertices[1]);
	}

	//init first path
	path__t* pp = malloc(sizeof(path__t));
	*pp->weights = 0;
	*pp->len = 1;
	pp->edges = malloc(sizeof(edge__t*));
	pp->edges[0] = ed;
	*pp->weights += *ed->weight;

	//add first path to pps
	path__t** pps = malloc(sizeof(path__t*));
	int pps_size = 0;
	pps[pps_size++] = pp;

	//keeping stock of nodes
	node__t* nps[*gp->size + 1];
	int nps_slider = 0;
	nps[nps_slider++] = ed->vertices[0];
	nps[nps_slider++] = ed->vertices[1];
	int count = 0;
	while(1){
		//examine all possible continuations of all paths in pps
		//add one with the shortest weights to pps
		//return this shortest path if it ends in end

		path__t* sppe = NULL;													//shortest path extension

		//let each path choose it's candidate
		for(int i = 0; i < pps_size; i++){
			pp = pps[i];															//path proposing

			node__t* nn = pp->edges[*pp->len - 1]->vertices[1];						//it's trailing node
			//to find the smallest edge this node has to offer
			//find first viable edge in nn->edges
			edge__t* ed2 = nn->edges[0];
			int v = 0;
			for(int j = 0; j < *nn->degree; j++){
				if(nn->edges[j] == pp->edges[*pp->len - 1]) continue;					//influx edge
				if(nn->edges[j]->vertices[0] == nn->edges[j]->vertices[1]) continue;	//loops

				//we've been this way before?
				int other = nn == nn->edges[j]->vertices[0] ? 1 : 0;
				int cont = 0;
				for(int k = 0; k < nps_slider; k++){
					if(nps[k] == nn->edges[j]->vertices[other]){
						cont = 1;
						break;
					}
				}
				if(cont) continue;
				
				ed2 = nn->edges[j];
				v = j;
				break;
			}
			for(int j = v + 1; j < *nn->degree; j++){
				if(nn->edges[j] == pp->edges[*pp->len - 1]) continue;					//influx edge
				if(nn->edges[j]->vertices[0] == nn->edges[j]->vertices[1]) continue;	//loops

				//we've been this way before?
				int other = nn == nn->edges[j]->vertices[0] ? 1 : 0;
				int cont = 0;
				for(int k = 0; k < nps_slider; k++){
					if(nps[k] == nn->edges[j]->vertices[other]){
						cont = 1;
						break;
					}
				}
				if(cont) continue;
				
				if(*ed2->weight > *nn->edges[j]->weight) ed2 = nn->edges[j];
			}
			//pendant case
			if(*nn->degree == 1) continue;

			//we've been this way before?
			int other = nn == ed2->vertices[0] ? 1 : 0;
			int cont = 0;
			for(int j = 0; j < nps_slider; j++){
				if(nps[j] == ed2->vertices[other]){
					cont = 1;
					break;
				}
			}
			if(cont) continue;

			//actually qualifies?
			if(sppe) if(*ed2->weight + *pp->weights >= *sppe->weights) continue;
			//cause the first one always qualifies

			//order this edge
			if(nn == ed2->vertices[1]){
				node__t* hold = ed2->vertices[0];
				ed2->vertices[0] = ed2->vertices[1];
				ed2->vertices[1] = hold;
				assert(nn == ed2->vertices[0]);
			}

			//construct new path
			path__t* nppe = malloc(sizeof(path__t));
			nppe->edges = malloc((*pp->len + 1) *sizeof(edge__t*));
			for(int j = 0; j < *pp->len; j++){
				nppe->edges[j] = pp->edges[j];
			}
			nppe->edges[*pp->len] = ed2;
			nppe->len[0] = pp->len[0] + 1;
			nppe->weights[0] = pp->weights[0] + ed2->weight[0];

			//maybe we're done here?
			if(ed2->vertices[1] == end){
				//free dummy and adjust nppe
				break_nodes(gp, start, dummy);
				for(int j = 0; j < *nppe->len - 1; j++){
					nppe->edges[j] = nppe->edges[j + 1];
				}
				(*nppe->len)--;
				free(dummy);
				free(sppe);
				return nppe;
			}

			//nah, update new best and continue
			free(sppe);
			sppe = nppe;

		}
		//something went terribly wrong and need to escape loop?
		if(count++ > *gp->size) return NULL;

		//in case all that ran and no update was made
		//likely cause there's no path
		if(!sppe) return NULL;
		//else sppe is now next best
		nps[nps_slider++] = sppe->edges[*sppe->len - 1]->vertices[1];
		pps = realloc(pps, (pps_size + 1) * sizeof(path__t*));
		pps[pps_size++] = sppe;
	}
}

//prints all connections np has
void print_node(node__t* np){
	int pos = 0;
	for(int i = 0; i < *np->degree;){
		edge__t* ed = find_common_edge(np, np->neighbours[pos]);
		for(int j = 0; j < *ed->multiplicity; j++){
			printf("%s is connected to %s with weight %d\n", 
				np->name, np->neighbours[pos]->name, *ed->weight);
			i++;
		}
		pos++;
	}
	printf("\n");
}

//prints all nodes in graph by printing each node
void print_graph(graph__t* gp){
	for(int i = 0; i < *gp->size; i++){
		print_node(gp->nodes[i]);
	}
}

//prints a path, node by node, edge by edge
void print_path(path__t* pp){
	for(int i = 0; i < *pp->len; i++){
		printf("%s to %s\n", pp->edges[i]->vertices[0]->name, pp->edges[i]->vertices[1]->name);
	}
}

void test(){
	graph__t* me = wheel_graph(5);

	char* name1 = "node1";
	char* name2 = "node2";
	char* name3 = "node3";
	char* name4 = "node4";

	set_name(me->nodes[0], name1);
	set_name(me->nodes[1], name2);
	set_name(me->nodes[2], name3);
	set_name(me->nodes[3], name4);

	graph__t* gr = complete_graph(3);

	char* name5 = "node5";
	char* name6 = "node6";
	char* name7 = "node7";
	set_name(gr->nodes[0], name5);
	set_name(gr->nodes[1], name6);
	set_name(gr->nodes[2], name7);

	graph__t* me2 = copy_graph(me);
	destroy_graph(me);

	append(me2, gr);
	print_graph(me2);
	destroy_node(me2, me2->nodes[1]);
	
	// print_node(me->nodes[1]);
	// print_node(me->nodes[2]);
	// print_node(me->nodes[3]);

	// print_node(me->nodes[0]);
	// print_node(find_node(me, "node3"));

	// edge__t* ee = find_common_edge(me->nodes[0], me->nodes[1]);
	edge__t* ee2 = me2->edges[0];
	print_node(ee2->vertices[0]);
	print_node(ee2->vertices[1]);

	destroy_graph(me2);
	destroy_graph(gr);
}

void test2(){
	int matrix[10][10] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 1, 0, 1, 1, 0, 0, 0, 0},
		{0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
		{0, 0, 0, 1, 0, 0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
	};

	int weights[10][10] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{3, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 5, 1, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 4, 2, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 5, 0, 6, 2, 0, 0, 0, 0},
		{0, 0, 0, 6, 1, 2, 0, 0, 0, 0},
		{0, 0, 0, 3, 0, 0, 0, 4, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
	};

	int matrix2[10][10] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
		{0, 1, 1, 0, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
	};

	int weights2[10][10] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{3, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{2, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{5, 2, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 2, 3, 0, 0, 0, 0, 0, 0},
		{0, 5, 3, 0, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 3, 2, 0, 0, 0, 0},
		{0, 0, 0, 0, 3, 0, 1, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 3, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 5, 3, 4, 0},
	};
	// graph__t* mee = mesh_graph(3);
	graph__t* mee = construct(10, matrix2, weights2);

	char* name1 = "node1";
	char* name2 = "node2";
	char* name3 = "node3";
	char* name4 = "node4";
	char* name5 = "node5";
	char* name6 = "node6";
	char* name7 = "node7";
	char* name8 = "node8";
	char* name9 = "node9";
	char* name10 = "node10";

	set_name(mee->nodes[0], name1);
	set_name(mee->nodes[1], name2);
	set_name(mee->nodes[2], name3);
	set_name(mee->nodes[3], name4);
	set_name(mee->nodes[4], name5);
	set_name(mee->nodes[5], name6);
	set_name(mee->nodes[6], name7);
	set_name(mee->nodes[7], name8);
	set_name(mee->nodes[8], name9);
	set_name(mee->nodes[9], name10);

	print_graph(mee);

	path__t* pp = dijkstra(mee, mee->nodes[0], mee->nodes[9]);
	printf("from %s to %s\n", mee->nodes[0]->name, mee->nodes[9]->name);
	if(pp){
		print_path(pp);
		printf("%d\n", pp->weights[0]);
	}

	destroy_graph(mee);
	return;

	graph__t* mee2 = cycle_graph(4);

	char* nameA = "nodeA";
	char* nameB = "nodeB";
	char* nameC = "nodeC";
	char* nameD = "nodeD";

	set_name(mee2->nodes[0], nameA);
	set_name(mee2->nodes[1], nameB);
	set_name(mee2->nodes[2], nameC);
	set_name(mee2->nodes[3], nameD);

	append(mee, mee2);

	join_nodes(mee, mee->nodes[1], mee->nodes[10]);

	path__t* p1 = get_circuit(mee, mee->nodes[1]);
	path__t* p2 = get_circuit(mee, mee->nodes[10]);
	print_path(p1);
	printf("}\n");
	print_path(p2);
	printf("{\n");

	p1 = splice_path(p1, p2, 1);
	print_path(p1);
	print_path(p2);

	print_node(mee->nodes[1]);
	print_node(mee->nodes[10]);

	par__t* bpt = bipartition(mee);
	if(!bpt){
		printf("no partition sha\n");
		return;
	}
	for(int i = 0; i < *bpt->s0; i++){
		printf("%s\n", bpt->nodes[0][i]->name);
		print_node(bpt->nodes[0][i]);
	}
	for(int i = 0; i < *bpt->s1; i ++){
		printf("%s\n", bpt->nodes[1][i]->name);
		print_node(bpt->nodes[1][i]);
	}
}

int main(int argc, char const *argv[]){
	test2();
	return 0;
}
