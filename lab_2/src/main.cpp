#include <iostream>
#include <vector>
#include <string>
#include <mpi.h>
#include <cmath>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;

const double E = 0.00001;
const int N = 1000;

// 1 88.36

void multMatrixes(double x[N], double y[N], double A[N][N], int MIN, int MAX){

    for(int i = MIN; i < MAX; i++)
        for(int j = 0; j < N; j++)
            y[i] += A[i][j] * x[i]; 
}

void subMatrixes(double x[N], double y[N], double z[N], int MIN, int MAX){
    for(int i = MIN; i < MAX; i++)
        z[i] = x[i] - y[i];
}

double scalMatrixes(double u[N], double v[N], int MIN, int MAX){
    double x = 0;
    for(int i = MIN; i < MAX; i++)
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

void printMatrix(double x[N], int MIN, int MAX)
{
    printf("\n");
    for(int i = MIN; i < MAX; i++)
        printf("%i\t%f\n", i+1, x[i]);
    printf("\n");
}

void fillMatrix(double x[N], double a, int MIN, int MAX){
    for (int i = MIN; i < MAX; i++)
        x[i] = a;
}

int kritEnd_bool(double x_n[N], double b[N], double A[N][N]){    
    
    double Ax_n[N];
    fillMatrix(Ax_n, 0, 0, N);
     
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            Ax_n[i] += A[i][j] * x_n[j]; 

    double z[N]{0};
    for(int i = 0; i < N; i++)
        z[i] = Ax_n[i] - b[i];

    double k = powMatrix(z)/powMatrix(b);

    if(k <= E)
        return 0;
    else
        return 1;
}

void mainProg(double size, double rank, int SREZ, int MIN, int MAX){
    double x_n[N];
    double b[N];

    int f = 1;
    
    double A[N][N];


    // Заполнение массива
    for (int i = MIN; i < MAX; i++) 
        for (int j = 0; j < N; j++) 
            if (i == j) 
                A[i][j] = 2;
             else 
                A[i][j] = 1;
            
        
    

    fillMatrix(x_n, 0, MIN, MAX);
    fillMatrix(b, N+1, MIN, MAX);

    /*
    if(rank == 0)
    {    
        fillMatrix(x_n, 0);
        fillMatrix(b, N+1);

        for(int i = 0; i < N; i++)
            for(int j = MIN; j < MAX; j++)
                if(i == j)
                    A[i][j] = 2;
                else
                    A[i][j] = 1;
        for (int i = 0; i < size; i++)
        {
            double[N][MAX-MIN] message = A;
            MPI_Send(&message, N*N, MPI_DOUBLE, i, i, MPI_COMM_WORLD);
        }
        auto message = x_n;
        MPI_Send(&message, N, MPI_DOUBLE, i, i+size, MPI_COMM_WORLD);
        auto message = b;
        MPI_Send(&message, N, MPI_DOUBLE, i, i+2*size, MPI_COMM_WORLD);
        
    }else{
        MPI_Recv(&A, N*N, MPI_DOUBLE, 0, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        A = received_message;

        double[N] received_message;
        MPI_Recv(&received_message, N*N, MPI_DOUBLE, 0, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        A = received_message;
    }
    */
    
    do{
        
        double Ax_n[N];
        fillMatrix(Ax_n, 0, MIN, MAX);
        multMatrixes(x_n, Ax_n, A, MIN, MAX);
        
        double y_n[N];
        fillMatrix(y_n, 0, MIN, MAX);
        subMatrixes(Ax_n, b, y_n, MIN, MAX);
      
        double Ay_n[N];
        fillMatrix(Ay_n, 0, MIN, MAX);
        multMatrixes(y_n, Ay_n, A, MIN, MAX);

        double r_n = scalMatrixes(y_n, Ay_n, MIN, MAX) / scalMatrixes(Ay_n, Ay_n, MIN, MAX);

        MPI_Barrier(MPI_COMM_WORLD);
        for(int i = MIN; i < MAX; i++)
            y_n[i] = r_n * y_n[i];

        subMatrixes(x_n, y_n, x_n, MIN, MAX);
        
        MPI_Barrier(MPI_COMM_WORLD);

        double received_message;
        double message;
      
        if(rank == 0)
            for (int i = 1; i < size; i++)
                for (int j = i*SREZ; j < (i+1)*SREZ; j++)
                {
                    MPI_Recv(&received_message, 1, MPI_DOUBLE, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    x_n[j] = received_message;
                }  
        
        else
            for (int i = MIN; i < MAX; i++)
            {
                message = x_n[i];
                MPI_Send(&message, 1, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);
            }
                

        MPI_Barrier(MPI_COMM_WORLD);

        if(rank == 0)
            f = kritEnd_bool(x_n, b, A);
    
        MPI_Barrier(MPI_COMM_WORLD);

        //printf("%d 1", f);
        
        int send[1]{f};

        MPI_Bcast(&send, 1, MPI_INT, 0, MPI_COMM_WORLD);        
        f = send[1];

    
        if(rank == 0)
            for (int i = 1; i < size; i++)
                for (int j = i*SREZ; j < (i+1)*SREZ; j++)
                {
                    message = x_n[j];
                    MPI_Send(&message, 1, MPI_DOUBLE, i, i, MPI_COMM_WORLD);
                }  
        else
            for (int i = MIN; i < MAX; i++)
            {
                MPI_Recv(&received_message, 1, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                x_n[i] = received_message;
            }
           
        MPI_Barrier(MPI_COMM_WORLD);

        
    }while(f != 0);

    if (rank == 0)
        printMatrix(x_n, 0, N);
    


}   

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int SREZ = N/size;
    int MIN = rank*SREZ;
    int MAX = (rank+1)*SREZ;      

    auto start_time = std::chrono::steady_clock::now(); 

    mainProg(size, rank, SREZ, MIN, MAX);
    
    if(rank == 0)
    {
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
        printf("Время: ");
        cout << elapsed_time / 1000.0 << endl;
    }

    MPI_Finalize();
    
    return 0;
}
