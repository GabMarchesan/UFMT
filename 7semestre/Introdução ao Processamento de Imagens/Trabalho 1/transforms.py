from common import read_img
import numpy as np

def gamma(img, lambd):
    """
    Aplica a correção gama (Power-Law) em uma imagem.

    Parâmetros:
        img (np.ndarray ou str): Imagem já carregada ou caminho da imagem.
        lambd (float): Valor do gamma.

    Retorna:
        np.ndarray: Imagem normalizada após a correção gama.
    """
    # Se img for caminho, lê a imagem
    if isinstance(img, str):
        img = read_img(img, grayscale=True)
    
    # Aplica a correção gama: s = r^gamma
    img_gamma = np.power(img, lambd)
    
    # Garante que os valores estão entre 0 e 1
    img_gamma = np.clip(img_gamma, 0, 1)
    
    return img_gamma

def negative(img):
    """
    Gera o negativo de uma imagem.

    Parâmetros:
        img (np.ndarray ou str): Imagem já carregada ou caminho da imagem.

    Retorna:
        np.ndarray: Imagem negativa normalizada.
    """
    # Se img for caminho, lê a imagem (mantendo cores se for colorida)
    if isinstance(img, str):
        img = read_img(img, grayscale=False)

    # Negativo: s = 1 - r
    img_neg = 1.0 - img

    # Garante que valores estão entre 0 e 1
    img_neg = np.clip(img_neg, 0, 1)

    return img_neg

def rgb2hsi(img):
    """Converte imagem RGB (0-1) para HSI."""
    R, G, B = img[:, :, 0], img[:, :, 1], img[:, :, 2]
    num = 0.5 * ((R - G) + (R - B))
    den = np.sqrt((R - G)**2 + (R - B)*(G - B)) + 1e-8
    theta = np.arccos(num / den)
    H = np.where(B <= G, theta, 2*np.pi - theta) / (2*np.pi)
    min_rgb = np.minimum(np.minimum(R, G), B)
    S = 1 - (3 / (R + G + B + 1e-8)) * min_rgb
    I = (R + G + B) / 3.0
    return np.stack([H, S, I], axis=-1)

def hsi2rgb(img):
    """Converte imagem HSI para RGB (0-1)."""
    H, S, I = img[:, :, 0]*2*np.pi, img[:, :, 1], img[:, :, 2]
    R = np.zeros_like(H)
    G = np.zeros_like(H)
    B = np.zeros_like(H)

    # Setor 1
    idx = (H < 2*np.pi/3)
    B[idx] = I[idx]*(1 - S[idx])
    R[idx] = I[idx]*(1 + (S[idx]*np.cos(H[idx]))/(np.cos(np.pi/3 - H[idx])+1e-8))
    G[idx] = 3*I[idx] - (R[idx] + B[idx])

    # Setor 2
    idx = (H >= 2*np.pi/3) & (H < 4*np.pi/3)
    H2 = H[idx] - 2*np.pi/3
    R[idx] = I[idx]*(1 - S[idx])
    G[idx] = I[idx]*(1 + (S[idx]*np.cos(H2))/(np.cos(np.pi/3 - H2)+1e-8))
    B[idx] = 3*I[idx] - (R[idx] + G[idx])

    # Setor 3
    idx = (H >= 4*np.pi/3)
    H3 = H[idx] - 4*np.pi/3
    G[idx] = I[idx]*(1 - S[idx])
    B[idx] = I[idx]*(1 + (S[idx]*np.cos(H3))/(np.cos(np.pi/3 - H3)+1e-8))
    R[idx] = 3*I[idx] - (G[idx] + B[idx])

    return np.clip(np.stack([R, G, B], axis=-1), 0, 1)

def _histeq_gray(img):
    """Equalização de histograma manual para imagem grayscale."""
    # Normaliza para 0-255
    img_int = (img*255).astype(np.uint8)
    h, w = img_int.shape
    L = 256
    total_pixels = h*w

    # 1. Calcular histograma manualmente
    hist = [0]*L
    for i in range(h):
        for j in range(w):
            hist[img_int[i,j]] += 1

    # 2. PDF
    pdf = [x/total_pixels for x in hist]

    # 3. CDF
    cdf = [0]*L
    cdf[0] = pdf[0]
    for k in range(1, L):
        cdf[k] = cdf[k-1] + pdf[k]

    # 4. Transformação
    T = [int(round((L-1)*x)) for x in cdf]

    # 5. Aplicar mapeamento
    img_eq = np.zeros_like(img_int)
    for i in range(h):
        for j in range(w):
            img_eq[i,j] = T[img_int[i,j]]

    return img_eq.astype(np.float32)/255.0

def histeq(img):
    """Equalização de histograma manual.
       - Se grayscale: equaliza direto.
       - Se colorida: equaliza apenas canal de intensidade no HSI.
    """
    # Se for caminho, ler imagem
    if isinstance(img, str):
        img = read_img(img, grayscale=False)

    # Se grayscale
    if len(img.shape) == 2:
        return _histeq_gray(img)

    # Se colorida
    hsi = rgb2hsi(img)
    I = hsi[:,:,2]
    I_eq = _histeq_gray(I)
    hsi[:,:,2] = I_eq
    return hsi2rgb(hsi)