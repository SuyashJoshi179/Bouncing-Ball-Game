#include <GL/glut.h> //Header files for Graphics Library
#include <iostream>  //For Including objects of iostream class
#include <math.h>    //Used  this header for mathematical functions like Square Root and power function
#include <deque>     //STL Deque is used
using namespace std;
GLfloat y_ball = 0, y_ball_prev = 0, x_blocks = 0;
float t = 0;
deque<int> q;
int Next, k1 = 0, blen = 13, ballState = 1; // Next: Current Platform level. blen: Current Platform length.
    //K1: Counter to take track of inner platform formation loop ballState: Current ball state(Jumping/rolling/free-fall).
float y_floor = 0;  // current floor value for animation.
bool Space = false; // if Space key is pressed
void myinit()
{
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};   //Parameters for specific light type for object
    //{floatRed, floatGreen, floatBlue, floatAlpha}
    GLfloat mat_shininess[] = {50.0};                //Parameter for intensity of shininess
    GLfloat light_position[] = {0.0, 1.0, 0.7, 0.0}; //Parameters for position of light on object

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);   //Enables specular highlight for light source (GL_LIGHT0)
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); //Enables shininess for object light up by light source (GL_LIGHT0)
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);   
    //For light source LIGHT0, enables the specific positions for light mentioned in light_position

    glEnable(GL_LIGHTING);       //Enables lighting condition
    glEnable(GL_LIGHT0);         //Enables light source with id LIGHT0
    glEnable(GL_COLOR_MATERIAL); //Enables to colour the object
    glEnable(GL_DEPTH_TEST);
    for (int i = 0; i < 28; i++)
    {
        q.push_back(0);
    }
    srand(time(0)); // Use current time as seed for random generator
}

void callback(int time)
{
    glutPostRedisplay(); //glutPostRedisplay marks the current window as needing to be redisplayed
    if (y_ball > -6.7)
    {
        glutTimerFunc((unsigned int)25, callback, time + 1); //calling a recursive callback function if game is not ended.
    }
}

int generate(int a)
{ //generate function to generate random intiger for choosing the level of blocks(cuboids),depending upon the previous level.
    if (a == 0)
        return rand() % 2; //"rand() % 2" is a function to generate random intigers from 1 or 2,ie the levels;
    else
        return rand() % 3; //"rand() % 3" is a function to generate random intigers from 1 to 3,ie the levels;
}

void ball()
{
    glPushMatrix();    //glPushMatrix pushes the current matrix stack down by one
    t += 0.025;        // Counter for time in seconds
    switch (ballState) // Decision based on currant ball state
    {
    case 0:                                                      // Is ball allready in jumping phase?, if yes,
        y_ball_prev = y_ball;                                    // Set previous y-cordinate of ball to current
        y_ball = y_floor + sqrt(2 * 20 * 5) * t - 10 * t * t;    // calculate new value of y
        if ((y_ball <= (q[7] * 3.9) - 3.0) && (y_ball_prev > (q[7] * 3.9) - 3.0)) 
        {                                 // Has ball Reached the floor and going down from above, if yes,
            y_floor = (q[7] * 3.9) - 3.0; // Set the new floor value
            y_ball = y_floor;             // Set current y-cordinate of ball as floor cordinate
            ballState = 1;                // Set ball to roll state
        }
        Space = false; // Dont accept the space charactor if ball is allready in jumping phase.
        break;
    case 1:                                                       // Is ball allready in rolling phase?, if yes,
        t = 0;                                                    // Set time t to zero
        if ((int)(y_floor * 10) > (int)(10 * (q[7] * 3.9) - 3.0)) // Is floor present beneath, if not
        {
            ballState = 2; // Set ball to free fall State
        }
        else if (Space) // or is space button pressed?, if yes,
        {
            ballState = 0; // Set ball to jumping phase.
            Space = false; // Set Space flag as not pressed.
        }
        break;
    case 2:                                             // Is ball allready in free fall phase?, if yes,
        y_ball_prev = y_ball;                           // Set previous y-cordinate of ball to current
        y_ball = y_floor - 10 * t * t;                  // calculate new value of y
        if ((y_ball < (q[7] * 3.9) - 3.0) && (y_ball_prev > (q[7] * 3.9) - 3.0)) 
        {                                 // Has ball Reached the floor and going down from above, if yes,
            y_floor = (q[7] * 3.9) - 3.0; // Set the new floor value
            y_ball = y_floor;             // Set current y-cordinate of ball as floor cordinate
            ballState = 1;                // Set ball to roll state
        }
        Space = false;
        break; // Dont accept the space charactor if ball is allready in free fall phase.

    default:
        break;
    }

    glTranslatef(-7, y_ball, -10.5); // Translate ball to the right side of the screen and draw according to y_ball.
    glColor3f(1.0f, 0.1f, 0.1f);     // Set colour to red
    glutSolidSphere(0.5, 50, 50);    // Draw Solid sphere of radius 0.5 and of resolution 50x50.
    glPopMatrix();                   //glPopMatrix removes the current matrix from stack
}

