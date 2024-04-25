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

using namespace std;

#define inf INT32_MAX

struct Node {
    int distance;
    vector<int> path;
};

// Function to find K shortest path lengths
void findKShortest(vector<vector<pair<int, int>>> &edges, int n, int k, int source, int destination) {
    // Vector to store distances and paths
    vector<vector<Node>> dis(n + 1, vector<Node>(k, {inf, vector<int>()}));

    // Initialization of priority queue
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push(make_pair(0, source));
    dis[source][0] = {0, {source}};

    // while pq has elements
    while (!pq.empty()) {
        // Storing the node value
        int u = pq.top().second;

        // Storing the distance value
        int d = pq.top().first;
        pq.pop();

        if (dis[u][k - 1].distance < d) continue;

        vector<pair<int, int>> v = edges[u];

        // Traversing the adjacency list
        for (int i = 0; i < v.size(); i++) {
            int dest = v[i].first;
            int cost = v[i].second;

            // Checking for the cost
            if (d + cost < dis[dest][k - 1].distance) {
                dis[dest][k - 1].distance = d + cost;
                dis[dest][k - 1].path = dis[u][k - 1].path;
                dis[dest][k - 1].path.push_back(dest);

                // Sorting the distances
                sort(dis[dest].begin(), dis[dest].end(), [](const Node &a, const Node &b) {
                    return a.distance < b.distance;
                });

                // Pushing elements to priority queue
                pq.push(make_pair((d + cost), dest));
            }
        }
    }

    // Printing K shortest paths
    for (int i = 0; i < k; i++) {
        cout << "Path " << i+1 << ": ";
        for (int j = 0; j < dis[destination][i].path.size(); j++) {
            cout << dis[destination][i].path[j] << " ";
        }
        cout << " Distance: " << dis[destination][i].distance << endl;
    }
}

void generate_random_pairs(int random_selected_pairs[][2], int noOfPairs, int noOfNodes)
{
    srand(time(0));

    for (int i = 0; i < noOfPairs; i++)
    {
        int randomNumber1 = rand() % noOfNodes;
        int randomNumber2 = rand() % noOfNodes;
        random_selected_pairs[i][0] = randomNumber1;
        random_selected_pairs[i][1] = randomNumber2;
    }
}


void print_pairs(int random_selected_pairs[][2])
{
    //printing random pairs
    for (int i = 0; i < 10; i++)
    {
        cout << random_selected_pairs[i][0] << "-" << random_selected_pairs[i][1] << endl;
    }
}


int countUniqueNodes(const string &filename)
{
    ifstream file(filename); // open the file
    unordered_set<int> uniqueNodes;
    int maxNode = 0; // Initialize maxNode to 0

    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            istringstream iss(line);
            int node_from, node_to;
            if (iss >> node_from >> node_to)
            {
                if (node_from == node_to)
                {
                    continue;
                }
                uniqueNodes.insert(node_from);
                uniqueNodes.insert(node_to);
                maxNode = max(maxNode, max(node_from, node_to)); // Update maxNode
            }
            else
            {
                cerr << "Parsing error" << endl;
                break;
            }
        }
    }

    file.close();

    return maxNode;
}

void completeEdgesVector(vector<vector<pair<int, int> > > &edges, const string &filename)
{
    ifstream file(filename); // open the file
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            istringstream iss(line);
            int node_from, node_to;
            if (iss >> node_from >> node_to)
            {
                if (node_from == node_to)
                {
                    continue;
                }

                // Assuming default weight is 1
                edges[node_from].push_back(make_pair(node_to, 1));
            }
            else
            {
                cerr << "Parsing error" << endl;
                break;
            }
        }
    }

    file.close();
}

int main(int argc, char **argv)
{
//    string filename = "euemail.txt";
//    int uniqueNodeCount = countUniqueNodes(filename);
//    vector<vector<pair<int, int> > > edges(uniqueNodeCount + 1);
//    completeEdgesVector(edges, filename);
    clock_t start, end;
    double cpu_time_used;

    const int K = 3;
    const int noOfPairs = 10;
    int random_selected_pairs[noOfPairs][2];

    // Generate random pairs only in the root process
//    generate_random_pairs(random_selected_pairs, noOfPairs, uniqueNodeCount);
//    print_pairs(random_selected_pairs);

//    cout << "Nodes = " << uniqueNodeCount << endl;
//    cout << "uniqueNodeCount = " << uniqueNodeCount << endl;
//    cout << "edges.size() = " << edges.size() << endl;
    cout << "K = " << K << endl;

    vector<vector<pair<int, int> > > edges(6);

    edges[2].push_back(make_pair(3, 2));
    edges[2].push_back(make_pair(4, 6));
    edges[3].push_back(make_pair(2, 8));
    edges[3].push_back(make_pair(4, 1));

    start = clock();
//    for (int i = 0; i < noOfPairs; i++)
//    {
//        findKShortest(edges, uniqueNodeCount, K, 1);
//    }
    findKShortest(edges, 4, K, 2, 3);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken for serial: %f seconds\n", cpu_time_used);

    return 0;
}
