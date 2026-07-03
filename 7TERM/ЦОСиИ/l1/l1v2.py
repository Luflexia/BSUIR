import cv2
import numpy as np
import os
import matplotlib.pyplot as plt

# Функция для проверки, является ли пиксель ярким
def is_pixel_bright(pixel, accuracy=0.5):
    pixel = pixel.astype(float)
    avg = (pixel[0] + pixel[1] + pixel[2]) / 3
    if avg == 0:
        return False
    if (abs((avg - pixel[0]) / avg) > accuracy or
        abs((avg - pixel[1]) / avg) > accuracy or
        abs((avg - pixel[2]) / avg) > accuracy):
        return True
    return False

# Функция для удаления яркого фона и создания масок
def remove_bright_background(image, accuracy=0.8, kernel_size=3, iterations=2):
    height, width, _ = image.shape
    result = image.copy()
    for y in range(height):
        for x in range(width):
            pixel = result[y, x]
            if not is_pixel_bright(pixel, accuracy):
                result[y, x] = [0, 0, 0]
    gray = cv2.cvtColor(result, cv2.COLOR_BGR2GRAY)
    _, mask_before = cv2.threshold(gray, 1, 255, cv2.THRESH_BINARY)
    kernel = np.ones((kernel_size, kernel_size), np.uint8)
    mask_after = cv2.morphologyEx(mask_before, cv2.MORPH_OPEN, kernel, iterations=iterations)
    mask_after = cv2.morphologyEx(mask_after, cv2.MORPH_CLOSE, kernel, iterations=iterations)
    final_result = cv2.bitwise_and(result, result, mask=mask_after)
    return final_result, mask_before, mask_after

# Функция для визуализации результатов с подписями
def visualize_results(image, result, mask_before, mask_after, output_path):
    fig, axs = plt.subplots(1, 3, figsize=(15, 5))
    axs[0].imshow(cv2.cvtColor(image, cv2.COLOR_BGR2RGB))
    axs[0].set_title('Оригинальное изображение')
    axs[0].axis('off')
    
    axs[1].imshow(mask_before, cmap='gray')
    axs[1].set_title('Маска до морфологии')
    axs[1].axis('off')
    
    axs[2].imshow(cv2.cvtColor(result, cv2.COLOR_BGR2RGB))
    axs[2].set_title('Результат после удаления фона')
    axs[2].axis('off')
    
    plt.tight_layout()
    plt.savefig(output_path)
    plt.close()

# Основная функция
if __name__ == '__main__':
    pics_dir = 'pics'
    no_bg_dir = 'noBG'
    collage_dir = 'L1'
    os.makedirs(no_bg_dir, exist_ok=True)
    os.makedirs(collage_dir, exist_ok=True)
    
    # Перебор всех файлов в папке pics
    for img_name in os.listdir(pics_dir):
        if img_name.lower().endswith(('.png', '.jpg', '.jpeg')):
            img_path = os.path.join(pics_dir, img_name)
            image = cv2.imread(img_path)
            if image is None:
                print(f"Не удалось загрузить {img_name}")
                continue
            
            # Обработка изображения
            result, mask_before, mask_after = remove_bright_background(image, accuracy=0.6, kernel_size=7, iterations=2)
            
            # Сохранение only without_bg в noBG и collage в L1
            base_name = os.path.splitext(img_name)[0]
            cv2.imwrite(os.path.join(no_bg_dir, f'{base_name}_without_bg.jpg'), result)
            
            # Создание коллажа
            output_filename = os.path.join(collage_dir, f'{base_name}_collage.jpg')
            visualize_results(image, result, mask_before, mask_after, output_filename)
            print(f"Изображение без фона сохранено в {no_bg_dir}, коллаж сохранен в {collage_dir} для {img_name}")