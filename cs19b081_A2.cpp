#include<iostream>
#include<algorithm>
using namespace std;

/* This structure is used when we need to pair two associated integer values */
struct point {
    int first, second;
    point(int x, int y): first(x), second(y) {}
};

/* Base class: here it holds the data variables and has some basic methods */
class Matrix {
    /* These functions are accessible from any function, including main */
    public:
        Matrix(int n, int m, int** A): rowCount(n), columnCount(m), grid(A) {}
        int binarySearch(int, int);
        point matrixSearch(int);

    /* The matrix dimensions can be accessed by the derived class */
    protected:
        int rowCount, columnCount;

    /* The matrix elements themselves are private and can't be accesses from derived class */
        int** grid;
};

/* Derived class: Add to the functionality of parent class */
class MatrixMedian : public Matrix {
    /* These functions are accessible from any function, including main */
    public:
        MatrixMedian(int n, int m, int** A): Matrix(n, m, A) {}
        int findMedian();

    /* This function is only for internal use in findMedian */
    private:
        int upperBound(int, int);
};

/* Returns column index of last element in row less than or equal to x */
int Matrix::binarySearch(int x, int k) {
    /* We are searching in the range [0, columnCount) */
    int left=0, right=columnCount;

    /* This loop keeps narrowing down the range */
    /* The loop invariant is that the desired element if present is in [left, right) */
    for(int middle=(left+right)/2; right-left>1; middle=(left+right)/2)
        (grid[k][middle]>x ? right : left)= middle;
    
    /* When we break out of loop, range will be [left, right=left+1), or {left} */
    /* So we only need to check if 'left' is the desired index */ 
    return grid[k][left]<=x ? left : -1;
}

/* Returns location of 'first' element in matrix equal to x */
point Matrix::matrixSearch(int x) {
    /* temp stores the result of the call to binarySearch */
    int temp;
    /* If x is present in a row i, then 1+binarySearch(x-1, i) should be the first column in row i where x is present */
    /* This is because binarySearch(x-1, i) gives the last column of an element less than x */
    for(int i=0; i<rowCount; i++)
        if((temp=1+binarySearch(x-1, i))!=columnCount && grid[i][temp]==x)
            return point(i, temp);

    /* If not found return (-1, -1) */
    return point(-1, -1);
}

/* Returns column index of first element in row greater than x */
int MatrixMedian::upperBound(int x, int k) {
    /* binarySearch(x, k) gives the last column of an element less than or equal to x */
    /* Therefore the next column should be the first column of an element greater than x */
    int temp= binarySearch(x, k);
    return temp+1!=columnCount ? temp+1 : -1;
}

/* Returns median of entire matrix */
int MatrixMedian::findMedian() {    
    int halfCount= (rowCount*columnCount)/2;

    /*Essentially, we will perform binary search over the entire range of the median */
    int left=-2147483648, right=2147483647;
    for(int middle=(left+right)/2; left<=right; middle=(left+right)/2) {
        /* An element is median if : */
        /* 1) The number of elements less than it (in all rows) is not greater than (N-1)/2 */
        /* 2) The number of elements less than or equal to it is greater than or equal to  (N+1)/2 */
        /* Using previously written upperBound function, we can quickly find both of the above counts across all rows */
        int lessThanOrEqualTo=0, lessThan=0;
        for(int i=0; i<rowCount; i++) {
            int temp1=upperBound(middle, i), temp2=upperBound(middle-1, i);
            lessThanOrEqualTo += temp1!=-1 ? temp1 : columnCount;
            lessThan += temp2!=-1 ? temp2 : columnCount;
        }

        /* If the condition doesn't hold, and the number of elements less than the element is greater than (N-1)/2, median is less than given candidate */
        /* Similarly if condition doesn't hold, and number of elements not greater than median candidate is less than or equal to (N-1)/2, median is greater than given candidate */
        if(lessThan<=halfCount && lessThanOrEqualTo>halfCount)
            return middle;
        else if(lessThan>halfCount)
            right=middle-1;
        else
            left=middle+1;
    }
    return -1;
}

int main() {
    /* Taking inputs */
    int n, m;
    cin>>n>>m;

    /* Allocating memory for the matrix */
    int** A=new int*[n];
    for(int i=0; i<n; i++)
        A[i]=new int[m];

    /* Reading in the matrix */
    for(int i=0; i<n; i++)
        for(int j=0; j<m; j++)
            cin>>A[i][j];

    /* Creating MatrixMedian object */
    MatrixMedian obj(n, m, A);

    int Q;
    cin>>Q;

    /* Performing operations indicated by queries */
    for(int i=0; i<Q; i++) {
        string input;
        cin>>input;
        if(input.compare("BS")==0) {
            int x, k;
            cin>>x>>k;
            cout<<obj.binarySearch(x, k)<<endl;
        } else if(input.compare("MS")==0) {
            int x;
            cin>>x;
            point temp=obj.matrixSearch(x);
            cout<<temp.first<<" "<<temp.second<<endl;
        } else {
            int median= obj.findMedian();
            cout<<median<<endl;
        }
    }
    
}