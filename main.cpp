/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <string.h>
#include <CommonThings.h>
#include <Maze.h>
#include <iostream>
#include <Timer.h>
#include <player.h>
//#include <BreadthFirst.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <array>
#include <stdlib.h>
#include <fstream>
#include <wall.h>
#include <iostream>
#include <cstring>
#include <string>
#include <climits>
#include <queue>
#include <map>
#include <vector>
#define MAX 15

/* GLUT callback Handlers */

using namespace std;

Maze *M = new Maze(15);                      // Set Maze grid size
Player *P = new Player();                    // create player
Player *bird = new Player();                 //create bird
Player *fish = new Player();                 //create fish
Player *frog = new Player();                 //create frog

wall W[600];                                 // wall with number of bricks
//wall H[100];

Timer *T0 = new Timer();                     // animation timer
Timer *T1 = new Timer();                     // animation timer


float wWidth, wHeight;                       // display window width and Height
int xPos,yPos;                               // Viewport mapping

string dir[35];
int arrCount = 35;
int counter = 0;
int tCounter = 0;
int graph[15][15];
char *buff = new char[10];


int desX, desY;
bool playerSelected = false;

//*********FOR SHORTEST*******
int m = 15;
int n = 15;
int s_row;
int s_col;
int d_row;
int d_col;

int row[] = { -1, 0, 0, 1 };
int col[] = { 0, -1, 1, 0 };
vector <int> x;                              //row
vector <int> y;                              //column

string choice;



void display(void);                      // Main Display : this runs in a loop
/*
void playerActions();
void birdActions();
void frogActions();
void fishActions();
*/

void resize(int width, int height)           // resizing case on the window
{
    wWidth = width;
    wHeight = height;

    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}

void init()
{
    glEnable(GL_COLOR_MATERIAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0,0.0,0.0,0.0);
    gluOrtho2D(0, wWidth, 0, wHeight);

    T0->Start();                                        // set timer to 0
    T1->Start();                                        // set timer to 0


    glEnable(GL_BLEND);                                 //display images with transparent
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    M->loadBackgroundImage("images/bak.jpg");           // Load maze background image

    /*
    M->loadChestImage("/Users/evanmadrigal/Desktop/Maze_New/images/chest.png");              // load chest image
    M->placeChest(3,3);                                 // place chest in a grid

    M->loadSetOfArrowsImage("/Users/evanmadrigal/Desktop/Maze_New/images/arrwset.png");      // load set of arrows image
    M->placeStArrws(5,3);                               // place set of arrows
    */

    P->initPlayer(M->getGridSize(),"images/perPok.png",4);   // initialize player pass grid size,image and number of frames
    P->loadArrowImage("images/arr.png");                // Load arrow image
    P->placePlayer(1,10);

    bird->initPlayer(M->getGridSize(),"images/flyingPok.png",4);     //Bird 2 frames
    bird->loadArrowImage("images/arr.png");                // Load arrow image
    bird->placePlayer(13,2);

    frog->initPlayer(M->getGridSize(),"images/frog.png",4);     //Frog 4 frames
    frog->loadArrowImage("images/arr.png");                // Load arrow image
    frog->placePlayer(0,0);

    fish->initPlayer(M->getGridSize(),"images/fish.png",4);     //Fish 4 frames
    fish->loadArrowImage("images/arr.png");                // Load arrow image
    fish->placePlayer(14,14);

    for(int i=0; i< M->getGridSize();i++)
    {
      W[i].wallInit(M->getGridSize(),"images/wall.png"); // Load walls
      W[i].placeWall(i,5);                              // place each brick

      //H[i].placeWall(-1,-1);
    }
cout << "Choose your character Map! (fish, human, frog, bird): ";
    fstream inFile;
cin >> choice;
cout << endl;
    if(choice == "fish")
    inFile.open("Fterrain.txt");
    else if(choice == "human")
    inFile.open("HumanTerrain.txt");
    else if(choice == "frog")
    inFile.open("FrogTerrain.txt");
    else if(choice == "bird")
    inFile.open("BirdTerrain.txt");
    char data;

    if(inFile.is_open()){
    for(int i = 0; i < M->getGridSize() ; i++)
    {
        for(int j = 0; j < M->getGridSize(); j++)
        {
            inFile >> data;
            if(data == '0') //place high mountain
            {
                W[tCounter].wallInit(M->getGridSize(),"images/HighMountains.png");
                W[tCounter].placeWall(j,i);
            }
            else if(data == '1') // place middle mountain
            {
               W[tCounter].wallInit(M->getGridSize(), "images/MedMountains.png");
               W[tCounter].placeWall(j,i);// H[i].wallInit(i,j)// H[i].wallInit(i,j)
            }
            else if(data == '2') // place footHills
            {
                W[tCounter].wallInit(M->getGridSize(), "images/foothills.png");
              W[tCounter].placeWall(j, i);// H[i].wallInit(i,j)
            }
            else if(data == '3') // place plain
            {
                W[tCounter].wallInit(M->getGridSize(), "images/plains.png");
               W[tCounter].placeWall(j, i);// H[i].wallInit(i,j)
            }
            else if(data == '4') // place forest
            {
               W[tCounter].wallInit(M->getGridSize(),"images/Forest.png");
                W[tCounter].placeWall(j,i);
            }
            else if(data == '5') // place desert
            {
                W[tCounter].wallInit(M->getGridSize(),"images/desert.png");
                W[tCounter].placeWall(j,i);
            }
            else if(data == '6') // place beach
            {
                W[tCounter].wallInit(M->getGridSize(),"images/beach.png");
                W[tCounter].placeWall(j,i);
            }
            else if(data == '7') // place ocean
            {
               W[tCounter].wallInit(M->getGridSize(),"images/Bwaterterrain.png");
               W[tCounter].placeWall(j,i);
            }
              tCounter++;
    }
    }
    inFile.close();
}
}

