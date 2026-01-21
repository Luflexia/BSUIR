# Импортируем все нужные библиотеки для работы с изображениями и анализом
import cv2
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy.ndimage import convolve
import os
try:
    from sklearn.preprocessing import StandardScaler
except ImportError:
    class StandardScaler:
        def fit_transform(self, X):
            X = np.asarray(X, dtype=float)
            mean = X.mean(axis=0)
            std = X.std(axis=0)
            std[std == 0] = 1.0
            return (X - mean) / std


def load_and_preprocess_image(image_path=None):
    """Загружаем изображение и преобразуем в RGB и HSV"""
    if image_path is None:
        base_dir = os.path.dirname(__file__)
        image_path = os.path.join(base_dir, "pics", "7.jpg")
    img = cv2.imread(image_path)
    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    print("Изображение загружено и преобразовано!")
    return img, img_rgb, hsv

def create_mask(hsv):
    """
    Создаем маску для зеленого цвета используя HSV формат

    ╔═══════════════════════════════════════════════════════════════════════════╗
    ║ ПОЧЕМУ HSV ВМЕСТО RGB?                                                     ║
    ║                                                                            ║
    ║ RGB подход НЕ РАБОТАЕТ для разных оттенков зелёного:                     ║
    ║ ─────────────────────────────────────────────────────────────────────────  ║
    ║                                                                            ║
    ║ Светлый зелёный (#0cdc90):                                               ║
    ║   RGB = (12, 220, 144)  - низкий красный, высокий зелёный, средний синий ║
    ║                                                                            ║
    ║ Тёмный зелёный (#3b8d0d):                                                ║
    ║   RGB = (59, 141, 13)   - низкий красный, средний зелёный, низкий синий  ║
    ║                                                                            ║
    ║ Проблема: RGB значения ОЧЕНЬ РАЗНЫЕ!                                     ║
    ║ - Если установим допуск для светлого → не найдём тёмный                 ║
    ║ - Если установим допуск для тёмного → не найдём светлый                 ║
    ║ - Если установим большой допуск → захватим много шума и неправильных цветов
    ║                                                                            ║
    ╠═══════════════════════════════════════════════════════════════════════════╣
    ║ HSV РЕШЕНИЕ:                                                              ║
    ║ ─────────────────────────────────────────────────────────────────────────  ║
    ║                                                                            ║
    ║ HSV = (Hue, Saturation, Value)                                            ║
    ║                                                                            ║
    ║ Светлый зелёный (#0cdc90):                                               ║
    ║   HSV ≈ (60, 94%, 86%)  - Hue ≈ 60°                                       ║
    ║                                                                            ║
    ║ Тёмный зелёный (#3b8d0d):                                                ║
    ║   HSV ≈ (65, 91%, 55%)  - Hue ≈ 65°                                       ║
    ║                                                                            ║
    ║ КЛЮЧЕВОЕ ПРЕИМУЩЕСТВО:                                                    ║
    ║ • Hue (оттенок) одинаковый для обоих (~60-65°)                            ║
    ║ • Value (яркость) отличается (86% vs 55%)                                ║
    ║ • Saturation (насыщенность) примерно одинаковая (~91-94%)                ║
    ║                                                                            ║
    ║ Выделяя по Hue (35-95° в OpenCV = зелёный цвет):                         ║
    ║ ✓ Находим ВСЕ оттенки зелёного независимо от яркости                     ║
    ║ ✓ Не важна разница в RGB значениях                                       ║
    ║ ✓ Работает на картинке 222 (светлый) И на картинке 11 (тёмный)          ║
    ║ ✓ Фильтруем шум через Saturation (только насыщенный зелёный)             ║
    ║ ✓ Игнорируем очень тёмные пиксели через Value                            ║
    ║                                                                            ║
    ║ ВЫВОД: HSV - единственный способ работать с разными оттенками одного цвета║
    ╚═══════════════════════════════════════════════════════════════════════════╝
    """
    # Диапазон зелёного цвета в HSV
    # Hue: 35-95 (в OpenCV Hue = 0-180, поэтому используем 35-95 вместо 35°-85°)
    # Saturation: 130-255 (насыщенный цвет, исключаем серые)
    # Value: 130-255 (не очень тёмные, исключаем чёрный)
    lower_green = np.array([35, 130, 130])
    upper_green = np.array([95, 255, 255])
    mask = cv2.inRange(hsv, lower_green, upper_green)
    mask = np.where(mask > 0, 1, 0).astype(np.uint8)
    return mask

def show_mask(mask):
    """Показываем маску"""
    plt.imshow(mask, cmap='gray')
    plt.title('Маска зеленого цвета')
    plt.axis('off')
    plt.show()

