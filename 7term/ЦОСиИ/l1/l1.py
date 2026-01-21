import cv2
import numpy as np
import os
import matplotlib.pyplot as plt

# 1. Коррекция яркости (линейная нормализация)
def correct_brightness(img):
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    v = hsv[:, :, 2]
    v = cv2.equalizeHist(v)
    hsv[:, :, 2] = v
    return cv2.cvtColor(hsv, cv2.COLOR_HSV2BGR)

# 2. Цветовая сегментация по синему цвету
def blue_mask(img):
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    lower_blue = np.array([90, 50, 40])
    upper_blue = np.array([140, 255, 255])
    mask = cv2.inRange(hsv, lower_blue, upper_blue)
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (7,7))
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)
    return mask

# 3. Выделение связных областей
def connected_components(mask):
    n_labels, labels, stats, centroids = cv2.connectedComponentsWithStats(mask)
    objects = []
    for i in range(1, n_labels):  # пропускаем фон
        obj_mask = (labels == i).astype(np.uint8)
        objects.append((obj_mask, stats[i]))
    return objects

# 4. Извлечение признаков объектов
def extract_features(objects):
    features = []
    for mask, stat in objects:
        area = stat[cv2.CC_STAT_AREA]
        left, top, width, height = stat[:4]
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        perimeter = cv2.arcLength(contours[0], True) if contours else 0
        compactness = (perimeter ** 2) / area if area else 0
        elongation = max(width, height) / min(width, height) if min(width, height) else 0
        m = cv2.moments(mask.astype(np.uint8))
        mu = [m['mu20'], m['mu02'], m['mu11']]
        features.append([area, perimeter, compactness, elongation] + mu)
    return np.array(features)

# 5. Ручная реализация k-medians на numpy
def k_medians_np(X, n_clusters=2, max_iter=100):
    np.random.seed(42)
    medoids = X[np.random.choice(len(X), n_clusters, replace=False)]
    for _ in range(max_iter):
        distances = np.sum(np.abs(X[:, None] - medoids), axis=2)
        labels = np.argmin(distances, axis=1)
        new_medoids = []
        for k in range(n_clusters):
            cluster_points = X[labels == k]
            if len(cluster_points) == 0:
                new_medoids.append(medoids[k])
                continue
            median = np.median(cluster_points, axis=0)
            medoid = cluster_points[np.argmin(np.sum(np.abs(cluster_points - median), axis=1))]
            new_medoids.append(medoid)
        new_medoids = np.array(new_medoids)
        if np.all(medoids == new_medoids):
            break
        medoids = new_medoids
    return labels

# Основная функция обработки
def process_images():
    input_dir = 'noBG'     # исходные изображения без фона
    output_dir = 'L2'      # результаты кластеризации
    os.makedirs(output_dir, exist_ok=True)
    for img_name in os.listdir(input_dir):
        if img_name.lower().endswith(('.png', '.jpg', '.jpeg')):
            img_path = os.path.join(input_dir, img_name)
            img = cv2.imread(img_path)
            if img is None:
                continue
            # 1. Коррекция яркости
            img_corr = correct_brightness(img)
            # 2. Цветовая сегментация
            mask = blue_mask(img_corr)
            # 3. Связные области
            objects = connected_components(mask)
            if not objects:
                print(f"Нет объектов на {img_name}")
                continue
            # 4. Признаки
            features = extract_features(objects)
            n_obj = len(features)
            n_clusters = min(2, n_obj)  # не больше числа объектов
            if n_obj == 0:
                print(f"Нет объектов для кластеризации на {img_name}")
                continue
            if n_clusters == 1:
                clusters = np.zeros(n_obj, dtype=int)  # все в одном кластере
            else:
                clusters = k_medians_np(features, n_clusters=n_clusters)

            # 5. Кластеризация
            clusters = k_medians_np(features, n_clusters=2)
            # 6. Визуализация: выделение контуров объектов цветом по кластеру
            result_img = img_corr.copy()
            for idx, (obj_mask, _) in enumerate(objects):
                contours, _ = cv2.findContours(obj_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
                col = [(0,255,0), (255,0,0), (0,128,255), (255,128,0)]
                cluster_color = col[clusters[idx] % len(col)]
                # Обводим только контур, не заливаем рисунки
                cv2.drawContours(result_img, contours, -1, cluster_color, thickness=8)
            cv2.imwrite(os.path.join(output_dir, f'{os.path.splitext(img_name)[0]}_result.jpg'), result_img)
            print(f'Обработано и сохранено: {img_name}')

if __name__ == '__main__':
    process_images()
