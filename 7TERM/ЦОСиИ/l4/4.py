import cv2
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy.ndimage import convolve
from sklearn.preprocessing import StandardScaler
from matplotlib.colors import ListedColormap
from sklearn.neural_network import MLPClassifier
from sklearn.datasets import fetch_openml
import pickle
import os


def load_and_preprocess_image(image_path="/Users/admin/PycharmProjects/PytSosi_now/L4/pics/7.jpg"):
    img = cv2.imread(image_path)
    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    print("Изображение загружено и преобразовано!")
    return img, img_rgb, hsv

def create_mask(hsv):
    lower_green = np.array([35, 130, 130])
    upper_green = np.array([95, 255, 255])
    mask = cv2.inRange(hsv, lower_green, upper_green)
    mask = np.where(mask > 0, 1, 0).astype(np.uint8)
    return mask

def show_mask(mask):
    pass

def show_labels(labels):
    pass

def show_all_results(mask_before, mask_after, labels, df, img_rgb, color_clusters):
    fig, axes = plt.subplots(2, 2, figsize=(16, 12))

    axes[0, 0].imshow(mask_before, cmap='gray')
    axes[0, 0].set_title('1. Маска зеленого цвета', fontsize=14, fontweight='bold')
    axes[0, 0].axis('off')

    axes[0, 1].imshow(mask_after, cmap='gray')
    axes[0, 1].set_title('2. Маска после морфологических операций', fontsize=14, fontweight='bold')
    axes[0, 1].axis('off')

    unique_labels = np.unique(labels)
    unique_labels = unique_labels[unique_labels > 0]

    labels_remapped = np.zeros_like(labels)
    for new_idx, old_label in enumerate(unique_labels, start=1):
        labels_remapped[labels == old_label] = new_idx

    bright_colors = ['#000000', '#FF0000', '#0000FF', '#00FF00', '#FFFF00', '#FF00FF', '#00FFFF', '#FF6600']
    cmap = ListedColormap(bright_colors[:len(unique_labels) + 1])

    axes[1, 0].imshow(labels_remapped, cmap=cmap, vmin=0, vmax=len(unique_labels))
    axes[1, 0].scatter(df['centroid_x'], df['centroid_y'], c='white', s=100, marker='x', linewidths=3)
    axes[1, 0].set_title(f'3. Разметка объектов ({len(df)} шт.) и центроиды', fontsize=14, fontweight='bold')
    axes[1, 0].axis('off')

    axes[1, 1].imshow(color_clusters)
    axes[1, 1].set_title('4. Кластеризованные объекты', fontsize=14, fontweight='bold')
    axes[1, 1].axis('off')

    plt.tight_layout()
    plt.show()

def erosion(bin_image, kernel):
    k_sum = np.sum(kernel)
    conv = convolve(bin_image, kernel, mode="constant", cval=0)
    return (conv == k_sum).astype(np.uint8)

def dilation(binary_image, kernel):
    conv = convolve(binary_image, kernel, mode="constant", cval=0)
    return (conv > 0).astype(np.uint8)

def apply_morphology(mask):
    kernel = np.ones((5,5), dtype=np.uint8)
    mask = erosion(mask, kernel)
    kernel = np.ones((11,11), dtype=np.uint8)
    mask = dilation(mask, kernel)
    return mask

def labeling_sequential(binary_image):
    h, w = binary_image.shape
    labels = np.zeros((h, w), dtype=np.int32)
    current_label = 1

    parent = {}

    def find(x):
        if x not in parent:
            parent[x] = x
        if parent[x] != x:
            parent[x] = find(parent[x])
        return parent[x]

    def union(x, y):
        root_x = find(x)
        root_y = find(y)
        if root_x != root_y:
            parent[root_x] = root_y

    for y in range(h):
        for x in range(w):
            if binary_image[y, x] == 0:
                continue

            neighbors = []

            if y > 0 and labels[y-1, x] > 0:
                neighbors.append(labels[y-1, x])
            if x > 0 and labels[y, x-1] > 0:
                neighbors.append(labels[y, x-1])

            if neighbors:
                min_label = min(neighbors)
                labels[y, x] = min_label

                for neighbor_label in neighbors:
                    if neighbor_label != min_label:
                        union(neighbor_label, min_label)
            else:
                labels[y, x] = current_label
                parent[current_label] = current_label
                current_label += 1

    for y in range(h):
        for x in range(w):
            if labels[y, x] > 0:
                labels[y, x] = find(labels[y, x])

    return labels

def count_objects(labels):
    unique_labels = np.unique(labels)
    print("Number of objects:", len(unique_labels))
    print("Object labels:", unique_labels)
    return unique_labels

