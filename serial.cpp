// C++ implementation of above approach
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

// Function to find K shortest path lengths
void findKShortest(vector<vector<pair<int, int> > > & g, int n, int m, int k)
{
for (int i = 1; i < 2; ++i) {
        cout << "Node " << i << " is connected to: ";
        for (const auto& edge : g[i]) {
            cout << "(" << edge.first << ", " << edge.second << ") ";
        }
        cout << endl;
    }

    // Vector to store distances
    vector< vector<int> > dis(n + 1, vector<int>(k, 1000000));

    // Initialization of priority queue
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
    pq.push(make_pair(0, 1));
    dis[1][0] = 0;

    // while pq has elements
    while (!pq.empty())
    {
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
	cout<<endl;
}


int countUniqueNodes(const string& filename) {
    ifstream file(filename); // open the file
    unordered_set<int> uniqueNodes;

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            int node_from, node_to;
            if (iss >> node_from >> node_to) {
                uniqueNodes.insert(node_from);
                uniqueNodes.insert(node_to);
            } else {
                cerr << "Parsing error" << endl;
                break;
            }
        }
    }

    file.close();

    return uniqueNodes.size();
}

void completeEdgesVector(vector<vector<pair<int, int> > > & edges, const string& filename) {
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
            } else {
                cerr << "Parsing error" << endl;
                break;
            }
        }
    }

    file.close();
}

int main(int argc, char** argv) {
    string filename = "euemail.txt";
    int uniqueNodeCount = countUniqueNodes(filename);
    vector<vector<pair<int, int> > > edges(uniqueNodeCount + 1);
    completeEdgesVector(edges, filename);

    // MPI Initialization
    // MPI_Init(&argc, &argv);

    // int rank, size;
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int K = 3;
    const int noOfPairs = 10;
    int random_selected_pairs[noOfPairs][2];
    
    // if (rank == 0) {
        
        // Generate random pairs only in the root process
        generate_random_pairs(random_selected_pairs, noOfPairs, uniqueNodeCount);
        print_pairs(random_selected_pairs);
    
        cout << "Nodes = " << uniqueNodeCount << endl;

        findKShortest(edges, uniqueNodeCount, edges.size(), K);
    // }

    // // Scatter the random_selected_pairs array
    // int rows_per_process = 2;
    // MPI_Scatter(random_selected_pairs, rows_per_process * 3, MPI_INT, random_selected_pairs, rows_per_process * 3, MPI_INT, 0, MPI_COMM_WORLD);

    // // Process local computation on assigned rows and print them
    // for (int i = 0; i < rows_per_process; ++i) {
    //     cout << "Process " << rank << ": Row " << i << ": ";
    //     for (int j = 0; j < 3; ++j) {
    //         cout << random_selected_pairs[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    // MPI Finalization
    // MPI_Finalize();

    return 0;
}
