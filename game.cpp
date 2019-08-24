#include<iostream>
#include<cmath>
#include<vector>
#include<math.h>
#include<GL/glut.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<math.h>

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
#define TIMER_ID 0
#define TIMER_ID1 1
#define TIMER_INTERVAL 20

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_mouse(int button, int state, int x, int y);
static void on_motion(int x, int y);
static void pomeranjeOblaka(int value);
static void pomeranjeMetaka(int value);
void pocetneVrednosti();
void nacrtajPostolje(void);
void nacrtajElipsu(double centerX, double centerY, double radiusX, double radiusY);
void nacrtajOblak();
void nacrtajTop();
void nacrtajMuniciju();

static int window_width, window_height;
static std::vector<double> xKoordinateOblaka(1000), yKoordinateOblaka(1000);
static std::vector<double> brzinaOblakaPoX(1000), brzinaOblakaPoY(1000);
static std::vector<int> bojaOblaka(1000);
static std::vector<int> metkovi(1000);
static std::vector<double> yKoordinataMunicije(1000), zKoordinataMunicije(1000);
static int brojacMetkova;
static double pocetnaXMunicije, pocetnaYMunicije, pocetnaZMunicije;
static int kretanjeOblaka;
static int mouse_x, mouse_y; /* Koordinate misa. */
static float matrix[16];/*kumulativna matrica rotacije*/
static double pomerajPoX, rotacijaUgla;
static int pritisnutoPucanje, drugiPut;

int main(int argc, char** argv){
    
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(700, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(argv[0]);
    
    pocetneVrednosti();
    
    glutKeyboardFunc(on_keyboard);
    glutMouseFunc(on_mouse);
    glutMotionFunc(on_motion);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(0.75, 0.85, 0.85, 0);
    glEnable(GL_DEPTH_TEST);
    
    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    
}

void pocetneVrednosti(){
    pomerajPoX = 0.0;
    rotacijaUgla = 45;
    kretanjeOblaka = 0;
    pritisnutoPucanje = 0;
    drugiPut = 0;
    brojacMetkova = 0;
    
    double pocetnaBrzinaPoX = 0.03;
    double pocetnaBrzinaPoY = 0.003;
    
    pocetnaYMunicije = -0.5;
    pocetnaZMunicije = 1.1;

    srand(time(NULL));
    for(int i = 0; i<100; i++){
        
        yKoordinateOblaka.at(i) = 0.7 + i;
        double xKoordinata = rand()/(float)RAND_MAX;
    
        double randomBroj = rand()/(float)RAND_MAX;
        if(randomBroj < 0.5){
            xKoordinateOblaka.at(i) = -1*xKoordinata;
        } else {
            xKoordinateOblaka.at(i) = xKoordinata;
        }
        double smerKretanja = rand()/(float)RAND_MAX;
        if(smerKretanja < 0.5){
            brzinaOblakaPoX.at(i) = pocetnaBrzinaPoX;
        } else {
            brzinaOblakaPoX.at(i) = -pocetnaBrzinaPoX;
        }
        brzinaOblakaPoY.at(i) = pocetnaBrzinaPoY;
        if(i!=0 && i%3 == 0){
            pocetnaBrzinaPoX += 0.01;
            pocetnaBrzinaPoY += 0.001;
        }
        double indikatorZaBoju = rand()/(float)RAND_MAX;
        if(indikatorZaBoju < 0.5)
            bojaOblaka.at(i) = 1;
        else
            bojaOblaka.at(i) = 0;
        
        metkovi.at(i) = 0;
        yKoordinataMunicije.at(i) = pocetnaYMunicije;
        zKoordinataMunicije.at(i) = pocetnaZMunicije;
    
    }
    
    /*inicijalizacija matrice rotacije*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}

static void on_mouse(int button, int state, int x, int y){
    /*cuvaju se pozicije misa */
    mouse_x = x;
    mouse_y = y; 
}

static void on_motion(int x, int y){
    /*promene pozicije misa, izracunavanje te promene
     i cuvanje novih pozicija*/
    int deltaX, deltaY;
    
    deltaX = x - mouse_x;
    deltaY = y - mouse_y;
    mouse_x = x;
    mouse_y = y;
    
    /*izracunavanje nove matrice rotacije */
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glLoadIdentity();
        
        if(pomerajPoX <= 1.8 && pomerajPoX >=-1.8)
            pomerajPoX += deltaX * 0.005;
        else if(pomerajPoX > 1.8)
            pomerajPoX = 1.8;
        else
            pomerajPoX = -1.8;
        if(rotacijaUgla <= 90 && rotacijaUgla >= 30)
            rotacijaUgla += deltaY;
        else if(rotacijaUgla > 90)
            rotacijaUgla = 90;
        else 
            rotacijaUgla = 30;
        
        glRotatef(pomerajPoX, 1, 0, 0);
        glMultMatrixf(matrix);        
    glPopMatrix();
    glutPostRedisplay();
}

static void on_reshape(int width, int height){
    window_width = width;
    window_height = height;
}

static void on_keyboard(unsigned char key, int x, int y){
    
    switch (key) {
    case 27:
        exit(0);
        break;        
    case 's':
    case 'S':
        if (!kretanjeOblaka) {
            glutTimerFunc(TIMER_INTERVAL, pomeranjeOblaka, TIMER_ID);
            kretanjeOblaka = 1;
        }
        break;
    case 'p':
    case 'P':
        pocetnaXMunicije = pomerajPoX;
        brojacMetkova += 1;
        pritisnutoPucanje = 1;
        glutTimerFunc(TIMER_INTERVAL, pomeranjeMetaka, TIMER_ID1);
        break;
    }

}


static void on_display(void){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /* Podesava se vidna tacka. */
    glViewport(0, 0, window_width, window_height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, (float) window_width / window_height, 1, 100);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 7, 
              0, 0, 0, 
              0, 1, 0);
    
    glEnable(GL_DEPTH_TEST);
    nacrtajOblak();
    nacrtajPostolje();
    
    glMultMatrixf(matrix);
    nacrtajTop();
    nacrtajMuniciju();
    glutSwapBuffers();
}

