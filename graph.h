/* Christian Walker
 * ECE 223 Spring 2014
 * graph.h

*/

#include <math.h>

typedef struct edge_node_tag{

    int edge_src;
    int weight;
    int pre_vert;
    int check;

    struct edge_node_tag *next;

} edge_node_t;

typedef struct cord_node_tag{
    int edge_src;
    double x;
    double y;
} cord_node_t;

typedef struct cord_tag {
    cord_node_t **vert;
    int entries;
} cord_t;

typedef struct graph_tag{

    edge_node_t **vert_t;
    int entries;

} graph_t;

void graph_add_edge(graph_t *G, int link_src, int link_dst, int weight);
void add_cord(cord_t *C, int link_src, double x, double y);
int weight(int link_src, int link_dest);
graph_t * graph_construct(int NumVertices);
cord_t * cord_construct(int NumVertices);
void graph_destruct(graph_t *G);
void cord_destruct(cord_t *C);
graph_t * graph_shortest_path(graph_t *G, int path_src);
void graph_debug_print(graph_t *G);
double Distance(double i_x, double i_y, double j_x, double j_y);
