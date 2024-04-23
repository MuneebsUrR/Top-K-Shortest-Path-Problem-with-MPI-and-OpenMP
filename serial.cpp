#include <string>
#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <queue>
#include <sstream>
#include <algorithm> 
#include <omp.h>
#include <mpi.h>

#include <unordered_set>
using namespace std;

// Function to find K shortest path lengths
void findKShortest(int edges[][3], int n, int m, int k)
{

	// Initialize graph
	vector<vector<pair<int, int> > > g(n + 1);
	for (int i = 0; i < m; i++) {
		// Storing edges
		g[edges[i][0]].push_back(
			{ edges[i][1], edges[i][2] });
	}
    //print g
	for(int i=0;i<g.size();i++){
		cout<<i<<"->";
		for(int j=0;j<g[i].size();j++){
			cout<<g[i][j].first<<" "<<g[i][j].second<<" ";
		}
		cout<<endl;
	}

	// Vector to store distances
	vector<vector<int> > dis(n + 1, vector<int>(k, 1e9));

	// Initialization of priority queue
	priority_queue<pair<int, int>, vector<pair<int, int> >,
				greater<pair<int, int> > >
		pq;
	pq.push({ 0, 1 });
	dis[1][0] = 0;

	// while pq has elements
	while (!pq.empty()) {
		// Storing the node value
		int u = pq.top().second;

		// Storing the distance value
		int d = (pq.top().first);
		pq.pop();
		if (dis[u][k - 1] < d)
			continue;
		vector<pair<int, int> > v = g[u];

		// Traversing the adjacency list
		for (int i = 0; i < v.size(); i++) {
			int dest = v[i].first;
			int cost = v[i].second;

			// Checking for the cost
			if (d + cost < dis[dest][k - 1]) {
				dis[dest][k - 1] = d + cost;

				// Sorting the distances
				sort(dis[dest].begin(), dis[dest].end());

				// Pushing elements to priority queue
				pq.push({ (d + cost), dest });
			}
		}
	}

	// Printing K shortest paths
	for (int i = 0; i < k; i++) {
		cout << dis[n][i] << " ";
	}
}

// Driver Code
int main()
{

	// Given Input
	int N = 4, M = 6, K = 3;
	int edges[][3]
		= { { 1, 2, 1 }, { 1, 3, 3 }, { 2, 3, 2 },
			{ 2, 4, 6 }, { 3, 2, 8 }, { 3, 4, 1 } };

	// Function Call
	findKShortest(edges, N, M, K);

    file.close();
}

int count_numberof_nodes() {
    std::unordered_set<int> uniqueElements;

    for (int i = 0; i < edges_count_index; ++i) {
        for (int j = 0; j < 2; ++j) {
            uniqueElements.insert(edges[i][j]);
        }
    }

    return uniqueElements.size();
}

int main(int argc, char** argv) {
    // MPI Initialization
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Given Input
    int K = 3;
    int M = 10;

    int random_selected_pairs[10][3]; // array to store the random nodes

    if (rank == 0) {
        // reading data from the file
        Read_EU_Email("euemail.txt");

        // Generate random pairs only in the root process
        generate_random_pairs(random_selected_pairs);
      
        // Function Call
        findKShortest(edges, N, M, K);
    }

    // Scatter the random_selected_pairs array
    int rows_per_process = 2;
    MPI_Scatter(random_selected_pairs, rows_per_process * 3, MPI_INT, random_selected_pairs, rows_per_process * 3, MPI_INT, 0, MPI_COMM_WORLD);

    // Process local computation on assigned rows and print them
    for (int i = 0; i < rows_per_process; ++i) {
        cout << "Process " << rank << ": Row " << i << ": ";
        for (int j = 0; j < 3; ++j) {
            cout << random_selected_pairs[i][j] << " ";
        }
        cout << endl;
    }

    // MPI Finalization
    MPI_Finalize();

    return 0;
}
