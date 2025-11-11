#PARA EXECUTAR O CÓDIGO APERTE ENTER
#CASO A ARANHA TE PEGAR, APERTE R PARA REINICIAR O JOGO

from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
import pywavefront
from pywavefront import visualization
import math
import random

# Variáveis Globais
T = 0  # Rotação do Batman
T3_x = -5  # Movimento do Batman na direção x
T3_z = 0  # Movimento do Batman na direção z
pulo = False  # Estado de pulo
velocidade_pulo = 0.25  # Velocidade do pulo
altura_inicial_pulo = 0.0  # Altura atual do pulo
direcao_pulo = 1.5  # Direção do pulo (subindo ou descendo)

# Posição da câmera
camera_x = 0.0
camera_y = 10.0
camera_z = 15.0
camera_angle_x = 0.0  # Ângulo horizontal da câmera
camera_angle_y = 0.0  # Ângulo vertical da câmera
camera_distance = 15.0  # Distância da câmera ao ponto de interesse

# Movimento da aranha
aranha_pos_x = -20.0  # Posição inicial da aranha
aranha_pos_z = 0.0  # Posição inicial da aranha
aranha_velocidade = 0.09  # Velocidade da aranha

# Conjunto de teclas pressionadas
keys_pressed = set()

# Variáveis para inércia e aceleração
velocidade_x = 0.0  # Velocidade no eixo X
velocidade_z = 0.0  # Velocidade no eixo Z
aceleracao = 0.02  # Aceleração do Batman
desaceleracao = 0.98  # Fator de desaceleração (inércia)
max_velocidade = 0.17  # Velocidade máxima do Batman

# Partículas
particulas = []

# Estado do jogo
colisao_ocorrida = False
game_over = False  # Estado do jogo
menu_inicial = True  # Estado do menu inicial

# Carregar modelos 3D
batman = pywavefront.Wavefront("batman.obj", collect_faces=True)
aranha = pywavefront.Wavefront("aranha.obj", collect_faces=True)
platform = pywavefront.Wavefront("chao.obj", collect_faces=True)

def update_camera():
    global camera_x, camera_y, camera_z
    camera_x = T3_x - camera_distance * math.sin(math.radians(camera_angle_x))
    camera_y = altura_inicial_pulo + 10.0 + camera_distance * math.sin(math.radians(camera_angle_y))
    camera_z = T3_z - camera_distance * math.cos(math.radians(camera_angle_x))

def display():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

    if menu_inicial:
        exibir_menu_inicial()
    else:
        update_camera()
        gluLookAt(camera_x, camera_y, camera_z, T3_x, altura_inicial_pulo, T3_z, 0.0, 1.0, 0.0)

        # Desenha a plataforma
        glPushMatrix()
        glTranslatef(0.0, -1.0, 0.0)
        glColor3f(0, 128, 0)
        visualization.draw(platform)
        glPopMatrix()

        # Desenha o Batman
        glPushMatrix()
        glTranslatef(T3_x, altura_inicial_pulo, T3_z)
        glRotatef(T, 0.0, 1.0, 0.0)
        glColor3f(0.5, 0.5, 0.5)
        visualization.draw(batman)
        glPopMatrix()

        # Desenha a aranha
        glPushMatrix()
        glTranslatef(aranha_pos_x, 0.0, aranha_pos_z)
        glColor3f(1.0, 1.0, 1.0)
        visualization.draw(aranha)
        glPopMatrix()

        # Desenha as partículas
        desenhar_particulas()

        # Checa colisão e exibe mensagem se necessário
        checa_colisao()
        exibir_mensagem_colisao()

    glutSwapBuffers()

def exibir_menu_inicial():
    glColor3f(1.0, 1.0, 1.0)
    glLoadIdentity()
    glRasterPos2f(-0.1, 0.0)
    titulo = "Batman vs Aranha"
    for char in titulo:
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ord(char))

    glRasterPos2f(-0.1, -0.1)
    instrucoes = "Pressione Enter para Iniciar"
    for char in instrucoes:
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ord(char))

def resize(w, h):
    glViewport(0, 0, w, h)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    aspect_ratio = float(w) / float(h) if h > 0 else 1.0
    gluPerspective(45.0, aspect_ratio, 1.0, 1000.0)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

def animacao(value):
    """Função de animação, lida com movimentos e atualizações de posição."""
    global pulo, altura_inicial_pulo, T3_x, T3_z, T, aranha_pos_x, aranha_pos_z, direcao_pulo
    global velocidade_x, velocidade_z, particulas

    if not game_over and not menu_inicial:
        if pulo:
            altura_inicial_pulo += velocidade_pulo * direcao_pulo
            if altura_inicial_pulo > 2.0:
                direcao_pulo = -1
            if altura_inicial_pulo < 0.0:
                altura_inicial_pulo = 0.0
                direcao_pulo = 1
                pulo = False

        # Aceleração e movimento com inércia
        if b'w' in keys_pressed:
            velocidade_x += aceleracao * math.sin(math.radians(T))
            velocidade_z += aceleracao * math.cos(math.radians(T))
        if b's' in keys_pressed:
            velocidade_x -= aceleracao * math.sin(math.radians(T))
            velocidade_z -= aceleracao * math.cos(math.radians(T))

        # Inércia (desaceleração)
        velocidade_x *= desaceleracao
        velocidade_z *= desaceleracao

        # Limita a velocidade máxima
        if abs(velocidade_x) > max_velocidade:
            velocidade_x = max_velocidade if velocidade_x > 0 else -max_velocidade
        if abs(velocidade_z) > max_velocidade:
            velocidade_z = max_velocidade if velocidade_z > 0 else -max_velocidade

        T3_x += velocidade_x
        T3_z += velocidade_z

        # Movimento da aranha
        dx = T3_x - aranha_pos_x
        dz = T3_z - aranha_pos_z
        distancia = math.sqrt(dx ** 2 + dz ** 2)
        if distancia > 2.0:
            aranha_pos_x += aranha_velocidade * (dx / distancia)
            aranha_pos_z += aranha_velocidade * (dz / distancia)

    glutPostRedisplay()
    glutTimerFunc(30, animacao, 1)

