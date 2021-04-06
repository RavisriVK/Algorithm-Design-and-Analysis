#include<iostream>
#include<climits>
#include<vector>
#include<queue>
#include<set>
#include<map>
using namespace std;

/*=================================================================================================
 *  Graph template takes weightType as parameter and represents an undirected graph
 *  void AddV(): Adds an additional vertex
 *  void AddE(int a, int b, weightType w): Adds an edge between two vertices, with the given weight
 *  void DelE(int a, int b): Deletes an edge between two vertices
 *  void BFS(int a): Prints BFS order from vertex a
 *  void SSP(int a): Prints shortest distances to each vertex from a
 * ===============================================================================================*/
template<typename weightType>
class Graph {
    public:
        /* Constructor simply takes vertexCount and initializes n */
        Graph(int vertexCount): n(vertexCount) {}

        /* AddV just increments n */ 
        void AddV() {
            /* This is more of a 'logical' operation, as no steps need to be taken to update the adjacency list if you use map */
            n++;
        }
        /* Adds an edge between two vertices, with the given weight */
        void AddE(int a, int b, weightType w) {
            /* Check if vertices provided exist */
            if(a<0 || a>=n || b<0 || b>=n) {
                cerr<<"Vertex indices provided are invalid. Cannot add edge."<<endl;
                return;
            }
            /* Update the adjacency lists of a and b */
            adj[a].insert(make_pair(w, b));
            adj[b].insert(make_pair(w, a));
        }

        /* Deletes an edge between two vertices */
        void DelE(int a, int b) {
            /* Check if vertices provided exist */
            if(a<0 || a>=n || b<0 || b>=n) {
                cerr<<"Vertex indices provided are invalid. Cannot delete edge."<<endl;
                return;
            }
            /* We check whether edge is present using 'exists' */
            bool exists=false;

            /* Go through adjacency list of a and delete edge to b- if edge is present mark 'exists' */
            for(auto it:adj[a]) {
                if(b==it.second) {
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

            /* Go through adjacency list of b and delete edge to a */
            for(auto it:adj[b]) {
                if(a==it.second) {
                    adj[b].erase(it);
                    break;
                }
            }
        }

        /* Prints BFS traversal of graph from vertex a */
        void BFS(int a) {
            /* We maintain a boolean array to check whether a vertex has been visited or not */
            vector<bool> visited(n);
            /* A queue is also maintained which contains the next vertices to be visited */
            queue<int> q;

            /* We start off by adding a to queue, to begin traversal from a */
            q.push(a);

            /* Proceed while queue is not empty */
            while(!q.empty()) {
                /* We obtain the current vertex, and delete from queue */
                int current=q.front();
                q.pop();

                /* If it is already visited, go to next vertex, else mark visited */
                if(visited[current]) continue;
                visited[current]=true;

                /* Print current vertex */
                cout<<current<<" ";

                /* Push all its children to the queue */
                /* The adjacency list is ordered so that lower weight edges come first */
                for(auto it:adj[current])
                    if(!visited[it.second])
                        q.push(it.second);
            }
            cout<<endl;
        }
        void SSP(int a) {
            /* d maintains the current shortest distance from a */
            vector<weightType> d(n);

            /* We obtain the functionality of priority_queue with key-pair update functionality using set<pair<weightType, int> > */
            set<pair<weightType, int> > q;

            /* Set d values of every vertex except a as MAX_INT, and add the vertices to the priority_queue */
            for(int i=0; i<n; i++)
                if(i!=a)
                    q.insert(make_pair((d[i]=INT_MAX), i));
            q.insert(make_pair((d[a]=0), a));

            /* In every iteration, we obtain vertex with smallest d value, and add it to the pile of vertices whose shortest distance from is known */
            while(!q.empty()) {
                /* 'current' is vertex with smallest d value */
                int current=q.begin()->second;
                q.erase(*q.begin());

                /* Update the d values of vertices adjacent to it, if need be */
                for(auto it:adj[current]) {
                    int neighbour=it.second;
                    weightType weight=it.first;
                    /* Check if d value of the neighbour can be reduced */
                    if((d[current]!=INT_MAX && d[neighbour]==INT_MAX) || d[current]+weight<d[neighbour]) {
                        /* This corresponds to a priority queue update operation */
                        q.erase(make_pair(d[neighbour], neighbour));
                        q.insert(make_pair((d[neighbour]=d[current]+weight), neighbour));
                    }
                }
            }

            /* Print the shortest distances from a */
            for(int i=0; i<n; i++)
                cout<<d[i]<<" ";
            cout<<endl;
        }

    private:
        /* n is the number of vertices */
        int n;

        /* We use this to implement our adjacency list for two purposes: */
        /*      1. set<pair<weightType, int> > automatically orders vertices by weight, and index in the manner needed */
        /*      2. map<int, set<pair<weightType, int> > > ensures that we don't need to allocate memory for set<pair<weightType, int> > if a vertex is isolated */
        map<int, set<pair<weightType, int> > > adj;
};

int32_t main() {
    /* Obtain number of vertices and number of queries */
    int n; cin>>n;
    int q; cin>>q;

    /* Create undirected graph with n vertices */
    Graph<long long> G(n);

    /* Obtain graph building arguments and process accordingly */
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
        }
    }

    /* Take two commands and output result of each */
    for(int i=0; i<2; i++) {
        string command; cin>>command;
        if(command.compare("BFS")==0) {
                int a; cin>>a;
                G.BFS(a);
        } else if(command.compare("SSP")==0) {
                int a; cin>>a;
                G.SSP(a);
        }
    }
}