void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT);        // clear display screen

        glPushMatrix();
         M->drawBackground();           // Display Background
        glPopMatrix();

        glPushMatrix();
        for(int i = 0; i < 500;i++)
        {
        W[i].drawWall();
        }
          glPopMatrix();

        glPushMatrix();
            P -> drawplayer();         // Draw Player move actions
        glPopMatrix();

        glPushMatrix();
            fish -> drawplayer();          // Draw Player move actions
        glPopMatrix();

          glPushMatrix();
            bird -> drawplayer();          // Draw Player move actions
        glPopMatrix();

          glPushMatrix();
            frog -> drawplayer();          // Draw Player move actions
        glPopMatrix();

    glutSwapBuffers();
}


void key(unsigned char key, int x, int y)
{

    glutPostRedisplay();
}


 void GetOGLPos(int x, int y)          // Mouse map coordinates
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    xPos =(int) (posX *M->getGridSize()/2 +M->getGridSize()/2); // update mouse position X
    yPos =(int) (posY *M->getGridSize()/2 +M->getGridSize()/2);

    cout<<"Mouse Click location : "<< xPos<<" "<<yPos <<endl;   // print out grid value
}

void movePlayer(Player *plyr,string dir, int frames)
{
    char *buff = new char[dir.length() + 1];
    std::strcpy(buff, dir.c_str());

    if(plyr->steps > plyr->unitWidth)
    {
        plyr->steps = 0;
        strcpy(buff, "stand");
        plyr->placePlayer(plyr->getPlayerLoc().x,plyr->getPlayerLoc().y);
        if(T1->GetTicks()>500){
        counter++;
        T1->Reset();
      }
    }
    else
    {
        if(plyr -> activePlayer)
            plyr->movePlayer(buff, frames);

            delete[] buff;
    }

}

 void idle(void)
{
//Run Several times
   //Your Code in this section
    if(T0->GetTicks()>300 && counter < 30)
      {
        // This is to make sure player move one unit
       movePlayer(P, dir[counter], 4);
       movePlayer(bird, dir[counter], 4);
       movePlayer(frog, dir[counter], 4);
       movePlayer(fish, dir[counter], 4);

       T0->Reset();
      }
    glutPostRedisplay();
}
//*************************************************START RANDOM TERRAIN READER
void fishM()
{
    fstream inFile;
        if(choice == "fish")
    inFile.open("Fterrain.txt");
    else if(choice == "human")
    inFile.open("HumanTerrain.txt");
    else if(choice == "frog")
    inFile.open("FrogTerrain.txt");
    else if(choice == "bird")
    inFile.open("BirdTerrain.txt");
    char data;

       if(inFile.is_open()){
       for(int i = 0; i < MAX ; i++)
       {
           for(int j = 0; j < MAX; j++)
           {
               inFile >> data;
               if(data == '0') //place high mountain
                       {
                          graph[i][j] = 0;
                       }
                else if(data == '1') // place middle mountain
                       {
                         graph[i][j] = 0;
                       }
                else if(data == '2') // place footHills
                       {
                           graph[i][j] = 0;
                       }
                else if(data == '3') // place plain
                       {
                          graph[i][j] = 0;
                       }
                else if(data == '4') // place forest
                       {
                         graph[i][j] = 0;
                       }
                else if(data == '5') // place desert
                       {
                           graph[i][j] = 0;
                       }
                else if(data == '6') // place beach
                       {
                           graph[i][j] = 0;
                       }
                else if(data == '7') // place ocean
                       {
                           graph[i][j] = 1;
                       }
               }
               }
           inFile.close();
       }
}

