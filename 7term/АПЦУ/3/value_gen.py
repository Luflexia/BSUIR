# generate_test_vectors.py
# Генерирует все 1024 комбинации и вычисляет expected_Y1, Y2 на основе логики MUX

with open('test_vectors.txt', 'w') as f:
    for i in range(1024):  # 0 to 1023
        bin_str = format(i, '010b')  # 10 бит: S1 S0 D1_3..D1_0 D2_3..D2_0
        S1 = bin_str[0]
        S0 = bin_str[1]
        D1 = [bin_str[2], bin_str[3], bin_str[4], bin_str[5]]  # D1_3 D1_2 D1_1 D1_0
        D2 = [bin_str[6], bin_str[7], bin_str[8], bin_str[9]]  # D2_3 D2_2 D2_1 D2_0
        
        # Выбор по S1S0 (00=idx0, 01=idx1, 10=idx2, 11=idx3)
        sel = int(S1 + S0, 2)
        Y1 = D1[3 - sel]  # Инвертируем индекс, т.к. D1_0 — idx3? Нет: по коду, "00"=D_0, "01"=D_1, "10"=D_2, "11"=D_3
        # Корректно: sel=0 ("00") -> D_0 (idx3 в списке? Список D1=[D1_3, D1_2, D1_1, D1_0], так что idx=3-sel? Нет.
        # D1[0]=D1_3 (для "11"), D1[1]=D1_2 ("10"), D1[2]=D1_1 ("01"), D1[3]=D1_0 ("00")
        # Так что Y1 = D1[3 - sel]
        Y1 = D1[3 - sel]
        Y2 = D2[3 - sel]
        
        # Запись: S1 S0 D1_3 D1_2 D1_1 D1_0 D2_3 D2_2 D2_1 D2_0 Y1 Y2
        line = S1 + S0 + ''.join(D1) + ''.join(D2) + Y1 + Y2 + '\n'
        f.write(line)

print("Файл test_vectors.txt сгенерирован (1024 строки).")