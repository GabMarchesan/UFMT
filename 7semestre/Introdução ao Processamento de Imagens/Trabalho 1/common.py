import cv2
import numpy as np

def read_img(path, grayscale=True):
    """
    Lê uma imagem do caminho fornecido e a retorna como um array NumPy normalizado no intervalo [0, 1].
    """
    flag = cv2.IMREAD_GRAYSCALE if grayscale else cv2.IMREAD_COLOR
    img = cv2.imread(path, flag)

    if img is None:
        raise FileNotFoundError(f"Não foi possível carregar a imagem no caminho: {path}")

    img = img.astype(np.float32) / 255.0
    return img


def save_img(img, path):
    """
    Salva a imagem no caminho especificado, após reescalá-la para valores entre 0 e 255.

    Parâmetros:
        img (np.ndarray): Imagem normalizada no intervalo [0, 1].
        path (str): Caminho onde a imagem será salva.
    """
    # Garante que os valores estão no intervalo [0,1]
    img = np.clip(img, 0, 1)

    # Converte para uint8 (0–255)
    img_to_save = (img * 255).astype(np.uint8)

    # Salva usando OpenCV
    success = cv2.imwrite(path, img_to_save)
    if not success:
        raise IOError(f"Não foi possível salvar a imagem em: {path}")
