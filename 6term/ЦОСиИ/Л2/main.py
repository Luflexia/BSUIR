import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile
from scipy.signal import convolve


def dft(x):
    N = len(x)
    X = np.zeros(N, dtype=complex)
    for k in range(N):
        for n in range(N):
            X[k] += x[n] * np.exp(-2j * np.pi * k * n / N)
    return X/N

def idft(X):
    N = len(X)
    x = np.zeros(N, dtype=complex)
    for n in range(N):
        for k in range(N):
            x[n] += X[k] * np.exp(2j * np.pi * k * n / N)
    return x

# Реализация свертки и корреляции
def convolution(x, y):
    n = len(x)
    m = len(y)
    result = np.zeros(n + m - 1, dtype=np.float64)
    for i in range(n+m-1):
        for j in range(m):
            if 0 <= i - j < n:
                result[i] += x[i-j] * y[j]
    return result

def correlation(x, y):
    n = len(x)
    m = len(y)
    result = np.zeros(n + m - 1, dtype=np.float64)
    for i in range(n):
        for j in range(m):
            result[i - j + (m - 1)] += x[i] * y[j]
    return result

# Параметры сигнала
Fs = 100  # Частота дискретизации (Гц)
duration = 1  # 1 секунда
t = np.linspace(0, 2* np.pi*duration, int(Fs * duration), endpoint=False)
# Генерация сигналов
f1 = 1  # Гц
f2 = 4  # Гц
signal_sin = np.sin(f1 * t)
signal_cos = np.cos(f2 * t)

# Визуализация сигналов
plt.figure(figsize=(12, 6))

plt.subplot(2, 1, 1)
plt.plot(t, signal_sin, label=f'sin(2π*{f1}t)')
plt.title('Сигнал sin')
plt.xlabel('Время (сек)')
plt.xlim(0, 2*np.pi)  # Явно задаем границы оси X
plt.grid()

plt.subplot(2, 1, 2)
plt.plot(t, signal_cos, label=f'cos(2π*{f2}t)')
plt.title('Сигнал cos')
plt.xlabel('Время (сек)')
plt.xlim(0, 2*np.pi)  # Явно задаем границы оси X
plt.grid()

plt.tight_layout()
plt.show()

# Сохранение в WAV-файлы
def save_to_wav(signal, filename, Fs):
    signal_normalized = np.int16((signal / np.max(np.abs(signal))) * 32767)
    wavfile.write(filename, Fs, signal_normalized)

save_to_wav(signal_sin, 'signal_sin.wav', Fs)
save_to_wav(signal_cos, 'signal_cos.wav', Fs)



# Вычисление свертки и корреляции
conv_result = convolution(signal_sin, signal_cos)
corr_result = correlation(signal_sin, signal_cos)
# conv_result = np.convolve(signal_sin, signal_cos)
# corr_result = np.correlate(signal_sin, signal_cos, mode='full')


# Визуализация результатов
plt.figure(figsize=(12, 6))

plt.subplot(2, 1, 1)
plt.plot(conv_result, label='Свертка sin и cos')
plt.title('Результат свертки')
plt.grid()

plt.subplot(2, 1, 2)
plt.plot(corr_result, label='Корреляция sin и cos')
plt.title('Результат корреляции')
plt.grid()

plt.tight_layout()
plt.show()

# Сохранение свертки в WAV
save_to_wav(conv_result, 'conv_result.wav', Fs)

# Свертка через Фурье-преобразование
def fft_convolution(x, y):
    n = len(x)
    m = len(y)
    L = n + m - 1
    x_pad = np.pad(x, (0, L - n))
    y_pad = np.pad(y, (0, L - m))
    X = dft(x_pad)
    Y = dft(y_pad)
    Z = X * Y
    z = idft(Z)
    z = z * L
    return np.real(z)

# Корреляция через Фурье-преобразование
def fft_correlation(x, y):
    n = len(x)
    m = len(y)
    L = n + m - 1
    x_pad = np.pad(x, (0, L - n))
    y_pad = np.pad(y, (0, L - m))
    X = dft(x_pad)
    Y = dft(y_pad)
    Y_conj = np.conj(Y)
    Z = X * Y_conj
    z = idft(Z)
    z = np.fft.fftshift(z)
    z = z * L
    return np.real(z)

# Вычисление через Фурье
fft_conv_result = fft_convolution(signal_sin, signal_cos)
fft_corr_result = fft_correlation(signal_sin, signal_cos)

# Визуализация результатов Фурье
plt.figure(figsize=(12, 6))

plt.subplot(2, 1, 1)
plt.plot(fft_conv_result, label='Свертка через Фурье')
plt.title('Свертка через Фурье')
plt.grid()

plt.subplot(2, 1, 2)
plt.plot(fft_corr_result, label='Корреляция через Фурье')
plt.title('Корреляция через Фурье')
plt.grid()

plt.tight_layout()
plt.show()

# Сохранение свертки через Фурье
save_to_wav(fft_conv_result, 'fft_conv_result.wav', Fs)

# Оценка вычислительной сложности
print('Оценка сложности:')
print("Сложность свертки: O(N*M)")
print("Сложность корреляции: O(N*M)")

# Опционально: автокорреляция и автопроверка
autoconv = convolution(signal_sin, signal_sin)

plt.figure()
plt.plot(autoconv, label='Свертка sin с собой')
plt.title('Свертка сигнала sin с собой')
plt.grid()
plt.show()

# Опционально: автокорреляция и автопроверка
autocorr = correlation(signal_sin, signal_sin)

plt.figure()
plt.plot(autocorr, label='Автокорреляция sin')
plt.title('Автокорреляция сигнала sin')
plt.grid()
plt.show()

# Опционально: автокорреляция и автопроверка
autocorr = convolution(signal_sin, signal_sin)

plt.figure()
plt.plot(autocorr, label='Автокорреляция sin')
plt.title('Автокорреляция сигнала sin')
plt.grid()
plt.show()