def checa_colisao():
    global aranha_pos_x, aranha_pos_z, T3_x, T3_z, colisao_ocorrida, game_over
    # Checa colisão entre Batman e Aranha considerando uma distância mínima
    distancia_x = abs(T3_x - aranha_pos_x)
    distancia_z = abs(T3_z - aranha_pos_z)
    limite_distancia = 2.0  # Aumenta o limite para garantir detecção

    if distancia_x < limite_distancia and distancia_z < limite_distancia:
        colisao_ocorrida = True
        game_over = True  # Ativa o estado de Game Over
        gerar_particulas_explosao()

def gerar_particulas_explosao():
    """Gera partículas pequenas triangulares quando ocorre uma colisão"""
    global particulas
    for _ in range(50):  # 50 partículas para a explosão
        particulas.append({
            'x': aranha_pos_x,
            'z': aranha_pos_z,
            'vx': random.uniform(-0.1, 0.1),
            'vz': random.uniform(-0.1, 0.1),
            'life': 1.0  # Vida inicial da partícula
        })

def desenhar_particulas():
    """Desenha as partículas na tela"""
    global particulas
    glColor3f(0, 0, 0)  # Cor preta para as partículas
    for particula in particulas:
        particula['x'] += particula['vx']
        particula['z'] += particula['vz']
        particula['life'] -= 0.05  # Diminui a vida da partícula

        if particula['life'] > 0:
            glPushMatrix()
            glTranslatef(particula['x'], 0.0, particula['z'])
            glBegin(GL_TRIANGLES)
            glVertex3f(0.0, 0.1, 0.0)
            glVertex3f(-0.05, -0.1, 0.0)
            glVertex3f(0.05, -0.1, 0.0)
            glEnd()
            glPopMatrix()
        else:
            particulas.remove(particula)

def normalKeys(key, x, y):
    global T, keys_pressed, pulo, game_over, menu_inicial

    if key == b'\x1b':  # Escape
        exit(0)

    if menu_inicial and key == b'\r':  # Enter
        menu_inicial = False

    if not game_over:
        if key == b'w':
            keys_pressed.add(b'w')
        if key == b's':
            keys_pressed.add(b's')
        if key == b'd':
            T -= 5  # Rotaciona o Batman para a esquerda
        if key == b'a':
            T += 5  # Rotaciona o Batman para a direita
        if key == b' ' and not pulo:
            pulo = True
    elif key == b'r':  # Reinicia o jogo
        reiniciar_jogo()

def normalKeysUp(key, x, y):
    global keys_pressed

    if key == b'w':
        keys_pressed.remove(b'w')
    if key == b's':
        keys_pressed.remove(b's')

def exibir_mensagem_colisao():
    global colisao_ocorrida
    if colisao_ocorrida:
        glColor3f(1.0, 0.0, 0.0)  # Cor vermelha para a mensagem de colisão
        glLoadIdentity()
        glRasterPos2f(-0.1, 0.0)
        mensagem = "COLISÃO! Jogo Finalizado"
        for char in mensagem:
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ord(char))
        glRasterPos2f(-0.1, -0.1)
        instrucoes = "Pressione R para Reiniciar"
        for char in instrucoes:
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ord(char))


def reiniciar_jogo():
    """Reinicia as variáveis do jogo"""
    global T3_x, T3_z, T, aranha_pos_x, aranha_pos_z, colisao_ocorrida, game_over, particulas
    T3_x, T3_z = -5.0, 0.0
    T = 0
    aranha_pos_x, aranha_pos_z = -20.0, 0.0
    colisao_ocorrida = False
    game_over = False
    particulas.clear()

def specialKeys(key, x, y):
    global camera_angle_x, camera_angle_y, camera_distance
    if key == GLUT_KEY_LEFT:
        camera_angle_x -= 5
    if key == GLUT_KEY_RIGHT:
        camera_angle_x += 5
    if key == GLUT_KEY_UP:
        camera_angle_y += 5
    if key == GLUT_KEY_DOWN:
        camera_angle_y -= 5

def main():
    glutInit()
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)
    glutInitWindowSize(800, 600)
    glutCreateWindow("Batman vs Aranha 3D")
    glEnable(GL_DEPTH_TEST)
    glEnable(GL_LIGHTING)
    glEnable(GL_LIGHT0)
    glClearColor(0.0, 1.0, 1.0, 1.0)
    glutReshapeFunc(resize)
    glutDisplayFunc(display)
    glutKeyboardFunc(normalKeys)
    glutKeyboardUpFunc(normalKeysUp)
    glutSpecialFunc(specialKeys)
    glutTimerFunc(25, animacao, 1)  # Atualiza a animação a cada 25ms
    glutMainLoop()

if __name__ == "__main__":
    main()
