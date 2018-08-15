/*
 *  TSP.cpp
 *  The following implements methods for solving the TSP
 *  current methods are brute force, shortest edge, nearest neighbor, and genetic algorithms.
 *
 *  created by Nicholas Hagopian-Zirkel
 *
 */

#include <cstdlib>
#include <math.h>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <list>
#include "TSP.h"

//includes GA functionality
/*
#include "ga/GASStateGA.h"
#include "ga/GA1DArrayGenome.h"
#include "ga/garandom.h"
#include "ga/std_stream.h"*/


#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

using namespace std;


TSP::TSP(int s){
    /*
     *      Build the random array and optimal array
     */
    flag=1;
    SIZE = s;
    srand((unsigned)time(NULL));
    //fill array (even points are x integers odd points are y integers
    random = new int[SIZE*2];
    for(int i = 0; i<(SIZE*2); i=i+2){
        //Doesn't allow points too close to edges
        random[i] = (rand() % (W_WIDTH-80)) + 40;
        random[i+1] = (rand() % (W_HEIGHT-80)) + 40;
    }
    int exit[1];
    draw(exit,-1);
    //+1 to start and end at initial point to ensure circle
    optTour = new int[SIZE+1];
    //first and last elem are the same
    optTour[0]=0;
    optTour[SIZE]=0;
}

TSP::~TSP(){
    /*
     *      Deconstruct arrays
     */


    delete [] random;
    delete [] optTour;
    random = NULL;
    optTour = NULL;
}

void TSP::bruteForce(){
    /*
     *      Brute force algorithm, checks every permutation
     */
    //draw data points in array

    int exit[1];
    draw(exit, -1);


    // -1 ignores pinning down initial point
    int remTour[SIZE];


    //simply fills an array with values 1 to SIZE
    for (int i=0;i<SIZE;i++) {remTour[i]=i+1;}
    //records best tour and current tour
    float btour, ctour;
    //sets initial tour length to beat

    btour=tourLen(remTour);
    //goes through all permutations

    do {
        ctour=tourLen(remTour);
        //tests to see if there is a new best tour
        if (btour>ctour) {
            btour=ctour;
            for (int i=0;i<SIZE-1;i++) {
                optTour[i+1]=remTour[i];
            }
            if (flag == 0){
                draw(remTour,30);
                printf("Shortest current tour is ");
                printTour(optTour,SIZE+1);
                printf(" and is of length %f\n", tourLen(optTour));
            }
        }
        //iterates through all permutations
    } while (std::next_permutation(remTour,remTour+SIZE));


    //end with
    draw(optTour, 0);
    if (flag == 0){
        printf("\n *** \n *** \n Generated tour is ");
        printTour(optTour,SIZE+1);
        printf("and ");}
    printf("The tour is %f units long\n", tourLen(optTour));
    if (flag==0) printf(" ***\n ***\n");
}

void TSP::nearestNeighbor(){
    /*
     *      Finds the nearest unconnected point and connects it
     */

    int exit[1];
    draw(exit, -1);

    //a number by definition larger than any distance
    int ezRecall = (W_WIDTH + W_HEIGHT) * (W_HEIGHT + W_WIDTH);

    int prevVer=0;
    float leastDist, checkDist;

    list<int> l;
    list<int>::const_iterator it1, itBest;

    //simply fills a list with values 1 to SIZE
    for (int i=0;i<SIZE-1;i++) {l.push_back(i+1);}
    int i = 0;
    //searches for nearest neighbor and saves it as the next vertex while first element of the tour isn't -1
    while (l.begin() != l.end()){
        leastDist=ezRecall;
        for (it1 = l.begin() ; it1 != l.end() ;it1++){
            checkDist = dist(random[2*(*it1)], random[(2*(*it1))+1],random[(2*prevVer)],random[(2*prevVer)+1]);
            if (checkDist<leastDist){
                //marks next vertex to visit
                itBest = it1;
                leastDist=checkDist;
            }
        }
        if (flag == 0){
            printf("Next nearest vertex is %f units away\n", leastDist);
            printf("Connecting %d to %d \n", prevVer, *itBest);}
        //vertex to find next nearest point to
        prevVer=*itBest;
        //adds just found best vert to optTour
        i++;
        optTour[i]=prevVer;
        l.erase(itBest);
        
        glClear (GL_COLOR_BUFFER_BIT);

        if (flag == 0){
            glBegin(GL_QUADS);
            for (GLint k = 0; k < SIZE*2; k+=2) {
                int valx = random[k];
                int valy = random[k+1];
                glVertex2f(valx+lleft, valy+bbottom);
                glVertex2f(valx+lleft+1, valy+bbottom+1);
                glVertex2f(valx+lleft+1, valy+bbottom);
                glVertex2f(valx+lleft, valy+bbottom+1);
            }
            glEnd();

            glBegin(GL_LINES);
            for(int a=1; a != i; a++){
                glVertex2f(random[2*optTour[a-1]], random[(2*optTour[a-1])+1]);
                glVertex2f(random[2*optTour[a]], random[(2*optTour[a])+1]);
            }
            glEnd();
            glutSwapBuffers();
            delay(100);
        }
    }
    if (flag == 0){
    printf("Next nearest vertex is %f units away\n", dist(random[2*(prevVer)], random[(2*(prevVer))+1],random[0],random[1]));
        printf("Connecting %d to 0 \n", prevVer);}

    l.clear();

    //end with
    draw(optTour, 0);
    if (flag == 0){
        printf("\n *** \n *** \n Generated tour is ");
        printTour(optTour,SIZE+1);
        printf("and ");
    }

    printf("The tour is %f units long\n", tourLen(optTour));

    if (flag==0) printf(" ***\n ***\n");
}

