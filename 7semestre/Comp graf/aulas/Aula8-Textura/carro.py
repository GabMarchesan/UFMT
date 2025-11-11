from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
import pywavefront
from pywavefront import visualization
import numpy as np
from PIL import Image

#biblioteca para carregar imagens
#pip install pillow



from OpenGL.arrays import vbo
from OpenGL.GL import shaders

T = 1
T2 = 1
T3 = 1



def display():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glMatrixMode(GL_MODELVIEW)
    
    
    vertices = carro.materials['default0'].vertices
    vertices = np.array(vertices, dtype=np.float32).reshape(-1,6)
    vbo_carro = vbo.VBO(vertices)
    
    
    vertices = roda.materials['Default_OBJ'].vertices
    #tex_2, vn_3, v_3 (8 floats e 32 bytes por vertice)
    vertices = np.array(vertices, dtype=np.float32).reshape(-1,8)
    vbo_roda = vbo.VBO(vertices)

    

    glPushMatrix()
    #Ações em todo o carro
    #glRotatef(T, 0.0, 1.0, 0.0)
    glTranslatef(T, T2, T3)
    #glScalef(T, T2, T3)

    glPushMatrix()
    #Corpo do carro
    glTranslatef(0.0, 1.0, 0.0)
    glColor3f(0.1, 0.0, 1.1)
    #visualization.draw(carro)
    
    #glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices)
    #glEnableVertexAttribArray(0)
    
    
    # ativa shader
    glUseProgram(carro_shader)
    
    # Define iluminação
    glUniform4f( LIGTH_LOCATIONS['Global_ambient'], 0.1, 0.1, 0.1, 1.0 )
    glUniform4f( LIGTH_LOCATIONS['Light_ambient'], 0.2, 0.2, 0.2, 1.0 )
    glUniform4f( LIGTH_LOCATIONS['Light_diffuse'], 0.9, 0.9, 0.9, 1.0 )
    glUniform3f( LIGTH_LOCATIONS['Light_location'], -5.0, 5.0, 0.0 )
    glUniform4f( LIGTH_LOCATIONS['Light_specular'], 0.7,0.7,0.7, 1.0 )
    
    # Define materiais
    glUniform4f( LIGTH_LOCATIONS['Material_ambient'], .1,.1,.1, 1.0 )
    glUniform4f( LIGTH_LOCATIONS['Material_diffuse'], 0.1,0.1,0.9, 1 )
    glUniform4f( LIGTH_LOCATIONS['Material_specular'], 0.9,0.9,0.9, 1 )
    glUniform1f( LIGTH_LOCATIONS['Material_shininess'], 0.6*128.0 )
    
    
    ## Para usar material exportado pelo Blender
    #glUniform4fv( LIGTH_LOCATIONS['Material_ambient'], 1, roda.materials['Material'].ambient)
    #glUniform4fv( LIGTH_LOCATIONS['Material_diffuse'], 1, roda.materials['Material'].diffuse)
    #glUniform4fv( LIGTH_LOCATIONS['Material_specular'], 1, roda.materials['Material'].specular)
    #glUniform1f( LIGTH_LOCATIONS['Material_shininess'], roda.materials['Material'].shininess)
    
    
    # liga VBO do carro
    vbo_carro.bind()
    
    # Ativa desenho por array
    glEnableClientState(GL_VERTEX_ARRAY)
    glEnableClientState(GL_NORMAL_ARRAY)
    
    # Passa ponteiros com vértices e normais
    glVertexPointer(3, GL_FLOAT, 24, vbo_carro+12)
    glNormalPointer(GL_FLOAT, 24, vbo_carro)
    glDrawArrays(GL_TRIANGLES, 0, vertices.shape[0])
    vbo_carro.unbind()
    
    glPopMatrix()    
    
    # Ativa textura e liga textura já carregada
    glActiveTexture(GL_TEXTURE0)
    glEnable(GL_TEXTURE_2D)
    glBindTexture(GL_TEXTURE_2D, pneu_ID)
    glUniform1f( TEX_LOCATIONS['tex'], 0 )
    
    # Desenha roda passando vértices, normais e mapeamento de textura
    vbo_roda.bind()
    glEnableClientState(GL_TEXTURE_COORD_ARRAY)
    glVertexPointer(3, GL_FLOAT, 32, vbo_roda+20)
    glNormalPointer(GL_FLOAT, 32, vbo_roda+8)
    glTexCoordPointer(2, GL_FLOAT, 32, vbo_roda)
    
    glUniform4f( LIGTH_LOCATIONS['Material_diffuse'], 0.1,0.1,0.1, 1 )
                
    glPushMatrix()
    #glColor3f(1.0, 0.1, 0.1)
    glTranslatef(1.2, 1.0, 3.0)
    #glRotatef(T2, 1.0, 0.0, 0.0)
    #visualization.draw(roda)
    glDrawArrays(GL_TRIANGLES, 0, vertices.shape[0])
    glPopMatrix()

    glPushMatrix()
    #glColor3f(1.0, 0.1, 0.1)
    glTranslatef(-1.2, 1.0, 3.0)
    #glRotatef(T2, 1.0, 0.0, 0.0)
    #visualization.draw(roda)
    glDrawArrays(GL_TRIANGLES, 0, vertices.shape[0])
    glPopMatrix()

    glPushMatrix()
    #glColor3f(1.0, 0.1, 0.1)
    glTranslatef(1.2, 1.0, -3.0)
    glRotatef(T, 0.0, 1.0, 0.0)
    #glRotatef(T2, 1.0, 0.0, 0.0)
    #visualization.draw(roda)
    glDrawArrays(GL_TRIANGLES, 0, vertices.shape[0])
    glPopMatrix()

    glPushMatrix()
    #glColor3f(1.0, 0.1, 0.1)
    glTranslatef(-1.2, 1.0, -3.0)
    glRotatef(T, 0.0, 1.0, 0.0)
    #glRotatef(T2, 1.0, 0.0, 0.0)
    #visualization.draw(roda)
    glDrawArrays(GL_TRIANGLES, 0, vertices.shape[0])
    glPopMatrix()
    
    # Desliga desenho por array
    glDisableClientState(GL_VERTEX_ARRAY)
    glDisableClientState(GL_NORMAL_ARRAY)
    glDisableClientState(GL_TEXTURE_COORD_ARRAY)

    glPopMatrix()
    
    vbo_roda.unbind()
    
    
    glUseProgram(0)
    
    glBegin(GL_LINES)
    glColor3f(1.0, 0.0, 0.0)
    glVertex3f(0.0, 0.0, 0.0)
    glVertex3f(10.0, 0.0, 0.0)
    glEnd()
    
    glBegin(GL_LINES)
    glColor3f(0.0, 1.0, 0.0)
    glVertex3f(0.0, 0.0, 0.0)
    glVertex3f(0.0, 10.0, 0.0)
    glEnd()
    
    glBegin(GL_LINES)
    glColor3f(0.0, 0.0, 1.0)
    glVertex3f(0.0, 0.0, 0.0)
    glVertex3f(0.0, 0.0, 10.0)
    glEnd()
    
    glPushMatrix()
    glTranslatef(-5.0, 5.0, 0.0)
    glutSolidSphere(0.8, 8, 8)
    glPopMatrix()
  


    glutSwapBuffers()
    
