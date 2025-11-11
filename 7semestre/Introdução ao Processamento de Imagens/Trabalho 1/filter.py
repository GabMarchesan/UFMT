import numpy as np
import cv2

def convolve(img, kernel2d, padd=False):
    """
    Realiza a convolução de uma imagem com um kernel 2D.
    
    Parâmetros:
    img : np.ndarray
        Imagem de entrada (grayscale ou colorida).
    kernel2d : np.ndarray
        Kernel 2D para convolução.
    padd : bool
        Se True, realiza padding na imagem para manter o tamanho original.
    
    Retorna:
    np.ndarray
        Imagem convoluída.
    """
    # Se for colorida, separa canais
    if len(img.shape) == 3:
        channels = []
        for c in range(img.shape[2]):
            channels.append(convolve(img[:, :, c], kernel2d, padd))
        return np.stack(channels, axis=-1)
    
    # Kernel dimensions
    kh, kw = kernel2d.shape
    kh2, kw2 = kh // 2, kw // 2
    
    # Padding se necessário
    if padd:
        img_padded = np.pad(img, ((kh2, kh2), (kw2, kw2)), mode='edge')
    else:
        img_padded = img
    
    h, w = img.shape
    output = np.zeros_like(img, dtype=np.float32)
    
    # Convolução
    for i in range(h):
        for j in range(w):
            region = img_padded[i:i+kh, j:j+kw]
            output[i, j] = np.sum(region * kernel2d)
    
    # Normaliza se imagem original estiver em [0,1]
    if img.dtype != np.uint8:
        output = np.clip(output, 0, 1)
    
    return output

def edgedetection(img):
    """
    Detecta bordas em uma imagem calculando a magnitude do gradiente.

    As derivadas são aproximadas usando kernels de diferença central 1x3 e 3x1.

    Parâmetros:
    img : np.ndarray
        Imagem de entrada (grayscale ou colorida).

    Retorna:
    np.ndarray
        Uma imagem de ponto flutuante no intervalo [0, 1] representando a
        magnitude do gradiente (bordas).
    """
    if len(img.shape) == 3:
        img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    if img.dtype == np.uint8:
        img = img.astype(np.float32) / 255.0

    Kx = np.array([[-1, 0, 1]], dtype=np.float32)
    Ky = np.array([[-1], [0], [1]], dtype=np.float32)

    Ix = convolve(img, Kx, padd=True)
    Iy = convolve(img, Ky, padd=True)
    
    magnitude = np.sqrt(Ix**2 + Iy**2)
    
    max_mag = np.max(magnitude)
    if max_mag > 0:
        magnitude = magnitude / max_mag
    
    return magnitude

def sobel(img):
    """
    Detecta bordas em uma imagem usando os operadores de Sobel.

    Calcula a magnitude do gradiente após aplicar os kernels de Sobel
    para as derivadas nas direções x e y.

    Parâmetros:
    img : np.ndarray
        Imagem de entrada (grayscale ou colorida).

    Retorna:
    np.ndarray
        Uma imagem de ponto flutuante no intervalo [0, 1] representando a
        magnitude do gradiente de Sobel.
    """
    if len(img.shape) == 3:
        img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    if img.dtype == np.uint8:
        img = img.astype(np.float32) / 255.0

    # Kernels de Sobel
    Kx = np.array([[-1, 0, 1],
                   [-2, 0, 2],
                   [-1, 0, 1]], dtype=np.float32)
    
    Ky = np.array([[-1, -2, -1],
                   [ 0,  0,  0],
                   [ 1,  2,  1]], dtype=np.float32)

    # Aplica a convolução para encontrar as derivadas Ix e Iy
    Ix = convolve(img, Kx, padd=True)
    Iy = convolve(img, Ky, padd=True)
    
    # Calcula a magnitude do gradiente
    magnitude = np.sqrt(Ix**2 + Iy**2)
    
    # Normaliza o resultado para o intervalo [0, 1] para visualização
    max_mag = np.max(magnitude)
    if max_mag > 0:
        magnitude = magnitude / max_mag
    
    return magnitude

def steerablefilter(img, alpha):
    """
    Calcula a derivada direcional de uma imagem usando um filtro direcionável
    baseado nos operadores de Sobel.

    Parâmetros:
    img : np.ndarray
        Imagem de entrada (grayscale ou colorida).
    alpha : float
        Ângulo da derivada em graus.

    Retorna:
    np.ndarray
        Uma imagem de ponto flutuante no intervalo [0, 1] representando a
        derivada direcional.
    """
    if len(img.shape) == 3:
        img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    if img.dtype == np.uint8:
        img = img.astype(np.float32) / 255.0
    
    # Converte o ângulo de graus para radianos
    alpha_rad = np.deg2rad(alpha)

    # Kernels base de Sobel
    Kx = np.array([[-1, 0, 1],
                   [-2, 0, 2],
                   [-1, 0, 1]], dtype=np.float32)
    
    Ky = np.array([[-1, -2, -1],
                   [ 0,  0,  0],
                   [ 1,  2,  1]], dtype=np.float32)
    
    # Constrói o kernel direcionável K_alpha
    Ka = Kx * np.cos(alpha_rad) + Ky * np.sin(alpha_rad)
    
    # Aplica o filtro direcionável na imagem
    directional_derivative = convolve(img, Ka, padd=True)

    # Normaliza o resultado para o intervalo [0, 1] para uma melhor visualização
    # A derivada direcional pode ter valores negativos, então uma normalização
    # min-max é apropriada aqui.
    min_val = np.min(directional_derivative)
    max_val = np.max(directional_derivative)
    if max_val > min_val:
        output = (directional_derivative - min_val) / (max_val - min_val)
    else:
        output = np.zeros_like(img)
        
    return output