static void pomeranjeOblaka(int value){

    if (value != TIMER_ID)
        return;
    unsigned int i = 0;
    for(i = 0; i<100; i++){
        xKoordinateOblaka.at(i) += brzinaOblakaPoX.at(i);
        yKoordinateOblaka.at(i) -= brzinaOblakaPoY.at(i);
        if(xKoordinateOblaka.at(i) + 0.2 >= 2 || xKoordinateOblaka.at(i) - 0.2 <= -2)
            brzinaOblakaPoX.at(i) *= -1;
    }
    glutPostRedisplay();
    
    if (kretanjeOblaka) {
        glutTimerFunc(TIMER_INTERVAL, pomeranjeOblaka, TIMER_ID);
    }
}


static void pomeranjeMetaka(int value){

    if (value != TIMER_ID1)
        return;
    metkovi.at(brojacMetkova-1) = 1;
    for(int i = 0; i<brojacMetkova - 1; i++)
        metkovi.at(i) = 0;
    for(int i = 0; i<brojacMetkova; i++){
        
        yKoordinataMunicije.at(i) += 0.05;
        zKoordinataMunicije.at(i) -= 0.05;
    }
    glutPostRedisplay();

    if (metkovi.at(brojacMetkova-1) ) {
        glutTimerFunc(TIMER_INTERVAL*brojacMetkova, pomeranjeMetaka, TIMER_ID1);
    }
}


void nacrtajPostolje(){
    glDisable(GL_LIGHTING);
    GLfloat pozicija_osvetljenja[] = { 1, 10, 8, 1 };

    GLfloat ambijentalno_osvetljenje[] = { 0.35, 0.35, 0.35, 1 };
    GLfloat difuzno_osvetljenje[] = { 0.75, 0.75, 0.75, 1 };
    GLfloat spekularno_osvetljenje[] = { 0.9, 0.9, 0.9, 1 };
    
    
    GLfloat ambijentalni_materijal[] = { 0.0, 1.0, 0.0, 1 };
    GLfloat difuzni_materijal[] = { 0.0, 0.6, 0.0, 1 };
    GLfloat spekularni_materijal[] = { 1, 1, 1, 1 };
    GLfloat shininess = 40;

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, pozicija_osvetljenja);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambijentalno_osvetljenje);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difuzno_osvetljenje);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spekularno_osvetljenje);

    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambijentalni_materijal);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difuzni_materijal);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spekularni_materijal);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    glPushMatrix();
            glTranslatef(0, -1.4, 1);
            glScalef(3.5, 0.5, 1);
            glutSolidCube(1);
    glPopMatrix();
    
}

void nacrtajElipsu(double centerX, double centerY, double radiusX, double radiusY){
    const float DEG2RAD = 3.14159 / 180;
    glBegin(GL_TRIANGLE_FAN);
        glVertex2d (centerX, centerY);
        for (int i = -1; i < 360; i++) {
            float degInRad = i * DEG2RAD;
            glVertex2d (centerX + cos (degInRad) * radiusX*0.3, 
                            centerY + sin (degInRad) * radiusY*0.3);
        }
        
    glEnd();
}


