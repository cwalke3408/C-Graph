/* graph.c
   Christian Walker
   ECE 223 Spring 2014
   MP6

   Purpose:  To create a graph that can perform Dijkstra's alogrithm to
	     find the shortest path.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "graph.h"

/*  Construct the Graph
    Takes in the parameter NumVertices and construct a adjacey matrix
    and set all the positions to NULL
    Return a header block
*/
graph_t * graph_construct(int NumVertices){

    graph_t *G = NULL;
    int i;

    G = (graph_t *) malloc(sizeof(graph_t));

    G->vert_t = (edge_node_t **) malloc(NumVertices * sizeof(edge_node_t *));
    for (i = 0; i < NumVertices; i++){
	G->vert_t[i] = NULL;
    }
    G->entries = NumVertices;

    return G;

}
/*  Create a header block to store the cords for the random graph
    Create a list for the (x,y) varibles so the distance can be
   calculated and then finally added to the graph with neighs.

*/
cord_t * cord_construct(int NumVertices){
    cord_t * C = NULL;
    int i;

    C = (cord_t *) malloc(sizeof(graph_t));

    C->vert = (cord_node_t **) malloc(NumVertices * sizeof(cord_node_t *));
    for(i = 0; i<NumVertices; i++){
	C->vert[i] = NULL;
    }

    C->entries = NumVertices;

    return C;
}

/*  Frees the memory block from the header block and  edge_node_t block
    Rove to get all G->vert_t[i] memory blocks, free G->vert_t array
    free the header block
*/
void graph_destruct(graph_t *G){

    int i;

    edge_node_t *rover;
    for(i = 0; i < G->entries; i++){
	while(G->vert_t[i] != NULL){
	    rover = G->vert_t[i];
	    G->vert_t[i] = rover->next;
	    free(rover);
	}
    }
    free(G->vert_t);
    free(G);
}

/*  Frees the memory block for the cord_list.
    Frees the each C->vert[i] block, then C->vert array that hold the pointers
    and then the header block

*/
void cord_destruct(cord_t *C){
    cord_node_t *temp;
    int i;

    for(i = 0; i < C->entries; i++){
	temp = C->vert[i];
	free(temp);
    }
    free(C->vert);
    free(C);
}

/*  Add position (x,y) to the list in the position in the array in order from
    the time this function is called.
*/
void add_cord(cord_t *C, int link_src, double x, double y){

    cord_node_t *new_cord = NULL;
    new_cord = (cord_node_t *) malloc(sizeof (cord_node_t));
    C->vert[link_src] = new_cord;
    new_cord->x = x;
    new_cord->y = y;

}

/*  Add vertice with the neighbor number position and the weight of neigh
    Add one vert at a time to the Graph with the weight between.
    Store in the vert at the array position G->vert_t[i]

*/
void graph_add_edge(graph_t *G, int link_src, int link_dst, int weight){

    // intialize
    edge_node_t *new_neigh = NULL;
    edge_node_t *rover;
    edge_node_t *prev;

    // add vertice

    rover = G->vert_t[link_src];
    prev  = G->vert_t[link_src];

    // Rove to the end of the list and add neighbor
    while(rover != NULL){
	    prev = rover;
 	    rover = rover->next;
    }

    // add weight
    new_neigh = (edge_node_t *) malloc(sizeof (edge_node_t));
    new_neigh->next = NULL;
    if(prev == NULL) G->vert_t[link_src] = new_neigh;
    else prev->next = new_neigh;
    new_neigh->edge_src = link_dst;
    new_neigh->weight = weight;
    new_neigh->check = 0;
}

/*  Finds the shortest distance between to vertices by using Gijktra's 
    algorthm.

*/
graph_t * graph_shortest_path(graph_t *G, int path_src){

    graph_t *short_list;
    graph_t *redbox_list;
    int cost, neigh, min_cost, extra, i, edge;
    edge_node_t *rover;


    //Create new list for storing shortest path
    short_list = graph_construct(G->entries);
    redbox_list = graph_construct(G->entries);

    min_cost = path_src;
    extra = 0;

   //Loop until all connected paths have been checked
    while(extra != 1000000){

    	rover = G->vert_t[min_cost];

    	while(rover != NULL){
    	    edge = rover->edge_src;
    	    if(redbox_list->vert_t[edge] != NULL){
        	if(rover->weight + extra < redbox_list->vert_t[edge]->weight){
        	    redbox_list->vert_t[edge]->weight = rover->weight + extra;
        	    redbox_list->vert_t[edge]->edge_src = min_cost;
        	}
    	    }
    	    else if(rover->edge_src == path_src);
    	    else {
    	        cost = extra + rover->weight;
    	        neigh = rover->edge_src;
        		graph_add_edge(redbox_list, neigh, min_cost, cost);
    	    }

    	    rover = rover->next;

    	}

    	// find smallest path
    	extra = 1000000;
    	for(i = 0; i < G->entries; i++){
	    if(redbox_list->vert_t[i] == NULL);
	    else if(redbox_list->vert_t[i]->check == 1);
	    else if(redbox_list->vert_t[i]->weight < extra){
	        min_cost = i;
	        extra = redbox_list->vert_t[i]->weight;
	    }
	}
    	if(extra == 1000000) break;

	redbox_list->vert_t[min_cost]->check = 1;

   	// add smallest path to short_list
	graph_add_edge(short_list, min_cost, redbox_list->vert_t[min_cost]->edge_src,
	redbox_list->vert_t[min_cost]->weight);
    }

    graph_destruct(redbox_list);

    return (short_list);
}

/*  A function to find the weight between the vertice and the edge for
    strongly connected graph.

*/
int weight(int link_src, int link_dest){
    int w, neg;
    signed int non_neg;

    non_neg = link_src - link_dest;
    if(non_neg < 0){
    	neg = non_neg * -2;
    	non_neg = neg + non_neg;
    }
    w = non_neg+(link_src-link_dest+2)*(link_src-link_dest+2)+3*link_dest;

    return w;

}

/*  Calculate the weight between the vertice and the edge in the 
    random graph.

*/
double Distance(double i_x, double i_y, double j_x, double j_y){
    double distance;
    distance = sqrt((i_x-j_x)*(i_x-j_x)+(i_y-j_y)*(i_y-j_y));
    return distance;
}

//  Prints out the graph vertices and its neighbors.

void graph_debug_print(graph_t *G){

    edge_node_t *rover;
    int i;

    for(i = 0; i < G->entries; i++){
	printf("\nVert: %d ", i);
	if(G->vert_t[i] != NULL){
	    rover = G->vert_t[i];
            while(rover != NULL){
	            printf("Edge: %d Weight: %d ", rover->edge_src, rover->weight);
	            rover = rover->next;
	    }
	}
    }
    printf("\n");

}
