#include<bits/stdc++.h>
using namespace std;

/* Floating point infinity */
#define INF ((float)(1.0/0.0))

/*================================================================================
 *  Exchange has the follwing public functions:
 *      setColours: initializes colours of vertices
 *      setEdges: creates edges between vertices of compatible colours
 *      setRates: sets the exchange rate between currencies
 *      maximizeTarget: finds maximum amount of target currency derivable
 * ==============================================================================*/
class Exchange {
    public:
        /* Initialize number of currencies and colours */
        Exchange(int N, int C): n(N), c(C) {}

        /* Initialize colours of vertices */
        void setColours(const vector<int>& colourValues) {
            colour=colourValues;
        }

        /* Initialize edges between compatible and same colours */
        void setEdges(const vector<vector<bool>>& colourEdges) {
            edge=colourEdges;
            for(int i=0; i<c; i++)
                edge[i][i]=true;
        }

        /* Initialize exchange rates between currencies */
        void setRates(const vector<vector<float>>& exchangeRates) {
            rate=exchangeRates;
        }

        /* Determine maximum amount of target currency derivable from source currency */
        float maximizeTarget(int s, int t, float X) {
            /* d[i][j] is initialized as rate[i][j] if edge exists or -1 if not */
            vector<vector<float>> d(n, vector<float>(n));
            for(int i=0; i<n; i++)
                for(int j=0; j<n; j++)
                    d[i][j]=(edge[colour[i]][colour[j]] ? rate[i][j] : -1);

            /* d[i][j] denotes the best conversion factor from i to j with 0...k as intermediate currencies */
            /* In the update step, we check if adding k-th vertex improves answer */
            for(int k=0; k<n; k++)
                for(int i=0; i<n; i++)
                    for(int j=0; j<n; j++)
                        if(d[i][k]!=-1 && d[k][j]!=-1 && d[i][k]*d[k][j]>d[i][j])
                            d[i][j]=d[i][k]*d[k][j];

            /* Check if target is reachable at all */
            if(d[s][t]<0.0) return -1.0;

            /* Check if increasing reachable cycle exists */
            for(int i=0; i<n; i++)
                for(int j=0; j<n; j++)
                    if(d[s][i]!=-1 && d[i][j]*d[j][i]>1.0)
                        return INF;

            /* Return best answer */
            return X*d[s][t];
        }

    private:
        /* Number of currencies and colours */
        int n, c;
        /* Exchange rates */
        vector<vector<float>> rate;
        /* Colours of vertices */
        vector<int> colour;
        /* Edges between colours */
        vector<vector<bool>> edge;
};

int main()
{
    /* Read number of colours and edges */
    int c; cin>>c;
    int m; cin>>m;

    /* Read edges */
    vector<vector<bool>> edges(c, vector<bool>(c));
    int in1, in2;
    for(int i=0; i<m; i++) {
        cin>>in1>>in2;
        edges[in1][in2]=edges[in2][in1]=true;
    }
    /* Read colours */
    int n; cin>>n;
    vector<int> colour(n);
    for(int i=0; i<n; i++)
        cin>>colour[i];

    /* Read exchange rates */
    vector<vector<float>> rate(n, vector<float>(n));
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            cin>>rate[i][j];

    /* Create Exchange object and initialize data */
    Exchange E(n, c);
    E.setEdges(edges);
    E.setColours(colour);
    E.setRates(rate);

    /* Process all queries */
    int q; cin>>q;
    while(q--) {
        int s, t;
        float X;
        cin>>s>>t>>X;
        float ans=E.maximizeTarget(s, t, X);
        if(ans==INF)
            cout<<"INF"<<endl;
        else if(ans<0)
            cout<<"-1"<<endl;
        else
            printf("%.3f\n", ans);
    }
}