#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <cstdio>
#include <sstream>
#include <fstream>
using namespace std;

struct Edge {
    int to, bro;
} e[10000];
int f[100] = { 0 }; // view's first edge
int k, n, m;
char inputFile[100];
int cost[1000] = { 0 }; // views space costs
int now[1000] = { 0 };  // current costs for querying a view
int isUsed[1000] = { 0 };


void addEdge(int x, int y) {
    m++;
    e[m].to = y;
    e[m].bro = f[x];
    f[x] = m;
}

bool getArguments(int argc, char* argv[]) {
    // find -k and -f
    int i = 0, kpos = -1, fpos = -1;
    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-k") == 0) {
            kpos = i;
        }
        if (strcmp(argv[i], "-f") == 0) {
            fpos = i;
        }
    }
    if (kpos == -1 || fpos == -1 || kpos + 1 >= argc || fpos >= argc)
        return false;
    stringstream ss;
    ss << argv[kpos + 1];
    ss >> k;
    memcpy(inputFile, argv[fpos + 1], strlen(argv[fpos + 1]) + 1);
    return true;
}

/**
 * update the cost c from node x (mode = 1)
 * or calculate benefit (mode = 0)
 */
int updateNowCost(int x, int c, int mode) {
    int ans = 0;
    if (now[x] == -1) {
        now[x] = c;
    } else if (now[x] > c) {
        ans += now[x] - c;
        now[x] = now[x] + mode * (-now[x] + c);
    }
    int i = f[x];
    while (i != 0) {
        ans += updateNowCost(e[i].to, c, mode);
        i = e[i].bro;
    }
    return ans;
}

bool readFromFile() {
    string filePath(inputFile);
    ifstream input(filePath);
    if (!input.is_open())
        return false;
    input >> n;
    int i, j, x, y, c, temp;
    for (i = 1; i <= n; i++) {
        input >> x >> c;
        cost[x] = c;
    }
    input >> temp;
    m = 0;
    bool isRoot[1000] = { 0 };
    for (i = 1; i <= temp; i++) {
        input >> x >> y;
        addEdge(x, y);
        isRoot[y] = 1;
    }
    // initialization
    for (i = 1; i <= n; i++)
        now[i] = -1;
    bool isFirst = true;
    for (i = 1; i <= n; i++)
        if (isRoot[i] == 0) {
            if (!isFirst) {
                cout << " -> ";
            } else {
                isFirst = false;
            }
            cout << i;
            isUsed[i] = 1;
            updateNowCost(i, cost[i], 0);
        }
    return true;
}

void greedy() {
    int i, j, max = 0, maxi, temp;
    while (k--) {
        max = -1;
        for (i = 1; i <= n; i++)
            if (isUsed[i] == 0) {
                temp = updateNowCost(i, cost[i], 0);
                if (temp > max) {
                    max = temp;
                    maxi = i;
                }
            }
        if (max == -1)
            break;
        isUsed[maxi] = 1;
        cout << " -> " << maxi;
        updateNowCost(maxi, cost[maxi], 1);
    }
}

int main(int argc, char* argv[]) {
    if (!getArguments(argc, argv)) {
        cout << "using -k to input value k and using -f to choose input file" << endl;
        return 0;
    }
    if (!readFromFile()) {
        cout << "input file doesn't exist" << endl;
        return 0;
    }
    greedy();
    cout << endl;
    return 0;
}
