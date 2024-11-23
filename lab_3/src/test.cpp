#include <stdio.h>
#include <stdlib.h>
#include <cmath>
const int N = 5;
void multMatrixes(double x[N], double y[N], double A[][N], int MIN, int MAX){

    for(int i = 0; i < N; i++)
        for(int j = MIN; j < MAX; j++)
            y[i] += A[i][j] * x[j]; 
}

int main(int argc, char** argv) {
    double y[N]{0};
    double u[N]{-2325, -79, 755, -467, -7597};
    double A[N][N]{{24, 769, 567, 2457, 568}, {58, 24, -568, 46, 56}, {769, 24, -68, 568, 68}, {0, 69, -58, 57, 75}, {35, 568, -454, 58, 568}};
    multMatrixes(u, y, A, 0, 3);
    multMatrixes(u, y, A, 3, 6);
    for (int i = 0; i < N; i++)
    {
        printf("%i\t%f\n", i, y[i]);
    }
    int g = 4/ 4;
    if(g%2 == 0)
{
    printf("1");
}
else{
    printf("%d", g);
}


    // Ведущий процесс рассылает массив всем остальным

    return 0;
}