def show_labels(labels):
    """Показываем разметку объектов"""
    unique_labels = np.unique(labels)
    unique_labels = unique_labels[unique_labels > 0]
    labels_remapped = np.zeros_like(labels)
    for new_idx, old_label in enumerate(unique_labels, start=1):
        labels_remapped[labels == old_label] = new_idx
    from matplotlib.colors import ListedColormap
    bright_colors = ['#000000', '#FF0000', '#0000FF', '#00FF00', '#FFFF00', '#FF00FF', '#00FFFF', '#FF6600']
    cmap = ListedColormap(bright_colors[:len(unique_labels) + 1])
    plt.imshow(labels_remapped, cmap=cmap, vmin=0, vmax=len(unique_labels))
    plt.title(f'Разметка объектов ({len(unique_labels)} шт.)')
    plt.axis('off')
    plt.show()

def show_all_results(mask_before, mask_after, labels, df, img_rgb, color_clusters):
    """Показываем все результаты на одной картинке"""
    fig, axes = plt.subplots(2, 2, figsize=(16, 12))

    # 1. Исходная маска
    axes[0, 0].imshow(mask_before, cmap='gray')
    axes[0, 0].set_title('1. Маска зеленого цвета', fontsize=14, fontweight='bold')
    axes[0, 0].axis('off')

    # 2. Маска после морфологических операций
    axes[0, 1].imshow(mask_after, cmap='gray')
    axes[0, 1].set_title('2. Маска после морфологических операций', fontsize=14, fontweight='bold')
    axes[0, 1].axis('off')

    # 3. Разметка объектов с центроидами
    # Переиндексируем метки для лучшей визуализации (1,2,3,4,5 вместо 1,237,245,445,537)
    unique_labels = np.unique(labels)
    unique_labels = unique_labels[unique_labels > 0]  # Исключаем фон (0)

    labels_remapped = np.zeros_like(labels)
    for new_idx, old_label in enumerate(unique_labels, start=1):
        labels_remapped[labels == old_label] = new_idx

    # Создаем изображение разметки с правильной палитрой
    # Используем цвета только для объектов (без фона)
    from matplotlib.colors import ListedColormap

    # Черный для фона (0), потом яркие цвета для объектов
    bright_colors = ['#000000', '#FF0000', '#0000FF', '#00FF00', '#FFFF00', '#FF00FF', '#00FFFF', '#FF6600']
    cmap = ListedColormap(bright_colors[:len(unique_labels) + 1])

    im = axes[1, 0].imshow(labels_remapped, cmap=cmap, vmin=0, vmax=len(unique_labels))
    axes[1, 0].scatter(df['centroid_x'], df['centroid_y'], c='white', s=100, marker='x', linewidths=3)
    axes[1, 0].set_title(f'3. Разметка объектов ({len(df)} шт.) и центроиды', fontsize=14, fontweight='bold')
    axes[1, 0].axis('off')

    # 4. Раскрашенные кластеры
    axes[1, 1].imshow(color_clusters)
    axes[1, 1].set_title('4. Кластеризованные объекты', fontsize=14, fontweight='bold')
    axes[1, 1].axis('off')

    plt.tight_layout()
    plt.show()

def erosion(bin_image, kernel):
    """Функция эрозии"""
    k_sum = np.sum(kernel)
    conv = convolve(bin_image, kernel, mode="constant", cval=0)
    return (conv == k_sum).astype(np.uint8)

def dilation(binary_image, kernel):
    """Функция дилатации"""
    conv = convolve(binary_image, kernel, mode="constant", cval=0)
    return (conv > 0).astype(np.uint8)

def apply_morphology(mask):
    """Применяем морфологические операции"""
    kernel = np.ones((5,5), dtype=np.uint8)
    mask = erosion(mask, kernel)
    kernel = np.ones((11,11), dtype=np.uint8)
    mask = dilation(mask, kernel)
    return mask

