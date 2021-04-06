#include<iostream>
using namespace std;
int max(int a, int b, int c) {return max(max(a, b), c);}

/*=========================================================================================================================================================
 * The class BloomBrain implements the following public functions:
 *      BloomBrain(int roomCount);              => Allocates memory for an array of seraphic numbers and an array to determine whether an element is empty
 *      long long update(int* sublimity);       => Updates the seraphic numbers array
 *      int play();                             => Finds the minimum seraphic number to guarantee a win against any group
 *      void leave(int roomNumber);             => Empties the given room
 *      void join(int seraphic);                => Adds a person with the given seraphic number
 *      int occupied();                         => Prints number of occupied rooms
 * =======================================================================================================================================================*/
class BloomBrain {
    public:
        /* Allocates memory for the two described arrays, and initializes emptiness array */
        BloomBrain(int roomCount): n(roomCount), occupancy(roomCount) {
            a=new int[n];
            empty=new bool[n];

            /* Initially, all rooms are occupied */
            for(int i=0; i<n; i++)
                empty[i]=false;
        }

        /* Updates the seraphic numbers of people, and finds the number of quality conversations */
        /* The number of quality conversations is equal to the number of inversions */
        long long update(int* sublimity) {
            /* We take the non-empty rooms into consideration to find number of quality conversations */
            int array[n], temp[n], p=0;
            for(int i=0; i<n; i++) {
                a[i]=sublimity[i];
                if(!empty[i])
                    array[p++]=a[i];
            }
            /* The solution is the number of inversions in [0, p) */
            return inversions(array, temp, 0, p);
        }

        /* The best seraphic number of a group is the maximum subarray value */
        int play() {
            return maxSubarray(0, n).ms;
        }

        /* Updates the emptiness array and seraphic numbers array, and decrements occupancy */
        void leave(int roomNumber) {
            a[roomNumber]=0;
            empty[roomNumber]=true;
            occupancy--;
        }

        /* Inserts a given seraphic number at first empty spot from end of array, and increments occupancy */
        void join(int seraphic) {
            for(int i=n-1; i>=0; i--) {
                if(empty[i]) {
                    a[i]=seraphic;
                    empty[i]=false;
                    break;
                }
            }
            occupancy++;
        }

        /* Returns the number of non-empty rooms */
        int occupied() {
            return occupancy;
        }

    private:
        /* In quad structure:  
                ts: total sum of segment
                ps: maximum prefix sum of segment
                ss: maximum suffix sum of segment
                ms: maximum subarray sum of segment
        */
        struct quad {
            int ts, ps, ss, ms;
            quad(int in1, int in2, int in3, int in4): ts(in1), ps(in2), ss(in3), ms(in4) {}
        };

        /* We can obtain a divide and conquer solution by considering for each segment, four sums as above */
        /* The update policy is :
            total sum= left(total sum) + right(total sum)
            prefix sum= max(left(prefix sum), left(total sum)+right(prefix sum))
            suffix sum= max(right(suffix sum), right(total sum)+left(suffix sum))
            maximum subarray sum=max(left(maxSum), (right)maxSum, left(suffix sum)+right(prefix sum))
        */
        quad maxSubarray(int low, int high) {
            if(high<=low+1)
                return quad(a[low], a[low], a[low], a[low]);
            quad l=maxSubarray(low, (low+high)/2), r=maxSubarray((low+high)/2, high);
            return quad(l.ts + r.ts, max(l.ps, l.ts+r.ps), max(r.ss, r.ts+l.ss), max(l.ms, r.ms, l.ss+r.ps));
        }

        /* Sorts 'array' and returns original number of inversions */
        long long inversions(int* array, int* temp, int low, int high) {
            /* Array is already sorted and there are no inversions if there is only one element */
            if((high-low)<=1)
                return 0;

            /* Number of elements in left and right subarrays */
            int n1=(high-low)/2, n2=(1+high-low)/2;

            /* We sort left and right segments and store the corresponding number of inversions */
            long long inversionCount=inversions(array, temp, low, low+n1) + inversions(array, temp, low+n1, high);

            /* Store 'array' in a temporary array */
            for(int i=low; i<high; i++)
                temp[i]=array[i];

            /* We can merge the left and right segments by selecting the smaller number from the small ends of each segment */
            /* if temp[in1]>temp[in2], then the elements after temp[in1] in the left segment are also greater than temp[in2] */
            /* Accordingly, we can update the inversion count */
            int p1=0, p2=0, p=0;
            while(p1<n1 || p2<n2) {
                if(p1<n1 && (p2>=n2 || temp[low+p1]<=temp[low+n1+p2])) {
                    array[low+p++]=temp[low+p1++];
                } else {
                    array[low+p++]=temp[low+n1+p2++];
                    inversionCount+= n1-p1;
                }
            }

            return inversionCount;
        }

        int* a;         // seraphic numbers array
        bool* empty;    // emptiness array
        int n;          // number of rooms
        int occupancy;  // number of occupied rooms
};

int main(){
    /* Read in room count and number of queries */
    int N, Q; cin>>N>>Q;

    /* Create a BloomBrain instance on N rooms */
    BloomBrain institute(N);

    /* Maintain this array to store seraphic numbers */
    int sublimity[N];

    /* Process each query */
    for(int i=0; i<Q; i++) {
        string input; cin>>input;
        if(input.compare("update")==0) {
            for(int j=0; j<N; j++)
                cin>>sublimity[j];
            cout<<institute.update(sublimity)<<endl;
        } else if(input.compare("play")==0) {
            cout<<institute.play()<<endl;
        } else if(input.compare("leave")==0) {
            int idx; cin>>idx;
            institute.leave(idx);
        } else if(input.compare("join")==0) {
            int sublimeness; cin>>sublimeness;
            institute.join(sublimeness);
        } else {
            cout<<institute.occupied()<<endl;
        }
    }
}