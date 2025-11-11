from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
import pywavefront
from pywavefront import visualization
import math

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
aranha_velocidade = 0.05  # Velocidade da aranha

# Conjunto de teclas pressionadas
keys_pressed = set()

# Variável para exibir mensagem de colisão
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

    if not game_over and not menu_inicial:
        if pulo:
            altura_inicial_pulo += velocidade_pulo * direcao_pulo
            if altura_inicial_pulo > 2.0:
                direcao_pulo = -1
            if altura_inicial_pulo < 0.0:
                altura_inicial_pulo = 0.0
                direcao_pulo = 1
                pulo = False

        move_speed = 0.1
        angle_rad = math.radians(T)
        if b'w' in keys_pressed:
            T3_x += move_speed * math.sin(angle_rad)
            T3_z += move_speed * math.cos(angle_rad)
        if b's' in keys_pressed:
            T3_x -= move_speed * math.sin(angle_rad)
            T3_z -= move_speed * math.cos(angle_rad)
        if b'd' in keys_pressed:
            T -= 5
        if b'a' in keys_pressed:
            T += 5

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

def exibir_mensagem_colisao():
    global colisao_ocorrida, game_over
    if colisao_ocorrida:
        glPushMatrix()
        glLoadIdentity()
        glColor3f(1.0, 0.0, 0.0)
        glRasterPos2f(-0.1, 0.0)
        mensagem = "Colisão detectada! Pressione R para Reiniciar!"
        for char in mensagem:
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ord(char))
        glPopMatrix()

    if game_over:
        glPushMatrix()
        glLoadIdentity()
        glColor3f(1.0, 0.0, 0.0)
        glRasterPos2f(-0.1, -0.1)
        mensagem_game_over = "Game Over!"
        for char in mensagem_game_over:
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ord(char))
        glPopMatrix()

def specialKeys(key, x, y):
    global camera_angle_x, camera_angle_y
    if key == GLUT_KEY_LEFT:
        camera_angle_x -= 5.0
    elif key == GLUT_KEY_RIGHT:
        camera_angle_x += 5.0
    elif key == GLUT_KEY_UP:
        camera_angle_y += 5.0
    elif key == GLUT_KEY_DOWN:
        camera_angle_y -= 5.0

def normalKeys(key, x, y):
    global pulo, game_over, T3_x, T3_z, altura_inicial_pulo, menu_inicial

    if key == b'w':
        keys_pressed.add(b'w')
    elif key == b's':
        keys_pressed.add(b's')
    elif key == b'a':
        keys_pressed.add(b'a')
    elif key == b'd':
        keys_pressed.add(b'd')
    elif key == b' ' and not pulo and not game_over:  # Pular
        pulo = True
        altura_inicial_pulo = 0.0
    elif key == b'\r' and menu_inicial:  # Iniciar jogo
        menu_inicial = False
    elif key == b'\x1b':  # Escape
        exit()
    elif key == b'r' and game_over:  # Reiniciar jogo
        reiniciar_jogo()


def normalKeysUp(key, x, y):
    if key == b'w':
        keys_pressed.discard(b'w')
    elif key == b's':
        keys_pressed.discard(b's')
    elif key == b'a':
        keys_pressed.discard(b'a')
    elif key == b'd':
        keys_pressed.discard(b'd')

def reiniciar_jogo():
    global T3_x, T3_z, altura_inicial_pulo, pulo, game_over, menu_inicial, colisao_ocorrida
    T3_x = -5  # Reinicia a posição do Batman
    T3_z = 0  # Reinicia a posição do Batman
    pulo = False
    altura_inicial_pulo = 0.0
    game_over = False
    colisao_ocorrida = False
    menu_inicial = True  # Volta ao menu inicial

def init():
    glClearColor(0.7, 0.7, 1.0, 0.0)
    glShadeModel(GL_SMOOTH)
    glEnable(GL_DEPTH_TEST)
    glEnable(GL_LIGHTING)
    glEnable(GL_LIGHT0)
    glLightfv(GL_LIGHT0, GL_POSITION, [10.0, 10.0, 10.0, 1.0])

# Configuração do GLUT
glutInit()
glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB)
glutInitWindowSize(1920, 1080)
glutCreateWindow("Batman vs Aranha")
init()

# Configuração das funções de callback do GLUT
glutDisplayFunc(display)
glutReshapeFunc(resize)
glutTimerFunc(30, animacao, 1)
glutSpecialFunc(specialKeys)
glutKeyboardFunc(normalKeys)
glutKeyboardUpFunc(normalKeysUp)

# Início do loop principal do GLUT
glutMainLoop()
