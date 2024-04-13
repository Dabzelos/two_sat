#include "sat_graph.h"
#include <iostream>
using namespace std;


graph* graph_init(int n) {
    graph* g = (graph*)malloc(sizeof(graph));
    g->count = n;
    g->adj_list = (list*)malloc(n * sizeof(list));
    for (int i = 0; i < n; i++) {
        g->adj_list[i].head = NULL;
    }
    return g;
}
void graph_print(graph* g) {
    for (int i = 0; i < g->count; i++) {
        node* cur = g->adj_list[i].head;
        cout << i << ": ";
        while (cur != NULL) {
            cout << cur->val << " ";
            cur = cur->next;
        }
        cout << endl;
    }
}
void graph_free(graph* g) {
    for (int i = 0; i < g->count; ++i) {
        node* temp;
        while (g->adj_list[i].head != NULL) {
            temp = g->adj_list[i].head;
            g->adj_list[i].head = g->adj_list[i].head->next;
            free(temp);
        }
    }
    free(g->adj_list);
    free(g);
}
void add_arc(graph* g, int a, int b) {
    if (a < g->count && b < g->count) {
        node* current_node = g->adj_list[a].head;
        while (current_node) {
            if (current_node->val == b) {
                return;
            }
            current_node = current_node->next;
        }
        node* pNode = (node*)malloc(sizeof(node));
        pNode->val = b;
        pNode->next = g->adj_list[a].head;
        g->adj_list[a].head = pNode;
    }
}
void add_edge(graph* g, int a, int b) {
    add_arc(g, a, b);
    add_arc(g, b, a);
}
void del_arc(graph* g, int a, int b) {
    if (a < g->count && b < g->count) {
        node* prev = NULL;
        node* cur = g->adj_list[a].head;
        while (cur != NULL && cur->val != b) {
            prev = cur;
            cur = cur->next;
        }
        if (prev == NULL && cur != NULL) {
            g->adj_list[a].head = cur->next; // ??
        }
        else if (cur != NULL) {
            prev->next = cur->next;
        }
        free(cur);
    }
}
void dfs_for_topsort(graph* g, int start, int* visited, int** res_pos) {
    visited[start] = 1;
    node* cur = g->adj_list[start].head;
    while (cur)
    {
        int n = cur->val;
        if (!visited[n])
        {
            dfs_for_topsort(g, n, visited, res_pos);
        }
        cur = cur->next;
    }
    **res_pos = start;
    (*res_pos)--;
}
int* dfs_time(graph* g)
{
    int* res = (int*)calloc(g->count, sizeof(int));
    int* visited = (int*)calloc(g->count, sizeof(int)); // выделяем память для массива цветов вершин
    int* res_pos = res + g->count - 1;
    for (int i = 0; i < g->count; i++) {   // проходим по всем вершинам графа
        if (!visited[i]) {// если вершина еще не покрашена,
            visited[i] = 1;
            dfs_for_topsort(g, i, visited, &res_pos);
        }
    }
    free(visited);
    return res;
}
graph* graph_transposition(graph* g)
{
    graph* gt = graph_init(g->count);
    for (int i = 0; i < gt->count; i++)
    {
        node* cur = g->adj_list[i].head;
        while (cur)
        {
            add_arc(gt, cur->val, i); // добавляем инцидентную вершину с инвертированным направлением ребра
            cur = cur->next;
        }

    }
    return gt;
}
void dfs_scc(graph* g, int start, bool* visited, int colour, int* colours) {
    visited[start] = 1;

    node* cur = g->adj_list[start].head;
    while (cur) {
        if (!visited[cur->val]) {

            dfs_scc(g, cur->val, visited, colour, colours);

        }
        cur = cur->next;
        if (colours[start] == 0)
        {
            colours[start] = colour;
        }
    }
}

