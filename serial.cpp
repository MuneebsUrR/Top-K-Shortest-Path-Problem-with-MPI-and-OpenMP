// C++ implementation of above approach
#include <string>
#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <queue>
#include <sstream>

using namespace std;

const int MAX_ROWS = 100000;
const int MAX_COLS = 3;
int edges[MAX_ROWS][MAX_COLS] = {0};

// Function to find K shortest path lengths
void findKShortest(int edges[][3], int n, int m, int k)
{

    // Initialize graph
    vector< vector<pair<int, int> > > g(n + 1);
    for (int i = 0; i < m; i++)
    {
        // Storing edges
        g[edges[i][0]].push_back(make_pair(edges[i][1], edges[i][2]));
    }

    // Vector to store distances
    vector< vector<int> > dis(n + 1, vector<int>(k, 1e9));

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

void generate_random_pairs(int random_selected_pairs[10][3])
{
    srand(time(0));
    // selecting 10 random pairs from the edges matrix
    for (int i = 0; i < 10; i++)
    {
        int random_pair = rand() % MAX_ROWS;
        random_selected_pairs[i][0] = edges[random_pair][0];
        random_selected_pairs[i][1] = edges[random_pair][1];
        random_selected_pairs[i][2] = edges[random_pair][2];
    }

    // printing random pairs
    for (int i = 0; i < 10; i++)
    {
        cout << random_selected_pairs[i][0] << "-" << random_selected_pairs[i][1] << "-" << random_selected_pairs[i][2] << endl;
    }
    cout << endl;
}

void Read_EU_Email(string filename)
{
    ifstream file(filename); // open the file
    int M = 0;
    if (file.is_open())
    {
        srand(time(0));
        string line;
        while (getline(file, line) && M < MAX_ROWS)
        {
            istringstream iss(line);
            int node_from, node_to;
            if (!(iss >> node_from >> node_to))
            {
                cerr << "Parsing error" << endl;
                break;
            }
            int weight = rand() % 10 + 1; // generate a random weight between 1 and 10

            if (node_from == node_to)
            {
                weight = 0; // condition for node to itself cost (0)
            }

            // storing the edges in the array
            edges[M][0] = node_from;
            edges[M][1] = node_to;
            edges[M][2] = weight;
            M++;
        }
    }

    file.close();
}

// Driver Code
int main()
{

    // Given Input
    int N = 10, K = 3;
    int M = 10;

    int random_selected_pairs[10][3]; // array to store the random nodes

    // reading data from the file
    Read_EU_Email("euemail.txt");
    generate_random_pairs(random_selected_pairs);

    // Function Call
    // findKShortest(edges, N, M, K);

    return 0;
}
