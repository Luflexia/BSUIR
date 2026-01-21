#!/usr/bin/env python3
"""
Лабораторная работа №8: Коллективные операции в MPI
========================================================

Задание:
- Создавать произвольное количество групп (из командной строки)
- В каждую группу включить случайное количество процессов
- Каждая группа умножает матрицы
- Замерить время вычисления в каждой группе
- Сравнить со временем парных операций
- Исходные данные из файлов (доступны всем узлам)
- Процессы читают свою порцию из файла
- Результаты пишут в файл группы

Использование:
  mpirun -np 8 python complete_mpi_lab.py [num_groups] 

Примеры:
  mpirun -np 8 python complete_mpi_lab.py 2    # 2 группы на 8 процессах
  mpirun -np 8 python complete_mpi_lab.py 3    # 3 группы на 8 процессах
  mpirun -np 8 python complete_mpi_lab.py      # 2 группы по умолчанию
"""

import sys
import os
import numpy as np
import time
from pathlib import Path
from mpi4py import MPI
import random

# ==================== КОНСТАНТЫ ====================

DATA_DIR = os.path.join(os.path.dirname(__file__), "lab_data")
MATRIX_SIZE = 500  # размер матриц по умолчанию 500x500


# ==================== ФУНКЦИИ ДЛЯ РАБОТЫ С ФАЙЛАМИ ====================

def create_test_matrices():

    if not os.path.exists(DATA_DIR):
        os.makedirs(DATA_DIR)

    matrix_a_file = os.path.join(DATA_DIR, "matrix_a.txt")
    matrix_b_file = os.path.join(DATA_DIR, "matrix_b.txt")

    # Если файлов нет — генерируем
    if not os.path.exists(matrix_a_file) or not os.path.exists(matrix_b_file):
        print(f"[Rank 0] Генерирую матрицы {MATRIX_SIZE}x{MATRIX_SIZE}...")
        matrix_a = np.random.rand(MATRIX_SIZE, MATRIX_SIZE)
        matrix_b = np.random.rand(MATRIX_SIZE, MATRIX_SIZE)
        np.savetxt(matrix_a_file, matrix_a, fmt='%.6f')
        np.savetxt(matrix_b_file, matrix_b, fmt='%.6f')
        print(f"[Rank 0] Матрицы сохранены в {DATA_DIR}/")
        return matrix_a, matrix_b
    else:
        # Файлы существуют — проверим размер и при несовпадении перегенерируем
        print(f"[Rank 0] Матрицы найдены в {DATA_DIR}/")
        matrix_a = np.loadtxt(matrix_a_file)
        matrix_b = np.loadtxt(matrix_b_file)
        if matrix_a.shape != (MATRIX_SIZE, MATRIX_SIZE) or matrix_b.shape != (MATRIX_SIZE, MATRIX_SIZE):
            print(f"[Rank 0] Размер файловых матриц {matrix_a.shape} и {matrix_b.shape} != {MATRIX_SIZE}x{MATRIX_SIZE}. Перегенерация…")
            matrix_a = np.random.rand(MATRIX_SIZE, MATRIX_SIZE)
            matrix_b = np.random.rand(MATRIX_SIZE, MATRIX_SIZE)
            np.savetxt(matrix_a_file, matrix_a, fmt='%.6f')
            np.savetxt(matrix_b_file, matrix_b, fmt='%.6f')
            print(f"[Rank 0] Матрицы обновлены в {DATA_DIR}/")
        return matrix_a, matrix_b


def get_matrix_row_range(filename, rank, total_processes):
    """
    Определение диапазона строк для процесса БЕЗ загрузки всей матрицы в память

    Args:
        filename: путь к файлу
        rank: номер процесса
        total_processes: всего процессов

    Returns:
        (total_rows, start_row, end_row) - общее число строк и диапазон для этого процесса
    """
    # Считаем количество строк в файле без загрузки содержимого
    with open(filename, 'r') as f:
        total_rows = sum(1 for _ in f)

    rows_per_process = total_rows // total_processes
    remainder = total_rows % total_processes

    if rank < remainder:
        start_row = rank * (rows_per_process + 1)
        end_row = start_row + rows_per_process + 1
    else:
        start_row = remainder * (rows_per_process + 1) + (rank - remainder) * rows_per_process
        end_row = start_row + rows_per_process

    return total_rows, start_row, end_row