int* graph_scc(graph* g)
{
    int colour = 1;
    int* colours = (int*)calloc(g->count, sizeof(int));
    bool* visited = (bool*)calloc(g->count, sizeof(bool));

    graph* gt = graph_transposition(g);
    int* res = dfs_time(gt);

    //graph_print(gt);

    for (int i = 0; i < g->count; i++) {
        if (!visited[res[i]])
        {
            dfs_scc(g, res[i], visited, colour, colours);
            colour++;
        }
    }
    graph_free(gt);
    free(visited);
    free(res);
    return colours;
}
twosat* get_2_sat_cnf_form(const char* bl_form)
{
    twosat* two_sat = (twosat*)malloc(sizeof(twosat));
    two_sat->clauses = (clause*)calloc(strlen(bl_form) / 4, sizeof(clause));
    int var_flar = 0;
    int n = 0;
    int clause_num = 0;
    int var_index = 0;
    int max_index = 0;
    char* str = (char*)bl_form;
    char vars[2][3] = { {0,0,0}, {0,0,0} };

    while (*str)
    {
        switch (n)
        {
        case 0:
            if (*str == '(')
            {
                str++; n = 1; break;
            }
            break;
        case 1:
            if (*str == 'x')
            {
                str++;
                vars[var_index][1] = 'x';
                n = 1;
            }
            else if (isdigit(*str))
            {
                vars[var_index][2] = *str;
                int temp = strtol(str, &str, 10);

                n = 2;
                max_index = max(temp, max_index);
            }
            else if (*str == '~') {
                vars[var_index][0] = '~';
                str++;
                n = 1;
            }
            break;
        case 2:
            if (var_flar)
            {
                n = 3;
            }
            else if (*str == '\\')
            {
                var_index = 1;
                str += 2;

                var_flar = 1;
                n = 1;
            }

            else if (*str == '/')
            {
                var_index = 1;
                str++;
                n = 1;
                var_flar = 1;
            }
            break;
        case 3:
            variable * var_1 = (variable*)malloc(sizeof(variable));
            var_1->name[0] = vars[0][0];
            var_1->name[1] = vars[0][1];
            var_1->name[2] = vars[0][2];
            variable* var_2 = (variable*)malloc(sizeof(variable));
            var_2->name[0] = vars[1][0];
            var_2->name[1] = vars[1][1];
            var_2->name[2] = vars[1][2];;
            two_sat->num_clauses = clause_num;
            two_sat->clauses[clause_num].var1 = var_1;
            two_sat->clauses[clause_num].var2 = var_2;
            clause_num++;
            two_sat->num_vars = max_index;
            two_sat->num_clauses = clause_num;
            memset(vars[1], 0, sizeof(vars[1]));
            memset(vars[0], 0, sizeof(vars[0]));

            var_index = 0;
            var_flar = 0;
            n = 0;
            str++;
            break;

        }
    }
    return two_sat;
}

void implication_graph(int* arr, twosat* two_sat, graph* g) {
    for (int i = 0; i < (two_sat->num_clauses); i++) {
        bool s_1 = two_sat->clauses[i].var1->name[0] == '~';
        bool s_2 = two_sat->clauses[i].var2->name[0] == '~';
        add_arc(g, arr[two_sat->clauses[i].var1->name[2] - '0' + not(s_1)*two_sat->num_vars - 1], arr[two_sat->clauses[i].var2->name[2] - '0' - 1 + (s_2)*two_sat->num_vars]);
        add_arc(g, arr[two_sat->clauses[i].var2->name[2] - '0' + not(s_2)*two_sat->num_vars - 1], arr[two_sat->clauses[i].var1->name[2] - '0' - 1] + (s_1)*two_sat->num_vars);
    }
}

bool* result_twosat(int* comp, twosat* two_sat) {
    bool* res = (bool*)malloc(sizeof(bool));
    for (int i = 0; i < two_sat->num_vars; i++) {
        if ((comp[i]) == (comp[i + two_sat->num_vars])) {
            cout << "No solution" << endl;
            return 0;
        }
        else {
            if ((comp[i]) > (comp[i + two_sat->num_vars])) {
                res[i] = 1;
                res[i + two_sat->num_vars] = 0;
            }
            if ((comp[i]) < (comp[i + two_sat->num_vars])) {
                res[i] = 0;
                res[i + two_sat->num_vars] = 1;
            }
        }
    }
    return res;
}

