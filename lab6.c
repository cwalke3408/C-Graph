/* lab6.c
 * Christian Walker
 * ECE 233 Spring 2014

 * Purpose: To create weakly connected, strongly connected, or random graph to
	    test Dijkstra's algorithm for shortest path and Network Diameter
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#include "graph.h"

#define TRUE 1
#define FALSE 0

int UnitNumber = -1;
int Graph = 1;
int Num_of_verts = 0;
int Approximate = 0;
int G_operation = 0;
int Source_vertex = -1;
int Destination = -1;
int Verbose = FALSE;
int Seed = 356;

void getCommandLine(int argc, char **argv);

int main(int argc, char **argv){

    getCommandLine(argc, argv);
    graph_t *G;
    srand48(Seed);

    if(Graph < 1 || Graph > 3){
    	Graph = 1;
    }
/*  Weakly-connected directed graph -g 1
	-n and -a are not used for this graph

*/
    if (Graph == 1){
        G = graph_construct(7);
	graph_add_edge(G, 1, 2, 3);
	graph_add_edge(G, 1, 6, 5);
	graph_add_edge(G, 2, 3, 7);
	graph_add_edge(G, 2, 4, 3);
	graph_add_edge(G, 2, 6, 10);
	graph_add_edge(G, 3, 4, 5);
	graph_add_edge(G, 3, 5, 1);
	graph_add_edge(G, 4, 5, 6);
	graph_add_edge(G, 5, 6, 7);
	graph_add_edge(G, 6, 0, 4);
	graph_add_edge(G, 6, 3, 8);
	graph_add_edge(G, 6, 4, 9);
    }
/*  Strongly-connected directed graph with N vertices.
	-g2 -n (# of vertices)
	-a -s -d are not used in this graph

*/
    else if(Graph == 2){
	    if(Num_of_verts < 2){
	        printf("use -n # to set number of vertices\n");
	        exit(1);
	    }
	    int nbr, i, w;
        G = graph_construct(Num_of_verts);

	    for(i = 0; i < Num_of_verts; i++){
	        for(nbr = 0; nbr < Num_of_verts; nbr++){
	        	if(nbr != i){
	        	    w = weight(i, nbr);
	        	    graph_add_edge(G, i, nbr, w);
	       	    }
	        }
	    }
    }

/*  Random graph.
    Vert 0 and N-1 are set vertices equal to (0,0) and (1,1)
    the rest are random vertices with x and y values between
    0 and 1.
	-g 3 -n (# of vertices) -a (approximate number of adjacent vertices)
*/
    else if(Graph == 3){
	if(Num_of_verts < 2){
	    printf("use -n # to set number of vertices\n");
	    exit(1);
	}

        cord_t * C;
        double x, y, radius, distance;
        int i,j;
    	int max_vert = 0;
	int min_vert = 10000;
	double num_verts = 0;
	double total_vert = 0;
	double avg;

        // add (0,0)
        G = graph_construct(Num_of_verts);
	C = cord_construct(Num_of_verts);

        radius = sqrt(Approximate/(Num_of_verts * 3.14));

        add_cord(C, 0, 0, 0);
        add_cord(C, Num_of_verts-1, 1,1);
        for(i = 1; i<Num_of_verts - 1; i++){
            x = drand48();
            y = drand48();
            add_cord(C, i, x, y);

        }

        for(i = 0; i<Num_of_verts; i++){
	    num_verts = 0;
            for(j = 0; j<Num_of_verts; j++){
                distance = Distance(C->vert[i]->x, C->vert[i]->y, C->vert[j]->x, C->vert[j]->y);
		if(distance == 0);
                else if(distance <= radius/2){
                    graph_add_edge(G,i,j,1);
		    total_vert++;
		    num_verts++;
                }
                else if(distance <= (3*radius)/4){
                    graph_add_edge(G,i,j,2);
		    total_vert++;
		    num_verts++;
                }
                else if(distance <= radius){
                    graph_add_edge(G,i,j,4);
		    total_vert++;
		    num_verts++;
                }
            }
	    if(num_verts > max_vert) max_vert = num_verts;
	    if(num_verts < min_vert) min_vert = num_verts;
        }

	avg = total_vert / Num_of_verts;

	printf("Max # of verts: %d    Min # of verts: %d  Avg: %.2f\n",
		max_vert, min_vert, avg);

	cord_destruct(C);

    }

