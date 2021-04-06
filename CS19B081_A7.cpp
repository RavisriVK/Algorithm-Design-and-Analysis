#include<bits/stdc++.h>
using namespace std;

/*==================================================================================
 *  References:
 *      1. CLRS Chapter 23 Page 634 => Prim
 *      2. https://cp-algorithms.com/graph/mst_kruskal_with_dsu.html => Kruskal
 * ================================================================================*/

/*==================================================================================
 *  Graph template takes weightType as parameter and represents an undirected graph
 *  void AddV(): Adds an additional vertex
 *  void AddE(int a, int b, weightType w): Adds given edge between two vertices
 *  void DelE(int a, int b): Deletes an edge between two vertices
 *  void BFS(int a): Prints BFS order from vertex a
 *  void SSP(int a): Prints shortest distances to each vertex from a
 * ================================================================================*/
template<typename weightType>
class Graph {
    public:
        /* Constructor initializes n */
        Graph(int vertexCount): n(vertexCount) {}

        /* Only increments number of vertices, and sets updated to false */
        void AddV() {
            updated=false;
            n++;
        }

        /* Adds an edge between a and b, and sets updated to false */
        void AddE(int a, int b, weightType w) {
            updated=false;
            /* Check if vertices provided exist */
            if(a<0 || a>=n || b<0 || b>=n) {
                cerr<<"Vertex indices provided are invalid. Cannot add edge."<<endl;
                return;
            }
            /* Update the adjacency lists of a and b */
            adj[a].push_back(make_pair(w, b));
            adj[b].push_back(make_pair(w, a));
        }

        /* Deletes an edge between two vertices and sets updated to false */
        void DelE(int a, int b) {
            updated=false;
            /* Check if vertices provided exist */
            if(a<0 || a>=n || b<0 || b>=n) {
                cerr<<"Vertex indices provided are invalid. Cannot delete edge."<<endl;
                return;
            }
            /* We check whether edge is present using 'exists' */
            bool exists=false;

            /* Go through adjacency list of a and delete edge to b */
            /* If edge is present mark 'exists' */
            for(auto it=adj[a].begin(); it!=adj[a].end(); it++) {
                if(b==it->second) {
                    adj[a].erase(it);
                    exists=true;
                    break;
                }
            }

            /* If edge doesn't exist, indicate on cerr */
            if(!exists) {
                cerr<<"Indicated edge does not exist. Cannot delete edge."<<endl;
                return;
            }

            /* In adjacency list of b and delete edge to a */
            for(auto it=adj[b].begin(); it!=adj[b].end(); it++) {
                if(a==it->second) {
                    adj[b].erase(it);
                    break;
                }
            }
        }

    protected:
        /* Number of vertices */
        int n;

        /* We use this to implement our adjacency list */
        map<int, list<pair<weightType, int> > > adj;

        /* Flag which indicates that tree is up to date */
        bool updated=false;

};


/*==================================================================================
 *  MST template also takes weightType as parameter and has these public functions:
 *  void Prim(int v): Starts with the cut ({v}, G.V - {v}) and generates MST
 *  void Kruskal(): Generates MST using union-find
 *  void TreeUptodate(): Checks if tree is up to date
 *  void DisplayTree(): Prints the edges of the tree in increasing order of weight
 * ================================================================================*/
template<typename weightType>
class MST : public Graph<weightType> {
    public:
        /* MST calls the base class constructor */
        MST(int n) : Graph<weightType>(n) {}

        /* Prim uses v to determine the starting cut of the graph when determining MST */
        void Prim(int v) {
            /* Throw out old tree */
            tree.clear();

            /* For every vertex we maintain the lowest weight edge connecting it to
                selected vertices; we hold the edge's weight and other endpoint */
            /* Initialize other endpoint as -1 and lowest weight as INF (not connected) */
            vector<pair<weightType, int>> vertexRelation(this->n, make_pair(INT_MAX, -1));
            /* In the first step, v will be selected- this step is simply to ensure this */
            vertexRelation[v].first=0;

            /* We order unselected vertices on lowest weight edge */
            set<pair<weightType, int>> minheap;
            /* We also maintain whether present in 'minheap' or not */
            vector<bool> selected(this->n);

            /* Add all the vertices to the minheap */
            for(int i=0; i<this->n; i++)
                minheap.insert(make_pair(vertexRelation[i].first, i));

            /* We continually obtain lowest weight crossing edge and add it to the graph */
            weightType minCost=0;
            for(int i=0; i<this->n; i++) {
                /* Get the root of minheap and then delete */
                auto root=*minheap.begin();
                minheap.erase(minheap.begin());

                /* The newly selected vertex is u */
                /* The other endpoint of the selected edge is v*/
                int u=root.second, v=vertexRelation[root.second].second;
                weightType edgeWeight=root.first;
                selected[u]=true;
                minCost+=edgeWeight;

                /* We add the edge to the tree- except in the first step */
                if(v>=0)
                    tree[min(u, v)].push_back(make_pair(edgeWeight, max(u, v)));

                /* We update the lowest weight crossing edge for every unselected vertex */
                for(auto it: this->adj[u]) {
                    if(!selected[it.second] && it.first<vertexRelation[it.second].first) {
                        minheap.erase(make_pair(vertexRelation[it.second].first, it.second));
                        vertexRelation[it.second]=make_pair(it.first, u);
                        minheap.insert(make_pair(vertexRelation[it.second].first, it.second));
                    }
                }
            }

            /* Print minimum cost and set updated to true */
            cout<<minCost<<endl;
            this->updated=true;
        }

