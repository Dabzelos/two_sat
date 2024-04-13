#pragma once


typedef struct variable {
    char name[3];
} variable;

typedef struct clause {
    variable* var1;
    variable* var2;
} clause;

typedef struct twosat {
    int num_vars;
    int num_clauses;
    clause* clauses;
} twosat;

//структура графа
typedef struct node {
    int val;
    struct node* next;
} node;
typedef struct list {
    node* head;
} list;
typedef struct graph {
    int count;
    list* adj_list;
} graph;


graph* graph_init(int n);
void graph_print(graph* g);
void graph_free(graph* g);
void add_arc(graph* g, int a, int b);
void add_edge(graph* g, int a, int b);
void del_arc(graph* g, int a, int b);


twosat* get_2_sat_cnf_form(const char* bl_form);
void implication_graph(int* arr, twosat* two_sat, graph* g);
bool* result_twosat(int* comp, twosat* two_sat);
int* graph_scc(graph* g);