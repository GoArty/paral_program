import random
import numpy as np
import threading, time
n_1 = 501  # Размерность матрицы
m_1 = 520
matrix_1 = np.array([[random.randint(-1000, 1000) for _ in range(m_1)] for _ in range(n_1)])

n_2 = 520  # Размерность матрицы
m_2 = 515
matrix_2 = np.array([[random.randint(-1000, 1000) for _ in range(m_2)] for _ in range(n_2)])

print("ожидаемый результат:")
print(np.matmul(matrix_1, matrix_2))

def multiplicationOfCube(n, final_matrix):
    for i in range(n):
        for j in range(m_2):
            c = 0
            for k in range(len(matrix_1[i])):
                c += matrix_1[i][k] * matrix_2[j][k]
            final_matrix[i, j] = c
    
def multiplicationOfCubeThreading(n, h, b, final_matrix):
    n_min = 0
    if h > 0:
        if h == 1:
            n_min = n-1
        else:
            n_min = b*(h-1)*n
        n = b*h*n
    if n == 0:
        n = n_1

    for i in range(n_min, n):
        if i >= len(matrix_1):
            break
        for j in range(m_2):
            c = 0
            for k in range(len(matrix_2[j])):
                c += matrix_1[i][k] * matrix_2[j][k]
            final_matrix[i, j] = c


def NThreadings(b):
    final_matrix = np.array([[random.randint(-1000, 1000) for _ in range(m_2)] for _ in range(n_1)])
    #print(final_matrix)

    for h in range(b):
        if n_1 %2 !=0:
            thread = threading.Thread(name='worker', 
                                target=multiplicationOfCubeThreading, 
                                args=(n_1//b +1, h, b, final_matrix,))
        else:
            thread = threading.Thread(name='worker', 
                                target=multiplicationOfCubeThreading, 
                                args=(n_1//b, h, b, final_matrix,))
        thread.start()
    
    t_start = time.time()
    while any(th.is_alive() 
              for th in threading.enumerate() 
              if th.name == 'worker'):
        continue
    
    tm = round(time.time() - t_start, 2)
    print(f'{b} thread время: {tm}')
    print(final_matrix)


if __name__ == '__main__':
    print("1-я матрица:")
    print(matrix_1)
    print("2-я матрица:")
    print(matrix_2)
    matrix_2 = matrix_2.T

    final_matrix = np.array([[random.randint(-1000, 1000) for _ in range(m_2)] for _ in range(n_1)])

    print("заготовка финальной матрицы:")
    print(final_matrix)

    t_start = time.time()

    multiplicationOfCube(n_1, final_matrix)

    tm = round(time.time() - t_start, 2)

    print(f'1 thread время: {tm}')
    print(final_matrix)
    
    NThreadings(1)
    NThreadings(2)
    NThreadings(4)
    NThreadings(6)
    NThreadings(8)
    NThreadings(10)