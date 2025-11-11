from OpenGL.GL import *
from OpenGL.GLUT import *

#pip install pyopengl
#
#se n funcionar, entrar em:
#https://www.lfd.uci.edu/~gohlke/pythonlibs/
#encontrar versão do PyOpenGL e PyOpenGL_accelerate

i = 0.0
def display():
    global i
    glClear(GL_COLOR_BUFFER_BIT)
    glColor3f(1.0, i, 0.0)
    glBegin(GL_POLYGON)
    glVertex3f(0.0, 0.0, 0.0)
    glColor3f(i, 0.0, i)
    glVertex3f(1.0, 0.0, 0.0)
    glColor3f(i, 0.0, 255)
    glVertex3f(0.0, 1.0, 0.0)
    glColor3f(i, i, i)
    i += 0.1
    glVertex3f(-1.0, 1.0,0.0)
    glEnd()
    
    
    glFlush()

def animacao(value):
    glutPostRedisplay()
    glutTimerFunc(500, animacao,1)

glutInit()
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB)
glutInitWindowSize(700, 700)
glutInitWindowPosition(100, 100)
wind = glutCreateWindow("hello")
glClearColor(0.0, 0.0, 0.0, 0.0)
glutDisplayFunc(display)
glutTimerFunc(500,animacao,1)
glutMainLoop()
