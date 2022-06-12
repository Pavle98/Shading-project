
#include <windows.h>
#include <GL/glut.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <conio.h>

static int spin = 0;
static int spin2 = 0;
static int obj = 1;
static int begining;

//klasika za 2d, x i y oznacavaju gde cemo da pisemo nesto
void
output(GLfloat x, GLfloat y, char *format ,...)
{
    //variable argument list
  va_list args;
  char buffer[200], *p;
  vsprintf(buffer, format , args);
  glPushMatrix();
  glTranslatef(x, y, 0);
  for (p = buffer; *p; p++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  glPopMatrix();
}



void
motion(int x, int y)
{
    //printf("%d, ", x);
    //spin je u pocetku 0, zato stavlja sunce na sredinu.
    //Ovu funkciju posle prosledjujemo glutMotionFunc koja se poziva kada se pritisne mis
    // i u principu prati pokret, tj povecava ili smanjuje X u odnosu na kretnju misa
  spin = (spin + (x - begining)) % 360;
  begining = x;


  //glutPostRedisplay() refreshuje

  glutPostRedisplay();
}

void
myinit(void)
{
   // glEnable and glDisable omogucavaju koriscenje razlicitih stvari

  glEnable(GL_LIGHT0);
glEnable(GL_LIGHT1);
glEnable(GL_LIGHT2);

//Omogucavamo baferovanje i uporedjivanje depht-a
  glEnable(GL_DEPTH_TEST);


}


 void keyPressed (unsigned char key, int x, int y) {
     obj = key - '0';
}


void
display(void)
{


//Menjamo poziciju svetla i time mozemo da uticemo kao na tip svetla
//If the w component of the position is 0, the light is treated as a directional source.
  GLfloat light_position[] =
  {1.0, 1.0, 1.5, 1.0};


  //glClear cisti bufere, color buffer se cisti da ne bi ostajali "tragovi" prethodnih kretnji,
  // a depth buffer koliko sam skapirao  ukoliko se ne cisti u svakom frejmu, on ce da poredi dubinu (ono sto smo gore enable-ovali)
  //sa nekim junkom koji se nalazi u bufferu i "Fragmenti" se nece iscrtati jer im je manja vrednost od tog junka (u nekim slucajevima)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode(GL_MODELVIEW);


  //glPushMatrix — push and pop the current matrix stack
  glPushMatrix();

 //Mnozi vrednost prethodne matrice sa datim merama, po x y z, znaci ovde smo samo podesili koliko duboko
 //hocemo da sve sto je nacrtano posle funkcije treba da ide
 //ovde koristimo za odnos objekat/osnova
  glTranslatef(0.0, 0.0, -5.0);


  glPushMatrix();

  //ovako dobijamo mousePos
   POINT mousePos;
GetCursorPos(&mousePos);

  //mnozimo trenutnu matricu (sunce) sa rotacionom matricom,
  //znaci pomeramo sunce u odnosu na objekat, po (uglu, x, y, z)
  glRotated((GLdouble) spin, mousePos.x, mousePos.y, 0.0);


    GLfloat red_light[] ={1.0, 0.0, 0.0, 1.0};

  GLfloat green_light[] =
{0.0, 1.0, 0.0, 1.0};
GLfloat white_light[] =
{1.0, 1.0, 1.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  //ovde koristimo translated za odnos sunce/objekat
  //1.5 je dubina
  glTranslated(0.0, 0.0, 1.5);
  //disableujemo gl_lightning da se ne menja boja sunca
  glDisable(GL_LIGHTING);
  //boja i velicina sunca
  glColor3f(1,1,0);
  glutWireSphere(0.1,20,20);
  glEnable(GL_LIGHTING);
  glPopMatrix();

  //menjanjeObjekata
    glutKeyboardFunc(keyPressed);
  switch (obj) {
  case 1:

    glLightfv(GL_LIGHT0, GL_AMBIENT, green_light);
   // glRotated((GLdouble) spin/3, mousePos.x/2, mousePos.y/2, 0.0);
    glutSolidTorus(0.275, 0.85, 20, 20);

    break;
  case 2:
      //mogucnost rotiranja objekta, iskljuciti liniju 119 radi boljeg pokazivanja efekta

      glRotated((GLdouble) spin, mousePos.x, mousePos.y, 0.0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, red_light);
    glutSolidTeapot(1.0);


    break;
  case 3:
        glLightfv(GL_LIGHT0, GL_AMBIENT, white_light);
    glutSolidSphere( 1,
                     15, 15);
    break;
  case 4:

    glutSolidIcosahedron();


    break;
  }

  glPopMatrix();
  //GL_DEPTH_TEST omogucavamo depth test unutar GL_ENABLE_BIT
  //sto nam omogucava da vidimo sunce u odnosu na objekat ispred ili iza njega
  //specifikacija objasnila dobro
  glPushAttrib(GL_ENABLE_BIT);
  //disableujemo zato sto ne zelimo da nam se menjaju boje
  //nad matricom gde pise ovo grafika projekat itd...
  glDisable(GL_LIGHTING);
  //postavlja mod trenutne matrice na steku
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  //crtamo na "pocetku" glLoadIdentity, u principu znaci da crtamo matricu
  //na prvom "papiru" pa mozemo da mrdamo sa glTranslate
  glLoadIdentity();
  //definisemo osobine 2d matrice
  gluOrtho2D(0, 3000, 0, 3000);
  glPushMatrix();
  output(80, 2800, "Shading project");
  output(80, 100, "Pavle Vukovic");
  //crtamo objekat
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
//crtamo sunce
  glPopMatrix();
  //dubina sunca, linija 169
  glPopAttrib();
  //U principu ova funkcija kaze da kada smo zavrsili sa frejmom, da zameni ono sto je komp radio u pozadini sa frontom.
  //Ukoliko to ne uradimo, sve ostaje pozadi i mi nista ne vidimo
  glutSwapBuffers();
}

void
myReshape(int width, int height)
{

  glMatrixMode(GL_PROJECTION);
  gluPerspective(42.0, (GLfloat) width / (GLfloat) height, 1.0, 20.0);
//gluPerspective — set up a perspective projection matrix
//prima i utice na tipa ugao gledanja po x y osi, dubini itd, perpektivu tacnije
}


int
main(int argc, char **argv)
{
  //  glutInit is used to initialize the GLUT library.
  glutInit(&argc, argv);
  //  glutInitDisplayMode sets the initial display mode.
  //double buffered, linija 187, rgb i depth jasno, da imamo depth i rgb bafere
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  //window size
  glutInitWindowSize(850, 500);
  //kreiramo prozor
  glutCreateWindow(argv[0]);
  //moji enableovi
  myinit();

  //linija 35, motion funkcija objasnjeno
  glutMotionFunc(motion);
  //  glutReshapeFunc sets the reshape callback for the current window.
  glutReshapeFunc(myReshape);

  glutDisplayFunc(display);

  glutMainLoop();
  return 0;
}
