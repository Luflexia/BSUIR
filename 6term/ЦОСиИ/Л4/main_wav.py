import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile
import os

# Параметры сигнала
Fs = 1000  # Частота дискретизации (Гц)
t = np.arange(0, 1, 1/Fs)  # Временной вектор от 0 до 1 секунды с шагом 1/Fs
f_low = 10     # Частота низкочастотного полезного сигнала (Гц)
f_high = 100  # Частота высокочастотного шума (Гц)

# Генерация сигнала
signal = np.sin(2 * np.pi * f_low * t)  # Генерация синусоидального полезного сигнала

# Визуализация исходного сигнала
plt.figure(figsize=(10, 4))
plt.plot(t, signal)
plt.title("Исходный сигнал")
plt.xlabel("Время (с)")
plt.ylabel("Амплитуда")
plt.grid()
plt.show()

# Добавление шума к сигналу
signal += 0.5 * np.sin(2 * np.pi * f_high * t)  # Добавление высокочастотной шумовой компоненты
signal += 0.2 * np.random.normal(size=t.shape)  # Добавление аддитивного белого гауссовского шума

# Визуализация сигнала с шумом
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
    window = blackman_window(N)
    h *= window
    h = -h
    h[M] += 1
    h /= np.sum(np.abs(h))
    return h

# Параметры фильтра
N = 101   # Порядок фильтра (нечетный для симметрии типа I)
fc = 50   # Частота среза (Гц)
fs = Fs   # Частота дискретизации (используем ту же, что и для сигнала)

# Создание ВЧ фильтра
h_hp = highpass_blackman(N, fc, fs)

# Применение ВЧ фильтра (свертка)
filtered_hp = np.convolve(signal, h_hp, mode='same')

# Визуализация результата ВЧ фильтрации
plt.figure(figsize=(10, 4))
plt.plot(t, signal, label="Исходный сигнал")
plt.plot(t, filtered_hp, label="После ВЧ фильтра")
plt.title("Результат ВЧ фильтрации (окно Блэкмана)")
plt.xlabel("Время (с)")
plt.ylabel("Амплитуда")
plt.legend()
plt.grid()
plt.show()

# Расчет и визуализация АЧХ ВЧ фильтра
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
    """Однородный рекурсивный НЧ фильтр первого порядка."""
    y = np.zeros_like(x)
    y[0] = x[0]
    for i in range(1, len(x)):
        y[i] = alpha * x[i] + (1 - alpha) * y[i-1]
    return y

# Параметры рекурсивного фильтра
alpha = 0.1  # Коэффициент сглаживания (0 < alpha < 1). Меньше alpha - сильнее сглаживание.

# Применение рекурсивного НЧ фильтра
filtered_recursive = recursive_lowpass(signal, alpha)

# Визуализация результата рекурсивной фильтрации
plt.figure(figsize=(10, 4))
plt.plot(t, signal, label="Исходный сигнал")
plt.plot(t, filtered_recursive, label="После рекурсивного фильтра")
plt.title("Результат рекурсивной фильтрации")
plt.xlabel("Время (с)")
plt.ylabel("Амплитуда")
plt.legend()
plt.grid()
plt.show()

# Расчет и визуализация АЧХ рекурсивного НЧ фильтра
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

# Обработка файла file.wav и сохранение результата
if os.path.exists('file.wav'):
    sample_rate, audio_data = wavfile.read('file.wav')
    if len(audio_data.shape) > 1:
        audio_data = audio_data[:, 0]
    audio_data = audio_data / np.max(np.abs(audio_data))
    
    # Применение фильтров к аудиофайлу
    filtered_audio = audio_data.copy()
    
    # ВЧ фильтр
    h_hp = highpass_blackman(N, fc, sample_rate)
    filtered_audio = np.convolve(filtered_audio, h_hp, mode='same')
    
    # НЧ фильтр
    filtered_audio = recursive_lowpass(filtered_audio, alpha)
    
    # Сохранение результата
    wavfile.write('filtered_file.wav', sample_rate, (filtered_audio * np.iinfo(np.int16).max).astype(np.int16))
    print("Файл filtered_file.wav успешно создан")
else:
    print("Файл file.wav не найден")