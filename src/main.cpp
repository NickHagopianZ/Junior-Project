/*
 * This is part of the junior independent study for The College of Wooster's
 * compsci department
 * This project looks to create near optimal solution to the traveling salesman problem in reasonable runtime
 * created by Nicholas Hagopian-Zirkel
 * opengl and glut implementation is built off of code created by D. Byrnes
 */

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

#include "TSP.h"
#include <iostream>

using namespace std;

TSP *inst ; //TSP object
bool b=0;
int tsp;                                //Display window
const int spacer = 70;                  //Window spacing

//Initializes background color and drawing color
void init (void)
{
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glColor3f (1.0, 1.0, 1.0);
    glShadeModel (GL_FLAT);
}


//Reshape callback: Allows the window to be resized, but sort
//will still work only on original array size
void reshape(int w, int h) {

    glViewport(0, 0, W_WIDTH, W_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(lleft, rright, bbottom, ttop);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutPostRedisplay();
}

//Needs this for some reason. It's not code breaking so I will leave it
void update(){}


//Menu opens on left click
void TSP_menu(int id)
{
    //toggles output flags
    if(id == 8){
        if (inst == NULL){
            inst = new TSP(10);
        }
        inst ->less();
        b=1;
    }
    // bruteForce
    else if(id == 1) {
        if(inst == NULL)
            inst = new TSP(10);
        if (11<inst->returnSize()){
            printf("Tour is too large\n Generating a tour with 10 points\n");
            inst -> ~TSP();
            inst = new TSP(10);
        }
        inst->bruteForce();
        b=0;
    }
    //nearestNeighbor
    else if(id==2){
        if(inst == NULL)
            inst = new TSP(100);
        inst->nearestNeighbor();
        b=0;
    }
    //shortestEdge
    else if(id==3){
        if(inst == NULL)
            inst = new TSP(100);
        inst->shortestEdge();
        b=0;
    }

    //cheapestInsert
    else if(id==5){
        if(inst == NULL)
            inst = new TSP(100);
        inst->cheapestInsert();
        b=0;
    }
    //two_opt
    else if(id==7){
        if(inst == NULL){
            inst = new TSP(100);
            b = 1;
        }
        inst->two_opt(b);
    }
    //three_opt
    else if(id==9){
        if(inst == NULL){
            inst = new TSP(100);
            b = 1;
        }
        inst->three_opt(b);

    }
    /*
    //input points
    else if(id==10){
      if(inst!= NULL)
        inst -> ~TSP();
      int points,x,y, maxX = 0, maxY=0;
      scanf("%d\n", points);
      TSP(points);
      int tour = new int[SIZE*2];
      for (i=0; i<n; i++){
        printf("%d more points to input \n input x coord:\n", (n-i);
        scanf("%d\n", x);
        if (x>maxX) maxX=x;
        tour[2*i]=x;
        printf("input y coord:\n");
        scanf("%d\n", y);
        if (y>maxY) maxY=y;
        tour[(2*i)+1]=y
      }
      //nX = W_WIDTH/maxY
      //nY = W_HEIGHT/maxX
      TSP->inputRandom(tour,points);
      b=1;
    }*/

    //Exit the program
    else if(id == 6) {
        printf("Exiting...\n");
        exit(1);}
}

void Generate_Points(int id){
  printf("Generating %d vertices\n", id);
  if (inst != NULL){
    inst -> ~TSP();}
  inst = new TSP(id);
  b=1;
}

/*void Generate_Situation(int id){
    //Nearest Neighbor / Shortest Edge Fails
    if (id == 1){
      if (inst != NULL){
        inst -> ~TSP();}
      inst = new TSP(5);

      inst->input(tour, 5)
      inst->nearestNeighbor();
      b=0;

    }
    //2-opt can improve
    else if (id ==2){

    }
    else if (id == 3){

    }
}*/



//Creates windows and menus, enters event loop
int main(int argc, char **argv) {
    printf("The following is the implementation of several heuristics for the traveling salesman problem.\n To begin use please left click on the window that was created.\n \n \n \n \n");


    glutInit(&argc, argv);

    glutInitWindowSize(W_WIDTH, W_HEIGHT);
    glutInitWindowPosition(0, W_HEIGHT + spacer);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    tsp = glutCreateWindow("Traveling Salesman Problem");
    init();

    /*glViewport ( 0, 0, m_Width, m_Height );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity ();
    glOrtho ( 0.0f, m_Width, 0.0, m_Height, 1.0, -1.0 );
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity ();*/

    int nPoints; //GenerateSit;
    nPoints = glutCreateMenu(Generate_Points);
    glutAddMenuEntry("Small (10 points)", 10);
    glutAddMenuEntry("Medium (50 points)", 50);
    glutAddMenuEntry("Large (100 points)", 100);
    glutAddMenuEntry("X-Large (500 points)", 500);

    /*int GenerateSit = glutCreateMenu(Generate_Situation)
    glutAddMenuEntry("Nearest Neighbor produces tour that's not optimal", 1);
    glutAddMenuEntry("three-Opt fix to optimal tour", 2);
    glutAddMenuEntry("three_Opt fix to optimal tour", 3);
     */
     
    glutCreateMenu(TSP_menu);
    glutAddMenuEntry("Brute Force",1);
    glutAddMenuEntry("Nearest Neighbor",2);
    //glutAddMenuEntry("Shortest Edge",3);
    glutAddMenuEntry("Insertion",5);
    glutAddMenuEntry("2-Opt",7);
    glutAddMenuEntry("3-Opt",9);
    //glutAddMenuEntry("Input Points", 10);
    glutAddSubMenu("Generate Points",nPoints);
    //glutAddSubMenu("Circumstances",GenerateSit);
    glutAddMenuEntry("Toggle Output", 8);
    glutAddMenuEntry("Quit",6);

    glutAttachMenu(GLUT_LEFT_BUTTON);







    glutReshapeFunc(reshape);
    glutDisplayFunc(update);

    glutMainLoop();

    return 0;

}