void PM()
{
    fstream inFile;
    if(choice == "fish")
    inFile.open("Fterrain.txt");
    else if(choice == "human")
    inFile.open("HumanTerrain.txt");
    else if(choice == "frog")
    inFile.open("FrogTerrain.txt");
    else if(choice == "bird")
    inFile.open("BirdTerrain.txt");
       char data;

       if(inFile.is_open()){
       for(int i = 0; i < MAX ; i++)
       {
           for(int j = 0; j < MAX; j++)
           {
               inFile >> data;
               if(data == '0') //place high mountain
                       {
                          graph[i][j] = 0;
                       }
                else if(data == '1') // place middle mountain
                       {
                         graph[i][j] = 1;
                       }
                else if(data == '2') // place footHills
                       {
                           graph[i][j] = 1;
                       }
                else if(data == '3') // place plain
                       {
                          graph[i][j] = 1;
                       }
                else if(data == '4') // place forest
                       {
                         graph[i][j] = 1;
                       }
                else if(data == '5') // place desert
                       {
                           graph[i][j] = 1;
                       }
                else if(data == '6') // place beach
                       {
                           graph[i][j] = 1;
                       }
                else if(data == '7') // place ocean
                       {
                           graph[i][j] = 0;
                       }
               }
               }
           inFile.close();
       }
}

void birdM()
{
    fstream inFile;
      if(choice == "fish")
    inFile.open("Fterrain.txt");
    else if(choice == "human")
    inFile.open("HumanTerrain.txt");
    else if(choice == "frog")
    inFile.open("FrogTerrain.txt");
    else if(choice == "bird")
    inFile.open("BirdTerrain.txt");
       char data;

       if(inFile.is_open()){
       for(int i = 0; i < MAX ; i++)
       {
           for(int j = 0; j < MAX; j++)
           {
               inFile >> data;
               if(data == '0') //place high mountain
                       {
                          graph[i][j] = 1;
                       }
                else if(data == '1') // place middle mountain
                       {
                         graph[i][j] = 1;
                       }
                else if(data == '2') // place footHills
                       {
                           graph[i][j] = 1;
                       }
                else if(data == '3') // place plain
                       {
                          graph[i][j] = 1;
                       }
                else if(data == '4') // place forest
                       {
                         graph[i][j] = 1;
                       }
                else if(data == '5') // place desert
                       {
                           graph[i][j] = 0;
                       }
                else if(data == '6') // place beach
                       {
                           graph[i][j] = 1;
                       }
                else if(data == '7') // place ocean
                       {
                           graph[i][j] = 1;
                       }
               }
               }
           inFile.close();
       }
}