void blocks()
{              //to draw the blocks randomly (in length and levels)
    int k = 0; // Used as counter to traverse through all elements in deque.
    for (int i : q)
    {
        glPushMatrix();    //glPushMatrix pushes the current matrix stack down by one
        glScalef(2, 2, 2); //glScale produces scaling along the x, y, and z axes
        glColor3f(1.5f, 1.5f, 0.0f);
        switch (i)
        {
        case 0:
            glTranslatef(-14.0 + k + x_blocks, -4.0, -10.5); //translates the cube drawn below
            glutSolidCube(1);                                //Draws cube of unit 1 at origin(0,0,0)
            break;
        case 1:
            glTranslatef(-14.0 + k + x_blocks, -0.1, -10.5); //translates the cube drawn below
            glutSolidCube(1);                                //Draws cube of unit 1 at origin(0,0,0)
            break;
        case 2:
            glTranslatef(-14.0 + k + x_blocks, 3.8, -10.5); //translates the cube drawn below
            glutSolidCube(1);                               //Draws cube of unit 1 at origin(0,0,0)
            break;
        default:
            break;
        }
        glPopMatrix(); //glPushMatrix pushes the current matrix stack down by one, duplicating the current matrix
        k++;           // Increament the counter
    }
    x_blocks -= 0.2;      // For smooth translation between two states of deque
    if (x_blocks <= -1.0) // Has translation done by one unit, if yes, time to shift elements by one unit.
    {                     // For every 5 iterations of outer loop, this block gets executed once.
        x_blocks = 0;     // Set Translation value to zero
        q.pop_front();    // Remove the element which will not be seen on screen anymore.
        if (k1 < 5)       // Maintain Distance of 5 cubes between two platforms by inserting 5 empty spaces in front of every platform.
            q.push_back(-1);
        else // if spaces allready given, draw platform according to the level given in Next variable.
            q.push_back(Next);
        k1 += 1;       // Counter to take track of inner platform formation loop, increment by 1.
        if (k1 > blen) // if time to generate new platform,
        {
            k1 = 0;                              //Set inner counter to 0.
            Next = generate(Next);               //to find out the next level
            blen = 8 + pow(rand() % 20, 2) / 20; //generating random length of block with minimum length as 8 units
        }
    }
}

void display(void)
{
    glMatrixMode(GL_MODELVIEW);        //Sets the current matrix mode to GL_MODELVIEW
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Sets the buffer(GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT) to be cleared
    glLoadIdentity();                  //Replaces the current matrix with the identity matrix
    ball();                            // Draw ball
    blocks();                          // draw blocks
    glFlush(); //Empties all commands in these buffers and forces all pending commands will to be executed immediately
}

void keyPressed(unsigned char key, int x, int y) // If any key is pressed.
{
    switch (key)
    {
    case ' ':         // Is space key pressed?, if yes,
        Space = true; // Set Space key flag as pressed.
        break;

    default:
        break;
    }
}

void myReshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);      //Specifies transformation of w and h from normalized device coordinates to window coordinates
    glMatrixMode(GL_PROJECTION); //Sets the current matrix mode to GL_PROJECTION
    glLoadIdentity();            //Replaces the current matrix with the identity matrix

    gluPerspective(60.0, (GLdouble)w / (GLdouble)h, 0.5, 100.0); //Specifies a viewing frustum into the world coordinate system
    glMatrixMode(GL_MODELVIEW); //Sets the current matrix mode to GL_MODELVIEW
    glLoadIdentity(); //Replaces the current matrix with the identity matrix
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);      //Initilising window size
    glutInitWindowPosition(200, 10);   //Initialising Window position
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); //Initialising window Modes
    glutCreateWindow("Bouncing Ball"); //Naming window
    glutDisplayFunc(display);          //Calling the function "display"
    glutKeyboardFunc(keyPressed);      //Calling the function "key pressd"
    glutTimerFunc(25, callback, 0);    //trigering "callback" function in 25ms
    glutReshapeFunc(myReshape);        //Calling "myReshape" function
    myinit();                          //Initialising variables and adding animation of blocks and ball
    glutMainLoop();                    //will call as necessary all the callbacks that have been registered.
    return 0;
}