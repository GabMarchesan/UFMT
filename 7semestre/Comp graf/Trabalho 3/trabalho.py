from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
import pywavefront
from pywavefront import visualization
import math

# Variáveis Globais
T = 0  # Rotação do batman
T3_x = 0  # Movimento para frente/trás na direção x
T3_z = 0  # Movimento para frente/trás na direção z
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
movimento_aranha = 0.0
direcao_aranha = 1.5  # Direção do movimento da aranha

# Conjunto de teclas pressionadas
keys_pressed = set()

# Carregar modelos 3D
batman = pywavefront.Wavefront("batman.obj")
aranha = pywavefront.Wavefront("aranha.obj")
platform = pywavefront.Wavefront("chao.obj")


def update_camera():
    global camera_x, camera_y, camera_z

    # Calcula a posição da câmera com base nos ângulos de visão e distância
    camera_x = T3_x - camera_distance * math.sin(math.radians(camera_angle_x))
    camera_y = altura_inicial_pulo + 10.0 + camera_distance * \
        math.sin(math.radians(camera_angle_y))
    camera_z = T3_z - camera_distance * math.cos(math.radians(camera_angle_x))


def display():
    # Função para desenhar na tela
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

    # Atualiza a posição da câmera
    update_camera()

    # Configura a câmera
    gluLookAt(camera_x, camera_y, camera_z,  # Posição da câmera
              # Ponto para onde a câmera está olhando (a aranha)
              T3_x, altura_inicial_pulo, T3_z,
              0.0, 1.0, 0.0)                # Direção "para cima" da câmera

    # Desenha a plataforma
    glPushMatrix()
    glTranslatef(0.0, -1.0, 0.0)  # Ajuste a posição para subir a plataforma
    glColor3f(0, 128, 0)  # Cor da plataforma (verde)
    visualization.draw(platform)  # Desenha a plataforma
    glPopMatrix()

    # Desenha o batman controlado pelo teclado
    glPushMatrix()
    glTranslatef(T3_x, altura_inicial_pulo, T3_z)  # Movimenta e aplica pulo
    glRotatef(T, 0.0, 1.0, 0.0)  # Aplica rotação
    glColor3f(0.5, 0.5, 0.5)    # Cor do batman (cinza)
    visualization.draw(batman)  # Desenha o cinza
    glPopMatrix()

    # Desenha a aranha com movimento automático
    glPushMatrix()
    glTranslatef(0, -2, movimento_aranha)  # Movimenta a aranha
    glColor3f(1.0, 1.0, 1.0)  # Cor da aranha (cinza)
    visualization.draw(aranha)  # Desenha a arnha
    glPopMatrix()

    glutSwapBuffers()  # Troca os buffers para evitar flickering


def specialKeys(key, x, y):
    # Função para controlar a câmera com as teclas de seta
    global camera_angle_x, camera_angle_y, camera_distance

    if key == GLUT_KEY_LEFT:
        camera_angle_x -= 5.0  # Rotaciona a câmera para a esquerda
    elif key == GLUT_KEY_RIGHT:
        camera_angle_x += 5.0  # Rotaciona a câmera para a direita
    elif key == GLUT_KEY_UP:
        camera_angle_y += 5.0  # Inclina a câmera para cima
    elif key == GLUT_KEY_DOWN:
        camera_angle_y -= 5.0  # Inclina a câmera para baixo


def normalKeys(key, x, y):
    # Função para controlar o robô com as teclas WASD e espaço
    global pulo, camera_distance

    if key == b'w':
        keys_pressed.add(b'w')  # Movimento para frente
    elif key == b's':
        keys_pressed.add(b's')  # Movimento para trás
    elif key == b'a':
        keys_pressed.add(b'a')  # Rotação para a esquerda
    elif key == b'd':
        keys_pressed.add(b'd')  # Rotação para a direita
    elif key == b' ':
        if not pulo:
            pulo = True  # Inicia o pulo se o robô não estiver pulando
    elif key == b'+':
        camera_distance = max(5.0, camera_distance - 1.0)  # Zoom in
    elif key == b'-':
        camera_distance += 1.0  # Zoom out


def normalKeysUp(key, x, y):
    # Função para registrar quando as teclas são soltas
    if key == b'w':
        keys_pressed.discard(b'w')
    elif key == b's':
        keys_pressed.discard(b's')
    elif key == b'a':
        keys_pressed.discard(b'a')
    elif key == b'd':
        keys_pressed.discard(b'd')


