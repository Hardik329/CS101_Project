
#include <bits/stdc++.h>
using namespace std;

vector<pair<int, vector<int>>> paths; // Keeping track of the augmenting paths along with their bottleneck flow

/* Returns true if there is a path from source 's' to sink
't' in residual graph. Also fills parent[] to store the
path */

// Returns true if there is a path from source to sink in residual graph and fills parent vector to store the path
bool bfs(vector<vector<int>> &residualGraph, int s, int t, vector<int> &parent)
{
    int V = residualGraph.size();

    // Create a visited vector where initially all vertices are unvisited
    vector<bool> visited(V);

    // Queue for BFS
    queue<int> q;

    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    // BFS
    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int v = 0; v < V; v++)
        {
            if (visited[v] == false && residualGraph[u][v] > 0)
            {
                // If the sink is reached, set the parent and return true
                if (v == t)
                {
                    parent[v] = u;
                    return true;
                }
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    // Sink is not reached, return false
    return false;
}

// Returns the maximum flow from s to t in the given graph
int EdmondsKarp(vector<vector<int>> graph, int s, int t)
{
    int V = graph.size();

    // Residual graph where residualGraph[i][j] indicates residual capacity of edge from i to j (0 if edge is absent)
    vector<vector<int>> residualGraph(V, vector<int>(V));

    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            residualGraph[i][j] = graph[i][j];

    vector<int> parent(V); // Stores parent of a vertex to get the path

    int max_flow = 0;

    // Augment the flow while there is path from source to sink
    while (bfs(residualGraph, s, t, parent))
    {
        // Finding minimum residual capacity (bottleneck flow) of the edges along the path
        int path_flow = INT_MAX;
        for (int v = t; v != s; v = parent[v])
        {
            int u = parent[v];
            path_flow = min(path_flow, residualGraph[u][v]);
        }
        vector<int> temp;

        // Updating the flow
        for (int v = t; v != s; v = parent[v])
        {
            temp.push_back(v);
            int u = parent[v];
            residualGraph[u][v] -= path_flow;
            residualGraph[v][u] += path_flow;
        }

        temp.push_back(s);

        reverse(temp.begin(), temp.end());

        paths.push_back({path_flow, temp});

        // Adding the path flow to the total flow
        max_flow += path_flow;
    }

    // Return the overall flow
    return max_flow;
}

int main()
{
    // Number of vertices
    int V;
    cout << "Enter number of vertices: ";
    cin >> V;

    // Graph matrix where graph[i][j] represents capacity of flow from i to j
    vector<vector<int>> graph(V, vector<int>(V));

    int s, t;
    cout << "Enter source vertex: ";
    cin >> s;
    cout << "Enter sink vertex: ";
    cin >> t;

    cout << "\nEnter the graph matrix:" << endl;
    for (auto &v : graph)
    {
        for (auto &i : v)
            cin >> i;
    }

    cout << "\nThe maximum possible flow is " << EdmondsKarp(graph, s, t);

    cout << "\n\n\nAugmenting paths:\n\n";

    for (auto p : paths)
    {
        auto path = p.second;
        auto flow = p.first;

        for (auto v : path)
            cout << v << " ";
        cout << "with flow of " << flow << endl;
    }

    cout << endl;

    return 0;
}