void nacrtajOblak(){
    
    glDisable(GL_LIGHTING);
    
    for(int i = 0; i<100; i++){
        double x = xKoordinateOblaka.at(i);
        double y = yKoordinateOblaka.at(i);
        glColor3f(bojaOblaka[i], bojaOblaka[i], bojaOblaka[i]);
        nacrtajElipsu(x, y, 1.2, 0.7);
        glColor3f(bojaOblaka[i], bojaOblaka[i], bojaOblaka[i]);
        nacrtajElipsu(x+0.23, y+0.12, 0.35, 0.35);
        glColor3f(bojaOblaka[i], bojaOblaka[i], bojaOblaka[i]);
        nacrtajElipsu(x+0.23, y-0.12, 0.35, 0.35);
        glColor3f(bojaOblaka[i], bojaOblaka[i], bojaOblaka[i]);
        nacrtajElipsu(x-0.23, y-0.12, 0.35, 0.35);
        glColor3f(bojaOblaka[i], bojaOblaka[i], bojaOblaka[i]);
        nacrtajElipsu(x-0.23, y+0.12, 0.35, 0.35);
    }
}

void nacrtajTop(){
        
    GLfloat pozicija_osvetljenja[] = { 1, 10, 8, 1 };

    GLfloat ambijentalno_osvetljenje[] = { 0.35, 0.35, 0.35, 1 };
    GLfloat difuzno_osvetljenje[] = { 0.75, 0.75, 0.75, 1 };
    GLfloat spekularno_osvetljenje[] = { 0.9, 0.9, 0.9, 1 };
    
    
    GLfloat ambijentalni_materijal[] = { 0.5, 0.5, 0.5, 1 };
    GLfloat difuzni_materijal[] = { 0.6, 0.6, 0.6, 1 };
    GLfloat spekularni_materijal[] = { 1, 1, 1, 1 };
    GLfloat shininess = 40;

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, pozicija_osvetljenja);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambijentalno_osvetljenje);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difuzno_osvetljenje);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spekularno_osvetljenje);

    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambijentalni_materijal);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difuzni_materijal);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spekularni_materijal);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glPushMatrix();
        GLUquadricObj *quadratic;
        quadratic = gluNewQuadric();
        glTranslatef(pomerajPoX, -0.5, 1.15);
        
        glRotatef(rotacijaUgla, 1, 0, 0);
        
        
        gluCylinder(quadratic, 0.1f, 0.1f, 0.7f, 72, 72);
    glPopMatrix();        
    
    glPushMatrix();
        GLUquadricObj *quadratic1;
        quadratic1 = gluNewQuadric();
        glRotatef(90, 0, 1, 0);
        glTranslatef(-1.4, -0.95, pomerajPoX+0.1);
        
        gluCylinder(quadratic1, 0.2f, 0.2f, 0.1f, 72, 72);

        glTranslatef(0, 0, -0.3);
        gluCylinder(quadratic1, 0.2f, 0.2f, 0.1f, 72, 72);
    glPopMatrix();
}


void nacrtajMuniciju(){
    glDisable(GL_LIGHTING);
    GLfloat pozicija_osvetljenja[] = { 1, 10, 8, 1 };

    GLfloat ambijentalno_osvetljenje[] = { 0.35, 0.35, 0.35, 1 };
    GLfloat difuzno_osvetljenje[] = { 0.75, 0.75, 0.75, 1 };
    GLfloat spekularno_osvetljenje[] = { 0.9, 0.9, 0.9, 1 };
    
    
    GLfloat ambijentalni_materijal[] = { 0, 0, 0, 1 };
    GLfloat difuzni_materijal[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat spekularni_materijal[] = { 1, 1, 1, 1 };
    GLfloat shininess = 40;

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, pozicija_osvetljenja);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambijentalno_osvetljenje);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difuzno_osvetljenje);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spekularno_osvetljenje);

    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambijentalni_materijal);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difuzni_materijal);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spekularni_materijal);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    
    for(int i = 0; i<brojacMetkova; i++){
        glPushMatrix();
        glTranslatef(pocetnaXMunicije, yKoordinataMunicije.at(i),                      
                    zKoordinataMunicije.at(i));
        glutSolidSphere(0.12, 500, 500);
        glPopMatrix();
    }
    
        
}