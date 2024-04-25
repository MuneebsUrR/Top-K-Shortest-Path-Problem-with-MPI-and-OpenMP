#include <string>
#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <queue>
#include <sstream>
#include <algorithm>
#include <unordered_set>
// #include <omp.h>
// #include <mpi.h>

using namespace std;

#define inf INT32_MAX

// Function to find K shortest path lengths
void findKShortest(vector<vector<pair<int, int> > > & g, int n, int m, int k)
{
    //print g
    for(int i = 0; i < g.size(); i++){
        cout << i << ": ";
        for(int j = 0; j < g[i].size(); j++){
            cout << g[i][j].first << " " << g[i][j].second << " | ";
        }
        cout << endl;
    }

    cout << "completed" << endl;

    // Vector to store distances
    vector< vector<int> > dis(n + 1, vector<int>(k, inf));

    // Initialization of priority queue
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
    pq.push(make_pair(0, 1));
    dis[1][0] = 0;

    // while pq has elements
    while (!pq.empty())
    {
        cout << dis.size() << endl;
        // Storing the node value
        int u = pq.top().second;

        // Storing the distance value
        int d = (pq.top().first);
        pq.pop();
        if (dis[u][k - 1] < d)
            continue;
        vector<pair<int, int> > v = g[u];

        // Traversing the adjacency list
        for (int i = 0; i < v.size(); i++)
        {
            int dest = v[i].first;
            int cost = v[i].second;

            // Checking for the cost
            if (d + cost < dis[dest][k - 1])
            {
                dis[dest][k - 1] = d + cost;

                // Sorting the distances
                sort(dis[dest].begin(), dis[dest].end());

                // Pushing elements to priority queue
                pq.push(make_pair((d + cost), dest));
            }
        }
    }


    // Printing K shortest paths
    for (int i = 0; i < k; i++)
    {
        cout << dis[n][i] << " ";
    }
}

void generate_random_pairs(int random_selected_pairs[][2], int noOfPairs, int noOfNodes) {
    srand(time(0));

    for (int i = 0; i < noOfPairs; i++) {
        int randomNumber1 = rand() % noOfNodes;
        int randomNumber2 = rand() % noOfNodes;
        random_selected_pairs[i][0] = randomNumber1;
        random_selected_pairs[i][1] = randomNumber2;
    }
}


void print_pairs(int random_selected_pairs[][2]){
    //printing random pairs
    for(int i =0 ;i<10;i++){
        cout<<random_selected_pairs[i][0]<<"-"<<random_selected_pairs[i][1]<<endl;
    }
}


int countUniqueNodes(const string& filename) {
    ifstream file(filename); // open the file
    unordered_set<int> uniqueNodes;
    int maxNode = 0; // Initialize maxNode to 0

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            int node_from, node_to;
            if (iss >> node_from >> node_to) {
                if (node_from == node_to) {
                    continue;
                }
                uniqueNodes.insert(node_from);
                uniqueNodes.insert(node_to);
                maxNode = max(maxNode, max(node_from, node_to)); // Update maxNode
            } else {
                cerr << "Parsing error" << endl;
                break;
            }
        }
    }

    file.close();

    return maxNode;
}

int completeEdgesVector(vector<vector<pair<int, int> > > & edges, const string& filename) {
    int count = 0;
    ifstream file(filename); // open the file
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            int node_from, node_to;
            if (iss >> node_from >> node_to) {
                // cout << "Node from: " << node_from << " Node to: " << node_to << endl;
                if (node_from == node_to) {
                    continue;
                }
                edges[node_from].push_back(make_pair(node_to, 1)); // Assuming default weight is 1
                count++;
            } else {
                cerr << "Parsing error" << endl;
                break;
            }
        }
    }

    file.close();
    return count;
}

int main(int argc, char** argv) {
    string filename = "euemail.txt";
    int uniqueNodeCount = countUniqueNodes(filename);
    vector<vector<pair<int, int> > > edges(uniqueNodeCount + 1);
    int noOfEdges = completeEdgesVector(edges, filename);
    clock_t start, end;
    double cpu_time_used;


    const int K = 3;
    const int noOfPairs = 10;
    int random_selected_pairs[noOfPairs][2];


    // Generate random pairs only in the root process
    generate_random_pairs(random_selected_pairs, noOfPairs, uniqueNodeCount);
    print_pairs(random_selected_pairs);

    cout << "Nodes = " << uniqueNodeCount << endl;

    cout << "uniqueNodeCount = " << uniqueNodeCount << endl;
    cout << "edges.size() = " << edges.size() << endl;
    cout << "K = " << K << endl;

    start = clock();
    findKShortest(edges, uniqueNodeCount, noOfEdges, K);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken for serial: %f seconds\n", cpu_time_used);


    return 0;
}
