#include<iostream>
#include<algorithm>
using namespace std;

/*============================================================================
 *  Class:      Matrix
 *  Purpose:    Hold grid data and provide 
 *              methods to perform related operations
 * ==========================================================================*/
class Matrix {
    public:
        /* Constructor takes size and pointer to matrix as inputs and initializes private variables */
        Matrix(int n, int** A): size(n), grid(A) {}

        /* The following functions perform operations as described by assignment description */
        void SPSUM();
        void MOD();
        void MRS(Matrix&, int k);
        void MADD(Matrix&, int x);

    private:
        int size;
        int** grid;
};

void Matrix::SPSUM() {
    /* Here we sum the modified rows and print */
    for(int i=0; i<size; i++) {
        int sum=0;
        for(int j=0; j<size; j++) {
            /* This condition chooses the correct modified value of the (i, j)th element */
            if((i+j)%2==0)
                sum+=grid[i][j];
            else 
                sum+=grid[j][i];
        }
        cout<<sum;

        /* We print newline in the last iteration */
        cout<<(i==size-1 ? "\n" : ",");
    }
}

void Matrix::MOD() {
    /* Output hold the product of the results of each row - careful about overflow!!*/
    long long output=1;

    for(int i=0; i<size; i++) {
        /* rowOutput stores the result of the the operation on a particular row, to be multiplied with total output */
        int rowOutput=0;
        if(i%3==0) {

            /* If i is divisible by 3, we need to find maximum of the row */
            rowOutput=grid[i][0];
            for(int j=1; j<size; j++)
                rowOutput=max(rowOutput, grid[i][j]);
        } else if(i%3==1) {

            /* If i gives remainder of 1, we need to find minimum of the row */
            rowOutput=grid[i][0];
            for(int j=1; j<size; j++)
                rowOutput=min(rowOutput, grid[i][j]);
        } else {

            /* If i gives remainder of 2, we need to get floored average */
            rowOutput=0;
            for(int j=0; j<size; j++)
                rowOutput+=grid[i][j];
            rowOutput/=size;
        }
        /* Updating the total output */
        output*=rowOutput;
    }
    cout<<output<<endl;
}

/* Here, second is the other matrix in the operation, and k is number of shifts */
void Matrix::MRS(Matrix& second, int k) {

    /* Shifts which are multiples of n*n bring matrix back to original position */
    k%=size*size;

    /* Printing sum of shifted matrices */
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            /* In the linear form of the matrix, (i*size+j)-k gives the index of the element in original matrix, - */
            /* which is mapped to (i, j)-> we add size*size so as to make the operand positive for modulo operator */
            int rowNumber= ( ( (size*size) + (i*size+j) - k ) % (size*size) ) / size;
            int columnNumber= ( ( (size*size) + (i*size+j) - k ) % (size*size) ) % size;

            /* T1 and T2 are the shift-accounted elements of first and second matrices */
            int T1= grid[rowNumber][columnNumber];
            int T2= second.grid[rowNumber][columnNumber];

            /* Getting sum */
            int sum= T1+T2;
            cout<<sum<<" ";
        }
        cout<<endl;
    }
}

/* Here, second is the other matrix in the operation and x is the number of the matrix to be transformed */
void Matrix::MADD(Matrix& second, int x) {

    /* Summing the matrices */
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            /* Here, the (i, j)th element of the tranformed matrix is simply the (j, i)th element of original */
            int sum;
            if(x==1) {
                sum=grid[j][i]+second.grid[i][j];
            } else {
                sum=grid[i][j]+second.grid[j][i];
            }
            cout<<sum<<" ";
        }
        cout<<endl;
    }
}

int main() {
    /* Taking inputs */
    int Q, n;
    cin>>Q>>n;

    /* Allocating memory for the matrices */
    int** A=new int*[n];
    for(int i=0; i<n; i++)
        A[i]=new int[n];
    
    int** B=new int*[n];
    for(int i=0; i<n; i++)
        B[i]=new int[n];

    /* Reading in the matrices */
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            cin>>A[i][j];

    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            cin>>B[i][j];

    /* Creating Matrix objects */
    Matrix first(n, A), second(n, B);

    /* Performing operations indicated by queries */
    for(int i=0; i<Q; i++) {
        string input;
        cin>>input;
        if(input.compare("SPSUM")==0) {
            first.SPSUM();
        } else if(input.compare("MOD")==0) {
            first.MOD();
        } else if(input.compare("MRS")==0) {
            int k;
            cin>>k;
            first.MRS(second, k);
        } else if(input.compare("MADD")==0) {
            int x;
            cin>>x;
            first.MADD(second, x);
        } else {
            cout<<"INVALID COMMAND"<<endl;
        }
    }
    
}