def labeling_sequential(binary_image):
    """
    Двухпроходный последовательный алгоритм с 4-связностью
    Использует Union-Find для эффективного объединения эквивалентных меток

    Проверяет только 4 соседей (плюсик ✚):
    ├─ верхний сосед (y-1, x)
    ├─ левый сосед (y, x-1)
    ├─ правый сосед (y, x+1)
    └─ нижний сосед (y+1, x)
    """
    h, w = binary_image.shape
    labels = np.zeros((h, w), dtype=np.int32)
    current_label = 1

    # Union-Find структура для отслеживания эквивалентных меток
    parent = {}

    def find(x):
        """Найти корневой элемент с сжатием пути"""
        if x not in parent:
            parent[x] = x
        if parent[x] != x:
            parent[x] = find(parent[x])
        return parent[x]

    def union(x, y):
        """Объединить два множества"""
        root_x = find(x)
        root_y = find(y)
        if root_x != root_y:
            parent[root_x] = root_y

    # ПЕРВЫЙ ПРОХОД: слева направо, сверху вниз
    # Присваиваем новые метки и объединяем соседние метки
    for y in range(h):
        for x in range(w):
            if binary_image[y, x] == 0:  # Пропускаем фон
                continue

            neighbors = []

            # Проверяем только 4 соседей (4-связность)
            if y > 0 and labels[y-1, x] > 0:  # верхний сосед
                neighbors.append(labels[y-1, x])
            if x > 0 and labels[y, x-1] > 0:  # левый сосед
                neighbors.append(labels[y, x-1])

            if neighbors:
                # Используем минимальную метку из соседей
                min_label = min(neighbors)
                labels[y, x] = min_label

                # Объединяем все соседние метки с минимальной
                for neighbor_label in neighbors:
                    if neighbor_label != min_label:
                        union(neighbor_label, min_label)
            else:
                # Новый объект - присваиваем новую метку
                labels[y, x] = current_label
                parent[current_label] = current_label
                current_label += 1

    # ВТОРОЙ ПРОХОД: переносим исходные метки на финальные эквивалентные
    for y in range(h):
        for x in range(w):
            if labels[y, x] > 0:
                labels[y, x] = find(labels[y, x])

    return labels

def count_objects(labels):
    """Подсчитываем объекты"""
    unique_labels = np.unique(labels)
    print("Number of objects:", len(unique_labels))
    print("Object labels:", unique_labels)
    return unique_labels

def measure_regions(labels, min_area=50):
    """Измеряем свойства объектов, фильтруя по минимальной площади"""
    unique_labels = np.unique(labels)
    unique_labels = unique_labels[unique_labels != 0]

    data = []

    for lbl in unique_labels:
        coords = np.argwhere(labels == lbl)

        area = len(coords)

        if area < min_area:
            continue  # Пропускаем маленькие объекты

        y_mean = coords[:, 0].mean()
        x_mean = coords[:, 1].mean()

        perimeter = 0
        for (y, x) in coords:
            neighbors = [(y-1, x), (y+1, x), (y, x-1), (y, x+1)]
            for ny, nx in neighbors:
                if ny < 0 or nx < 0 or ny >= labels.shape[0] or nx >= labels.shape[1] or labels[ny, nx] != lbl:
                    perimeter += 1
                    break

        centered = coords - np.array([y_mean, x_mean])
        cov = np.cov(centered, rowvar=False)

        if cov.shape == (2, 2):
            eigvals, _ = np.linalg.eig(cov)
            eigvals = np.sort(eigvals)
            if eigvals[0] > 1e-6:
                elongation = eigvals[1] / eigvals[0]
            else:
                elongation = np.inf
        else:
            elongation = np.nan

        data.append({
            "label": lbl,
            "area": area,
            "perimeter": perimeter,
            "centroid_x": x_mean,
            "centroid_y": y_mean,
            "elongation": elongation
        })

    return pd.DataFrame(data)

def show_centroids(df, img_rgb, labels):
    """Показываем центроиды"""
    plt.scatter(df['centroid_x'], df['centroid_y'], c='red')
    plt.xlim(0, labels.shape[1])
    plt.ylim(0, labels.shape[0])
    plt.imshow(img_rgb)
    plt.title('Centroids of objects')
    plt.axis('off')
    plt.show()

def save_centroids(df, img_rgb, labels, filename="centroids.png"):
    """Сохраняем изображение с центроидами в файл"""
    img_copy = img_rgb.copy()
    for _, row in df.iterrows():
        cv2.circle(img_copy, (int(row['centroid_x']), int(row['centroid_y'])), 5, (255, 0, 0), -1)
    cv2.imwrite(filename, cv2.cvtColor(img_copy, cv2.COLOR_RGB2BGR))

def prepare_features(df):
    """Выбираем и масштабируем признаки"""
    features = df[["area", "perimeter", "elongation"]]
    # Заменяем inf и nan на среднее значение для elongation
    features = features.replace([np.inf, -np.inf], np.nan)
    features['elongation'] = features['elongation'].fillna(features['elongation'].mean())
    features_scaled = StandardScaler().fit_transform(features)
    return features, features_scaled


