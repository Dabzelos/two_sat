#include "sat_graph.h"
#include <iostream>
using namespace std;

void test_2_sat()
{
    const char* expression = "(~x2\/~x3)(x1\/x2)(x3\/~x1)(~x4\/x2)";
    // (~x2\/~x3)(x1\/x2)(x3\/~x1)(~x4\/x2)(x4\/~x1)
    // (x1\/x2)(x1\/x3)(~x2\/x3)(~x2\/x1)
    //(x1\/x2)(~x2\/x1)(~x2\/~x1)(~x1\/x2)
    twosat* two_sat = get_2_sat_cnf_form(expression);
    int n = two_sat->num_vars;
    int* arr = (int*)calloc(2 * n, sizeof(int));
    for (int i = 0; i < 2 * n; i++) {
        arr[i] = i;
    }
    struct graph* g;
    g = graph_init(2 * n);
    g->count = 2 * n;
    implication_graph(arr, two_sat, g);
    graph_print(g);
    int* comp = graph_scc(g);
    for (int j = 0; j < g->count; j++) {
        cout << j << " " << "in" << " " << comp[j] << " " << "component" << endl;
    }
    bool* result = result_twosat(comp, two_sat);
    cout << "RESULT:" << endl;
    for (int j = 0; j < two_sat->num_vars; j++) {
        cout << "x" << j + 1 << " " << "=" << " " << result[j] << endl;
    }
}
int main() {
    setlocale(LC_ALL, "rus");
    test_2_sat();
}