#include<bits/stdc++.h>
using namespace std;

/* MIN3 is a macro to find the minimum of three numbers */
#define MIN3(a, b, c) (min(min((a), (b)), (c)))

/*=============================================================================================
 *  class EDD contains methods to find the edit distance between two strings
 *      void assign(const string& a, const string& b): sets a and b as the two strings
 *      void compdist(): computes the edit table for the two given strings
 *      int dist(): returns the edit distance between the two given strings
 *      void table(): prints the edit table for the two given strings
 * ============================================================================================*/

class EDD {
    public:
        /* Sets input strings and initializes related parameters */
        /* const string& is used as no modifications are made and to avoid copying the strings */
        void assign(const string& a, const string& b) {
            /* Update boolean paramters */
            assigned=true;
            computed=false;

            /* Set the input strings */
            s1=a; s2=b;
        }
        void compdist() {
            /* If inputs unassigned throw exception */
            if(!assigned)
                throw "strings not found!";

            /* If already computed return */
            if(computed)
                return;
            
            /* Allocate sufficient memory for the edit table */
            dp.clear();
            dp.resize(s1.length()+1, vector<int>(s2.length()+1));

            /* Set the base case values for the table */
            for(int i=0; i<=s1.length(); i++)
                dp[i][0]=i;
            for(int i=0; i<=s2.length(); i++)
                dp[0][i]=i;

            /* Update the edit distances for prefixes s1[1...i] and s2[1...j] */
            for(int i=1; i<=s1.length(); i++)
                for(int j=1; j<=s2.length(); j++)
                    dp[i][j]=MIN3(dp[i-1][j-1]+(s1[i-1]!=s2[j-1]), dp[i][j-1]+1, dp[i-1][j]+1);

            /* Update 'computed' */
            computed=true;
        }
        int dist() {
            /* If inputs unassigned throw exception */
            if(!assigned)
                throw "strings not found!";

            /* Compute edit table if not already computed */
            if(!computed)
                compdist();

            /* Return edit distance between s1 and s2 */
            return dp[s1.length()][s2.length()];
        }
        void table() {
            /* If inputs unassigned throw exception */
            if(!assigned)
                throw "strings not found!";

            /* Compute edit table if not already computed */
            if(!computed)
                compdist();

            /* Print edit table */
            for(int i=1; i<=s1.length(); i++) {
                for(int j=1; j<=s2.length(); j++)
                    cout<<dp[i][j]<<" ";
                cout<<endl;
            }
        }
    private:
        /* 'assigned' notes whether inputs were set, and 'computed' notes whether edit table has been created */
        bool assigned=false, computed=false;

        /* s1 and s2 are the input strings */
        string s1, s2;
        
        /* dp is the edit table; dp[i][j] is the edit distance between s1[1..i] and s2[1..j] */
        vector<vector<int>> dp;
};

int main() {
    /* Create EDD object */
    EDD editTable;

    /* Find number of queries and process accordingly */
    int q; cin>>q;
    while(q--) {
        /* Try each query: if strings are unassigned catch the exception and notify user */
        try {
            string operation; cin>>operation;
            if(operation.compare("assign")==0) {
                string a, b; cin>>a>>b;
                editTable.assign(a, b);
            } else if(operation.compare("compdist")==0) {
                editTable.compdist();
            } else if(operation.compare("table")==0) {
                editTable.table();
            } else if(operation.compare("dist")==0) {
                cout<<editTable.dist()<<endl;
            }
        }
        catch(const char* error_name) {
            cerr<<error_name<<endl;
        }
    }
}