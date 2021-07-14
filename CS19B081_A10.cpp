#include<bits/stdc++.h>
using namespace std;

/*===============================================================================
 *  Arena contains all information about a minesweeper grid and has functions
 *  to process moves made by a user
 *      setArena: Sets dimensions, allocates memory, implements effect of mines
 *      play: Does a dfs-like traversal to check how many cells can be revealed
 *      lost: Checks if user has lost
 *      revealed: Returns number of cells revealed
 * =============================================================================*/

class Arena {
    public:
        /* Forms grid and adds mines */
        void setArena(int n, const vector<int>& minePositions){
            /* Set dimensions and allocate memory */
            N=n; grid.resize(N, vector<cell>(N));

            /* Go through every mine */
            for(auto position: minePositions) {
                int r=position/N, c=position%N;

                /* Mark cell as mine */
                grid[r][c].mine=true;

                /* Increment adjMines count of neighbours */
                for(int i=max(r-1, 0); i<=min(r+1, N-1); i++)
                    for(int j=max(c-1, 0); j<=min(c+1, N-1); j++)
                        grid[i][j].adjMines++;
            }
        }

        /* Visits a cell in the grid and processes the move */
        void play(int r, int c) {
            /* Increment revealCount if previously unseen and mark seen */
            revealCount+=grid[r][c].unseen;
            grid[r][c].unseen=false;

            /* If grid is a mine, blow player up */
            /* If the cell and its neighbours aren't mines, visit the neighbours */
            if(grid[r][c].mine)
                blownUp=true;
            else if(grid[r][c].adjMines==0)
                for(int i=max(r-1, 0); i<=min(r+1, N-1); i++)
                    for(int j=max(c-1, 0); j<=min(c+1, N-1); j++)
                        if(grid[i][j].unseen) play(i, j);
        }

        /* lost() and revealed() simply return blownUp and revealCount */
        bool lost() {return blownUp;}
        int revealed() {return revealCount;}

    private:
        /* cell represents elementary unit of grid */
        struct cell {
            bool mine=false, unseen=true;
            int adjMines=0;
        };

        int N, revealCount=0;
        vector<vector<cell>> grid;
        bool blownUp=false;
};

int main() {
    /* Read dimensions and mine locations */
    int N, M; cin>>N>>M;
    vector<int> locations(M); for(auto& it: locations) cin>>it;

    /* Create and set arena */
    Arena A;
    A.setArena(N, locations);

    /* While the game isn't over, go through player's moves */
    for(bool gameOver=false; !gameOver; gameOver|=cin.eof()) {
        /* Read in move and play it */
        int revealCount, r, c; cin>>r>>c;
        A.play(r, c);

        /* Process the result to see if he lost or won- also note revealCount */
        if(A.lost()) {
            cout<<"Lost"<<endl;
            gameOver=true;
        } else if((revealCount=A.revealed())==N*N-M) {
            cout<<"Won"<<endl;
            gameOver=true;
        } else {
            cout<<revealCount<<endl;
        }
    }
}