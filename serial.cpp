// C++ implementation of above approach
#include <bits/stdc++.h>
#include<string>
#include <iostream>
#include<ctime>
#include<fstream>
#include <vector>
#include<cstdlib>
using namespace std;
const int MAX_ROWS = 100000;
const int MAX_COLS = 3;
int edges[MAX_ROWS][MAX_COLS]{0};

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

void generate_random_nodes(int random_selected_nodes[10],int start, int end){
	srand(time(0));
	random_selected_nodes[0] = start;
	for(int i = 1; i<10; i++){
		random_selected_nodes[i] = rand() % (end - start + 1) + start;

		while(random_selected_nodes[i] == random_selected_nodes[i-1]){
			random_selected_nodes[i] = rand() % (end - start + 1) + start;
		}
		
	}

	// sort the array
    std::sort(random_selected_nodes, random_selected_nodes + 10);

	//print the random nodes
	for(int i = 0; i<10; i++){
		cout<<random_selected_nodes[i]<<" ";
	}
}

void Read_EU_Email(string filename){

}

// Driver Code
int main()
{

	// Given Input
	int N = 4, M = 6, K = 3;

	//start and end node to pick random 10 nodes between them
	int start_node = 0;
	int end_node = 30;
	int random_selected_nodes[10]; //array to store the random nodes
	generate_random_nodes(random_selected_nodes,start_node, end_node);

	int edges[][3]
		= { { 1, 2, 1 }, { 1, 3, 3 }, { 2, 3, 2 },
			{ 2, 4, 6 }, { 3, 2, 8 }, { 3, 4, 1 } };

	// Function Call
	findKShortest(edges, N, M, K);

	return 0;
}
