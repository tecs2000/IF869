#include <bits/stdc++.h>
using namespace std;

void dfs_post_order(const vector<vector<int>> &graph, int start, vector<bool> &visited, vector<int> &result)
{
    stack<pair<int, int>> s; // Stack to store {vertex, index of next neighbor to visit}

    s.push({start, 0});

    while (!s.empty())
    {
        auto &[current, nextNeighbor] = s.top();

        if (nextNeighbor < graph[current].size())
        {
            int neighbor = graph[current][nextNeighbor];
            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                s.push({neighbor, 0});
            }
            else
            {
                ++nextNeighbor;
            }
        }
        else
        {
            s.pop();
            result.push_back(current);
        }
    }
}

vector<int> dfs_post_order_all(const vector<vector<int>> &graph)
{
    int numVertices = graph.size();
    vector<int> result;
    vector<bool> visited(numVertices, false);

    for (int vertex = 0; vertex < numVertices; ++vertex)
    {
        if (!visited[vertex])
        {
            visited[vertex] = true;
            dfs_post_order(graph, vertex, visited, result);
        }
    }

    reverse(result.begin(), result.end()); // Reverse to get post-order
    return result;
}

void dfs_visit(const vector<vector<int>> &graph, int start, vector<int> &visited, vector<int> &result)
{
    visited[start] = true;
    result.push_back(start);

    for (int neighbor : graph[start])
    {
        if (!visited[neighbor])
        {
            dfs_visit(graph, neighbor, visited, result);
        }
    }
}

vector<vector<int>> findSCC(vector<vector<int>> &graph, vector<vector<int>> &graphT)
{
    vector<int> F = dfs_post_order_all(graph);
    vector<int> visited(graph.size(), 0);

    vector<vector<int>> scc;

    for (int i = 0; i < graph.size(); i++)
    {
        int u = F[i];
        if (!visited[u])
        {
            vector<int> ord;
            dfs_visit(graphT, u, visited, ord);
            sort(ord.begin(), ord.end());
            scc.push_back(ord);
        }
    }

    auto comparator = [](const vector<int> &a, const vector<int> &b)
    {
        return a[0] < b[0];
    };

    sort(scc.begin(), scc.end(), comparator);

    return scc;
}

int main()
{
    int numCases;
    cin >> numCases;

    for (int k = 0; k < numCases; k++)
    {
        int numVertices;
        cin >> numVertices;

        vector<vector<int>> graph(numVertices);
        vector<vector<int>> graphT(numVertices);

        for (int i = 0; i < numVertices; i++)
        {
            int idx, numEdges;
            scanf("%d %d:", &idx, &numEdges);

            for (int e = 0; e < numEdges; e++)
            {
                int v;
                scanf("%d", &v);
                graph[i].push_back(v);
                graphT[v].push_back(i);
            }
        }

        vector<vector<int>> scc;
        scc = findSCC(graph, graphT);

        printf("%d\n", scc.size());
        for (int l = 0; l < scc.size(); l++)
        {
            for (int j = 0; j < scc[l].size(); j++)
            {
                if(j == 0){
                    printf("%d", scc[l][j]);
                }
                else
                    printf(" %d", scc[l][j]);
            }
            printf("\n");
        }

        printf("\n");
    }
    return 0;
}