def read_matrix_rows(filename, rank, total_processes):
    """
    Чтение процессом ТОЛЬКО своей порции строк из файла матрицы
    Оптимизация: читает только нужные строки, не загружая весь файл

    Args:
        filename: путь к файлу
        rank: номер процесса
        total_processes: всего процессов

    Returns:
        numpy array с порцией строк матрицы
    """
    total_rows, start_row, end_row = get_matrix_row_range(filename, rank, total_processes)

    rows_to_read = []

    # Читаем файл построчно и берём только нужные строки
    with open(filename, 'r') as f:
        for i, line in enumerate(f):
            if start_row <= i < end_row:
                # Парсим строку как числа
                row = np.fromstring(line, sep=' ')
                rows_to_read.append(row)

    if not rows_to_read:
        # Если нет строк (в крайнем случае), возвращаем пустую матрицу
        return np.array([]).reshape(0, MATRIX_SIZE)

    return np.array(rows_to_read)


def write_results_to_file(group_id, rank, local_result, group_size):

    if not os.path.exists(DATA_DIR):
        os.makedirs(DATA_DIR)

    result_file = os.path.join(DATA_DIR, f"group_{group_id}_results.txt")

    # Каждый процесс пишет свой результат
    if rank == 0:
        # Первый процесс группы пишет заголовок
        with open(result_file, 'w') as f:
            f.write(f"Результаты умножения матриц группы {group_id}\n")
            f.write(f"Размер группы: {group_size} процессов\n")
            f.write(f"Размер матриц: {MATRIX_SIZE}x{MATRIX_SIZE}\n")
            f.write("="*60 + "\n\n")

    # Все процессы дописывают свой результат
    with open(result_file, 'a') as f:
        f.write(f"Процесс rank {rank} (группа {group_id}):\n")
        f.write(f"Размер результата: {local_result.shape}\n")
        np.savetxt(f, local_result, fmt='%.6f')
        f.write("\n")


# ==================== MPI ФУНКЦИИ ====================

def group_matrix_multiply(matrix_a_part, matrix_b, group_id, group_rank, group_size, comm_group):

    print(f"[Group {group_id}, Rank {group_rank}] Получена матрица A {matrix_a_part.shape}")
    print(f"[Group {group_id}, Rank {group_rank}] Получена матрица B {matrix_b.shape}")

    # Выполняем умножение
    result = np.dot(matrix_a_part, matrix_b)

    print(f"[Group {group_id}, Rank {group_rank}] Вычисление завершено: результат {result.shape}")

    return result


def pairwise_matrix_multiply(matrix_a, matrix_b, rank, size):

    if rank == 0:
        print("\n[Pairwise] Начинаю парные операции...")

        # Разделяем матрицу A между процессами
        parts = np.array_split(matrix_a, size)
        local_a = MPI.COMM_WORLD.scatter(parts, root=0)
    else:
        local_a = MPI.COMM_WORLD.scatter(None, root=0)

    # Рассылаем матрицу B
    matrix_b_dist = MPI.COMM_WORLD.bcast(matrix_b if rank == 0 else None, root=0)

    # Вычисляем
    local_result = np.dot(local_a, matrix_b_dist)

    # Собираем результаты
    if rank == 0:
        results = MPI.COMM_WORLD.gather(local_result, root=0)
        final_result = np.vstack(results)
        return final_result
    else:
        MPI.COMM_WORLD.gather(local_result, root=0)
        return None


def create_groups(rank, size, num_groups):
    """
    Создание групп процессов

    Returns:
        (group_id, group_rank, group_size, comm_group)
    """
    # Распределяем процессы по группам
    group_assignments = []
    current_group = 0

    for proc_rank in range(size):
        if current_group < num_groups:
            group_assignments.append(current_group)
            current_group += 1
        else:
            # Начинаем заново если процессов больше чем групп
            current_group = 0
            group_assignments.append(current_group)
            current_group += 1

    # Перемешиваем для случайного распределения
    if rank == 0:
        random.shuffle(group_assignments)

    # Рассылаем назначения всем процессам
    group_assignments = MPI.COMM_WORLD.bcast(group_assignments, root=0)

    group_id = group_assignments[rank]

    # Создаём коммуникаторы для групп
    comm_group = MPI.COMM_WORLD.Split(group_id, rank)
    group_rank = comm_group.Get_rank()
    group_size = comm_group.Get_size()

    return group_id, group_rank, group_size, comm_group


