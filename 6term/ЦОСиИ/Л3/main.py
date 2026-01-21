import numpy as np
import matplotlib.pyplot as plt
import pywt

# Генерация сигнала
n_samples = 1024
t = np.linspace(0, 1, n_samples)
f = np.sin(2 * np.pi * t) + np.cos(4 * 2 * np.pi * t)

# Добавление шума
np.random.seed(42)
noise = np.random.uniform(-1, 1, n_samples)
f_noisy = f + noise

# Визуализация исходного и зашумленного сигналов
plt.figure(figsize=(10, 6))
plt.plot(t, f, label='Исходный сигнал')
plt.plot(t, f_noisy, label='Зашумленный', alpha=0.7)
plt.grid(True)
plt.legend()
plt.title('Сравнение сигналов')
plt.show()
print(pywt.Wavelet('db2').dec_lo)
print(pywt.Wavelet('db2').dec_hi)
# Коэффициенты Добеши D2
lo_d = np.array(pywt.Wavelet('db2').dec_lo)
hi_d = np.array(pywt.Wavelet('db2').dec_hi)
lo_r = lo_d[::-1]
hi_r = hi_d[::-1]


# Функция прямого преобразования с коррекцией длины
def dwt(signal, lo_filter, hi_filter):
    pad = len(lo_filter)
    padded = np.pad(signal, (pad // 2, pad - pad // 2), mode='symmetric')
    lo = np.convolve(padded, lo_filter, mode='valid')[::2]
    hi = np.convolve(padded, hi_filter, mode='valid')[::2]
    return lo, hi


# Многоуровневое разложение
def multilevel_dwt(signal, levels):
    coeffs = []
    current = signal.copy()
    for _ in range(levels):
        a, d = dwt(current, lo_d, hi_d)
        coeffs.append((a, d))
        current = a
    return coeffs


# 4-уровневое разложение
coeffs = multilevel_dwt(f_noisy, 4)

# Визуализация коэффициентов
plt.figure(figsize=(12, 10))
titles = ['Уровень 1', 'Уровень 2', 'Уровень 3', 'Уровень 4']
for i in range(4):
    plt.subplot(4, 2, 2 * i + 1)
    plt.plot(coeffs[i][0])
    plt.title(f'Аппроксимация L{i + 1}')
    plt.grid(True)

    plt.subplot(4, 2, 2 * i + 2)
    plt.plot(coeffs[i][1])
    plt.title(f'Детали L{i + 1}')
    plt.grid(True)
plt.tight_layout()
plt.show()


# Исправленная функция обратного преобразования
def idwt(a, d, lo_filter, hi_filter):
    # Восстановление нулей
    a_up = np.zeros(2 * len(a))
    a_up[::2] = a
    d_up = np.zeros(2 * len(d))
    d_up[::2] = d

    # Свертка с фильтрами
    a_conv = np.convolve(a_up, lo_filter, mode='full')
    d_conv = np.convolve(d_up, hi_filter, mode='full')

    # Выравнивание длин
    min_length = min(len(a_conv), len(d_conv))
    reconstructed = a_conv[:min_length] + d_conv[:min_length]

    # Обрезание лишних коэффициентов
    trim = len(lo_filter) // 2
    return reconstructed[trim:-trim]


# Функция восстановления сигнала
def reconstruct(coeffs):
    current_a = coeffs[-1][0]
    for i in reversed(range(len(coeffs) - 1)):
        current_a = idwt(current_a, coeffs[i][1], lo_r, hi_r)
    return idwt(current_a, coeffs[0][1], lo_r, hi_r)[:len(f_noisy)]


# Первое восстановление без изменений
f_restored = reconstruct(coeffs)

# Визуализация до коррекции
plt.figure(figsize=(12, 5))
plt.subplot(211)
plt.plot(t, f_noisy)
plt.title('Зашумленный сигнал')
plt.grid(True)

plt.subplot(212)
plt.plot(t, f_restored)
plt.title('Восстановленный без коррекции')
plt.grid(True)
plt.tight_layout()
plt.show()

# Модификация коэффициентов
modified_coeffs = [list(c) for c in coeffs]
modified_coeffs[3][1] = modified_coeffs[3][1] * 0.5  # L4 детали уменьшены вдвое
modified_coeffs[2][1] = np.zeros_like(modified_coeffs[2][1])  # L3 детали обнулены

# Восстановление с коррекцией
f_denoised = reconstruct(modified_coeffs)

# Финальное сравнение
plt.figure(figsize=(12, 5))
plt.subplot(211)
plt.plot(t, f, label='Исходный')
plt.title('Исходный сигнал')
plt.grid(True)

plt.subplot(212)
plt.plot(t, f_denoised, label='Очищенный')
plt.title('Результат обработки')
plt.grid(True)
plt.tight_layout()
plt.show()