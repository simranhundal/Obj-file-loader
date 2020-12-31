// Simran Hundal
// 109807418
// CSCI 172
// Project 5


#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>

#include <SOIL.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

bool WireFrame= false;

GLuint cubetex; //texture pointers

//values for zoom and rotation
int rotx = 0;
int roty = 0;
float zoom=1;


//custom definition for storing object file variables

typedef struct Texture
{
    float X;
    float Y;
} Objtex;
typedef struct Vertex
{
    float X;
    float Y;
    float Z;
} ObjVertex;
typedef struct Face
{
    int V1;
    int V2;
    int V3;

    int T1;
    int T2;
    int T3;

    int N1;
    int N2;
    int N3;

} Face;


//declaring custom struct's for obj variables
//setting length at 150 for bigger obj files
Vertex objvert[9];
Vertex objnorm[7];
Texture objtex[5];
Face objface[13];



const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat filevalgh_sfilevalniness[] = { 100.0f };



/* GLUT callback Handlers */
static void resize(int width, int height)
{
    double Ratio;

    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
        glViewport((GLsizei) (width-height)/2,0,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (50.0f,1,0.1f, 100.0f);
}



static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -5);
    gluLookAt(0,0,3,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // your code here
    glBindTexture(GL_TEXTURE_2D, cubetex);
    //rotating based on rot value
    glRotatef(rotx,1,0,0);
    glRotatef(roty,0,1,0);



    glPushMatrix();
    //changing zoom after rotation is done
    glTranslated(0,0,zoom);


    glPushMatrix ();


    glBegin(GL_TRIANGLES);

    //looping through the faces to draw object with triangles
    for (int i = 0; i < 13; i++)
    {

        //forming normal vertex and tex cordinates based on face values
        //sub 1 since face index starts at 1 while c++ starts at 0
        glNormal3f(objnorm[(objface[i].N1) -1].X, objnorm[(objface[i].N1)-1].Y, objnorm[(objface[i].N1)-1].Z);
        glTexCoord2f(objtex[(objface[i].T1)-1].X, objtex[(objface[i].T1) -1].Y);
        glVertex3f(objvert[(objface[i].V1) -1].X, objvert[(objface[i].V1)-1].Y, objvert[(objface[i].V1)-1].Z);


        glNormal3f(objnorm[(objface[i].N2) -1].X, objnorm[(objface[i].N2)-1].Y, objnorm[(objface[i].N2)-1].Z);
        glTexCoord2f(objtex[(objface[i].T2)-1].X, objtex[(objface[i].T2) -1].Y);
        glVertex3f(objvert[(objface[i].V2) -1].X, objvert[(objface[i].V2)-1].Y, objvert[(objface[i].V2)-1].Z);

        glNormal3f(objnorm[(objface[i].N3) -1].X, objnorm[(objface[i].N3)-1].Y, objnorm[(objface[i].N3)-1].Z);
        glTexCoord2f(objtex[(objface[i].T3)-1].X, objtex[(objface[i].T3) -1].Y);
        glVertex3f(objvert[(objface[i].V3) -1].X, objvert[(objface[i].V3)-1].Y, objvert[(objface[i].V3)-1].Z);

    }

    glEnd();
    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{


    switch (key)
    {
    case 27 :
    case 'q':
        exit(0);
        break;
    // changing wireframe
    case 'w':
        WireFrame = !WireFrame;
        break;
        break;

    }
}



void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    //changing relevant zoom and rot values based on key presses
    case GLUT_KEY_DOWN:
        rotx += 5;

        break;
    case GLUT_KEY_UP:
        rotx -= 5;

        break;
    case GLUT_KEY_LEFT:
        roty -= 5;

        break;
    case GLUT_KEY_RIGHT:
        roty += 5;

        break;

    case GLUT_KEY_HOME:
        zoom -= 1;

        break;
    case GLUT_KEY_END:
        zoom += 1;

        break;


    }
    glutPostRedisplay();
}



static void idle(void)
{
    glutPostRedisplay();
}
// function to set texture based on image file name
void setTexture(string name, GLuint &tex)
{
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, tex); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
    int width, height; // width & height for the image reader
    unsigned char* image;
    string filename = "images/" + name + ".png";
    image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (width), height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // binding image data
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
void loadobj(string name)
{
//loading obj file into global vectors
    FILE * file = fopen("Cube-1.obj", "r");
    //setting indexes for file variables
        int indv = 0;
        int indn = 0;
        int indt = 0;
        int indf = 0;
		while(true){

		//reading the 1st val of the line
		char fstval[1];
		int line = fscanf(file, "%s", fstval);

		//exit loop when there are no more lines left
		if (line == EOF){
			break;
		}
		//when there is vertex
		if ( strcmp( fstval , "v" ) == 0 ){
			float vertexobj[3];
			fscanf(file, "%f %f %f\n", &vertexobj[0], &vertexobj[1], &vertexobj[2]);

			objvert[indv].X = vertexobj[0];
			objvert[indv].Y = vertexobj[1];
			objvert[indv].Z = vertexobj[2];

			cout << "V " << objvert[indv].X << " " << objvert[indv].Y << " " << objvert[indv].Z<< endl;
			indv++;

		}
		//for textures
		else if ( strcmp( fstval, "vt" ) == 0 ){
            float texobj[2];
			fscanf(file, "%f,%f\n", &texobj[0], &texobj[1]);

			objtex[indt].X = texobj[0];
			objtex[indt].Y = texobj[1];

			cout << "T " << objtex[indt].X << " " << objtex[indt].Y << " " << endl;
			indt++;
		}
		//for normals
		else if ( strcmp( fstval, "vn" ) == 0 ){
            float normobj[3];
			fscanf(file, "%f %f %f\n", &normobj[0], &normobj[1], &normobj[2]);

			objnorm[indn].X = normobj[0];
			objnorm[indn].Y = normobj[1];
			objnorm[indn].Z = normobj[2];

			cout << "N " << objnorm[indn].X << " " << objnorm[indn].Y << " " << objnorm[indn].Z<< endl;
			indn++;
		}
		//for faces
		else if ( strcmp( fstval, "f" ) == 0 ){
			int vindex[3];
			int texIndex[3];
            int norIndex[3];
			fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vindex[0], &texIndex[0], &norIndex[0], &vindex[1], &texIndex[1], &norIndex[1], &vindex[2], &texIndex[2], &norIndex[2] );

			objface[indf].V1 = vindex[0];
			objface[indf].V2 = vindex[1];
			objface[indf].V3 = vindex[2];

			objface[indf].T1 = texIndex[0];
			objface[indf].T2 = texIndex[1];
			objface[indf].T3 = texIndex[2];

			objface[indf].N1 = norIndex[0];
			objface[indf].N2 = norIndex[1];
			objface[indf].N3 = norIndex[2];

            cout << "F " << objface[indf].V1 << "/" << objface[indf].T1 << "/" << objface[indf].N1 << " ";
            cout << objface[indf].V2 << "/" << objface[indf].T2 << "/" << objface[indf].N2 << " ";
            cout << objface[indf].V3 << "/" << objface[indf].T3 << "/" << objface[indf].N3 << "\n";
            indf++;
        }

    }
}

static void init(void)
{

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    //inilatalising texture values for sprite and background aswell as alpha for transparent images
    setTexture("cube", cubetex);
    loadobj("Cube-1.obj");
    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, filevalgh_sfilevalniness);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);



}


/* Program entry point */



int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(100,100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project 5");
    init();

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);


    glutMainLoop();

    return EXIT_SUCCESS;
}