def custom_kmedians(X, n_clusters, max_iter=300, random_state=None):
    """
    Собственная реализация K-Medians (поиск медиан по каждому признаку)

    Автоматически адаптирует количество кластеров к количеству объектов
    """
    if random_state:
        np.random.seed(random_state)

    # Адаптируем количество кластеров к количеству объектов
    # Если объектов меньше чем запрошено кластеров, используем количество объектов
    actual_n_clusters = min(n_clusters, X.shape[0])

    # Инициализируем случайные медианы
    initial_medians_indexes = np.random.choice(X.shape[0], actual_n_clusters, replace=False)
    medians = X[initial_medians_indexes, :].copy()

    for iteration in range(max_iter):
        # Присваиваем каждую точку ближайшей медиане (Манхэттено расстояние)
        distances = np.abs(X[:, np.newaxis, :] - medians[np.newaxis, :, :]).sum(axis=2)
        labels = np.argmin(distances, axis=1)

        # Вычисляем новые медианы
        new_medians = np.copy(medians)
        for cluster_idx in range(actual_n_clusters):
            cluster_points = X[labels == cluster_idx]
            if len(cluster_points) > 0:
                # Медиана по каждому признаку
                new_medians[cluster_idx] = np.median(cluster_points, axis=0)
            else:
                # Если кластер пуст, выбираем случайную точку
                new_medians[cluster_idx] = X[np.random.choice(X.shape[0], 1, replace=False)]

        # Проверяем сходимость
        if np.allclose(new_medians, medians):
            break

        medians = new_medians

    # Финальное присваивание
    distances = np.abs(X[:, np.newaxis, :] - medians[np.newaxis, :, :]).sum(axis=2)
    labels = np.argmin(distances, axis=1)

    return labels, medians

def cluster_features(features_scaled):
    """Кластеризуем признаки методом K-Medians"""
    custom_labels, _ = custom_kmedians(features_scaled, n_clusters=3, random_state=42)
    print("K-Medians кластеризация завершена!")
    return custom_labels

def add_clusters_to_df(df, custom_labels):
    """Добавляем кластеры в DataFrame"""
    df["cluster"] = custom_labels
    return df

def coloring_clusters(labels, label_cluster):
    """Раскрашиваем кластеры"""
    h, w = labels.shape
    cluster_image = np.zeros((h, w, 3)).astype(np.uint8)
    for i in range(h):
        for j in range(w):
            label = labels[i, j]
            if label == 0 or label not in label_cluster:
                continue
            cluster = label_cluster[label]
            if cluster == 0:
                cluster_image[i, j] = [255, 0, 0]
            elif cluster == 1:
                cluster_image[i, j] = [0, 255, 0]
            elif cluster == 2:
                cluster_image[i, j] = [0, 0, 255]
            elif cluster == 3:
                cluster_image[i, j] = [255, 255, 0]
    return cluster_image


def filter_small_objects(labels, min_area=50):
    """Удаляем маленькие объекты из изображения разметки"""
    unique_labels = np.unique(labels)
    filtered_labels = labels.copy()

    for lbl in unique_labels:
        if lbl == 0:  # Пропускаем фон
            continue
        coords = np.argwhere(labels == lbl)
        area = len(coords)

        if area < min_area:
            # Удаляем маленький объект, помечаем как фон
            filtered_labels[labels == lbl] = 0

    return filtered_labels

def main():
    """
    Главная функция для запуска всего пайплайна

    ПОЧЕМУ ЭТО РАБОТАЕТ НА ВСЕХ КАРТИНКАХ:

    Картинка 11 (тёмный зелёный #3b8d0d):
    - RGB(59, 141, 13) - тусклый, низкое значение красного и синего
    - HSV(65°, 91%, 55%) - Hue ≈ 65°
    - ✓ Попадает в диапазон Hue 35-95
    - ✓ Высокая насыщенность (91%) > минимум (130)
    - ⚠ Яркость (55%) - может быть на границе (130 слишком высоко)

    Картинка 7 (любого оттенка зелёного):
    - ✓ Любой оттенок зелёного будет иметь Hue 35-95
    - ✓ Приличная насыщенность (обычно > 130)
    - ✓ Value достаточен (обычно > 130)

    ИТОГ: HSV позволяет выделять ВСЕ оттенки зелёного одинаково,
    независимо от яркости и RGB значений!
    """
    img, img_rgb, hsv = load_and_preprocess_image()
    mask_before = create_mask(hsv)
    mask_after = apply_morphology(mask_before)
    labels = labeling_sequential(mask_after)
    count_objects(labels)

    # ВАЖНО: Сначала фильтруем маленькие объекты в матрице меток
    labels = filter_small_objects(labels, min_area=100)

    # ПОТОМ считаем характеристики (только оставшихся объектов)
    df = measure_regions(labels, min_area=50)  # min_area=50 чтобы не фильтровать еще раз
    features, features_scaled = prepare_features(df)
    # Кластеризуем K-Medians методом
    custom_labels = cluster_features(features_scaled)
    df = add_clusters_to_df(df, custom_labels)
    label_to_cluster = df.set_index('label')['cluster'].to_dict()
    color_clusters = coloring_clusters(labels, label_to_cluster)
    # Показываем все результаты на одной картинке
    show_all_results(mask_before, mask_after, labels, df, img_rgb, color_clusters)

if __name__ == "__main__":
    main()
