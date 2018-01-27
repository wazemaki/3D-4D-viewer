#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <cmath>
#include "h/matrix.h"
#include "h/vec.h"
#include "h/camera.h"
#include "h/h_torus.h"
#include "h/cube.h"
#include "h/tesseract.h"
#include "h/transformations.h"
#include "h/transformations_4d.h"
#include "h/bodyGroup.h"

#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 601

using namespace std;

camera cam(
    vec({0.0f,0.0f,0.0f}), // ahova nezunk
    vec({0.0f,0.0f,1.0f}) // merre van a felfele
    );
    
//htorus t(.5f, 2.0f, 30, .8, .1, .1);

tesseract c1(1.0f, .8, .2, .2); // piros
//cube c2(1.0f, .8, .8, .1); // sarga

body* activeB = &c1; // aktiv body amire raengedjuk kesobb a forgatasokat

bodyGroup bgroup({&c1}, DRAWMODE_LINES_POINTS, true);
// bele kell pakolni a testeket, ez az objektum gyujti ossze es rajzolja ki a testek pontjait,eleit,lapjait

//vec light({4,4,4,1});


void display(void); 
void update(int q);

string info; // info szoveg

 /* ---  billentyuk, eger kezelese ---  */

void mouseButton( GLint button, GLint action, GLint xMouse , GLint yMouse );
void mouseMove(GLint xMouse, GLint yMouse);

vec mouse(2); // eger helyzete
GLint keyStates[256]; // egyes billentyuk statuszai

enum useractions{ uact_none, uact_rotate };
useractions uact = uact_none;


void keyDown (unsigned char key, int x, int y) {
    if(key == 27) exit( 0 ); // ESC
    keyStates[key] = 1;
}

void keyUp (unsigned char key, int x, int y) {
    keyStates[key] = 0;
}

void keyboard() {
    string infTxt = "";
    if(keyStates['s']) { 
        activeB->addTransformation(rotate4d(.02f,0,0,0,0,0)); // xy
        infTxt += "XY ";
    }
    if(keyStates['w']) { 
        activeB->addTransformation(rotate4d(-.02f,0,0,0,0,0)); // xy
        infTxt += "-XY ";
    }
    if(keyStates['d']) { 
        activeB->addTransformation(rotate4d(0,.02f,0,0,0,0)); // xz
        infTxt += "XZ ";
    }
    if(keyStates['e']) { 
        activeB->addTransformation(rotate4d(0,-.02f,0,0,0,0)); // xz
        infTxt += "-XZ ";
    }
    if(keyStates['g']) { 
        activeB->addTransformation(rotate4d(0,0,.02f,0,0,0)); // xt
        infTxt += "XT ";
    }
    if(keyStates['t']) { 
        activeB->addTransformation(rotate4d(0,0,-.02f,0,0,0)); // xt
        infTxt += "-XT ";
    }
    if(keyStates['f']) { 
        activeB->addTransformation(rotate4d(0,0,0,.02f,0,0)); // yz
        infTxt += "YZ ";
    }
    if(keyStates['r']) { 
        activeB->addTransformation(rotate4d(0,0,0,-.02f,0,0)); // yz
        infTxt += "-YZ ";
    }
    if(keyStates['h']) { 
        activeB->addTransformation(rotate4d(0,0,0,0,.02f,0)); // yt
        infTxt += "YT ";
    }
    if(keyStates['z']) { 
        activeB->addTransformation(rotate4d(0,0,0,0,-.02f,0)); // yt
        infTxt += "-YT ";
    }
    if(keyStates['j']) { 
        activeB->addTransformation(rotate4d(0,0,0,0,0,.02f)); // zt
        infTxt += "ZT ";
    }
    if(keyStates['u']) { 
        activeB->addTransformation(rotate4d(0,0,0,0,0,-.02f)); // zt
        infTxt += "-ZT ";
    }
    
    info = infTxt;
}

// eger mozgasara lefuto esemeny
void mouseMove(GLint xMouse, GLint yMouse){
    vec d = vec({(float)xMouse,(float)yMouse}) - mouse;
    if( d.length() > 10 || uact == uact_rotate ){        
        uact = uact_rotate;
        cam.rotate(d.get(vec_coord_x));
        cam.elevate(d.get(vec_coord_y) * 2);
        cam.refresh();
        mouse.set({(float)xMouse,(float)yMouse});
    }
}