void TSP::cheapestInsert(){
    /*
     * uses linked lists to find the cheapest insert
     */

    //draw data points in array
    int exit[1];
    draw(exit, -1);


    float curDis, bestDis;
    //first two points are inserted (since random is random 1,2 are arbitrary)
    optTour[1]=1;
    curDis = 2 * dist(random[0], random[1], random[2], random[3]);

    list<int> l;
    l.push_front(1);
    l.push_front(0);
    list<int>::const_iterator it1, it2, itBest, lastElem;

    //starts with 3rd point (vertex 2) as the point to add
    for (int i=2; i<SIZE-1; i++){
        lastElem = l.end();
        lastElem--;
        //dist created by inserting as last elem, - distance between last element and first element
        bestDis = memDist(*lastElem, i, 0) - dist(random[0], random[1], random[2*(*lastElem)], random[(2*(*lastElem))+1]);
        itBest = lastElem;
        it1 = l.begin();
        it2 = l.begin();
        //loops through consecutive points to find the cheapest insert
        while(it2 != lastElem){
            it2++;
            curDis = memDist(*it1, i, *it2) - dist(random[2*(*it1)], random[(2*(*it1))+1], random[2*(*it2)], random[(2*(*it2))+1]);
            if (curDis<bestDis){
                bestDis=curDis;
                itBest=it1;
                if (flag == 0){
                    printf("Placing vertex %d between %d and %d\n", i, *it1, *it2);
                    printf("for a shortest added distance of %f\n",bestDis);
                }
            }
            it1++;

        }
        if (flag == 0){
            printf("Placing vertex %d placed between %d and %d\n", i, *it1, *it2);
            printf("for shortest added distance of %f \n",bestDis);
        }
        //iterator should be 1 after position of insert
        itBest++;
        l.insert(itBest, i);

        //draws opt so far

        if (flag == 0){
            glClear (GL_COLOR_BUFFER_BIT);
            glBegin(GL_QUADS);

            for (GLint k = 0; k < SIZE*2; k+=2) {
                int valx = random[k];
                int valy = random[k+1];
                glVertex2f(valx+lleft, valy+bbottom);
                glVertex2f(valx+lleft+1, valy+bbottom+1);
                glVertex2f(valx+lleft+1, valy+bbottom);
                glVertex2f(valx+lleft, valy+bbottom+1);
            }
            glEnd();

            glBegin(GL_LINES);
            int a=0, b=0;
            it2 = l.begin();
            while (it2 != l.end()){
                it1=it2;
                it2++;
                b = *it1;
                a = *it2;
                glVertex2f(random[2*a], random[(2*a)+1]);
                glVertex2f(random[2*b], random[(2*b)+1]);
            }
            it2 = l.end();
            glVertex2f(random[0], random[1]);
            glVertex2f(random[2*b], random[(2*b)+1]);
            delay(50);
            glEnd();
            
            glutSwapBuffers();
            
        }

    }
    //l is now optimal tour, we will put this in the optimal tour array so two-opt can use it
    it1 = l.begin();
    for (int i = 0; it1 != l.end(); i++){
        it1++;
        optTour[i+1]=*it1;
    }
    l.clear();
    //end with
    draw(optTour, 0);
    if (flag == 0){
        printf("\n *** \n *** \n Generated tour is ");
        printTour(optTour,SIZE+1);
        printf("and ");}
    printf("The tour is %f units long\n", tourLen(optTour));
    if (flag==0) printf(" ***\n ***\n");
}



