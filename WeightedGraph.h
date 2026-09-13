/*
 * WeightedGraph.h
 *
 */
#ifndef H_weightedGraph
#define H_weightedGraph

#include <algorithm>
#include <cfloat>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <queue>
#include <vector>

using namespace std;

class WeightedGraphType {
  protected:
    int gSize;        // number of vertices
    list<int> *graph; // Store adjacency list
    double **weights; // Store weights of edges

  public:
    double *smallestWeight;
    WeightedGraphType(int size = 0);
    ~WeightedGraphType();

    list<int> getAdjancencyList(int index) { return graph[index]; }

    double getWeight(int i, int j) { return weights[i][j]; }

    void printAdjacencyList();
    void printAdjacencyMatrix();
    double shortestPath(int vertex);
    vector<int> breadthFirstSearch(int start, int destination);
};

WeightedGraphType::WeightedGraphType(int size) {
    gSize = 0;
    ifstream infile;
    char fileName[50] = "Weights.txt";

    //	cout << "Enter graph adjacency matrix file name: ";
    //	cin >> fileName;
    //	cout << endl;

    infile.open(fileName);

    if (!infile) {
        cout << "Cannot open input file." << endl;
        return;
    }

    gSize = size;

    graph = new list<int>[gSize];

    weights = new double *[gSize];
    smallestWeight = new double[gSize];

    for (int i = 0; i < gSize; i++)
        weights[i] = new double[gSize];

    for (int i = 0; i < gSize; i++) {
        for (int j = 0; j < gSize; j++) {
            double value;
            infile >> value;
            if (value == 0)
                weights[i][j] =
                    DBL_MAX; // system constant - maximum value of double
            else {
                weights[i][j] = value;
                graph[i].push_back(j);
            }
        }
    }
    infile.close();
}

WeightedGraphType::~WeightedGraphType() {
    for (int i = 0; i < gSize; i++)
        delete[] weights[i];

    delete[] weights;
    delete[] smallestWeight;

    for (int index = 0; index < gSize; index++)
        graph[index].clear();

    delete[] graph;
}

void WeightedGraphType::printAdjacencyMatrix() { // print adjacency matrix for
                                                 // debug purpose
    cout << "\nAdjacency Matrix" << endl;
    for (int i = 0; i < gSize; i++) {
        for (int j = 0; j < gSize; j++) {
            cout << setw(8)
                 << (weights[i][j] == DBL_MAX
                         ? 0.0
                         : weights[i][j]); // as adjacency value, zero means no
                                           // direct connection
        }
        cout << endl;
    }
}

void WeightedGraphType::printAdjacencyList() { // print adjacency list for debug
                                               // purpose
    cout << "\nAdjacency List" << endl;
    for (int index = 0; index < gSize; index++) {
        cout << index << ": ";
        for (int e : graph[index])
            cout << e << " ";
        cout << endl;
    }

    cout << endl;
}

double WeightedGraphType::shortestPath(int vertex) {
    for (int j = 0; j < gSize; j++)
        smallestWeight[j] = weights[vertex][j];

    bool *weightFound;
    weightFound = new bool[gSize];

    for (int j = 0; j < gSize; j++)
        weightFound[j] = false;

    weightFound[vertex] = true;
    smallestWeight[vertex] = 0;

    for (int i = 0; i < gSize - 1; i++) {
        double minWeight = DBL_MAX;
        int v;

        for (int j = 0; j < gSize; j++)
            if (!weightFound[j])
                if (smallestWeight[j] < minWeight) {
                    v = j;
                    minWeight = smallestWeight[v];
                }

        weightFound[v] = true;

        for (int j = 0; j < gSize; j++)
            if (!weightFound[j])
                if (minWeight + weights[v][j] < smallestWeight[j])
                    smallestWeight[j] = minWeight + weights[v][j];
    } // end for
    return smallestWeight[vertex];
} // end shortestPath

vector<int> WeightedGraphType::breadthFirstSearch(int start, int destination) {
    vector<int> visited(gSize, 0);
    vector<int> parent(gSize, -1);
    queue<int> shortestPath;
    // vector<double> distance(gSize, DBL_MAX);

    shortestPath.push(start);
    visited[start] = 1;

    while (!shortestPath.empty()) {
        int current = shortestPath.front();
        shortestPath.pop();

        // checks if destination has been found
        if (current == destination)
            break;

        for (int j = 0; j < gSize; j++) {
            if (weights[current][j] != DBL_MAX && !visited[j]) {
                visited[j] = 1;
                parent[j] = current;
                shortestPath.push(j);
            }
        }
    }
    // error checking if destination was not found (returns empty)
    if (!visited[destination])
        return {};

    // returning reorganise path as it contains [destination to end]
    vector<int> organisedPath;
    int cur = destination;

    // going backward from parent[] (saves this from later on)
    while (cur != -1) {
        organisedPath.push_back(cur);
        cur = parent[cur];
    }
    reverse(organisedPath.begin(), organisedPath.end());

    return organisedPath;
}
#endif
