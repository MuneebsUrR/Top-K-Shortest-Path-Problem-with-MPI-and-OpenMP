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
#include <omp.h>
#include <mpi.h>

using namespace std;

#define inf INT32_MAX

// Node used in the dis matrix / vector, contains the distance and the path
struct Node
{
    int distance;
    vector<int> path;

    Node(int distance, vector<int> path) : distance(distance), path(path)
    {}
};

// Function to find K shortest path lengths
void findKShortest(vector<vector<pair<int, int>>> &edges, int n, int k, int source, int destination)
{
    cout << "Searching a path from " << source << " to " << destination << " with K = " << k << endl;

    vector<vector<Node>> dis(n + 1, vector<Node>(k, Node(inf, vector<int>())));

    // Initialization of priority queue
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push(make_pair(0, source));

    // Setting the distance of source as 0
    dis[source][0].distance = 0;
    dis[source][0].path.push_back(source);

    // Loop until priority queue is empty
    while (!pq.empty())
    {
        int u, d;
        #pragma omp critical
        {
            // Storing the node value
            u = pq.top().second;

            // Storing the distance value
            d = pq.top().first;
            pq.pop();
        }


        if (dis[u][k - 1].distance < d) 
        {
            continue;
        }

        vector<pair<int, int>> v = edges[u];

        // Traversing the adjacency list
        #pragma omp parallel for
        for (int i = 0; i < v.size(); i++)
        {
            int dest = v[i].first;
            int cost = v[i].second;

            // Checking for the cost
            if (d + cost < dis[dest][k - 1].distance)
            {
                dis[dest][k - 1].distance = d + cost;
                dis[dest][k - 1].path = dis[u][k - 1].path;
                dis[dest][k - 1].path.push_back(dest);

                // Sorting the distances
                sort(dis[dest].begin(), dis[dest].end(), [](const Node &a, const Node &b) {
                    return a.distance < b.distance;
                });

                // Pushing elements to priority queue
                #pragma omp critical
                {
                    pq.push(make_pair((d + cost), dest));
                }
            }
        }
    }

    // Printing K shortest paths
    for (int i = 0; i < k; i++)
    {
        cout << "Shortest Path #" << i + 1 << ": ";
        for (int j = 0; j < dis[destination][i].path.size(); j++)
        {
            cout << dis[destination][i].path[j] << " ";
        }
        cout << " Distance: " << dis[destination][i].distance << endl;
    }
    
    cout << endl << endl;
}

void generateRandomPairs(int randomSelectedPairs[][2], int noOfPairs, int noOfNodes)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < noOfPairs; i++)
    {
        int r1 = rand() % noOfNodes;
        int r2 = rand() % noOfNodes;

        randomSelectedPairs[i][0] = r1;
        randomSelectedPairs[i][1] = r2;

        // Display generated pair
        cout << "Pair #" << i + 1 << ": \t" << r1 << " -> " << r2 << endl;
    }

    cout << endl;
}

// Returns the highest value of a source or destintation node from file
int getMaximumNodeValue(const string &filename)
{
    int maxNode = 0;

    ifstream file(filename);

    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            istringstream iss(line);
            int node_from, node_to;
            if (iss >> node_from >> node_to)
            {
                maxNode = max(maxNode, node_from);
                maxNode = max(maxNode, node_to);
            }
            else
            {
                cerr << "Parsing error" << endl;
                break;
            }
        }
    }
    else
    {
        cerr << "File not found on path '" << filename << "'" << endl;
    }

    file.close();

    return maxNode;
}

void setEdgesVector(vector<vector<pair<int, int>>> &edges, const string &filename)
{
    ifstream file(filename);

    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            istringstream iss(line);
            int node_from, node_to, weight;

            // Parse the line
            // will not work if the line contains source destination weight
            // for weight to work, append '>> weight' to the iss
            //
            if (iss >> node_from >> node_to >> weight)
            {
                // Skip self loops
                if (node_from == node_to)
                {
                    continue;
                }

                // An assumption is made that the weight is 1
                edges[node_from].push_back(make_pair(node_to, weight));
            }
            else
            {
                cerr << "Parsing error" << endl;
                break;
            }
        }
    }
    else
    {
        cerr << "File not found on path '" << filename << "'" << endl;
    }

    file.close();
}

int main(int argc, char **argv)
{
    // Do not comment this line
    const int K = 3;

    // string filename = "euemail.txt";
    // string filename = "enronemail.txt";
    string filename = "emails.txt";
    
    int maxNodeValue = getMaximumNodeValue(filename);
    
    vector<vector<pair<int, int>>> edges(maxNodeValue + 1);
    setEdgesVector(edges, filename);

    clock_t start, end;
    double cpu_time_used;

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Generate and print random pairs
    const int noOfPairs = 10;
    int randomPairs[noOfPairs][2];
    if (rank == 0)
    {
        generateRandomPairs(randomPairs, noOfPairs, maxNodeValue);
    }

    int rows_per_process = 2;
    int received_pairs[rows_per_process][2]; // buffer to receive scattered pairs
    
    start = clock();
    MPI_Scatter(randomPairs, rows_per_process * 2, MPI_INT, received_pairs, rows_per_process * 2, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < rows_per_process; i++)
    {
        findKShortest(edges, maxNodeValue, K, received_pairs[i][0], received_pairs[i][1]);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
    {
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Time taken for parallel: %f seconds\n", cpu_time_used);
    }
    
    MPI_Finalize();

    return 0;
}