// eger akcioinak kezelese
void mouseButton( GLint button, GLint action, GLint xMouse , GLint yMouse ){
    
 //   vec camC;
    
    switch(action){
        case 0: // lenyom
            switch (button){
                case 0: // bal gomb
                    mouse.set({(float)xMouse,(float)yMouse});
                    break;
                    
                    // testek kijelolese, ez most nem kell
//                case 2: // jobb gomb
//                    mouse.set({(float)xMouse,(float)yMouse});
//                    if(bgroup.activeBody != NULL){
//                        camC.set({
//                            (float)(xMouse - WINDOW_WIDTH / 2) / 50,
//                            (float)(WINDOW_HEIGHT - yMouse - WINDOW_HEIGHT / 2) / 50,
//                            -12.0f,
//                            1.0f
//                        }); 
//                      //  pr = cam.projectionInverse(camC);
////                        bgroup.activeBody->Transformation(eltol(pr.get(0),pr.get(1),pr.get(2)));
//                      //  bgroup.activeBody = NULL;
//                    }
//                    break;
                    
                case 3: // gorgo elore
                    if( cam.R > 4 ) cam.zoom(-1.0f);
                    cam.refresh();
                    break;
                case 4: // gorgo hatra
                    if( cam.R < 100 ) cam.zoom(1.0f);
                    cam.refresh();
                    break;
            }
            break;
        case 1: // felenged
            if(button == 0 && uact == uact_none){
            //    bgroup.searchBody(xMouse,WINDOW_HEIGHT - yMouse);
            }
            uact = uact_none;
    }
}

 /* --- / billentyuk, eger kezelese ---  */

// informacio kiirasa a kepernyore
void writeInfo(){
    glColor3f(1,0,0);

    glPushMatrix();
    glLoadIdentity();

    glMatrixMode( GL_MODELVIEW );

    glLoadIdentity();

    glRasterPos2i(10,10);
    for ( int i = 0; i < info.length(); ++i ){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, info[i]);
    }

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
}

// a glut redisplay fgv-e
void display(void){
    glClear( GL_COLOR_BUFFER_BIT );  // Minden letorlese a kepernyorol
    bgroup.draw();                   // bodygroup minden elemenek kirajzolasa
    writeInfo();                     // info szovek kiirasa (ha van)
    
    glutSwapBuffers( );
}

// minden ciklusban lefuto update fgv.
void update( int q ){ 
    keyboard(); // nyomva tartott billentyuk elkapasa

    //if(bgroup.activeBody != NULL){
    //    bgroup.activeBody->addTransformation(forgat(.00f,.00f,.05f));
    //    bgroup.activeBody->Transformation();
    //}
    
    bgroup.transformation(); // transzformacios matrixok alkalmazasa minden egyes testre

    bgroup.cameraTransformation(&cam); // kameratranformacio raengedese minden egyes testre
    
    glutPostRedisplay( ); // kirajzolas engedelyezese (display)
    glutTimerFunc( 30, update, 0); // kov update fgv beallitasa
}

// testek kezdeti ertekeinek beallitasa
void init3d(){
    
    cam.initWtvMatrix(      // kamera window-to-viewport matrixanak beallitasa (ez nem valtozik)
        vec({0,0}), // window center
        vec({10,10}), // window size
        vec({WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}), // view center
        vec({1000,1000})); // view size
    
    cam.refresh();      // kamera transzformacios matrix feltoltese...
    
    /* kockak kezdeti helye */
    
    //c1.Transformation();
    bgroup.transformation();
    
 //   bgroup.light = &light;
    
}

/* A GLUT beallitasa */
void init(){
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);   // Set display mode.
    glutInitWindowPosition (50, 100);   // Set top-left display-window position.
    glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);      // Set display-window width and height.
    glutCreateWindow ("4D to 3D to 2D"); // Create display window.
    
    glClearColor (.7,.7,.7, 0.0);	// Set display-window color
    glMatrixMode (GL_PROJECTION);		// Set projection parameters.
    glShadeModel ( GL_FLAT );
    gluOrtho2D (0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(10);
    glutTimerFunc( 30, update, 0 );
    glutMouseFunc( mouseButton );
    glutMotionFunc( mouseMove );
    
    glutKeyboardFunc( keyDown );
    glutKeyboardUpFunc( keyUp );
    
    glutDisplayFunc( display );
}

int main (int argc, char** argv){
    glutInit (&argc, argv);  // Initialize GLUT.
    init(); // a glut beallitasa

    init3d(); // testek kezdeti ertekenek beallitasa
    
    glutMainLoop ( ); // Display everything and wait.
    return 0;
}