def main():

    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()

    global MATRIX_SIZE

    # Получаем количество групп и размер матриц из аргументов
    num_groups = int(sys.argv[1]) if len(sys.argv) > 1 else 2
    if len(sys.argv) > 2:
        try:
            matrix_size_arg = int(sys.argv[2])
            if matrix_size_arg > 0:
                MATRIX_SIZE = matrix_size_arg
        except ValueError:
            if rank == 0:
                print("Предупреждение: некорректный размер матрицы в аргументах, используем значение по умолчанию.")

    if rank == 0:
        print("="*60)
        print(f"Запуск на {size} процессах")
        print(f"Матрицы: {MATRIX_SIZE}x{MATRIX_SIZE}")
        print("="*60 + "\n")

    if rank == 0:
        print(f"Создание {num_groups} групп процессов\n")

    # ==================== ГРУППОВЫЕ ОПЕРАЦИИ ====================

    # Создаём матрицы (только на процессе 0)
    if rank == 0:
        matrix_a, matrix_b = create_test_matrices()
    else:
        matrix_a, matrix_b = None, None

    # Рассылаем матрицы всем процессам
    matrix_a = comm.bcast(matrix_a if rank == 0 else None, root=0)
    matrix_b = comm.bcast(matrix_b if rank == 0 else None, root=0)

    comm.Barrier()

    # Создаём группы
    group_id, group_rank, group_size, comm_group = create_groups(rank, size, num_groups)

    if rank == 0:
        print(f"Группы созданы\n")
        print(f"{'='*60}")
        print("ЧАСТЬ 1: ГРУППОВЫЕ ОПЕРАЦИИ")
        print(f"{'='*60}\n")

    comm.Barrier()

    # Время начала групповых операций
    group_start_time = time.time()

    # Каждый процесс читает свою порцию из файла
    matrix_a_part = read_matrix_rows(
        os.path.join(DATA_DIR, "matrix_a.txt"),
        group_rank,
        group_size
    )

    print(f"[Group {group_id}, Rank {group_rank}] Прочитана часть матрицы A {matrix_a_part.shape}")

    # Выполняем умножение в группе
    group_result = group_matrix_multiply(
        matrix_a_part,
        matrix_b,
        group_id,
        group_rank,
        group_size,
        comm_group
    )

    # Записываем результаты в файл
    write_results_to_file(group_id, group_rank, group_result, group_size)

    comm_group.Barrier()

    group_elapsed = time.time() - group_start_time

    comm.Barrier()

    # ==================== ПАРНЫЕ ОПЕРАЦИИ ====================

    if rank == 0:
        print(f"\n{'='*60}")
        print("ЧАСТЬ 2: ПАРНЫЕ ОПЕРАЦИИ (для сравнения)")
        print(f"{'='*60}\n")

    comm.Barrier()

    # Время начала парных операций
    pairwise_start_time = time.time()

    # Выполняем парные операции
    pairwise_result = pairwise_matrix_multiply(matrix_a, matrix_b, rank, size)

    pairwise_elapsed = time.time() - pairwise_start_time

    # ==================== СРАВНЕНИЕ ====================

    comm.Barrier()

    if rank == 0:
        print(f"\n{'='*60}")
        print("СРАВНЕНИЕ РЕЗУЛЬТАТОВ")
        print(f"{'='*60}\n")

        print(f"Парные операции ({num_groups} групп):")
        print(f"   Время: {group_elapsed:.4f} сек")
        print(f"   Размер группы: {group_size} процессов")
        print(f"   Результат: {pairwise_result.shape}")

        print(f"\nГрупповые операции (стандартный MPI):")
        print(f"   Время: {pairwise_elapsed:.4f} сек")
        print(f"   Результат: {pairwise_result.shape}")

        # Проверка корректности
        expected = np.dot(matrix_a, matrix_b)
        is_correct = np.allclose(pairwise_result, expected, rtol=1e-5)
        print(f"\nПроверка корректности: {'ВЕРНО' if is_correct else 'ОШИБКА'}")

        # Сравнение времени
        if pairwise_elapsed > 0:
            ratio = group_elapsed / pairwise_elapsed
            print(f"\nСравнение времени:")
            if ratio < 1:
                print(f"   Парные операции быстрее в {1/ratio:.2f}x раз")
            else:
                print(f"   Групповые операции быстрее в {ratio:.2f}x раз")

        print(f"\n{'='*60}")
        print(f"Результаты сохранены в {DATA_DIR}/")
        print(f"   - group_*_results.txt (результаты каждой группы)")
        print(f"{'='*60}\n")

    comm.Barrier()


if __name__ == "__main__":
    main()

