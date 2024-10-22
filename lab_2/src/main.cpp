#include <iostream>
#include <vector>
#include <string>
#include <mpi.h>
#include <cmath>

using namespace std;

const double E = 0.00001;
const int N = 2;
double A[N][N];

void multMatrixes(double x[N], double y[N]){
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            y[i] += A[i][j] * x[j]; 
}

void subMatrixes(double x[N], double y[N], double z[N]){
    for(int i = 0; i < N; i++)
        z[i] = x[i] - y[i];
}

double scalMatrixes(double u[N], double v[N]){
    double x = 0;
    for(int i = 0; i < N; i++)
        x += u[i] * v[i]; 
    return x;
}

double powMatrix(double u[N]){
    double x = 0;
    for(int i = 0; i < N; i++)
        x += u[i] * u[i];
    x = sqrt(x);
    return x;
}

bool kritEnd_bool(double Ax_n[N], double b[N]){
    double z[N]{0};
    subMatrixes(Ax_n, b, z);
    double k = powMatrix(z)/powMatrix(b);
    cout << k << endl;
    if(k < E)
        return true;
    else
        return false;
}

void mainProg(int world_size, int world_rank){
    double x_n[N]{0};
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            if(i == j)
                A[i][j] = 2;
            else
                A[i][j] = 1;
    do{
        double b[N]{N+1};
        double Ax_n[N]{0};
        multMatrixes(x_n, Ax_n);
        double y_n[N]{0};
        subMatrixes(Ax_n, b, y_n);
        double Ay_n[N]{0};
        multMatrixes(y_n, Ay_n);
        double r_n = scalMatrixes(y_n, Ay_n) / scalMatrixes(Ay_n, Ay_n);
        for(int i = 0; i < N; i++)
            y_n[i] *= r_n;
        double x_n1[N];
        subMatrixes(x_n, y_n, x_n1); 
        for(int i = 0; i < N; i++){
            x_n[i] = x_n1[i];
            cout << x_n[i] << endl;
        }
        cout << endl;
    }while (kritEnd_bool);
    
}   

int main(int argc, char* argv[]) {
    // Initialize the MPI environment 
    MPI_Init(&argc, &argv);
    
    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    // Create a vector to store the array elements
    mainProg(world_rank, world_rank);
    
    // Finalize the MPI environment.
    MPI_Finalize();
    
    return 0;
}
