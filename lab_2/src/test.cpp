#include <iostream>
using namespace std;
const int N = 3;
double A[N][N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
void multMatrixes(double x[N], double y[N]){
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            y[i] += A[i][j] * x[j]; 
}

int main(){
    const int N = 3;
    double x[N] = {1, 2, 3};
    double y[N]{0};
    multMatrixes(x, y);
    for (int i=0; i<N; i++) {
        cout << y[i] << " ";
    }
    cout << endl;
    return 0;
}