        /* Generates MST via Kruskal's algorithm */
        void Kruskal() {
            /* Throw out old tree */
            tree.clear();

            /* Sort all edges based on weight after storing in a vector */
            vector<pair<weightType, pair<int, int>>> edges;
            for(auto it1:this->adj) 
                for(auto it2: it1.second)
                    if(it1.first < it2.second)
                        edges.push_back(make_pair(it2.first, make_pair(it1.first, it2.second)));
            sort(edges.begin(), edges.end());

            /* Set parents and ranks for all the vertices via make_set function */
            weightType minCost = 0;
            parent.resize(this->n);
            rank.resize(this->n);
            for (int i = 0; i < this->n; i++)
                make_set(i);

            /* If an edge doesn't create a cycle, then we add it to the tree */
            for (auto it: edges) {
                if (find_set(it.second.first) != find_set(it.second.second)) {
                    minCost += it.first;
                    int u=min(it.second.first, it.second.second);
                    int v=max(it.second.first, it.second.second);
                    tree[u].push_back(make_pair(it.first, v));

                    /* All sets connected to u become connected to v and vice versa */
                    union_sets(u, v);
                }
            }

            /* Print minimum cost and set updated to true */
            cout<<minCost<<endl;
            this->updated=true;
        }

        /* Checks if tree is an updated MST */
        void TreeUptodate() {
            cout<<(this->updated ? "YES" : "NO")<<endl;
        }

        /* Prints edges in increasing order of weight, with index as tie-breaker */
        void DisplayTree() {
            /* Obtain all the edges in a vector and sort */
            vector<pair<weightType, pair<int, int>>> edges;
            for(auto it1:tree) 
                for(auto it2: it1.second)
                    edges.push_back(make_pair(it2.first, make_pair(it1.first, it2.second)));
            sort(edges.begin(), edges.end());

            /* Print all the edges */
            for(auto it:edges)
                cout<<it.second.first<<" "<<it.second.second<<" "<<it.first<<endl;
        }

    private:
        /* Since we only display, 'tree' here only store (u, v) if u<v */
        map<int, list<pair<weightType, int> > > tree;

        vector<int> parent, rank;

        /* Sets the parent and rank of a vertex (singleton set) */
        void make_set(int v) {
            parent[v] = v;
            rank[v] = 0;
        }

        /* We find the representative of the set which v belongs to */
        int find_set(int v) {
            if (v == parent[v])
                return v;
            return parent[v] = find_set(parent[v]);
        }

        /* Merge the sets which a and b belong to and set representative */
        void union_sets(int a, int b) {
            a = find_set(a);
            b = find_set(b);
            if (a != b) {
                if (rank[a] < rank[b])
                    swap(a, b);
                parent[b] = a;
                if (rank[a] == rank[b])
                    rank[a]++;
            }
        }
};

int32_t main() {
    /* Obtain number of vertices and number of queries */
    int n; cin>>n;
    int q; cin>>q;

    /* Create undirected graph with n vertices */
    MST<long long> G(n);

    /* Obtain inputs and process accordingly */
    while((q--)>0) {
        string input; cin>>input;
        if(input.compare("ADDV")==0) {
            G.AddV();
        } else if(input.compare("ADDE")==0) {
            int a, b; long long w; cin>>a>>b>>w;
            G.AddE(a, b, w);
        } else if(input.compare("DELE")==0) {
            int a, b; cin>>a>>b;
            G.DelE(a, b);
        } else if(input.compare("Prims")==0) {
            int v; cin>>v;
            G.Prim(v);
        } else if(input.compare("Kruskal")==0) {
            G.Kruskal();
        } else if(input.compare("TreeUptodate")==0) {
            G.TreeUptodate();
        } else {
            G.DisplayTree();
        }
    }
}