def measure_regions(labels, min_area=50):
    unique_labels = np.unique(labels)
    unique_labels = unique_labels[unique_labels != 0]

    data = []

    for lbl in unique_labels:
        coords = np.argwhere(labels == lbl)

        area = len(coords)

        if area < min_area:
            continue

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
    plt.scatter(df['centroid_x'], df['centroid_y'], c='red')
    plt.xlim(0, labels.shape[1])
    plt.ylim(0, labels.shape[0])
    plt.imshow(img_rgb)
    plt.title('Centroids of objects')
    plt.axis('off')
    plt.show()

def save_centroids(df, img_rgb, labels, filename="centroids.png"):
    img_copy = img_rgb.copy()
    for _, row in df.iterrows():
        cv2.circle(img_copy, (int(row['centroid_x']), int(row['centroid_y'])), 5, (255, 0, 0), -1)
    cv2.imwrite(filename, cv2.cvtColor(img_copy, cv2.COLOR_RGB2BGR))

def prepare_features(df):
    features = df[["area", "perimeter", "elongation"]]
    features = features.replace([np.inf, -np.inf], np.nan)
    features['elongation'] = features['elongation'].fillna(features['elongation'].mean())
    features_scaled = StandardScaler().fit_transform(features)
    return features, features_scaled


def custom_kmedians(X, n_clusters, max_iter=300, random_state=None):
    if random_state:
        np.random.seed(random_state)

    actual_n_clusters = min(n_clusters, X.shape[0])

    initial_medians_indexes = np.random.choice(X.shape[0], actual_n_clusters, replace=False)
    medians = X[initial_medians_indexes, :].copy()

    for iteration in range(max_iter):
        distances = np.abs(X[:, np.newaxis, :] - medians[np.newaxis, :, :]).sum(axis=2)
        labels = np.argmin(distances, axis=1)

        new_medians = np.copy(medians)
        for cluster_idx in range(actual_n_clusters):
            cluster_points = X[labels == cluster_idx]
            if len(cluster_points) > 0:
                new_medians[cluster_idx] = np.median(cluster_points, axis=0)
            else:
                new_medians[cluster_idx] = X[np.random.choice(X.shape[0], 1, replace=False)]

        if np.allclose(new_medians, medians):
            break

        medians = new_medians

    distances = np.abs(X[:, np.newaxis, :] - medians[np.newaxis, :, :]).sum(axis=2)
    labels = np.argmin(distances, axis=1)

    return labels, medians

def cluster_features(features_scaled):
    custom_labels, _ = custom_kmedians(features_scaled, n_clusters=3, random_state=42)
    print("K-Medians кластеризация завершена!")
    return custom_labels

def add_clusters_to_df(df, custom_labels):
    df["cluster"] = custom_labels
    return df

def coloring_clusters(labels, label_cluster):
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
    unique_labels = np.unique(labels)
    filtered_labels = labels.copy()

    for lbl in unique_labels:
        if lbl == 0:
            continue
        coords = np.argwhere(labels == lbl)
        area = len(coords)

        if area < min_area:
            filtered_labels[labels == lbl] = 0

    return filtered_labels

def _needs_180_flip(mask, moments):
    if moments["m00"] == 0:
        return False
    cy = moments["m01"] / moments["m00"]
    return cy > mask.shape[0] / 2

def _verticalize_and_correct(digit_mask, cnt):
    moments = cv2.moments(cnt)
    if moments["m00"] == 0:
        return None

    cx = moments["m10"] / moments["m00"]
    cy = moments["m01"] / moments["m00"]

    rect = cv2.minAreaRect(cnt)
    (_, _), (width, height), angle = rect

    rotation_angle = angle if width < height else angle + 90
    if width >= height:
        width, height = height, width

    M = cv2.getRotationMatrix2D((cx, cy), rotation_angle, 1.0)

    cos, sin = np.abs(M[0, 0]), np.abs(M[0, 1])
    new_w = int(height * sin + width * cos)
    new_h = int(height * cos + width * sin)
    M[0, 2] += (new_w / 2) - cx
    M[1, 2] += (new_h / 2) - cy

    rotated = cv2.warpAffine(digit_mask, M, (new_w, new_h), flags=cv2.INTER_NEAREST)

    new_moments = cv2.moments(rotated)
    if _needs_180_flip(rotated, new_moments):
        cx_new = new_moments["m10"] / new_moments["m00"]
        cy_new = new_moments["m01"] / new_moments["m00"]
        M_flip = cv2.getRotationMatrix2D((cx_new, cy_new), 180, 1.0)
        rotated = cv2.warpAffine(rotated, M_flip, rotated.shape[::-1], flags=cv2.INTER_NEAREST)

    return rotated