void TSP::shortestEdge(){
    /*
     * sorts all edges in shortest order
     */
    /*
    int exit[1];
    draw(exit, -1);

    struct point{
        float dist;
        int x,y;
    };
    int t;
    list<point> l;
    list<point>::const_iterator it1;
    it1=l.begin();
    for(int i = 0; i<SIZE;i++){
        printf("i = %d\n", i);
        for (int j=i+1; j!=SIZE;j++){
            printf("j = %d\n", j);
            point a;
            float d = dist(random[2*i],random[(2*i)+1],random[(2*j)],random[(2*j)+1]);
            a.dist = d;
            a.x=i;
            a.y=j;
            t=1;
            while (t=1) {
                if ((*it1.dist) < d) it1++;
                if ((*it1.dist) > d) it1--;
                //need instance where it's right
                t=0;
            };
        }
    }
    */
    //quicksort(allEdges, 0, n-1);
    //sort edges
    //check if edge connects to a vertex with 1 or 0 connections
    //find next shortest edge


    //end with
    /*draw(optTour, 0);
    if (flag == 0){
        printf("\n *** \n *** \n Generated tour is ");
        printTour(optTour,SIZE+1);
        printf("and ");}
    printf("The tour is %f units long\n ", tourLen(optTour));
    if (flag==0) printf(" ***\n ***\n");*/
}

void TSP::two_opt(bool buildOpt){

    //fills opt tour if 2-opt is being run by itself and not with another heuristic
    if (buildOpt == 1){
        for (int i=1;i<SIZE;i++) {optTour[i]=i;}
    }
    draw(optTour, 0);

    int time_out=-1;
    int tTo=30;//may be too large, we will try diff values
    int i, j, swap1, swap2;
    float bestLen, curLen;
    bestLen = tourLen(optTour);
    while (time_out<tTo){
        time_out++;
        for(i =0; i<SIZE-2;i++){
            for (j=i+1; j<SIZE-1; j++){
                swap1 = optTour[i];
                swap2 = optTour[j];
                optTour[j]=swap1;
                optTour[i]=swap2;
                curLen = tourLen(optTour);
                if (curLen < bestLen){
                    time_out=-1;
                    bestLen=curLen;
                    draw(optTour, 0);
                    if (flag == 0){
                        printf("Current shortest tour is %f\n", bestLen);
                        delay(50);
                    }
                }
                else {
                    optTour[j]=swap2;
                    optTour[i]=swap1;
                }
            }
        }
    }
    draw(optTour, 0);
    if (flag == 0){
        printf("\n *** \n *** \n Generated tour is ");
        printTour(optTour,SIZE+1);
        printf("and ");}
    printf("The tour is %f units long\n", tourLen(optTour));
    if (flag==0) printf(" ***\n ***\n");
}



void TSP::three_opt(bool buildOpt){
    //fills opt tour if 3-opt is being run by itself and not with another heuristic
    if (buildOpt == 1){
        for (int i=1;i<SIZE;i++) {optTour[i]=i;}
    }
    draw(optTour, 0);

    int time_out=-1;
    int tTo=10;//may be too large, we will try diff values
    int i, j, k, swap1, swap2, swap3 ,bi,bj,bk;
    int arr[3];
    float bestLen, curLen;
    bestLen = tourLen(optTour);

    while (time_out<tTo){
        time_out++;
        for(i =0; i<SIZE-3;i++){
            for (j=i+1; j<SIZE-2; j++){
                for (k=j+1; k<SIZE-1; k++){
                    swap1 = optTour[i];
                    swap2 = optTour[j];
                    swap3 = optTour[k];
                    bi = swap1;
                    bj = swap2;
                    bk = swap3;
                    
                    arr[0] = i;
                    arr[1] = j;
                    arr[2] = k;
                    
                    do {
                        optTour[arr[0]] = swap1;
                        optTour[arr[1]] = swap2;
                        optTour[arr[2]] = swap3;
                        curLen = tourLen(optTour);
                        //tests to see if there is a new best tour
                        if (curLen<bestLen) {
                            time_out = 0;
                            draw(optTour, 0);
                            bestLen=curLen;
                            bi = arr[0];
                            bj = arr[1];
                            bk = arr[2];
                            if (flag == 0){
                                printf("Tour is improved to %f units\n", bestLen);
                                delay(10);
                            }
                        }
                        //iterates through 6 permutations
                    } while (std::next_permutation(arr,arr+3));
                    //makes optTour best tour;
                    optTour[bi] = swap1;
                    optTour[bj] = swap2;
                    optTour[bk] = swap3;
                    
                }
            }
        }
    }
}







void TSP::less(){
    flag = !flag;
    printf("Output is now ");
    if (flag==0) printf("on\n");
    else printf("off\n");
}
/*
 * The following are helper functions for the TSP heuristics
 * memDist - returns distance of 3 points
 * tourLen - calculates distance between all points of a tour
 * draw - draws the given tour
 * printTour - outputs all the points in a tour
 * dist - calculates distance between two points in a tour
 * delay - delays for some amount of time
 */