def Keys(key, x, y):
    global T
    global T2
    global T3
    
    if(key == GLUT_KEY_LEFT ): 
        T -= 1 
    elif(key == GLUT_KEY_RIGHT ): 
        T += 1 
    elif(key == GLUT_KEY_UP ): 
        T2 -= 1
    elif(key == GLUT_KEY_DOWN ): 
        T2 += 1 
    elif(key == GLUT_KEY_PAGE_UP ): 
        T3 -= 1 
    elif(key == GLUT_KEY_PAGE_DOWN ): 
        T3 += 1         
       
def animacao(value):
    glutPostRedisplay()
    glutTimerFunc(30, animacao,1)
    
    
def resize(w, h):
    glViewport(0, 0, w, h)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(45.0, w/h, 1.0, 100.0)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()
    gluLookAt(15.0, 12.0, 14.0,
                0.0, 3.0, 0.0,
                0.0, 1.0, 0.0)

  

def init():
    glClearColor (0.3, 0.3, 0.3, 0.0)
    glShadeModel( GL_SMOOTH )
    glClearColor( 0.0, 0.0, 0.0, 1.0 )
    glClearDepth( 1.0 )
    glEnable( GL_DEPTH_TEST )
    glDepthFunc( GL_LEQUAL )
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST )

    glDepthFunc( GL_LEQUAL )
    glEnable( GL_DEPTH_TEST )
    
    # Compila shaders
    vertexShader = shaders.compileShader(open('carro.vert', 'r').read(), GL_VERTEX_SHADER)
    fragmentShader = shaders.compileShader(open('carro.frag', 'r').read(), GL_FRAGMENT_SHADER)
    
    # Cria programa
    global carro_shader
    carro_shader = glCreateProgram()
    glAttachShader(carro_shader, vertexShader)
    glAttachShader(carro_shader, fragmentShader)
    glLinkProgram(carro_shader)
    
    # Cria variáveis nos shaders
    global LIGTH_LOCATIONS
    LIGTH_LOCATIONS = {
        'Global_ambient': glGetUniformLocation( carro_shader, 'Global_ambient' ),
        'Light_ambient': glGetUniformLocation( carro_shader, 'Light_ambient' ),
        'Light_diffuse': glGetUniformLocation( carro_shader, 'Light_diffuse' ),
        'Light_location': glGetUniformLocation( carro_shader, 'Light_location' ),
        'Light_specular': glGetUniformLocation( carro_shader, 'Light_specular' ),
        'Material_ambient': glGetUniformLocation( carro_shader, 'Material_ambient' ),
        'Material_diffuse': glGetUniformLocation( carro_shader, 'Material_diffuse' ),
        'Material_shininess': glGetUniformLocation( carro_shader, 'Material_shininess' ),
        'Material_specular': glGetUniformLocation( carro_shader, 'Material_specular' ),
    }
    
    global ATTR_LOCATIONS
    ATTR_LOCATIONS = {
        'Vertex_position': glGetAttribLocation( carro_shader, 'Vertex_position' ),
        'Vertex_normal': glGetAttribLocation( carro_shader, 'Vertex_normal' )
    }
    
    # Configura como de textura
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND)

    # Cria textura e liga ao pneu_ID
    global pneu_ID
    pneu_img = Image.open('textura_roda2.obj.png')
    #pneu_img = pneu_img.resize((1024,1024), resample=Image.LANCZOS)
    w, h, pneu_img = pneu_img.size[0], pneu_img.size[1], pneu_img.tobytes("raw", "RGB", 0, -1)
    pneu_ID = glGenTextures(1)
    glBindTexture(GL_TEXTURE_2D, pneu_ID)
    
    # Configura Mipmap e antialiasing
    #glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pneu_img)
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, pneu_img)
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST)
    #glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR)
    
    global TEX_LOCATIONS
    TEX_LOCATIONS = {
        'tex': glGetAttribLocation( carro_shader, 'tex' )
    }

    

glutInit()
glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB)
glutInitWindowSize(1920, 1080)
glutInitWindowPosition(100, 100)
wind = glutCreateWindow("Cubo")
init()
roda = pywavefront.Wavefront("roda2.obj")
carro = pywavefront.Wavefront("carro.obj")
glutDisplayFunc(display)
glutReshapeFunc(resize)
glutTimerFunc(30,animacao,1)
glutSpecialFunc(Keys)
glutMainLoop()