def animacao(value):
    # Função de animação que controla movimento, rotação e pulo
    global movimento_aranha, direcao_aranha, pulo, altura_inicial_pulo, direcao_pulo, T3_x, T3_z, T

    # Movimenta a aranha para frente e para trás
    movimento_aranha += 0.1 * direcao_aranha
    if movimento_aranha > 30.0 or movimento_aranha < -30.0:
        direcao_aranha *= -1  # Inverte a direção quando atinge o limite

    # Lógica para o pulo do batman
    if pulo:
        altura_inicial_pulo += velocidade_pulo * direcao_pulo
        if altura_inicial_pulo > 2.0:
            direcao_pulo = -1  # Inicia descida quando atinge a altura máxima
        if altura_inicial_pulo < 0.0:
            altura_inicial_pulo = 0.0
            direcao_pulo = 1  # Reinicia o pulo
            pulo = False  # Conclui o pulo

    # Movimento contínuo baseado nas teclas pressionadas
    move_speed = 0.1
    angle_rad = math.radians(T)
    if b'w' in keys_pressed:
        # Move para frente na direção x
        T3_x += move_speed * math.sin(angle_rad)
        # Move para frente na direção z
        T3_z += move_speed * math.cos(angle_rad)
    if b's' in keys_pressed:
        T3_x -= move_speed * math.sin(angle_rad)  # Move para trás na direção x
        T3_z -= move_speed * math.cos(angle_rad)  # Move para trás na direção z
    if b'd' in keys_pressed:
        T -= 5  # Rotaciona para a esquerda
    if b'a' in keys_pressed:
        T += 5  # Rotaciona para a direita

    glutPostRedisplay()  # Solicita a redisplay da janela
    glutTimerFunc(30, animacao, 1)  # Configura a próxima chamada da animação


def resize(w, h):
    # Função para ajustar o tamanho da viewport
    glViewport(0, 0, w, h)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()

    # Ajusta a perspectiva para mostrar todo o cenário
    aspect_ratio = float(w) / float(h)
    if aspect_ratio > 1:
        glOrtho(-camera_distance * aspect_ratio, camera_distance *
                aspect_ratio, -camera_distance, camera_distance, -1000, 1000)
    else:
        glOrtho(-camera_distance, camera_distance, -camera_distance /
                aspect_ratio, camera_distance / aspect_ratio, -1000, 1000)

    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()


def init():
    # Função de inicialização da cena 3D
    glClearColor(0.7, 0.7, 1.0, 0.0)  # Cor de fundo da tela (azul claro)
    glShadeModel(GL_SMOOTH)  # Modelo de sombreamento suave
    glClearDepth(1.0)
    glEnable(GL_DEPTH_TEST)
    glDepthFunc(GL_LEQUAL)
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)

    # Configuração da iluminação
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, [0.1, 0.1, 0.1, 1.0])  # Luz ambiente global
    glLightfv(GL_LIGHT0, GL_AMBIENT, [0.2, 0.2, 0.2, 1.0])  # Luz ambiente da fonte de luz 0
    glLightfv(GL_LIGHT0, GL_DIFFUSE, [0.5, 0.5, 0.5, 1.0])  # Luz difusa
    glLightfv(GL_LIGHT0, GL_SPECULAR, [0.7, 0.7, 0.7, 1.0])  # Luz especular
    glLightfv(GL_LIGHT0, GL_POSITION, [10.0, 10.0, 10.0, 0.0])  # Luz direcional

    # Ativa a iluminação e a luz 0
    glEnable(GL_LIGHTING)
    glEnable(GL_LIGHT0)

    # Permite que a cor seja influenciada pela luz
    glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE)
    glEnable(GL_NORMALIZE)


# Configuração do GLUT
glutInit()
glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB)
glutInitWindowSize(1920, 1080)
glutInitWindowPosition(100, 100)
wind = glutCreateWindow("Batman vs Aranha")
init()

# Configuração das funções de callback do GLUT
glutDisplayFunc(display)
glutReshapeFunc(resize)
glutTimerFunc(30, animacao, 1)
glutSpecialFunc(specialKeys)  # Controle da câmera com setas
glutKeyboardFunc(normalKeys)
glutKeyboardUpFunc(normalKeysUp)

# Início do loop principal do GLUT
glutMainLoop()