def extract_and_rotate_objects(labels_map):
    rotated_objects = []
    num_labels = labels_map.max()

    for label_id in range(1, num_labels + 1):
        digit_mask = (labels_map == label_id).astype(np.uint8) * 255

        contours, _ = cv2.findContours(digit_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        if not contours:
            continue

        rotated = _verticalize_and_correct(digit_mask, contours[0])
        if rotated is None:
            continue

        rotated_contours, _ = cv2.findContours(rotated, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        if rotated_contours:
            x, y, w, h = cv2.boundingRect(rotated_contours[0])

            padding = 10
            x_start = max(0, x - padding)
            y_start = max(0, y - padding)
            x_end = min(rotated.shape[1], x + w + padding)
            y_end = min(rotated.shape[0], y + h + padding)
            cropped = rotated[y_start:y_end, x_start:x_end]
            rotated_objects.append(cropped)

    return rotated_objects

def visualize_rotated_objects(rotated_objects, name=""):
    if not rotated_objects:
        print("Нет объектов для визуализации.")
        return

    target_size = 200
    padding = 20

    normalized_objects = []
    for obj in rotated_objects:
        h, w = obj.shape

        max_dim = max(h, w)
        scale = (target_size - 2 * padding) / max_dim
        new_w = int(w * scale)
        new_h = int(h * scale)

        resized = cv2.resize(obj, (new_w, new_h), interpolation=cv2.INTER_NEAREST)

        cell = np.zeros((target_size, target_size), dtype=np.uint8)

        y_offset = (target_size - new_h) // 2
        x_offset = (target_size - new_w) // 2
        cell[y_offset:y_offset + new_h, x_offset:x_offset + new_w] = resized

        normalized_objects.append(cell)

    cols = 5
    rows = int(np.ceil(len(normalized_objects) / cols))

    grid_rows = []
    for row_idx in range(rows):
        row_cells = []
        for col_idx in range(cols):
            obj_idx = row_idx * cols + col_idx
            if obj_idx < len(normalized_objects):
                row_cells.append(normalized_objects[obj_idx])
            else:
                row_cells.append(np.zeros((target_size, target_size), dtype=np.uint8))

        row_with_separators = []
        for i, cell in enumerate(row_cells):
            row_with_separators.append(cell)
            if i < len(row_cells) - 1:
                separator = np.ones((target_size, 10), dtype=np.uint8) * 128
                row_with_separators.append(separator)

        grid_rows.append(np.hstack(row_with_separators))

        if row_idx < rows - 1:
            h_separator = np.ones((10, grid_rows[0].shape[1]), dtype=np.uint8) * 128
            grid_rows.append(h_separator)

    final_grid = np.vstack(grid_rows)

    plt.figure(figsize=(15, 3 * rows))
    plt.imshow(final_grid, cmap='gray', vmin=0, vmax=255)
    plt.title(f"Повёрнутые объекты {name}", fontsize=16, fontweight='bold', pad=20)
    plt.axis('off')
    plt.tight_layout()
    plt.show()


def create_mnist_model():
    model = MLPClassifier(
        hidden_layer_sizes=(256, 128, 64),
        activation='relu',
        solver='adam',
        max_iter=50,
        random_state=42,
        verbose=True,
        early_stopping=True,
        validation_fraction=0.1
    )
    return model


def train_mnist_model(model):
    print("Загружаем датасет MNIST...")
    mnist = fetch_openml('mnist_784', version=1, as_frame=False, parser='auto')
    X, y = mnist.data, mnist.target.astype(int)

    X = X / 255.0

    n_samples = 60000
    X_train, y_train = X[:n_samples], y[:n_samples]
    X_test, y_test = X[n_samples:], y[n_samples:]

    print(f"Обучаем модель на {n_samples} примерах...")
    model.fit(X_train, y_train)

    accuracy = model.score(X_test, y_test)
    print(f"Точность на тестовых данных: {accuracy:.4f}")
    return model


def get_or_train_model(model_path="mnist_mlp_model.pkl"):
    if os.path.exists(model_path):
        print("Загружаем сохранённую модель...")
        with open(model_path, 'rb') as f:
            model = pickle.load(f)
    else:
        print("Обучаем новую модель на MNIST...")
        model = create_mnist_model()
        model = train_mnist_model(model)
        with open(model_path, 'wb') as f:
            pickle.dump(model, f)
        print(f"Модель сохранена в {model_path}")
    return model


def preprocess_for_mnist(image):
    if len(image.shape) == 3:
        image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    _, binary = cv2.threshold(image, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)

    if np.mean(binary) > 127:
        binary = 255 - binary

    coords = cv2.findNonZero(binary)
    if coords is not None:
        x, y, w, h = cv2.boundingRect(coords)
        digit = binary[y:y+h, x:x+w]
    else:
        digit = binary

    max_dim = max(digit.shape)
    scale = 20.0 / max_dim
    new_w = int(digit.shape[1] * scale)
    new_h = int(digit.shape[0] * scale)
    if new_w == 0:
        new_w = 1
    if new_h == 0:
        new_h = 1

    resized_digit = cv2.resize(digit, (new_w, new_h), interpolation=cv2.INTER_AREA)

    canvas = np.zeros((28, 28), dtype=np.uint8)

    x_offset = (28 - new_w) // 2
    y_offset = (28 - new_h) // 2
    canvas[y_offset:y_offset+new_h, x_offset:x_offset+new_w] = resized_digit

    moments = cv2.moments(canvas)
    if moments["m00"] != 0:
        cx = int(moments["m10"] / moments["m00"])
        cy = int(moments["m01"] / moments["m00"])
        shift_x = 14 - cx
        shift_y = 14 - cy
        M = np.float32([[1, 0, shift_x], [0, 1, shift_y]])
        canvas = cv2.warpAffine(canvas, M, (28, 28))

    normalized = canvas.astype('float32') / 255.0
    return normalized.flatten().reshape(1, -1), canvas


def predict_digit(model, image):
    preprocessed, visual = preprocess_for_mnist(image)
    predictions = model.predict_proba(preprocessed)[0]
    predicted_digit = np.argmax(predictions)
    confidence = predictions[predicted_digit]
    return predicted_digit, confidence, predictions, visual


def visualize_predictions(model, objects, title="Распознавание цифр"):
    if not objects:
        print("Нет объектов для распознавания.")
        return

    n_objects = len(objects)
    cols = min(5, n_objects)
    rows = int(np.ceil(n_objects / cols))

    fig, axes = plt.subplots(rows * 2, cols, figsize=(3 * cols, 5 * rows))
    if rows * 2 == 1:
        axes = np.array([axes])
    if cols == 1:
        axes = axes.reshape(-1, 1)
    axes = axes.reshape(rows * 2, cols)

    for idx, obj in enumerate(objects):
        row = (idx // cols) * 2
        col = idx % cols

        predicted, confidence, probs, visual = predict_digit(model, obj)

        axes[row, col].imshow(visual, cmap='gray')
        axes[row, col].set_title(f"Предсказание: {predicted}\nУверенность: {confidence:.2%}", fontsize=10)
        axes[row, col].axis('off')

        colors = ['green' if i == predicted else 'steelblue' for i in range(10)]
        bars = axes[row + 1, col].bar(range(10), probs, color=colors)
        axes[row + 1, col].set_xticks(range(10))
        axes[row + 1, col].set_ylim(0, 1)
        axes[row + 1, col].set_xlabel('Цифра')
        axes[row + 1, col].set_ylabel('Вероятность')

        for i, (bar, prob) in enumerate(zip(bars, probs)):
            if prob > 0.05:
                axes[row + 1, col].text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.02,
                                        f'{prob:.0%}', ha='center', va='bottom', fontsize=7)

    for idx in range(n_objects, rows * cols):
        row = (idx // cols) * 2
        col = idx % cols
        axes[row, col].axis('off')
        axes[row + 1, col].axis('off')

    plt.suptitle(title, fontsize=16, fontweight='bold')
    plt.tight_layout()
    plt.show()


def rotate_180(image):
    return cv2.rotate(image, cv2.ROTATE_180)


def main():
    img, img_rgb, hsv = load_and_preprocess_image()
    mask_before = create_mask(hsv)
    mask_after = apply_morphology(mask_before)
    labels = labeling_sequential(mask_after)
    count_objects(labels)

    labels = filter_small_objects(labels, min_area=100)

    df = measure_regions(labels, min_area=50)
    print(df)
    features, features_scaled = prepare_features(df)
    print(features)
    print(features_scaled)
    custom_labels = cluster_features(features_scaled)
    df = add_clusters_to_df(df, custom_labels)
    print(df)
    label_to_cluster = df.set_index('label')['cluster'].to_dict()
    print(label_to_cluster)
    color_clusters = coloring_clusters(labels, label_to_cluster)
    show_all_results(mask_before, mask_after, labels, df, img_rgb, color_clusters)

    rotated_objects = extract_and_rotate_objects(labels)
    print(f"\nПовёрнуто объектов: {len(rotated_objects)}")

    print("\n" + "="*50)
    print("РАСПОЗНАВАНИЕ ЦИФР НЕЙРОСЕТЬЮ (MNIST)")
    print("="*50)

    model = get_or_train_model()

    print("\n--- Распознавание выровненных цифр ---")
    visualize_predictions(model, rotated_objects, "Распознавание выровненных цифр")

    print("\n--- Распознавание перевёрнутых цифр (180°) ---")
    flipped_objects = [rotate_180(obj) for obj in rotated_objects]
    visualize_predictions(model, flipped_objects, "Распознавание перевёрнутых цифр (180°)")

    plt.show()


if __name__ == "__main__":
    main()