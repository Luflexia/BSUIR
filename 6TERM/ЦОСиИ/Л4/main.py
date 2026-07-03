import numpy as np
import matplotlib.pyplot as plt

# Параметры сигнала
Fs = 1000  # Частота дискретизации (Гц)
f_low = 10     # Низкочастотный сигнал (Гц)
f_high = 100  # Высокочастотный шум (Гц)

# Генерация сигнала
t = np.linspace(0, 1, Fs)
signal = np.sin(2 * np.pi * t) + np.cos(2 * np.pi * 4*t)
# Визуализация
plt.figure(figsize=(10, 4))
plt.plot(t, signal)
plt.title("Исходный сигнал")
plt.xlabel("Время (с)")
plt.ylabel("Амплитуда")
plt.grid()
plt.show()

signal += 0.5 * np.sin(2 * np.pi * f_high * t)  # Шумовая компонента
signal += 0.2 * np.random.normal(size=t.shape)  # Аддитивный шум

# Визуализация
plt.figure(figsize=(10, 4))
plt.plot(t, signal)
plt.title("Исходный сигнал с шумом")
plt.xlabel("Время (с)")
plt.ylabel("Амплитуда")
plt.grid()
plt.show()

def blackman_window(N):
    """Генерация окна Блэкмана."""
    n = np.arange(N)
    return 0.42 - 0.5 * np.cos(2 * np.pi * n / (N - 1)) + 0.08 * np.cos(4 * np.pi * n / (N - 1))

def highpass_blackman(N, fc, fs):
    """Проектирование ВЧ фильтра с окном Блэкмана."""
    M = N // 2
    h = np.zeros(N)
    for i in range(N):
        if i == M:
            h[i] = 1 - 2 * fc / fs
        else:
            h[i] = -np.sin(2 * np.pi * fc * (i - M) / fs) / (np.pi * (i - M))
    # Умножение на окно Блэкмана
    window = blackman_window(N)
    h *= window
    # Инвертируем для получения ВЧ фильтра
    h = -h
    h[M] += 1
    # Нормировка
    h /= np.sum(np.abs(h))
    return h

# Параметры фильтра
N = 101   # Порядок фильтра (нечетный для симметрии)
fc = 50   # Частота среза (Гц)
fs = Fs   # Частота дискретизации

# Создание фильтра
h_hp = highpass_blackman(N, fc, fs)

# Применение фильтра (свертка)
filtered_hp = np.convolve(signal, h_hp, mode='same')

# Визуализация
plt.figure(figsize=(10, 4))
plt.plot(t, signal, label="Исходный сигнал")
plt.plot(t, filtered_hp, label="После ВЧ фильтра")
plt.title("Результат ВЧ фильтрации (окно Блэкмана)")
plt.xlabel("Время (с)")
plt.ylabel("Амплитуда")
plt.legend()
plt.grid()
plt.show()

# АЧХ фильтра
H = np.fft.fft(h_hp, 2048)
freq = np.fft.fftfreq(2048, 1/fs)[:1024]
plt.figure(figsize=(10, 4))
plt.plot(freq, 20 * np.log10(np.abs(H[:1024])))
plt.axvline(fc, color='r', linestyle='--', label=f'Fс = {fc} Гц')
plt.title("АЧХ ВЧ фильтра (окно Блэкмана)")
plt.xlabel("Частота (Гц)")
plt.ylabel("Усиление (дБ)")
plt.ylim(-60, 5)
plt.grid()
plt.legend()
plt.show()

def recursive_lowpass(x, alpha):
    """Однородный рекурсивный НЧ фильтр."""
    y = np.zeros_like(x)
    y[0] = x[0]
    for i in range(1, len(x)):
        y[i] = alpha * x[i] + (1 - alpha) * y[i-1]
    return y

# Параметры фильтра
alpha = 0.05  # Коэффициент сглаживания (0 < alpha < 1)

# Применение фильтра
filtered_recursive = recursive_lowpass(signal, alpha)

# Визуализация
plt.figure(figsize=(10, 4))
plt.plot(t, signal, label="Исходный сигнал")
plt.plot(t, filtered_recursive, label="После рекурсивного фильтра")
plt.title("Результат рекурсивной фильтрации")
plt.xlabel("Время (с)")
plt.ylabel("Амплитуда")
plt.legend()
plt.grid()
plt.show()

# АЧХ фильтра
w = np.linspace(0, np.pi, 1000)
H = alpha / (1 - (1 - alpha) * np.exp(-1j * w))
freq = w * Fs / (2 * np.pi)
plt.figure(figsize=(10, 4))
plt.plot(freq, 20 * np.log10(np.abs(H)))
plt.title("АЧХ рекурсивного НЧ фильтра")
plt.xlabel("Частота (Гц)")
plt.ylabel("Усиление (дБ)")
plt.grid()
plt.show()