void frogM()
{
    fstream inFile;
    if(choice == "fish")
    inFile.open("Fterrain.txt");
    else if(choice == "human")
    inFile.open("HumanTerrain.txt");
    else if(choice == "frog")
    inFile.open("FrogTerrain.txt");
    else if(choice == "bird")
    inFile.open("BirdTerrain.txt");
       char data;

       if(inFile.is_open()){
       for(int i = 0; i < MAX ; i++)
       {
           for(int j = 0; j < MAX; j++)
           {
               inFile >> data;
               if(data == '0') //place high mountain
                       {
                          graph[i][j] = 0;
                       }
                else if(data == '1') // place middle mountain
                       {
                         graph[i][j] = 0;
                       }
                else if(data == '2') // place footHills
                       {
                           graph[i][j] = 1;
                       }
                else if(data == '3') // place plain
                       {
                          graph[i][j] = 1;
                       }
                else if(data == '4') // place forest
                       {
                         graph[i][j] = 1;
                       }
                else if(data == '5') // place desert
                       {
                           graph[i][j] = 0;
                       }
                else if(data == '6') // place beach
                       {
                           graph[i][j] = 1;
                       }
                else if(data == '7') // place ocean
                       {
                           graph[i][j] = 1;
                       }
               }
               }
           inFile.close();
       }
}

//*************************************************END RANDOM TERRAIN READER
//*************************************************START OF SHORTEST PATH


struct Node
{
   int x, y;

   vector < pair < int, int > > path;

   bool const operator==(const Node& ob) const
   {
       return x == ob.x && y == ob.y;
   }

   bool operator<(const Node& ob) const
   {
       return x < ob.x || (x == ob.x && y < ob.y);
   }

};


bool isValid(int matrix[][MAX], int x, int y, int m, int n)
{
   return (x >= 0 && x < m) && (y >= 0 && y < n) && (matrix[x][y] == 1);
}

void printPath(vector<pair<int, int > > path)
{
    cout << "Shortest Path" << endl;
    for (pair<int, int> p: path){
        x.push_back(p.first);
        y.push_back(p.second);
        cout << p.second << " " << p.first << endl;
    }
    cout << endl;

    for(int i = 0; i < arrCount; i++){
        if(x[i] > x[i + 1] && y[i] == y[i + 1])
        {
           dir[i] = "down";
        }
        else if(x[i] < x[i + 1] && y[i] == y[i + 1])
        {
           dir[i] = "up";
        }
        else if(x[i] == x[i + 1] && y[i] > y[i + 1])
        {
            dir[i] = "left";
        }
        else if(x[i] == x[i + 1] && y[i] < y[i + 1])
        {
            dir[i] = "right";
        }
        else
        {
            dir[i] = "stand";
        }
    }
x.clear();
y.clear();
}

int findPath(int m, int n, int matrix[MAX][MAX], int x, int y, int d_row, int d_col)
{
   vector<pair<int, int > > path;
   path.push_back(make_pair(x,y));

   queue<Node> Q;
   Node src = {x, y, path};
   Q.push(src);


   map<Node, bool> visited;
   visited[src] = true;

   while (!Q.empty())
   {
       Node curr = Q.front();
       Q.pop();

       int i = curr.x, j = curr.y;
       path = curr.path;

       if (i == d_row && j == d_col)
       {
           printPath(path);
           return path.size() - 1;
       }

       for (int k = 0; k < 4; k++)
       {
           int x = i + row[k];
           int y = j + col[k];

           if (isValid(matrix, x, y,m,n))
           {
               path.push_back(make_pair(x,y));

               Node next = {x, y, path};

               if (!visited.count(next))
               {
                   Q.push(next);
                   visited[next] = true;
               }
               path.pop_back();
           }
       }
   }

   return INT_MAX;
}

void dirCalls(string direction[])
{
    for(int i = 0; i < arrCount; i++)
    {
       cout << direction[i] << endl;
    }
    cout << endl;
}
//*******************************************END OF SHORTEST PATH

