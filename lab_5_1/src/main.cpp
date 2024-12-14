#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

void initMatrix(vector<vector<int>>& matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) 
        for (int j = 0; j < cols; j++) 
            matrix[i][j] = rand() % 2;
}

int countLiveNeighbors(const vector<vector<int>>& matrix, int rows, int cols, int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) 
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int nx = (x + i + rows) % rows;
            int ny = (y + j + cols) % cols;
            count += matrix[nx][ny];
        }
    return count;
}

void evolveStep(const vector<vector<int>>& currentMatrix, vector<vector<int>>& nextMatrix, int rows, int cols, int startRow, int endRow) {
    for (int i = startRow; i < endRow; ++i) 
        for (int j = 0; j < cols; ++j) {
            int liveNeighbors = countLiveNeighbors(currentMatrix, rows, cols, i, j);
            if (currentMatrix[i][j] == 1) {
                if (liveNeighbors < 2 || liveNeighbors > 3) 
                    nextMatrix[i][j] = 0;
                
            } else 
                if (liveNeighbors == 3) 
                    nextMatrix[i][j] = 1;  
        }
}

void evolveStepWithThreads(vector<vector<int>>& currentMatrix, vector<vector<int>>& nextMatrix, int rows, int cols, int numThreads) {
    vector<thread> threads;
    int chunkSize = rows / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startRow = i * chunkSize;
        int endRow = (i == numThreads - 1) ? rows : (i + 1) * chunkSize;
        threads.emplace_back(evolveStep, ref(currentMatrix), ref(nextMatrix), rows, cols, startRow, endRow);
    }

    for (auto& t : threads)
        t.join();
}

void threadMain(vector<vector<int>>& currentMatrix, vector<vector<int>>& nextMatrix, int rows, int cols, int numSteps, int numThreads) {
    initMatrix(currentMatrix, rows, cols);

    auto start = chrono::high_resolution_clock::now();

    for (int step = 0; step < numSteps; ++step) {
        evolveStepWithThreads(currentMatrix, nextMatrix, rows, cols, numThreads);
        currentMatrix = nextMatrix;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Среднее время выполнения с "<< numThreads <<" потоками: " << duration.count() / numSteps  << " секунд" << endl;
}

int main() {
    srand(time(0));

    int rows = 10000;
    int cols = 10000;
    int numSteps = 100;

    vector<vector<int>> currentMatrix(rows, vector<int>(cols));
    vector<vector<int>> nextMatrix(rows, vector<int>(cols));

    threadMain(currentMatrix, nextMatrix, rows, cols, numSteps, 2);
    threadMain(currentMatrix, nextMatrix, rows, cols, numSteps, 4);
    threadMain(currentMatrix, nextMatrix, rows, cols, numSteps, 6);
    threadMain(currentMatrix, nextMatrix, rows, cols, numSteps, 8);
    threadMain(currentMatrix, nextMatrix, rows, cols, numSteps, 10);
    threadMain(currentMatrix, nextMatrix, rows, cols, numSteps, 12);
    threadMain(currentMatrix, nextMatrix, rows, cols, numSteps, 14);
    threadMain(currentMatrix, nextMatrix, rows, cols, numSteps, 16);

    initMatrix(currentMatrix, rows, cols);

    auto start = chrono::high_resolution_clock::now();

    for (int step = 0; step < numSteps; ++step) {
        evolveStep(currentMatrix, nextMatrix, rows, cols, 0, rows);
        currentMatrix = nextMatrix;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Среднее ремя выполнения без потоков: " << duration.count() / numSteps  << " секунд" << endl;

    return 0;
}