float TSP::memDist(int a, int b, int c){
    float m = 0;
    m = dist(random[2*a], random[(2*a)+1], random[2*b], random[(2*b)+1]);
    m = m + dist(random[2*b], random[(2*b)+1], random[2*c], random[(2*c)+1]);
    return m;
}

float TSP::tourLen(int *tour){
    float totDist=0;
    if (tour[0] != 0){
        totDist=dist(random[0],random[1],random[2*tour[0]],random[(2*tour[0])+1]);}
    int i=0;
    for (; i<SIZE-2;){
    totDist=totDist + dist(random[2*tour[i]],random[(2*tour[i])+1],random[2*tour[i+1]],random[(2*tour[i+1])+1]);
        i++;
    }
    if (tour[0] != 0){
        totDist=totDist + dist(random[0],random[1],random[(2*tour[SIZE-2])],random[(2*tour[SIZE-2])+1]);}
    return totDist;
}

//insertion needs a different draw function
void TSP::drawIns(int *tour, int del, int s){
    glClear (GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    for (GLint k = 0; k < SIZE*2; k+=2) {
        int valx = random[k];
        int valy = random[k+1];
        glVertex2f(valx+lleft, valy+bbottom);
        glVertex2f(valx+lleft+1, valy+bbottom+1);
        glVertex2f(valx+lleft+1, valy+bbottom);
        glVertex2f(valx+lleft, valy+bbottom+1);
    }
    glEnd();
    
    //checks if printing more than points
    if (del != -1){
        glBegin(GL_LINES);
        if (s==SIZE-1){
            glVertex2f(random[0], random[1]);
            glVertex2f(random[2*tour[0]], random[(2*tour[0])+1]);
        }
        for(int i=0; i<s-1; i++){
            glVertex2f(random[2*tour[i]], random[(2*tour[i])+1]);
            glVertex2f(random[2*tour[i+1]], random[(2*tour[i+1])+1]);
            delay(del);
        }
    }
    glEnd();
    glutSwapBuffers();
    
}


void TSP::draw(int *tour, int del){
    int s;
    //if start of tour is 0 then it is a full tour
    if (tour[0]==0) s=SIZE+1;
    //this shouldn't happen but it avoids seg faults
    else s = SIZE-1;

    glClear (GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    for (GLint k = 0; k < SIZE*2; k+=2) {
        int valx = random[k];
        int valy = random[k+1];
        glVertex2f(valx+lleft, valy+bbottom);
        glVertex2f(valx+lleft+1, valy+bbottom+1);
        glVertex2f(valx+lleft+1, valy+bbottom);
        glVertex2f(valx+lleft, valy+bbottom+1);
    }
    glEnd();

    //checks if printing more than points
    if (del != -1){
        glBegin(GL_LINES);
        if (s==SIZE-1){
            glVertex2f(random[0], random[1]);
            glVertex2f(random[2*tour[0]], random[(2*tour[0])+1]);
        }
        for(int i=0; i<s-1; i++){
            glVertex2f(random[2*tour[i]], random[(2*tour[i])+1]);
            glVertex2f(random[2*tour[i+1]], random[(2*tour[i+1])+1]);
            delay(del);
        }
    }
    glEnd();
    glutSwapBuffers();

}

void TSP::printTour(int *tour, int totSize){
    for (int i = 0; i<totSize-1; i++){
        printf("%d,",tour[i]);
    }
    printf("%d", tour[totSize-1]);
}

float TSP::dist(int x1,int y1,int x2,int y2){
     /*
      * Dist between (x1,y1) and (x2,y2)
      */

     x1 = float(x1);
     x2 = float(x2);
     y1 = float(y1);
     y2 = float(y2);
     float out = sqrt(abs((x1-x2)*(x1-x2)) + abs((y1-y2)*(y1-y2)));
     return out;
}

void TSP:: delay(int milliseconds){
    long pause;
    clock_t now, then;

    pause = milliseconds * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while( (now - then) < pause )
        now = clock();
}

void TSP::swap(int *a, int *b){
    int t = *a;
    *a = *b;
    *b = t;
}

int TSP::partition(int *edges, int l, int r){
    int pivot = edges[r];
    int i = (l-1);
    for (int j = l; j <= r - 1; j++){
        if (edges[j] <= pivot){
            i++;
            swap(&edges[i], &edges[j]);
        }
    }
    swap(&edges[i +1], &edges[r]);
    return (i+1);
}

void TSP::quicksort(int *edges, int l, int r){
    if (l<r){
        int p = partition(edges, l, r);
        quicksort(edges, l, p-1);
        quicksort(edges, p+1, r);
    }
}

/*
 * The following generates situations
 * Situation 1 - Nearest Neighbor fails
 * Situation 2 - can be fixed with 2-opt
 * Situation 3 - can be fixed by 3-opt
 */