void mouse(int btn, int state, int x, int y){

    switch(btn){
        case GLUT_LEFT_BUTTON:

        if(state==GLUT_DOWN){
              GetOGLPos(x,y);
              if(xPos == P-> getPlayerLoc().x && yPos == P->getPlayerLoc().y)
              {
                  P->activePlayer = true;
                  fish->activePlayer = false;
                  bird->activePlayer = false;
                  frog->activePlayer = false;
                  playerSelected = true;

              }
               if(xPos == fish-> getPlayerLoc().x && yPos == fish->getPlayerLoc().y)
              {
                  P->activePlayer = false;
                  fish->activePlayer = true;
                  bird->activePlayer = false;
                  frog->activePlayer = false;
                  playerSelected = true;

              }
               if(xPos == bird-> getPlayerLoc().x && yPos == bird->getPlayerLoc().y)
              {
                  P->activePlayer = false;
                  fish->activePlayer = false;
                  bird->activePlayer = true;
                  frog->activePlayer = false;
                  playerSelected = true;

              }
               if(xPos == frog-> getPlayerLoc().x && yPos == frog->getPlayerLoc().y)
              {
                  P->activePlayer = false;
                  fish->activePlayer = false;
                  bird->activePlayer = false;
                  frog->activePlayer = true;
                  playerSelected = true;
              }
             }
            break;

      case GLUT_RIGHT_BUTTON:

        if(state==GLUT_DOWN){
              GetOGLPos(x,y);
         if(playerSelected){
              desX = xPos;
              desY = yPos;
            if(P->activePlayer)
                {
                    /*
                 int matrix[MAX][MAX] =
                    {
                        {1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
                        {1,1,1,1,1,1,1,0,0,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,0,0,0,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,0,0,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,0,0,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,0,0,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,0,0,0,0,1,1,1},
                        {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
                        {1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
                        {1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
                        {1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
                        {1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
                    };*/
                    PM();
                    s_row = P->getPlayerLoc().y;
                    s_col = P->getPlayerLoc().x;
                    d_row = desY;
                    d_col = desX;
                    findPath(m, n, graph, s_row, s_col, d_row, d_col); //changed matrix to graph;
                    dirCalls(dir);
                    counter = 0;
                }
            else if (fish->activePlayer)
                {/*
                 int matrix[MAX][MAX] =
                    {
                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0},
                        {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
                        {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
                        {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
                        {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
                        {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
                    };*/
                    fishM();
                    s_row = fish->getPlayerLoc().y;
                    s_col = fish->getPlayerLoc().x;
                    d_row = desY;
                    d_col = desX;
                    findPath(m, n, graph, s_row, s_col, d_row, d_col);
                   dirCalls(dir);
                counter = 0;
                }
            else if(frog->activePlayer)
                {/*
                 int matrix[MAX][MAX] =
                    {
                                               {1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
                                               {1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
                                               {1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
                                               {1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
                                               {1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
                                               {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                               {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                               {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                               {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                               {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                               {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
                                               {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
                                               {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
                                               {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                               {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                    };*/
                    frogM();
                    s_row = frog->getPlayerLoc().y;
                    s_col = frog->getPlayerLoc().x;
                    d_row = desY;
                    d_col = desX;
                    findPath(m, n, graph, s_row, s_col, d_row, d_col);
                dirCalls(dir);
                counter = 0;
                }
            else if(bird-> activePlayer){
                    /*
                 int matrix[MAX][MAX] =
                    {
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                        {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
                        {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
                        {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                    };*/
                    birdM();
                    s_row = bird->getPlayerLoc().y;
                    s_col = bird->getPlayerLoc().x;
                    d_row = desY;
                    d_col = desX;
                   findPath(m, n, graph, s_row, s_col, d_row, d_col);
                 dirCalls(dir);
                counter = 0;
                }
         } //End of active player if statement
          break;
    }
     glutPostRedisplay();
}
}

void Specialkeys(int key, int x, int y)
{
     cout<<"Player Location  :  "<<P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
     cout<<"Player Location  :  "<<fish->getPlayerLoc().x<< "    "<<fish->getPlayerLoc().y<<endl;
     cout<<"Player Location  :  "<<bird->getPlayerLoc().x<< "    "<<bird->getPlayerLoc().y<<endl;
     cout<<"Player Location  :  "<<frog->getPlayerLoc().x<< "    "<<frog->getPlayerLoc().y<<endl;

    switch(key)
    {

   }
   // comment this after you debug

  glutPostRedisplay();

}

/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (800, 800);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Maze");                    //program title
   init();

   glutDisplayFunc(display);                     //callback function for display
   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard
   glutSpecialFunc(Specialkeys);
   glutMouseFunc(mouse);
   glutIdleFunc(idle);
   glutMainLoop();

   return EXIT_SUCCESS;
}