/*  graph_shortest_path(G, 1);
    Find the shortest path between source and destination vertices.
    -h 1 -s (source) -d (destination)
*/
    if(G_operation == 1) {
	if(Source_vertex < 0 || Destination < 0){
	    printf("Use -s # and -d # to set source and destination\n");
	    graph_destruct(G);
	    exit(1);
	}
	if(Source_vertex >= G->entries || Destination >= G->entries){
	    printf("Source and Destination must be smaller than number of vertices");
	    graph_destruct(G);
	    exit(2);
	}
	    graph_t *S_list;
	    graph_t *reverse;
	    clock_t start, end;
	    int pre, i, j;
	    i = 0;

	    reverse = graph_construct(G->entries);

        start = clock();
	S_list = graph_shortest_path(G, Source_vertex);
	pre = Destination;
        if(S_list->vert_t[pre] == NULL){
	    printf("\tNo path from %d to %d\n", Source_vertex, Destination);
	    graph_destruct(G);
	    graph_destruct(reverse);
	    graph_destruct(S_list);
	    exit(3);
	}
	printf("Weight: %d\n", S_list->vert_t[pre]->weight);

	// flip the order so we can print from Source to Destination
        while(S_list->vert_t[pre] != NULL && i != G->entries){
	    graph_add_edge(reverse, i, pre, 0);
	    pre = S_list->vert_t[pre]->edge_src;
	    i++;
	}

	    printf("%d", Source_vertex);
	    for(j = i-1; j >= 0; j--){
	        printf(" -- %d", reverse->vert_t[j]->edge_src);
	    }
	    printf("\n");
	end = clock();
	printf(" TIME = %g\n", 1000*((double)(end-start))/CLOCKS_PER_SEC);

	graph_destruct(reverse);
	graph_destruct(S_list);

    }

/*   Find the Network Diameter (shortest-longest path of all vertices)
    -h2
    -s or -d is not need
*/
    else if(G_operation == 2){
	int max = 0;
    	int i, j;
	int unconnected_count;
	int graph_connected = 1;
	int pre = -1;
	int max_vert = 0;

	clock_t start, end;

	graph_t *current_SG;
	graph_t *max_graph;
	graph_t *max_r;

	max_r = graph_construct(G->entries);

        start = clock();
	for(i = 0; i < G->entries; i++){
	    current_SG = graph_shortest_path(G, i);
	    unconnected_count = 0;
            for(j = 0; j < G->entries; j++){
                if(current_SG->vert_t[j] == NULL) unconnected_count++;
                else if(current_SG->vert_t[j]->weight > max){
                    max = current_SG->vert_t[j]->weight;
		    pre = j;
		    max_vert = i;
                }
            }
	    if(unconnected_count > 1) graph_connected = 0;
	    graph_destruct(current_SG);
        }

 	max_graph = graph_shortest_path(G, max_vert);
	i = 0;

	if (pre != -1){
            while(max_graph->vert_t[pre] != NULL && i != G->entries){
	        graph_add_edge(max_r, i, pre, 0);
	        pre = max_graph->vert_t[pre]->edge_src;
	        i++;
    	    }
        }
	end = clock();

	if(graph_connected == 0) printf("-----Graph is unconnected------\n");
	printf("max: %d\n", max);

	if(max_r->vert_t[0] != NULL){
	    printf("Source: %d    Destination: %d\n", pre, max_r->vert_t[0]->edge_src);
	    printf("%d", pre);
	}

	for(j=i-1; j>=0; j--){
	    printf(" - %d", max_r->vert_t[j]->edge_src);
 	}
	printf("\n");

	printf(" TIME = %g\n", 1000*((double)(end-start))/CLOCKS_PER_SEC);

	graph_destruct(max_graph);
	graph_destruct(max_r);
    }

    if(Verbose == TRUE) graph_debug_print(G);

    graph_destruct(G);

    return 0;
}


void getCommandLine(int argc, char **argv)
{
    int c;

    while ((c = getopt(argc, argv, "g:n:r:a:h:s:d:veoru:")) != -1){
	switch(c) {
	    case 'u': UnitNumber = atoi(optarg);    break;
	    case 'g': Graph = atoi(optarg);	    break;
	    case 'n': Num_of_verts = atoi(optarg);  break;
	    case 'a': Approximate = atoi(optarg);   break;
	    case 'h': G_operation = atoi(optarg);   break;
	    case 's': Source_vertex = atoi(optarg); break;
	    case 'd': Destination = atoi(optarg);   break;
	    case 'v': Verbose = TRUE;		    break;
 	    case 'r': Seed = atoi(optarg);	    break;
	    default:
		printf("Lab6 command Line options\n");
		printf("--- General options ---\n");
                printf("    -g 0        Graph Type\n");
                printf("    -n N        Number of vertices in graph\n");
                printf("    -a A        Approximate number of adjacent vertices\n");
                printf("    -h[1|2]     Graph Operation\n");
                printf("    -s S        Number of the source vertex\n");
                printf("    -d D        Number of the destination vertex\n");
                printf("    -v          enable verbose output\n");
                printf("    -r 12345    seed for the uniform number generator\n");
                exit(1);
	}
    }
}
