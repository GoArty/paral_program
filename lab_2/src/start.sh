#!/bin/bash

# Массив значений для n
values=(1 2 4 6)

# Компиляция программы
mpic++ -o main.o main.cpp

# Проверка успешности компиляции
if [ $? -ne 0 ]; then
    echo "Компиляция не удалась"
    exit 1
fi

# Запуск программы для каждого значения n
for n in "${values[@]}"; do
    echo "Запуск с n = $n"
    mpirun --oversubscribe -np $n main.o
done
