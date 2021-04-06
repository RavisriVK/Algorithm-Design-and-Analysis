#include<iostream>
#include<stack>
using namespace std;

/* This class provides support for basic stack functionality */
class Stack {
    public:
        /* Pushes element to top of stack */
        void Push(int x) {
            node* temp=new node(x, topOfStack);
            topOfStack=temp;
        }
        /* Removes and returns top element */
        int Pop() {
            /* Return -1 if empty */
            if(topOfStack==NULL) 
                return -1;

            /* Remove top element and update topOfStack pointer */
            node* toDelete=topOfStack;
            topOfStack=topOfStack->next;

            /* Free memory and return the element */
            int output=toDelete->value;
            delete toDelete;
            return output;
        }
        /* Returns top element */
        int Top() {
            /* Return -1 if empty */
            if(topOfStack==NULL)
                return -1;

            /* Return element at top of stack */
            return topOfStack->value;
        }
        /* Checks if stack is empty */
        bool isEmpty() {
            return topOfStack==NULL;
        }
        /* Deletes all stack elements */
        ~Stack() {
            while(topOfStack!=NULL) {
                node* temp=topOfStack;
                topOfStack=topOfStack->next;
                delete temp;
            }
        }
    private:
        /* Internal definition of node used by Stack */
        struct node {
            int value;
            node* next;
            node(int x, node* p): value(x), next(p) {}
        };

        /* This variable is a pointer to the top of the stack */
        node* topOfStack=NULL;
};

/* This class maintains common details of a graph and contains procedures acting on them */
class Graph {
    public:
    /* The Graph constructor takes the vertex count and initializes the adjacency matrix */
        Graph(int vertexCount) {
            n=vertexCount;
            adj=new bool*[n];
            for(int i=0; i<n; i++) {
                adj[i]=new bool[n];
                for(int j=0; j<n; j++)
                    adj[i][j]=false;
            }
        }

        /* Adding the an edge (i, j) to the adjacency matrix */
        void operator += (pair<int, int> edge) {
            adj[edge.first][edge.second]=true;
            adj[edge.second][edge.first]=true;
        }

        /* Adding the an edge (i, j) from the adjacency matrix */
        void operator -= (pair<int, int> edge) {
            adj[edge.first][edge.second]=false;
            adj[edge.second][edge.first]=false;
        }

        /* Returns true if a cycle is present in the graph */
        bool detectCycle() {
            /* Initializing the visited vertex to reflect none of the vertices have been visited */
            bool visited[n];
            for(int i=0; i<n; i++)
                visited[i]=false;
            
            /* Check, for every component, if a cycle is present */
            for(int i=0; i<n; i++) 
                if(!visited[i] && dfs_detectCycle(i, -1, visited))
                    return true;
            
            return false;
        }

        int components() {
            /* Initializing the visited vertex to reflect none of the vertices have been visited */
            bool visited[n];
            for(int i=0; i<n; i++)
                visited[i]=false;

            int componentCount=0;

            /* We go through all components here */
            for(int i=0; i<n; i++) {
                if(!visited[i]) {
                    componentCount++;
                    dfs_components(i, visited);
                }
            }
            return componentCount;
        }

        bool reach(int u, int v) {
            /* Initializing the visited vertex to reflect none of the vertices have been visited */
            bool visited[n];
            for(int i=0; i<n; i++)
                visited[i]=false;

            /* If u and v are in the same component then there is a path between them */
            return dfs_reach(u, v, visited);
        }

    private:
        /* Returns true if a back-edge (necessary and sufficient condition for cycle) is detected */
        bool dfs_detectCycle(int vertex, int parent, bool* visited) {
            visited[vertex]=true;
            for(int i=0; i<n; i++)
                /* Here we check if the edge exists and is incident on a previously visited vertex- not parent */
                if(adj[vertex][i] && i!=parent && (visited[i] || dfs_detectCycle(i, vertex, visited)))
                    return true;
            return false;
        }

        /* Marks all vertices reachable from the given vertex in a dfs-traversal */
        void dfs_components(int vertex, bool* visited) {
            /* The stack s maintains a list of nodes, in which we are currently exploring edges from top node */
            Stack s;
            s.Push(vertex);

            while(!s.isEmpty()) {
                /* We consider the top node and mark it as visited */
                int current=s.Pop();
                if(visited[current]) continue;
                visited[current]=true;

                /* We add all unvisited neighbours to stack */
                for(int i=0; i<n; i++)
                    if(adj[current][i] && !visited[i])
                        s.Push(i);
            }
        }

        /* Checks if two vertices belong to the same component */
        bool dfs_reach(int start, int end, bool* visited) {
            /* The stack s maintains a list of nodes, in which we are currently exploring edges from top node */
            Stack s;
            s.Push(start);

            while(!s.isEmpty()) {
                /* We consider the top node and mark it as visited */
                int current=s.Pop();
                if(visited[current]) continue;
                visited[current]=true;

                /* Since the current variable iterates through vertices reachable from start, we only test if current==end */
                if(current==end)
                    return true;

                /* We add all unvisited neighbours to stack */
                for(int i=0; i<n; i++)
                    if(adj[current][i] && !visited[i])
                        s.Push(i);
            }
            return false;
        }

        /* n is the number of vertices in the graph, and adj is the adjacency matrix */
        int n;
        bool** adj;
};

int main() {
    /* Read in number of stack queries */
    int Q; cin>>Q;

    /* Create stack */
    Stack s;
    for(int i=0; i<Q; i++) {
        /* Read in operation type */
        string input; cin>>input;

        /* Perform relevant operation */
        if(input.compare("push")==0) {
            int x; cin>>x;
            s.Push(x);
        } else if(input.compare("pop")==0) {
            cout<<(s.Pop())<<endl;
        } else if(input.compare("top")==0) {
            cout<<(s.Top())<<endl;
        } else {
            cout<<(s.isEmpty())<<endl;
        }
    }

    /* Read in number of vertices and graph queries */
    int n, m; cin>>n>>m;

    /* Create graph object */
    Graph g(n);
    for(int i=0; i<m; i++) {
        /* Read in input */
        string input; cin>>input;

        /* Perform relevant operation */
        if(input.compare("add")==0) {
            int u, v; cin>>u>>v;
            g+= pair<int, int>(u, v);
        } else if(input.compare("del")==0) {
            int u, v; cin>>u>>v;
            g-= pair<int, int>(u, v);
        } else if(input.compare("cycle")==0) {
            cout<<(g.detectCycle())<<endl;
        } else if(input.compare("components")==0) {
            cout<<(g.components())<<endl;
        } else {
            int u, v; cin>>u>>v;
            cout<<(g.reach(u, v))<<endl;
        }
    }
}