/*
CPE/CSC 471 Lab base code Wood/Dunn/Eckhardt
*/

#include <iostream>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"

#include "WindowManager.h"
#include "Shape.h"
// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using namespace glm;
shared_ptr<Shape> shape, tinkShape;

class Wall {

public:
   float x, y, z; //maybe only x & z
};

class TinkerPos {

public:
   float x, y, z; //maybe only x & z
};

class PacmanPos {

public:
   float x, y, z; //maybe only x & z
};
static PacmanPos pacPos;
int pacLives = 4;
static float moveXPacTotal = 12., moveZPacTotal = 1.;
static vec3 moveGhost = vec3(8.0,0,-13), moveGhost2 = vec3(8.0,0,-13), moveGhost3 = vec3(8.0,0,-13), moveGhost4 = vec3(8.0,0,-13);
int tinkPosition = 0, tinkPosition2 = 0, tinkPosition3 = 0, tinkPosition4 = 0, pacManPostionNum = 0;
mat4 translateGhost = glm::translate(glm::mat4(1.0f), moveGhost);
mat4 translateGhost2 = glm::translate(glm::mat4(1.0f), vec3(0.0,0,-13));
mat4 translateGhost3 = glm::translate(glm::mat4(1.0f), vec3(8.0,0,-5));
mat4 translateGhost4 = glm::translate(glm::mat4(1.0f), vec3(2.0,0,-5));
mat4 RotateY;

void printMatrix(mat4 matrix){
   for (int i = 0; i < 4; i++) {   //print matrix
      std::cout << "| ";
      for (int j = 0; j < 4; j++) {
         std::cout << matrix[i][j] << " | ";
      }
      cout << endl;
   }
}

bool cmpfPacGhost(float A, float B, float epsilon = 1.f)
{
    return (fabs(A - B) < epsilon);
}

bool cmpfPac(float A, float B, float epsilon = 0.3f)
{
    return (fabs(A - B) < epsilon);
}

bool cmpf(float A, float B, float epsilon = 0.005f)
{
    return (fabs(A - B) < epsilon);
}


int dirUp[8]    = {1, 5, 6, 7 , 11, 12, 13 , 15};
int dirDown[8]  = {2, 6, 8, 9 , 11, 12, 14, 15};
int dirLeft[8]  = {4, 7, 9, 10, 12, 13, 14, 15};
int dirRight[8] = {3, 5, 8, 10, 11, 13, 14, 15};

float direction[8][11] = {
   { 8., 9. , 8. , 9. , 0. , 0. , 0. , 8. , 14., 14., 9. },
   { 6., 11., 7. , 6. , 0. , 0. , 0. , 6. , 6. , 6. , 6. },
   { 6., 5. , 14., 15., 10., 14., 10., 13., 15., 7. , 6. },
   { 6., 8. , 12., 11., 14., 13., 9. , 8. , 15., 9. , 6. },
   {11., 7. , 11., 7. , 6. , 0. , 11., 7. , 11., 13., 7. },
   {11., 9. , 11., 9. , 6. , 0. , 11., 9. , 11., 14., 9. },
   {6. , 5. , 12., 11., 13., 14., 7. , 5. , 12., 5. , 12.},
   {1. , 3. , 13., 13., 10., 13., 10., 10., 7. , 3. , 7. }
};

vec3 position[8][11] = {
   { vec3(-12, 0, -13), vec3(-10, 0, -13), vec3(-8, 0, -13), vec3(-6, 0, -13), vec3(0, 0, 5) ,vec3(0, 0, 5), vec3(0.0, 0, 5), vec3(2.0, 0, -13), vec3(4.0, 0, -13), vec3(6.0, 0, -13), vec3(8.0, 0, -13) },
   
   { vec3(-12.0, 0, -11), vec3(-10.0, 0, -11), vec3(-8.0, 0, -11), vec3(-6.0, 0, -11), vec3(0, 0, 5), vec3(0.0, 0, 5) ,vec3(0.0, 0, 5), vec3(2.0, 0, -11), vec3(4.0, 0, -11), vec3(6.0, 0, -11), vec3(8.0, 0, -11) },
   
   { vec3(-12.0, 0, -9), vec3(-10.0, 0, -9), vec3(-8.0, 0, -9), vec3(-6.0, 0, -9), vec3(-4.0, 0, -9), vec3(-2.0, 0, -9), vec3(0.0, 0, -9), vec3(2.0, 0, -9), vec3(4.0, 0, -9), vec3(6.0, 0, -9), vec3(8.0, 0, -9) },
   
   { vec3(-12.0, 0, -7), vec3(-10.0, 0, -7), vec3(-8.0, 0, -7), vec3(-6.0, 0, -7), vec3(-4.0, 0, -7), vec3(-2.0, 0, -7), vec3(0.0, 0, -7), vec3(2.0, 0, -7), vec3(4.0, 0, -7), vec3(6.0, 0, -7), vec3(8.0, 0, -7) } ,
   
   { vec3(-12.0, 0, -5), vec3(-10.0, 0, -5), vec3(-8.0, 0, -5), vec3(-6.0, 0, -5), vec3(-4.0, 0, -5), vec3(-2.0, 0, -5), vec3(0.0, 0, -5), vec3(2.0, 0, -5), vec3(4.0, 0, -5), vec3(6.0, 0, -5), vec3(8.0, 0, -5) },
   
   { vec3(-12.0, 0, -3), vec3(-10.0, 0, -3), vec3(-8.0, 0, -3), vec3(-6.0, 0, -3), vec3(-4.0, 0, -3), vec3(-2.0, 0, -3), vec3(0.0, 0, -3), vec3(2.0, 0, -3), vec3(4.0, 0, -3), vec3(6.0, 0, -3), vec3(8.0, 0, -3) },
   
   { vec3(-12.0, 0, -1), vec3(-10.0, 0, -1), vec3(-8.0, 0, -1), vec3(-6.0, 0, -1), vec3(-4.0, 0, -1), vec3(-2.0, 0, -1), vec3(0.0, 0, -1), vec3(2.0, 0, -1), vec3(4.0, 0, -1), vec3(6.0, 0, -1), vec3(8.0, 0, -1) },
   
   { vec3(-12.0, 0, 1), vec3(-10.0, 0, 1), vec3(-8.0, 0, 1), vec3(-6.0, 0, 1), vec3(-4.0, 0, 1), vec3(-2.0, 0, 1), vec3(0.0, 0, 1), vec3(2.0, 0, 1), vec3(4.0, 0, 1), vec3(6.0, 0, 1), vec3(8.0, 0, 1) }
   
};

vec3 positionRing[17][23] = {
   
   { vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5),  vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5)},

   { vec3(0, 0, 5), vec3(-12, 0, -13), vec3(-11, 0, -13), vec3(-10, 0, -13), vec3(0, 0, 5), vec3(-8, 0, -13), vec3(-7, 0, -13), vec3(-6, 0, -13), vec3(0, 0, 5) ,vec3(0, 0, 5), vec3(0.0, 0, 5), vec3(0, 0, 5),vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(2.0, 0, -13), vec3(3, 0, -13), vec3(4.0, 0, -13), vec3(5, 0, -13), vec3(6.0, 0, -13), vec3(7, 0, -13),vec3(8.0, 0, -13), vec3(0, 0, 5)},
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -12), vec3(0, 0, 5), vec3(-10.0, 0, -12), vec3(0, 0, 5), vec3(-8.0, 0, -12), vec3(0, 0, 5), vec3(-6.0, 0, -12), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0.0, 0, 5) ,vec3(0.0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(2.0, 0, -12), vec3(0, 0, 5), vec3(4.0, 0, -12), vec3(0, 0, 5), vec3(6.0, 0, -12), vec3(0, 0, 5), vec3(8.0, 0, -12), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -11), vec3(0, 0, 5), vec3(-10.0, 0, -11), vec3(-9.0, 0, -11), vec3(-8.0, 0, -11), vec3(0, 0, 5), vec3(-6.0, 0, -11), vec3(0.0, 0, 5) ,vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(2.0, 0, -11), vec3(0.0, 0, 5), vec3(4.0, 0, -11), vec3(0.0, 0, 5), vec3(6.0, 0, -11), vec3(0.0, 0, 5), vec3(8.0, 0, -11), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -10), vec3(0, 0, 5), vec3(-10.0, 0, -10), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(-6.0, 0, -10), vec3(0.0, 0, 5) ,vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(2.0, 0, -10), vec3(0.0, 0, 5), vec3(4.0, 0, -10), vec3(0.0, 0, 5), vec3(6.0, 0, -10), vec3(0.0, 0, 5), vec3(8.0, 0, -10), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -9), vec3(0, 0, 5), vec3(-10.0, 0, -9), vec3(-9.0, 0, -9), vec3(-8.0, 0, -9), vec3(-7.0, 0, -9), vec3(-6.0, 0, -9), vec3(-5.0, 0, -9), vec3(-4.0, 0, -9), vec3(-3.0, 0, -9), vec3(-2.0, 0, -9), vec3(-1.0, 0, -9), vec3(0.0, 0, -9), vec3(1.0, 0, -9), vec3(2.0, 0, -9), vec3(3.0, 0, -9), vec3(4.0, 0, -9), vec3(5.0, 0, -9), vec3(6.0, 0, -9), vec3(0, 0, 5), vec3(8.0, 0, -9), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -8), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(-8.0, 0, -8), vec3(0, 0, 5), vec3(-6.0, 0, -8), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(-2.0, 0, -8), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(4.0, 0, -8), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(8.0, 0, -8), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -7), vec3(0, 0, 5), vec3(-10.0, 0, -7), vec3(-9.0, 0, -7), vec3(-8.0, 0, -7), vec3(0, 0, 5), vec3(-6.0, 0, -7), vec3(-5.0, 0, -7), vec3(-4.0, 0, -7), vec3(-3.0, 0, -7), vec3(-2.0, 0, -7), vec3(-1.0, 0, -7), vec3(0.0, 0, -7), vec3(0, 0, 5), vec3(2.0, 0, -7), vec3(3.0, 0, -7), vec3(4.0, 0, -7), vec3(5.0, 0, -7), vec3(6.0, 0, -7), vec3(0, 0, 5) ,vec3(8.0, 0, -7), vec3(0, 0, 5) } ,
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -6), vec3(0, 0, 5), vec3(-10.0, 0, -6), vec3(0.0, 0, 5), vec3(-8.0, 0, -6), vec3(0, 0, 5), vec3(-6.0, 0, -6), vec3(0, 0, 5), vec3(-4.0, 0, -6), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0.0, 0, -6), vec3(0, 0, 5), vec3(2.0, 0, -6), vec3(0, 0, 5), vec3(4.0, 0, -6), vec3(0.0, 0, 5), vec3(6.0, 0, -6), vec3(0, 0, 5), vec3(8.0, 0, -6), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -5), vec3(-11.0, 0, -5),  vec3(-10.0, 0, -5), vec3(0, 0, 5), vec3(-8.0, 0, -5), vec3(-7.0, 0, -5), vec3(-6.0, 0, -5), vec3(0, 0, 5), vec3(-4.0, 0, -5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0.0, 0, -5), vec3(1.0, 0, -5), vec3(2.0, 0, -5), vec3(0, 0, 5), vec3(4.0, 0, -5), vec3(5.0, 0, -5), vec3(6.0, 0, -5), vec3(7.0, 0, -5), vec3(8.0, 0, -5), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -4), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(-8.0, 0, -4), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(-4.0, 0, -4), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0.0, 0, -4), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(4.0, 0, -4), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -3), vec3(-11.0, 0, -3), vec3(-10.0, 0, -3), vec3(0, 0, 5), vec3(-8.0, 0, -3), vec3(-7.0, 0, -3), vec3(-6.0, 0, -3), vec3(0, 0, 5), vec3(-4.0, 0, -3), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0.0, 0, -3), vec3(1.0, 0, -3), vec3(2.0, 0, -3), vec3(0, 0, 5), vec3(4.0, 0, -3), vec3(5.0, 0, -3), vec3(6.0, 0, -3), vec3(7.0, 0, -3), vec3(8.0, 0, -3), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -2), vec3(0, 0, 5), vec3(-10.0, 0, -2), vec3(0, 0, 5), vec3(-8.0, 0, -2), vec3(0, 0, 5), vec3(-6.0, 0, -2), vec3(0, 0, 5), vec3(-4.0, 0, -2), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0.0, 0, -2), vec3(0, 0, 5), vec3(2.0, 0, -2), vec3(0, 0, 5), vec3(4.0, 0, -2), vec3(0, 0, 5), vec3(6.0, 0, -2), vec3(0.0, 0, 5), vec3(8.0, 0, -2), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -1), vec3(0, 0, 5), vec3(-10.0, 0, -1), vec3(-9, 0, -1), vec3(-8.0, 0, -1), vec3(0, 0, 5), vec3(-6.0, 0, -1), vec3(-5., 0, -1.), vec3(-4.0, 0, -1.), vec3(-3., 0, -1.), vec3(-2., 0, -1.), vec3(-1., 0, -1.), vec3(0.0, 0, -1), vec3(0, 0, 5), vec3(2.0, 0, -1), vec3(3., 0, -1), vec3(4.0, 0, -1), vec3(0, 0, 5), vec3(6.0, 0, -1), vec3(7.0, 0, -1), vec3(8.0, 0, -1) , vec3(0, 0, 5)},
   
   { vec3(0, 0, 5), vec3(-12.0, 0, 0), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(-8.0, 0, 0), vec3(0, 0, 5), vec3(-6.0, 0, 0), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(-2., 0, 0.), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(4.0, 0, 0.), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(8.0, 0, 0.) , vec3(0, 0, 5)},
   
   { vec3(0, 0, 5), vec3(-12.0, 0, 1), vec3(0, 0, 5), vec3(-10.0, 0, 1), vec3(-9.0, 0, 1), vec3(-8.0, 0, 1), vec3(-7.0, 0, 1), vec3(-6.0, 0, 1), vec3(-5.0, 0, 1), vec3(-4.0, 0, 1), vec3(-3.0, 0, 1), vec3(-2.0, 0, 1), vec3(-1.0, 0, 1), vec3(0.0, 0, 1), vec3(1.0, 0, 1), vec3(2.0, 0, 1), vec3(3.0, 0, 1),  vec3(4.0, 0, 1), vec3(0, 0, 5), vec3(6.0, 0, 1), vec3(7., 0, 1), vec3(8.0, 0, 1), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5),  vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5) }
   
};

vec3 tempPositionRing[17][23] = {
   
   { vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5),  vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5)},

   { vec3(0, 0, 5), vec3(-12, 0, -13), vec3(-11, 0, -13), vec3(-10, 0, -13), vec3(0, 0, 5), vec3(-8, 0, -13), vec3(-7, 0, -13), vec3(-6, 0, -13), vec3(0, 0, 5) ,vec3(0, 0, 5), vec3(0.0, 0, 5), vec3(0, 0, 5),vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(2.0, 0, -13), vec3(3, 0, -13), vec3(4.0, 0, -13), vec3(5, 0, -13), vec3(6.0, 0, -13), vec3(7, 0, -13),vec3(8.0, 0, -13), vec3(0, 0, 5)},
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -12), vec3(0, 0, 5), vec3(-10.0, 0, -12), vec3(0, 0, 5), vec3(-8.0, 0, -12), vec3(0, 0, 5), vec3(-6.0, 0, -12), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0.0, 0, 5) ,vec3(0.0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(2.0, 0, -12), vec3(0, 0, 5), vec3(4.0, 0, -12), vec3(0, 0, 5), vec3(6.0, 0, -12), vec3(0, 0, 5), vec3(8.0, 0, -12), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -11), vec3(0, 0, 5), vec3(-10.0, 0, -11), vec3(-9.0, 0, -11), vec3(-8.0, 0, -11), vec3(0, 0, 5), vec3(-6.0, 0, -11), vec3(0.0, 0, 5) ,vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(2.0, 0, -11), vec3(0.0, 0, 5), vec3(4.0, 0, -11), vec3(0.0, 0, 5), vec3(6.0, 0, -11), vec3(0.0, 0, 5), vec3(8.0, 0, -11), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -10), vec3(0, 0, 5), vec3(-10.0, 0, -10), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(-6.0, 0, -10), vec3(0.0, 0, 5) ,vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(0.0, 0, 5), vec3(2.0, 0, -10), vec3(0.0, 0, 5), vec3(4.0, 0, -10), vec3(0.0, 0, 5), vec3(6.0, 0, -10), vec3(0.0, 0, 5), vec3(8.0, 0, -10), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -9), vec3(0, 0, 5), vec3(-10.0, 0, -9), vec3(-9.0, 0, -9), vec3(-8.0, 0, -9), vec3(-7.0, 0, -9), vec3(-6.0, 0, -9), vec3(-5.0, 0, -9), vec3(-4.0, 0, -9), vec3(-3.0, 0, -9), vec3(-2.0, 0, -9), vec3(-1.0, 0, -9), vec3(0.0, 0, -9), vec3(1.0, 0, -9), vec3(2.0, 0, -9), vec3(3.0, 0, -9), vec3(4.0, 0, -9), vec3(5.0, 0, -9), vec3(6.0, 0, -9), vec3(0, 0, 5), vec3(8.0, 0, -9), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -8), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(-8.0, 0, -8), vec3(0, 0, 5), vec3(-6.0, 0, -8), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(-2.0, 0, -8), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(4.0, 0, -8), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(8.0, 0, -8), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -7), vec3(0, 0, 5), vec3(-10.0, 0, -7), vec3(-9.0, 0, -7), vec3(-8.0, 0, -7), vec3(0, 0, 5), vec3(-6.0, 0, -7), vec3(-5.0, 0, -7), vec3(-4.0, 0, -7), vec3(-3.0, 0, -7), vec3(-2.0, 0, -7), vec3(-1.0, 0, -7), vec3(0.0, 0, -7), vec3(0, 0, 5), vec3(2.0, 0, -7), vec3(3.0, 0, -7), vec3(4.0, 0, -7), vec3(5.0, 0, -7), vec3(6.0, 0, -7), vec3(0, 0, 5) ,vec3(8.0, 0, -7), vec3(0, 0, 5) } ,
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -6), vec3(0, 0, 5), vec3(-10.0, 0, -6), vec3(0.0, 0, 5), vec3(-8.0, 0, -6), vec3(0, 0, 5), vec3(-6.0, 0, -6), vec3(0, 0, 5), vec3(-4.0, 0, -6), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0.0, 0, -6), vec3(0, 0, 5), vec3(2.0, 0, -6), vec3(0, 0, 5), vec3(4.0, 0, -6), vec3(0.0, 0, 5), vec3(6.0, 0, -6), vec3(0, 0, 5), vec3(8.0, 0, -6), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -5), vec3(-11.0, 0, -5),  vec3(-10.0, 0, -5), vec3(0, 0, 5), vec3(-8.0, 0, -5), vec3(-7.0, 0, -5), vec3(-6.0, 0, -5), vec3(0, 0, 5), vec3(-4.0, 0, -5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0.0, 0, -5), vec3(1.0, 0, -5), vec3(2.0, 0, -5), vec3(0, 0, 5), vec3(4.0, 0, -5), vec3(5.0, 0, -5), vec3(6.0, 0, -5), vec3(7.0, 0, -5), vec3(8.0, 0, -5), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -4), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(-8.0, 0, -4), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(-4.0, 0, -4), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0.0, 0, -4), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(4.0, 0, -4), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -3), vec3(-11.0, 0, -3), vec3(-10.0, 0, -3), vec3(0, 0, 5), vec3(-8.0, 0, -3), vec3(-7.0, 0, -3), vec3(-6.0, 0, -3), vec3(0, 0, 5), vec3(-4.0, 0, -3), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0.0, 0, -3), vec3(1.0, 0, -3), vec3(2.0, 0, -3), vec3(0, 0, 5), vec3(4.0, 0, -3), vec3(5.0, 0, -3), vec3(6.0, 0, -3), vec3(7.0, 0, -3), vec3(8.0, 0, -3), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -2), vec3(0, 0, 5), vec3(-10.0, 0, -2), vec3(0, 0, 5), vec3(-8.0, 0, -2), vec3(0, 0, 5), vec3(-6.0, 0, -2), vec3(0, 0, 5), vec3(-4.0, 0, -2), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0.0, 0, -2), vec3(0, 0, 5), vec3(2.0, 0, -2), vec3(0, 0, 5), vec3(4.0, 0, -2), vec3(0, 0, 5), vec3(6.0, 0, -2), vec3(0.0, 0, 5), vec3(8.0, 0, -2), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(-12.0, 0, -1), vec3(0, 0, 5), vec3(-10.0, 0, -1), vec3(-9, 0, -1), vec3(-8.0, 0, -1), vec3(0, 0, 5), vec3(-6.0, 0, -1), vec3(-5., 0, -1.), vec3(-4.0, 0, -1.), vec3(-3., 0, -1.), vec3(-2., 0, -1.), vec3(-1., 0, -1.), vec3(0.0, 0, -1), vec3(0, 0, 5), vec3(2.0, 0, -1), vec3(3., 0, -1), vec3(4.0, 0, -1), vec3(0, 0, 5), vec3(6.0, 0, -1), vec3(7.0, 0, -1), vec3(8.0, 0, -1) , vec3(0, 0, 5)},
   
   { vec3(0, 0, 5), vec3(-12.0, 0, 0), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(-8.0, 0, 0), vec3(0, 0, 5), vec3(-6.0, 0, 0), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(-2., 0, 0.), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(4.0, 0, 0.), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(8.0, 0, 0.) , vec3(0, 0, 5)},
   
   { vec3(0, 0, 5), vec3(-12.0, 0, 1), vec3(0, 0, 5), vec3(-10.0, 0, 1), vec3(-9.0, 0, 1), vec3(-8.0, 0, 1), vec3(-7.0, 0, 1), vec3(-6.0, 0, 1), vec3(-5.0, 0, 1), vec3(-4.0, 0, 1), vec3(-3.0, 0, 1), vec3(-2.0, 0, 1), vec3(-1.0, 0, 1), vec3(0.0, 0, 1), vec3(1.0, 0, 1), vec3(2.0, 0, 1), vec3(3.0, 0, 1),  vec3(4.0, 0, 1), vec3(0, 0, 5), vec3(6.0, 0, 1), vec3(7., 0, 1), vec3(8.0, 0, 1), vec3(0, 0, 5) },
   
   { vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5),  vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5), vec3(0, 0, 5) }
   
};

int checkBalls(){
   for (int i = 0; i < 17; i++){
      for (int j = 0; j < 23; j ++){
         if (positionRing[i][j].x != 0 && positionRing[i][j].z != 5)
            return 1;
      }
   }
   return  0;
}

bool isInArray(vec3 pos, vec3 **array) {
   int rows = sizeof array / sizeof array[0];
   int cols = sizeof array[0] / sizeof(int);
   for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
         if (cmpf(pos.x, (array[i][j]).x) && cmpf(pos.z, (array[i][j]).z))
            return true;
      }
   }
   return false;
}

int takePacVector(vec3 pacPosition) {
   for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 11; j++) {
         if (cmpfPac(pacPosition.x, (position[i][j]).x) && cmpfPac(pacPosition.z, (position[i][j]).z))
            return direction[i][j];
      }
   }
   return 0;
}

int takeVector(vec3 ghostPos) {
   for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 11; j++) {
         if (cmpf(ghostPos.x, (position[i][j]).x) && cmpf(ghostPos.z, (position[i][j]).z))
            return direction[i][j];
      }
   }
   return 0;
}

vec3 moveCharacter(int num, unsigned randVal, vec3 ghostPos, vec3 pacPos){
   srand (time(0) * randVal);
   int i = 0, j = 0, tempi, tempj;
   vec3 getPos;
   vec3 up, down, left, right;
   up = vec3(0,0,-2);
   down = vec3(0,0,2);
   left = vec3(-2,0,0);
   right = vec3(2,0,0);
   vec3 dir5[4] = {up, right, up, right};
   vec3 dir6[5] = {up, down, up, down, down};
   vec3 dir7[4] = {up, left, up, left};
   vec3 dir8[6] = {down, right, down, right, down, down};
   vec3 dir9[5] = {down, left, down, left, left};
   vec3 dir10[5] = {left, right, left, right, left};
   vec3 dir11[8] = {up, right ,right, down, up, right, down, right};
   vec3 dir12[6] = {up, left, down, up, left, down};
   vec3 dir13[6] = {up, right, left, up, right, left};
   vec3 dir14[8] = {down, right, left, down, right, left, down, left};
   vec3 dir15[8] = {up, down, left, right, up, down, left, right};
   
   // check whether or not the pacman is on your sight
   //example:
   //ghost vector position (-4,0,-7)
   //ghost direction num = 14

   for (i = 0; i < 17; i ++) {
      for (j = 0; j < 23; j++) {
         if (i == 9){
            cout << "" << endl;
         }
         if (cmpf(ghostPos.x, (positionRing[i][j]).x) && cmpf(ghostPos.z, (positionRing[i][j]).z)){
            goto label;
         }
      }
   }
   //get the i, j for the ghost positions
   label:
   getPos = positionRing[i][j];
   tempi = i;
   tempj = j;
   
   switch(num) {
      case 1:
         return up;
         break;
      case 2:
         return down;
         break;
      case 3:
         return right;
         break;
      case 4:
         return left;
         break;
      case 5:
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //check right
            j++;
            if (j > 23) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return right;
            }
         }
         // if pacman not on the right then check up
         // back to start
         i = tempi;
         j = tempj;
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //up
            i--;
            if (i < 0) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return up;
            }
         }
         i = tempi;
         j = tempj;
         break;
      case 6:
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //check up
            i--;
            if (i < 0) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return up;
            }
         }
         // back to start
         i = tempi;
         j = tempj;
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //down
            i++;
            if (i > 17) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return down;
            }
         }
         i = tempi;
         j = tempj;
         break;
      case 7:
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //check left
            j--;
            if (j > 23) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return left;
            }
         }
         // back to start
         i = tempi;
         j = tempj;
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //up
            i--;
            if (i < 0) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return up;
            }
         }
         i = tempi;
         j = tempj;
         break;
      case 8:
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //check right
            j++;
            if (j > 23) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return right;
            }
         }
         // back to start
         i = tempi;
         j = tempj;
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //down
            i++;
            if (i > 17) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return down;
            }
         }
         i = tempi;
         j = tempj;

         break;
      case 9:
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //check left
            j--;
            if (j < 0) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return left;
            }
         }
         // back to start
         i = tempi;
         j = tempj;
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //down
            i++;
            if (i > 17) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return down;
            }
         }
         i = tempi;
         j = tempj;
         
         break;
      case 10:
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //check left
            j--;
            if (j < 0) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return left;
            }
         }
         // back to start
         i = tempi;
         j = tempj;
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //right
            j++;
            if (j > 23) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return right;
            }
         }
         i = tempi;
         j = tempj;
         break;
      case 11:
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //check up
            i--;
            if (i < 0) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return up;
            }
         }
         // back to start
         i = tempi;
         j = tempj;
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //down
            i++;
            if (i > 17) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return down;
            }
         }
         i = tempi;
         j = tempj;
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //right
            j++;
            if (j > 23) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return right;
            }
         }
         i = tempi;
         j = tempj;
         break;
      case 12:
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //check up
            i--;
            if (i < 0) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return up;
            }
         }
         // back to start
         i = tempi;
         j = tempj;
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //down
            i++;
            if (i > 17) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return down;
            }
         }
         i = tempi;
         j = tempj;
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //check left
            j--;
            if (j < 0) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return left;
            }
         }
         // back to start
         i = tempi;
         j = tempj;
         break;
      case 13:
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //check left
            j--;
            if (j < 0) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return left;
            }
         }
         // back to start
         i = tempi;
         j = tempj;
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //right
            j++;
            if (j > 23) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return right;
            }
         }
         i = tempi;
         j = tempj;
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //up
            i--;
            if (i < 0) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return up;
            }
         }
         i = tempi;
         j = tempj;
         break;
      case 14:
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //check left
            j--;
            if (j < 0) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return left;
            }
         }
         // back to start
         i = tempi;
         j = tempj;
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //right
            j++;
            if (j > 23) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return right;
            }
         }
         i = tempi;
         j = tempj;
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //down
            i++;
            if (i > 17) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return down;
            }
         }
         i = tempi;
         j = tempj;
         break;
      case 15:
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //check left
            j--;
            if (j < 0) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return left;
            }
         }
         // back to start
         i = tempi;
         j = tempj;
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //right
            j++;
            if (j > 23) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return right;
            }
         }
         i = tempi;
         j = tempj;
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //down
            i++;
            if (i > 17) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return down;
            }
         }
         i = tempi;
         j = tempj;
         while (!(cmpf(tempPositionRing[i][j].x, 0.) && cmpf(tempPositionRing[i][j].z, 5.))) {
            //up
            i--;
            if (i < 0) {
               break;
            }
            if ((cmpfPacGhost(tempPositionRing[i][j].x, pacPos.x) && cmpfPacGhost(tempPositionRing[i][j].z, pacPos.z))){
               return up;
            }
         }
         i = tempi;
         j = tempj;
         break;
   }
   
   int opt510 = rand() % 1;
   int opt1114 =  rand() % 2;
   int opt15 = rand() % 3;
   switch(num) {
      case 1:
         return up;
         break;
      case 2:
         return down;
         break;
      case 3:
         return right;
         break;
      case 4:
         return left;
         break;
      case 5:
         opt510 = rand() % 3;
         return dir5[opt510];
         break;
      case 6:
         opt510 = rand() % 4;
         return dir6[opt510];
         break;
      case 7:
         opt510 = rand() % 3;
         return dir7[opt510];
         break;
      case 8:
         opt510 = rand() % 5;
         return dir8[opt510];
         break;
      case 9:
         opt510 = rand() % 4;
         return dir9[opt510];
         break;
      case 10:
         opt510 = rand() % 4;
         return dir10[opt510];
         break;
      case 11:
         opt1114 =  rand() % 7;
         return dir11[opt1114];
         break;
      case 12:
         opt1114 =  rand() % 5;
         return dir12[opt1114];
         break;
      case 13:
         opt1114 =  rand() % 5;
         return dir13[opt1114];
         break;
      case 14:
         opt1114 =  rand() % 7;
         return dir14[opt1114];
         break;
      case 15:
         opt15 = rand() % 7;
         return dir15[opt15];
         break;
      default:
         vec3(0,0,0);
   }
   return vec3(0,0,0);
}

bool isMultipleof2 (int n)
{
    while ( n > 0.00 )
        n = n - 7;
  
    if (n == 0.00)
        return true;
  
    return false;
}

double get_last_elapsed_time()
{
	static double lasttime = glfwGetTime();
	double actualtime =glfwGetTime();
	double difference = actualtime- lasttime;
	lasttime = actualtime;
	return difference;
}
class camera
{
public:
   glm::vec3 pos, rot;
   int w, a, s, d;
   int q, e;
   camera()
   {
      w = a = s = d = q = e = 0;
      pos = rot = glm::vec3(0, 0, 0);
//      pos = vec3(0,0,5);
      // for presentation milestone 2
      pos = vec3(2, -20, 6.);
      rot.x = 3.14/2;
   }
   glm::mat4 process(double ftime)
   {
      float speed = 0;
      if (w == 1)
      {
         speed = .5 *ftime;
      }
      else if (s == 1)
      {
         speed = -.5*ftime;
      }
      float yangle=0;
      if (a == 1)
         yangle = -.5*ftime;
      else if(d==1)
         yangle = .5*ftime;
      
      float zangle = 0;
      if (q == 1) {
         zangle = -.5 * ftime;
      }
      else if(e == 1) {
         zangle = .5 * ftime;
      }
      rot.y += yangle;
      rot.x += zangle;
      glm::mat4 Rx = glm::rotate(glm::mat4(1), rot.x, glm::vec3(1, 0, 0));
      glm::mat4 Ry = glm::rotate(glm::mat4(1), rot.y, glm::vec3(0, 1, 0));
      glm::vec4 dir = glm::vec4(0, 0, speed,1);
      dir = dir * Rx * Ry;
      pos += glm::vec3(dir.x, dir.y, dir.z);
      glm::mat4 T = glm::translate(glm::mat4(1), pos);
      
      return Rx * Ry * T;
   }
};

camera mycam;
int pacmanAlive = 1;
int hasWon = 1;
int gameDone = 1;
class Application : public EventCallbacks
{
   
public:

	WindowManager * windowManager = nullptr;
	// Our shader program
   std::shared_ptr<Program> prog, psky, walls, progball, tink1, tink2, tink3, tink4, ring, pacDying, gameOver, lives, won;

	// Contains vertex information for OpenGL
   GLuint VertexArrayID;
   GLuint VertexArrayID2;
   GLuint VertexArrayIDWall;

	// Data necessary to give our box to OpenGL
	GLuint VertexBufferID, VertexBufferID2, VertexNormDBox, VertexTexBox, IndexBufferIDBox, VertexBufferIDWall;

	//texture data
	GLuint Texture, TextureN, TextureWon;
	GLuint Texture2, Texture3, Texture4, Texture5, TextureRing, TexturGameOver, TextureLives, TexturePacDying;
   int gup = 0, gdown = 0, gleft = 0, gright = 0;
	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{
			mycam.w = 1;
		}
		if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			mycam.w = 0;
		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			mycam.s = 1;
		}
		if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		{
			mycam.s = 0;
		}
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			mycam.a = 1;
		}
		if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
			mycam.a = 0;
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			mycam.d = 1;
		}
		if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			mycam.d = 0;
		}
      if (key == GLFW_KEY_Q && action == GLFW_PRESS)
      {
         mycam.q = 1;
      }
      if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
      {
         mycam.q = 0;
      }
      if (key == GLFW_KEY_E && action == GLFW_PRESS)
      {
         mycam.e = 1;
      }
      if (key == GLFW_KEY_E && action == GLFW_RELEASE)
      {
         mycam.e = 0;
      }
      
      if (key == GLFW_KEY_UP && action == GLFW_PRESS)
      {
         gup = 1;
         gdown = 0;
         gright = 0;
         gleft = 0;
      }
      if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
      {
         gup = 0;
         gdown = 1;
         gright = 0;
         gleft = 0;
      }
      if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
      {
         gup = 0;
         gdown = 0;
         gright = 0;
         gleft = 1;
      }
      if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
      {
         gup = 0;
         gdown = 0;
         gright = 1;
         gleft = 0;
      }
      
	}

	// callback for the mouse when clicked move the triangle when helper functions
	// written
	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;
		float newPt[2];
		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			std::cout << "Pos X " << posX <<  " Pos Y " << posY << std::endl;

			//change this to be the points converted to WORLD
			//THIS IS BROKEN< YOU GET TO FIX IT - yay!
			newPt[0] = 0;
			newPt[1] = 0;

			std::cout << "converted:" << newPt[0] << " " << newPt[1] << std::endl;
			glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
			//update the vertex array with the updated points
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*6, sizeof(float)*2, newPt);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	//if the window is resized, capture the new size and reset the viewport
	void resizeCallback(GLFWwindow *window, int in_width, int in_height)
	{
		//get the window size - may be different then pixels for retina
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}

	/*Note that any gl calls must always happen after a GL state is initialized */
	void initGeom()
	{
      //grass
		//generate the VAO
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		//generate vertex buffer to hand off to OGL
		glGenBuffers(1, &VertexBufferID);
		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);

		GLfloat rect_vertices[] = {
			// front
			-1.0, -1.0,  1.0,//LD
			1.0, -1.0,  1.0,//RD
			1.0,  1.0,  1.0,//RU
			-1.0,  1.0,  1.0,//LU
		};
		//make it a bit smaller
		for (int i = 0; i < 12; i++)
			rect_vertices[i] *= 0.5;
		//actually memcopy the data - only do this once
		glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_DYNAMIC_DRAW);

		//we need to set up the vertex array
		glEnableVertexAttribArray(0);
		//key function to get up how many elements to pull out at a time (3)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		//color
		GLfloat cube_norm[] = {
			// front colors
			0.0, 0.0, 1.0,
			0.0, 0.0, 1.0,
			0.0, 0.0, 1.0,
			0.0, 0.0, 1.0,

		};
		glGenBuffers(1, &VertexNormDBox);
		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, VertexNormDBox);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_norm), cube_norm, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		//color
		glm::vec2 cube_tex[] = {
			// front colors
			glm::vec2(0.0, 2.0),
			glm::vec2(2.0, 2.0),
			glm::vec2(2.0, 0.0),
			glm::vec2(0.0, 0.0),

		};
		glGenBuffers(1, &VertexTexBox);
		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, VertexTexBox);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_tex), cube_tex, GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &IndexBufferIDBox);
		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
		GLushort cube_elements[] = {

			// front
			0, 1, 2,
			2, 3, 0,
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

      // ------ walls
      //generate the VAO
      glGenVertexArrays(1, &VertexArrayIDWall);
      glBindVertexArray(VertexArrayIDWall);
      
      //generate vertex buffer to hand off to OGL
      glGenBuffers(1, &VertexBufferIDWall);
      //set the current state to focus on our vertex buffer
      glBindBuffer(GL_ARRAY_BUFFER, VertexBufferIDWall);
      
      GLfloat cube_vertices[] = {
         // front
         -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,
         -1.0,  1.0,  1.0,
         // back
         -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
         -1.0,  1.0, -1.0,
         //tube 8 - 11
         -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,
         -1.0,  1.0,  1.0,
         //12 - 15
         -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
         -1.0,  1.0, -1.0
      };
      //actually memcopy the data - only do this once
      glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_DYNAMIC_DRAW);

      //we need to set up the vertex array
      glEnableVertexAttribArray(0);
      //key function to get up how many elements to pull out at a time (3)
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
      glGenBuffers(1, &IndexBufferIDBox);
      //set the current state to focus on our vertex buffer
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
      GLushort cube_elements2[] = {
         // front
         0, 1, 2,
         2, 3, 0,
         // back
         7, 6, 5,
         5, 4, 7,
         //tube 8-11, 12-15
         8,12,13,
         8,13,9,
         9,13,14,
         9,14,10,
         10,14,15,
         10,15,11,
         11,15,12,
         11,12,8
         
      };
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements2), cube_elements2, GL_STATIC_DRAW);
      glBindVertexArray(0);
      // ---------------------------------------

		string resourceDirectory = "../../resources" ;
		// Initialize mesh.
		shape = make_shared<Shape>();
		//shape->loadMesh(resourceDirectory + "/t800.obj");
		shape->loadMesh(resourceDirectory + "/sphere.obj");
		shape->resize();
		shape->init();
      
      tinkShape = make_shared<Shape>();
      tinkShape->loadMesh(resourceDirectory + "/tinker.obj");
      tinkShape->resize();
      tinkShape->init();

		int width, height, channels;
		char filepath[1000];

		//texture 1
		string str = resourceDirectory + "/pacman2.png";
		strcpy(filepath, str.c_str());
		unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &Texture);
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		//texture Night
		str = resourceDirectory + "/night.jpg";
		strcpy(filepath, str.c_str());
		data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &TextureN);
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureN);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		//texture 2
		str = resourceDirectory + "/tinkredcopy.png";
		strcpy(filepath, str.c_str());
		data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &Texture2);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Texture2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
      
      //texture 2
      str = resourceDirectory + "/PacDying.png";
      strcpy(filepath, str.c_str());
      data = stbi_load(filepath, &width, &height, &channels, 4);
      glGenTextures(1, &TexturePacDying);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, TexturePacDying);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      
      str = resourceDirectory + "/gameover.png";
      strcpy(filepath, str.c_str());
      data = stbi_load(filepath, &width, &height, &channels, 4);
      glGenTextures(1, &TexturGameOver);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, TexturGameOver);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      
      str = resourceDirectory + "/win.png";
      strcpy(filepath, str.c_str());
      data = stbi_load(filepath, &width, &height, &channels, 4);
      glGenTextures(1, &TextureWon);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, TextureWon);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      
      str = resourceDirectory + "/lives.png";
      strcpy(filepath, str.c_str());
      data = stbi_load(filepath, &width, &height, &channels, 4);
      glGenTextures(1, &TextureLives);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, TextureLives);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      
      //texture 2
      str = resourceDirectory + "/tinkorange.png";
      strcpy(filepath, str.c_str());
      data = stbi_load(filepath, &width, &height, &channels, 4);
      glGenTextures(1, &Texture3);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, Texture3);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      
      //texture 2
      str = resourceDirectory + "/pinky.png";
      strcpy(filepath, str.c_str());
      data = stbi_load(filepath, &width, &height, &channels, 4);
      glGenTextures(1, &Texture5);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, Texture5);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      
      //texture 2
      str = resourceDirectory + "/lightbluetink.png";
      strcpy(filepath, str.c_str());
      data = stbi_load(filepath, &width, &height, &channels, 4);
      glGenTextures(1, &Texture4);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, Texture4);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      //texture 2
      str = resourceDirectory + "/yellowring.png";
      strcpy(filepath, str.c_str());
      data = stbi_load(filepath, &width, &height, &channels, 4);
      glGenTextures(1, &TextureRing);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, TextureRing);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

		//[TWOTEXTURES]
		//set the 2 textures to the correct samplers in the fragment shader:
		GLuint Tex1Location = glGetUniformLocation(prog->pid, "tex");//tex, tex2... sampler in the fragment shader
		GLuint Tex2Location = glGetUniformLocation(prog->pid, "tex2");
		// Then bind the uniform samplers to texture units:
		glUseProgram(prog->pid);
		glUniform1i(Tex1Location, 0);
		glUniform1i(Tex2Location, 1);

      Tex1Location = glGetUniformLocation(pacDying->pid, "tex");//tex, tex2... sampler in the fragment shader
      Tex2Location = glGetUniformLocation(pacDying->pid, "tex2");
      // Then bind the uniform samplers to texture units:
      glUseProgram(pacDying->pid);
      glUniform1i(Tex1Location, 0);
      glUniform1i(Tex2Location, 1);
      
      Tex1Location = glGetUniformLocation(won->pid, "tex");//tex, tex2... sampler in the fragment shader
      Tex2Location = glGetUniformLocation(won->pid, "tex2");
      // Then bind the uniform samplers to texture units:
      glUseProgram(won->pid);
      glUniform1i(Tex1Location, 0);
      glUniform1i(Tex2Location, 1);
      
		Tex1Location = glGetUniformLocation(psky->pid, "tex");//tex, tex2... sampler in the fragment shader
		Tex2Location = glGetUniformLocation(psky->pid, "tex2");
		// Then bind the uniform samplers to texture units:
		glUseProgram(psky->pid);
		glUniform1i(Tex1Location, 0);
		glUniform1i(Tex2Location, 1);
      
      Tex1Location = glGetUniformLocation(tink1->pid, "tex");//tex, tex2... sampler in the fragment shader
      Tex2Location = glGetUniformLocation(tink1->pid, "tex2");
      // Then bind the uniform samplers to texture units:
      glUseProgram(tink1->pid);
      glUniform1i(Tex1Location, 0);
      glUniform1i(Tex2Location, 1);

      Tex1Location = glGetUniformLocation(tink2->pid, "tex");//tex, tex2... sampler in the fragment shader
      Tex2Location = glGetUniformLocation(tink2->pid, "tex2");
      // Then bind the uniform samplers to texture units:
      glUseProgram(tink2->pid);
      glUniform1i(Tex1Location, 0);
      glUniform1i(Tex2Location, 1);
      
      Tex1Location = glGetUniformLocation(tink3->pid, "tex");//tex, tex2... sampler in the fragment shader
      Tex2Location = glGetUniformLocation(tink3->pid, "tex2");
      // Then bind the uniform samplers to texture units:
      glUseProgram(tink3->pid);
      glUniform1i(Tex1Location, 0);
      glUniform1i(Tex2Location, 1);
      
      Tex1Location = glGetUniformLocation(tink4->pid, "tex");//tex, tex2... sampler in the fragment shader
      Tex2Location = glGetUniformLocation(tink4->pid, "tex2");
      // Then bind the uniform samplers to texture units:
      glUseProgram(tink4->pid);
      glUniform1i(Tex1Location, 0);
      glUniform1i(Tex2Location, 1);
      
      Tex1Location = glGetUniformLocation(ring->pid, "tex");//tex, tex2... sampler in the fragment shader
      Tex2Location = glGetUniformLocation(ring->pid, "tex2");
      // Then bind the uniform samplers to texture units:
      glUseProgram(ring->pid);
      glUniform1i(Tex1Location, 0);
      glUniform1i(Tex2Location, 1);
      
      Tex1Location = glGetUniformLocation(gameOver->pid, "tex");//tex, tex2... sampler in the fragment shader
      Tex2Location = glGetUniformLocation(gameOver->pid, "tex2");
      // Then bind the uniform samplers to texture units:
      glUseProgram(gameOver->pid);
      glUniform1i(Tex1Location, 0);
      glUniform1i(Tex2Location, 1);
      
      Tex1Location = glGetUniformLocation(lives->pid, "tex");//tex, tex2... sampler in the fragment shader
      Tex2Location = glGetUniformLocation(lives->pid, "tex2");
      // Then bind the uniform samplers to texture units:
      glUseProgram(lives->pid);
      glUniform1i(Tex1Location, 0);
      glUniform1i(Tex2Location, 1);
	}

	//General OGL initialization - set OGL state here
	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);
		//glDisable(GL_DEPTH_TEST);
		// Initialize the GLSL program.
		prog = std::make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + "/shader_vertex.glsl", resourceDirectory + "/shader_fragment.glsl");
		if (!prog->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addUniform("campos");
      prog->addUniform("texoff");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");
		prog->addAttribute("vertTex");
      
      lives = std::make_shared<Program>();
      lives->setVerbose(true);
      lives->setShaderNames(resourceDirectory + "/shader_vertex_lives.glsl", resourceDirectory + "/shader_fragment_lives.glsl");
      if (!lives->init())
      {
         std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
         exit(1);
      }
      lives->addUniform("P");
      lives->addUniform("V");
      lives->addUniform("M");
      lives->addUniform("campos");
      lives->addUniform("texoff");
      lives->addAttribute("vertPos");
      lives->addAttribute("vertNor");
      lives->addAttribute("vertTex");
      
      won = std::make_shared<Program>();
      won->setVerbose(true);
      won->setShaderNames(resourceDirectory + "/shader_vertex_lives.glsl", resourceDirectory + "/shader_fragment_lives.glsl");
      if (!won->init())
      {
         std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
         exit(1);
      }
      won->addUniform("P");
      won->addUniform("V");
      won->addUniform("M");
      won->addUniform("campos");
      won->addUniform("texoff");
      won->addAttribute("vertPos");
      won->addAttribute("vertNor");
      won->addAttribute("vertTex");
      
      gameOver = std::make_shared<Program>();
      gameOver->setVerbose(true);
      gameOver->setShaderNames(resourceDirectory + "/shader_vertex_lives.glsl", resourceDirectory + "/shader_fragment_lives.glsl");
      if (!gameOver->init())
      {
         std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
         exit(1);
      }
      gameOver->addUniform("P");
      gameOver->addUniform("V");
      gameOver->addUniform("M");
      gameOver->addUniform("campos");
      gameOver->addUniform("texoff");
      gameOver->addAttribute("vertPos");
      gameOver->addAttribute("vertNor");
      gameOver->addAttribute("vertTex");


		psky = std::make_shared<Program>();
		psky->setVerbose(true);
		psky->setShaderNames(resourceDirectory + "/skyvertex.glsl", resourceDirectory + "/skyfrag.glsl");
		if (!psky->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		psky->addUniform("dn");
      psky->addUniform("onoff");
		psky->addUniform("P");
		psky->addUniform("V");
		psky->addUniform("M");
		psky->addUniform("campos");
		psky->addAttribute("vertPos");
		psky->addAttribute("vertNor");
		psky->addAttribute("vertTex");
      
      
      pacDying = std::make_shared<Program>();
      pacDying->setVerbose(true);
      pacDying->setShaderNames(resourceDirectory + "/shader_vertex_PacDying.glsl", resourceDirectory + "/shader_fragment_PacDying.glsl");
      if (!pacDying->init())
      {
         std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
         exit(1);
      }
      pacDying->addUniform("P");
      pacDying->addUniform("texoff");
      pacDying->addUniform("V");
      pacDying->addUniform("M");
      pacDying->addAttribute("vertPos");
      
      tink1 = std::make_shared<Program>();
      tink1->setVerbose(true);
      tink1->setShaderNames(resourceDirectory + "/shader_vertex_tink1.glsl", resourceDirectory + "/shader_fragment_tink1.glsl");
      if (!tink1->init())
      {
         std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
         exit(1);
      }
      tink1->addUniform("P");
      tink1->addUniform("V");
      tink1->addUniform("M");
      tink1->addAttribute("vertPos");
      
      // Initialize the GLSL program.
      tink2 = std::make_shared<Program>();
      tink2->setVerbose(true);
      tink2->setShaderNames(resourceDirectory + "/shader_vertex_tink2.glsl", resourceDirectory + "/shader_fragment_tink2.glsl");
      if (!tink2->init())
      {
         std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
         exit(1);
      }
      tink2->addUniform("P");
      tink2->addUniform("V");
      tink2->addUniform("M");
      tink2->addAttribute("vertPos");

      // Initialize the GLSL program.
      tink3 = std::make_shared<Program>();
      tink3->setVerbose(true);
      tink3->setShaderNames(resourceDirectory + "/shader_vertex_tink3.glsl", resourceDirectory + "/shader_fragment_tink3.glsl");
      if (!tink3->init())
      {
         std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
         exit(1);
      }
      tink3->addUniform("P");
      tink3->addUniform("V");
      tink3->addUniform("M");
      tink3->addAttribute("vertPos");
      
      // Initialize the GLSL program.
      tink4 = std::make_shared<Program>();
      tink4->setVerbose(true);
      tink4->setShaderNames(resourceDirectory + "/shader_vertex_tink4.glsl", resourceDirectory + "/shader_fragment_tink4.glsl");
      if (!tink4->init())
      {
         std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
         exit(1);
      }
      tink4->addUniform("P");
      tink4->addUniform("V");
      tink4->addUniform("M");
      tink4->addAttribute("vertPos");
      
      // Initialize the GLSL program.
      ring = std::make_shared<Program>();
      ring->setVerbose(true);
      ring->setShaderNames(resourceDirectory + "/shader_vertex_ring.glsl", resourceDirectory + "/shader_fragment_ring.glsl");
      if (!ring->init())
      {
         std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
         exit(1);
      }
      ring->addUniform("P");
      ring->addUniform("V");
      ring->addUniform("M");
      ring->addAttribute("vertPos");
      
      progball = std::make_shared<Program>();
      progball->setVerbose(true);
      progball->setShaderNames(resourceDirectory + "/shader_vertex_ball.glsl", resourceDirectory + "/shader_fragment_ball.glsl");
      progball->init();
      progball->addUniform("P");
      progball->addUniform("V");
      progball->addUniform("camoff");
      progball->addUniform("M");
      progball->addAttribute("vertPos");
      
      walls = std::make_shared<Program>();
      walls->setVerbose(true);
      walls->setShaderNames(resourceDirectory + "/shader_vertex_wall.glsl", resourceDirectory + "/shader_fragment_wall.glsl");
      walls->init();
      walls->addUniform("P");
      walls->addUniform("V");
      walls->addUniform("M");
      walls->addAttribute("vertPos");
	}

   

	/****DRAW
	This is the most important function in your program - this is where you
	will actually issue the commands to draw any geometry you have set up to
	draw
	********/
	void render()
	{
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      
      TinkerPos tink1Pos, tink2Pos, tink3Pos, tink4Pos;
      cout << "PacPos.x: " << moveXPacTotal << endl << "PacPos.z: " << moveZPacTotal << endl;
      mat4 M5, M6, M7;
      // Get current frame buffer size.
      int width, height;
      glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
      glViewport(0, 0, width, height);
      
      // Clear framebuffer.
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
      static double totaltime = 0;
      double frametime = get_last_elapsed_time();
      totaltime += frametime;
      float angle;
      vec2 toff;
      // check all the balls
      // YOU WON
      hasWon = checkBalls();
      if (hasWon == 0){
         pacmanAlive = 0;
      }
      glm::mat4 V, M, P; //View, Model and Perspective matrix
      V = mycam.process(frametime);
      M = glm::mat4(1);
      P = glm::perspective((float)(3.14159 / 4.), (float)((float)width/ (float)height), 0.1f, 1000.0f); //so much type casting... GLM metods are quite funny ones
      if (pacmanAlive == 1) {
//      if (pacLives != 4) {
         
         
         // Create the matrix stacks - please leave these alone for now
         float sangle = -3.1415926 / 4.;
         mat4 rotateX = glm::rotate(glm::mat4(1.0f), sangle, glm::vec3(1.0f, 0.0f, 0.0f));
         //		glm::vec3 camp = -mycam.pos;
         glm::mat4 TransSky = glm::translate(glm::mat4(1.0f), vec3(0.0f, 0.0f, -0.5f));
         glm::mat4 SSky = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.02f));
         
         M =  TransSky * rotateX * SSky;
         
         //this is the earth
         psky->bind();
         glUniformMatrix4fv(psky->getUniform("P"), 1, GL_FALSE, &P[0][0]);
         glUniformMatrix4fv(psky->getUniform("V"), 1, GL_FALSE, &V[0][0]);
         glUniformMatrix4fv(psky->getUniform("M"), 1, GL_FALSE, &M[0][0]);
         glUniform3fv(psky->getUniform("campos"), 1, &mycam.pos[0]);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, Texture2);
         glActiveTexture(GL_TEXTURE1);
         glBindTexture(GL_TEXTURE_2D, TextureN);
         //		static float ttime = 0;
         //		ttime += frametime;
         //		float dn = sin(ttime)*0.5 +0.5;
         //		glUniform1f(psky->getUniform("dn"), dn);
         //		glDisable(GL_DEPTH_TEST);
         //		shape->draw(psky, false);
         //		glEnable(GL_DEPTH_TEST);
         psky->unbind();
         
         glm::mat4 RotateX;
         glm::mat4 TransZ;
         glm::mat4 S;
         
         angle = -3.1415926/2.0;
         
         RotateX = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(8.5f, 0.0f, -13));
         S = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
         
         // --------- IGNORE
         glDisable(GL_BLEND);
         progball->bind();
         V = mycam.process(frametime);
         //send the matrices to the shaders
         glUniformMatrix4fv(progball->getUniform("P"), 1, GL_FALSE, &P[0][0]);
         glUniformMatrix4fv(progball->getUniform("V"), 1, GL_FALSE, &V[0][0]);
         S = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
         //      TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.f + moveSideways, 0.0f, -5 + moveUpDown));
         M =  TransZ * RotateY * RotateX * S;
         glUniformMatrix4fv(progball->getUniform("M"), 1, GL_FALSE, &M[0][0]);
         glDisable(GL_DEPTH_TEST);
         //      shape->draw(progball, false);
         progball->unbind();
         glEnable(GL_DEPTH_TEST);
         
         // ------------- walls
         walls->bind();
         Wall wall1, wall2, wall3, wall4, wall5, wall6, wall7, wall8, wall9, wall10, wall11, wall12, wall13, wall14;
         mat4 M1, M2, M3, M4;
         
         glUniformMatrix4fv(walls->getUniform("P"), 1, GL_FALSE, &P[0][0]);
         glUniformMatrix4fv(walls->getUniform("V"), 1, GL_FALSE, &V[0][0]);
         glBindVertexArray(VertexArrayIDWall);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
         
         //wall 1
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 8.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-13.0f, 0.0f, -6));
         M = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         //      printMatrix(M);
         wall1.x = M[3][0];
         wall1.z = M[3][2];
         //      cout << wall1.x << " -- " << wall1.z << endl;
         
         //wall 2
         S = scale(glm::mat4(1.0f), glm::vec3(0.06f, 0.20f, 8.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(9.0f, 0.0f, -6));
         M = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         //      printMatrix(M);
         wall2.x = M[3][0];
         wall2.z = M[3][2];
         //      cout << wall2.x << " -- " << wall2.z << endl;
         
         //wall 3
         angle = 3.141592/2;
         mat4 rotWall = rotate(mat4(1), angle, vec3(0,1,0));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0.0f, 2.0));
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 11.f));
         M2 = TransZ * rotWall * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M2[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         //      printMatrix(M);
         wall3.x = M[3][0];
         wall3.z = M[3][2];
         //      cout << wall3.x << " -- " << wall3.z << endl;
         
         //wall 4 pt 1
         angle = 3.141592/2;
         rotWall = rotate(mat4(1), angle, vec3(0,1,0));
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 4.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-9, 0.0f, -14.0));
         M = TransZ * rotWall * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         // printMatrix(M);
         wall4.x = M[3][0];
         wall4.z = M[3][2];
         //      cout << wall4.x << " -- " << wall4.z << endl;
         
         //wall 4 pt2
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 4.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(5, 0.0f, -14.0));
         M = TransZ * rotWall * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         //wall 4 pt3
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 3.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0.0f, -10.0));
         M = TransZ * rotWall * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         //wall 4 pt5
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 2.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-5, 0.0f, -12.0));
         M = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         //wall 4 pt4
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 2.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(1, 0.0f, -12.0));
         M = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         
         //wall 5
         angle = 3.141592/2;
         rotWall = rotate(mat4(1), angle, vec3(0,1,0));
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 3.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-11.0, 0.0f, -9.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         //      printMatrix(M1);
         wall5.x = M1[3][0];
         wall5.z = M1[3][2];
         //      cout << wall5.x << " -- " << wall5.z << endl;
         
         // wall 13
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 3.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(7.0, 0.0f, -9.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         //wall 6
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 2.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-11.0, 0.0f, 0.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         //      printMatrix(M1);
         wall6.x = M1[3][0];
         wall6.z = M1[3][2];
         //      cout << wall6.x << " -- " << wall6.z << endl;
         
         //wall 14pt1
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 2.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(5.0, 0.0f, 0.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         //wall 7
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 2.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-9, 0.0f, -4.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         //      printMatrix(M1);
         wall7.x = M1[3][0];
         wall7.z = M1[3][2];
         //      cout << wall7.x << " -- " << wall7.z << endl;
         
         // wall 8
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 2.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-5, 0.0f, -4.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 9
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 2.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(3, 0.0f, -4.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 10
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 2.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(1, 0.0f, 0.0));
         M1 = TransZ * rotWall * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 11
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 2.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(7, 0.0f, -4.0));
         M1 = TransZ * rotWall * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 18
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 2.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(1, 0.0f, -8.0));
         M1 = TransZ * rotWall * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         //wall 12
         S = scale(glm::mat4(1.0f), glm::vec3(1.f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(6, 0.0f, -11.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         //      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         //wall 13
         S = scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.0f, 0.25f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(5, 0.0f, -6.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         //wall 14
         S = scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.0f, 0.25f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(7, 0.0f, -2.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         //wall 15
         S = scale(glm::mat4(1.0f), glm::vec3(1.f, 0.20f, 2.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0.0f, -4.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 16
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-9, 0.0f, -13.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 19
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-7, 0.0f, -1.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 20
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-7, 0.0f, -7.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 21
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-7, 0.0f, -11.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 22
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(1, 0.0f, -1.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 23
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(1, 0.0f, -7.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 24
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(3, 0.0f, -11.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 25
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-10, 0.0f, 0.0));
         M1 = TransZ * rotWall * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 14 pt2
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(6, 0.0f, 0.0));
         M1 = TransZ * rotWall * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 26
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-10, 0.0f, -4.0));
         M1 = TransZ * rotWall * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 27
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-10, 0.0f, -8.0));
         M1 = TransZ * rotWall * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 13 pt2
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(6, 0.0f, -8.0));
         M1 = TransZ * rotWall * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 12
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(5, 0.0f, -11.0));
         M1 = TransZ * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 28
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-8, 0.0f, -10.0));
         M1 = TransZ * rotWall * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 29
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-6, 0.0f, -4.0));
         M1 = TransZ * rotWall * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 30
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-4, 0.0f, -8.0));
         M1 = TransZ * rotWall * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 31
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-4, 0.0f, 0.0));
         M1 = TransZ * rotWall * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         // wall 32
         S = scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.20f, 1.f));
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(2, 0.0f, -4.0));
         M1 = TransZ * rotWall * S;
         glUniformMatrix4fv(walls->getUniform("M"), 1, GL_FALSE, &M1[0][0]);
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
         
         walls->unbind();
         
         
         // --------------
         
         // tink1
         // TINKER 1
         // Draw the box using GLSL.
         // little ghost
         tink1->bind();
         static float moveX = 8.0, moveZ = -5.0;
         static int second = 0;
         second += 1;
         V = mycam.process(frametime);
         //send the matrices to the shaders
         angle = -3.1415926/2.0;
         S = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
         RotateX = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
         translateGhost = glm::translate(glm::mat4(1.0f), vec3(moveX, 0, moveZ));
         M =  translateGhost * RotateX * S;
         tink1Pos.x = M[3][0];
         tink1Pos.z = M[3][2];
         
         tinkPosition = takeVector(vec3(tink1Pos.x, 0, tink1Pos.z));
         if (tinkPosition != 0) {
            moveGhost = moveCharacter(tinkPosition, 3, vec3(tink1Pos.x, 0, tink1Pos.z), vec3(moveXPacTotal, 0, moveZPacTotal));
         }
         if (moveGhost.x >= 2.0 && moveGhost.z <= 0){
            moveX += 0.05;
         } else if (moveGhost.x <= -2.0 && moveGhost.z >= 0) {
            moveX -= 0.05;
         } else if (moveGhost.x <= 0.0 && moveGhost.z >= 2) {
            moveZ += 0.05;
         } else if (moveGhost.x >= 0.0 && moveGhost.z <= -2.0) {
            moveZ -= 0.05;
         }
         translateGhost = glm::translate(glm::mat4(1.0f), vec3(moveX, 0, moveZ));
         glUniformMatrix4fv(tink1->getUniform("P"), 1, GL_FALSE, &P[0][0]);
         glUniformMatrix4fv(tink1->getUniform("V"), 1, GL_FALSE, &V[0][0]);
         glBindVertexArray(VertexArrayID);
         //actually draw from vertex 0, 3 vertices
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, Texture2);
         glUniformMatrix4fv(tink1->getUniform("M"), 1, GL_FALSE, &M[0][0]);
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
         glBindVertexArray(0);
         //      tinkShape->draw(tink1, false);
         tink1->unbind();
         
         tink2->bind();
         static float moveX2 = -12.0, moveZ2 = -13.0;
         //      moveX2 = 0.0, moveZ2 = -13.0;
         V = mycam.process(frametime);
         //send the matrices to the shaders
         angle = -3.1415926/2.0;
         S = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.7f, 0.0f));
         RotateX = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
         translateGhost2 = glm::translate(glm::mat4(1.0f), vec3(moveX2, 0, moveZ2));
         M2 =  translateGhost2 * RotateX * S;
         tink2Pos.x = M2[3][0];
         tink2Pos.z = M2[3][2];
         
         tinkPosition2 = takeVector(vec3(tink2Pos.x, 0, tink2Pos.z));
         if (tinkPosition2 != 0) {
            moveGhost2 = moveCharacter(tinkPosition2, 2, vec3(tink2Pos.x, 0, tink2Pos.z), vec3(moveXPacTotal, 0, moveZPacTotal));
         }
         if (moveGhost2.x >= 2.0 && moveGhost2.z <= 0){
            moveX2 += 0.05;
         } else if (moveGhost2.x <= -2.0 && moveGhost2.z >= 0) {
            moveX2 -= 0.05;
         } else if (moveGhost2.x <= 0.0 && moveGhost2.z >= 2) {
            moveZ2 += 0.05;
         } else if (moveGhost2.x >= 0.0 && moveGhost2.z <= -2.0) {
            moveZ2 -= 0.05;
         }
         translateGhost2 = glm::translate(glm::mat4(1.0f), vec3(moveX2, 0, moveZ2));
         
         glUniformMatrix4fv(tink2->getUniform("P"), 1, GL_FALSE, &P[0][0]);
         glUniformMatrix4fv(tink2->getUniform("V"), 1, GL_FALSE, &V[0][0]);
         glBindVertexArray(VertexArrayID);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, Texture3);
         glUniformMatrix4fv(tink2->getUniform("M"), 1, GL_FALSE, &M2[0][0]);
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
         glBindVertexArray(0);
         //      glUniformMatrix4fv(tink2->getUniform("M"), 1, GL_FALSE, &M2[0][0]);
         //      tinkShape->draw(tink2, false);
         tink2->unbind();
         
         //tinker 3
         tink3->bind();
         static float moveX3 = -4.0, moveZ3 = -7.0;
         //      moveX3 = 8.0, moveZ3 = -5.0;
         V = mycam.process(frametime);
         //send the matrices to the shaders
         angle = -3.1415926/2.0;
         S = glm::scale(glm::mat4(1.0f), glm::vec3(1.f, 1.f, 0.0f));
         RotateX = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
         translateGhost3 = glm::translate(glm::mat4(1.0f), vec3(moveX3, 0, moveZ3));
         M3 =  translateGhost3 * RotateX * S;
         tink3Pos.x = M3[3][0];
         tink3Pos.z = M3[3][2];
         //      printMatrix(M3);
         
         tinkPosition3 = takeVector(vec3(tink3Pos.x, 0, tink3Pos.z));
         if (tinkPosition3 != 0) {
            moveGhost3 = moveCharacter(tinkPosition3, 8, vec3(tink3Pos.x, 0, tink3Pos.z), vec3(moveXPacTotal, 0, moveZPacTotal));
         }
         if (moveGhost3.x >= 2.0 && moveGhost3.z <= 0){
            moveX3 += 0.05;
         } else if (moveGhost3.x <= -2.0 && moveGhost3.z >= 0) {
            moveX3 -= 0.05;
         } else if (moveGhost3.x <= 0.0 && moveGhost3.z >= 2) {
            moveZ3 += 0.05;
         } else if (moveGhost3.x >= 0.0 && moveGhost3.z <= -2.0) {
            moveZ3 -= 0.05;
         }
         translateGhost3 = glm::translate(glm::mat4(1.0f), vec3(moveX3, 0, moveZ3));
         
         glUniformMatrix4fv(tink3->getUniform("P"), 1, GL_FALSE, &P[0][0]);
         glUniformMatrix4fv(tink3->getUniform("V"), 1, GL_FALSE, &V[0][0]);
         glBindVertexArray(VertexArrayID);
         //actually draw from vertex 0, 3 vertices
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, Texture4);
         glUniformMatrix4fv(tink3->getUniform("M"), 1, GL_FALSE, &M3[0][0]);
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
         glBindVertexArray(0);
         
         tink3->unbind();
         
         tink4->bind();
         static float moveX4 = 8.0, moveZ4 = 1.0;
         V = mycam.process(frametime);
         //send the matrices to the shaders
         angle = -3.1415926/2.0;
         S = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.0f));
         RotateX = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
         translateGhost4 = glm::translate(glm::mat4(1.0f), vec3(moveX4, 0.1, moveZ4));
         M4 =  translateGhost4 * RotateX * S;
         tink4Pos.x = M4[3][0];
         tink4Pos.z = M4[3][2];
         
         tinkPosition4 = takeVector(vec3(tink4Pos.x, 0, tink4Pos.z));
         if (tinkPosition4 != 0) {
            moveGhost4 = moveCharacter(tinkPosition4, 3, vec3(tink4Pos.x, 0, tink4Pos.z), vec3(moveXPacTotal, 0, moveZPacTotal));
         }
         if (moveGhost4.x >= 2.0 && moveGhost4.z <= 0){
            moveX4 += 0.05;
         } else if (moveGhost4.x <= -2.0 && moveGhost4.z >= 0) {
            moveX4 -= 0.05;
         } else if (moveGhost4.x <= 0.0 && moveGhost4.z >= 2) {
            moveZ4 += 0.05;
         } else if (moveGhost4.x >= 0.0 && moveGhost4.z <= -2.0) {
            moveZ4 -= 0.05;
         }
         translateGhost4 = glm::translate(glm::mat4(1.0f), vec3(moveX4, 0.1, moveZ4));
         glUniformMatrix4fv(tink4->getUniform("P"), 1, GL_FALSE, &P[0][0]);
         glUniformMatrix4fv(tink4->getUniform("V"), 1, GL_FALSE, &V[0][0]);
         glBindVertexArray(VertexArrayID);
         //actually draw from vertex 0, 3 vertices
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, Texture5);
         glUniformMatrix4fv(tink4->getUniform("M"), 1, GL_FALSE, &M4[0][0]);
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
         glBindVertexArray(0);
         //      tinkShape->draw(tink1, false);
         tink4->unbind();
         
         
         // Draw the box using GLSL.
         // this is the pacman!!!!
         prog->bind();
         
         static float moveXPac = -12., moveZPac = 1.;
         static float wUp = -3.1415926 / 2., wDown = 3.1415926 / 2., wLeft = -3.1415926, wRight = 0;
         //send the matrices to the shaders
         glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
         glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
         static float moveUpDown = 0.0, moveSideways =0.0;
         glUniform3fv(prog->getUniform("campos"), 1, &mycam.pos[0]);
         glBindVertexArray(VertexArrayID);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
//         if (pacmanAlive == 1) {
         float trigger = totaltime * 10.f;
         int trigger_i  = (int) trigger;
         int tx = trigger_i % 2;
         int ty = trigger_i / 2;
         toff = vec2(tx,ty);
         glUniform2fv(prog->getUniform("texoff"), 1, &toff.x);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, Texture);
         //         }
         TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(moveXPacTotal, 0.f, moveZPacTotal));
         S = glm::scale(glm::mat4(1.0f), glm::vec3(1.f, 1.f, 0.0f));
         angle = 3.1415926 / 2.0f;
         RotateX = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
         M = TransZ * RotateX * RotateY * S;
         pacPos.x = M[3][0];
         pacPos.z = M[3][2];
         cout << "THIS IS THE PACMAN" << endl;
         printMatrix(M);
         moveXPacTotal = moveXPac + moveSideways;
         moveZPacTotal = moveZPac + moveUpDown;
         if (gup == 1){
            RotateY = rotate(glm::mat4(1.0f), wUp, glm::vec3(0.0f, 0.0f, 1.0f));
            pacManPostionNum = takePacVector(vec3(pacPos.x, 0.0, pacPos.z));
            if (pacManPostionNum != 0){
               for (int i = 0; i < 8; i++){
                  if (find(begin(dirUp), end(dirUp), (int) pacManPostionNum) != end(dirUp)){
                     cout << "Found it" << endl;
                  } else {
                     gup = 0;
                  }
               }
            }
            moveUpDown -= 0.05;
         }
         if (gdown == 1) {
            RotateY = rotate(glm::mat4(1.0f), wDown, glm::vec3(0.0f, 0.0f, 1.0f));
            pacManPostionNum = takePacVector(vec3(pacPos.x, 0.0, pacPos.z));
            if (pacManPostionNum != 0){
               for (int i = 0; i < 8; i++){
                  if (find(begin(dirDown), end(dirDown), (int) pacManPostionNum) != end(dirDown)){
                     cout << "Found it" << endl;
                  } else {
                     gdown = 0;
                  }
               }
            }
            moveUpDown += 0.05;
         }
         if (gleft == 1) {
            RotateY = rotate(glm::mat4(1.0f), wLeft, glm::vec3(0.0f, 0.0f, 1.0f));
            pacManPostionNum = takePacVector(vec3(pacPos.x, 0.0, pacPos.z));
            if (pacManPostionNum != 0){
               for (int i = 0; i < 8; i++){
                  if (find(begin(dirLeft), end(dirLeft), (int) pacManPostionNum) != end(dirLeft)){
                     cout << "Found it" << endl;
                  } else {
                     gleft = 0;
                  }
               }
            }
            moveSideways -= 0.05;
         }
         if (gright == 1) {
            RotateY = rotate(glm::mat4(1.0f), wRight, glm::vec3(0.0f, 0.0f, 1.0f));
            pacManPostionNum = takePacVector(vec3(pacPos.x, 0.0, pacPos.z));
            if (pacManPostionNum != 0){
               for (int i = 0; i < 8; i++){
                  if (find(begin(dirRight), end(dirRight), (int) pacManPostionNum) != end(dirRight)){
                     cout << "Found it" << endl;
                  } else {
                     gright = 0;
                  }
               }
            }
            moveSideways += 0.05;
         }
         
         for (int i = 0; i < 17; i++){
            for (int j = 0; j < 23; j++) {
               if (abs(pacPos.x - (positionRing[i][j]).x) < 0.5 && abs(pacPos.z - (positionRing[i][j]).z) < 0.5) {
                  (positionRing[i][j]).x = 0;
                  (positionRing[i][j]).z = 5;
               }
            }
         }
         
         // compare pacmans position with tink1,tink2,tink3,tink4:
         if ((cmpfPacGhost(pacPos.x, tink1Pos.x) && cmpfPacGhost(pacPos.z, tink1Pos.z))
             || (cmpfPacGhost(pacPos.x, tink2Pos.x) && cmpfPacGhost(pacPos.z, tink2Pos.z))
             || (cmpfPacGhost(pacPos.x, tink3Pos.x) && cmpfPacGhost(pacPos.z, tink3Pos.z))
             || (cmpfPacGhost(pacPos.x, tink4Pos.x) && cmpfPacGhost(pacPos.z, tink4Pos.z))
             ) {
            pacLives--;
            pacmanAlive = 0;
            moveXPacTotal = -12.;
            moveZPacTotal = 1.;
            moveUpDown = 0.0, moveSideways = 0.0;
            gleft = 0, gright = 0, gup = 0, gdown = 0;
            moveX4 = 8.0, moveZ4 = 1.0;
            moveX3 = -4.0, moveZ3 = -7.0;
            moveX2 = -12.0, moveZ2 = -13.0;
            moveX = 8.0, moveZ = -5.0;
//            hasWon = 0;
//            pacLives++;
         }
         
         glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
         glBindVertexArray(0);
         
         prog->unbind();
         
         // ----- ring here
         ring->bind();
         glUniformMatrix4fv(ring->getUniform("P"), 1, GL_FALSE, &P[0][0]);
         glUniformMatrix4fv(ring->getUniform("V"), 1, GL_FALSE, &V[0][0]);
         
         glBindVertexArray(VertexArrayID);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, TextureRing);
         
         S = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.2f, 0.0f));
         angle = 3.1415926 / 2.0f;
         RotateX = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
         glEnable(GL_BLEND);
         for (int i = 0; i < 17; i++){
            for (int j = 0; j < 23; j++){
               TransZ = glm::translate(glm::mat4(1.0f), positionRing[i][j]);
               M5 = TransZ * RotateX * S;
               glUniformMatrix4fv(ring->getUniform("M"), 1, GL_FALSE, &M5[0][0]);
               glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
            }
         }
         glDisable(GL_BLEND);
         glBindVertexArray(0);
         ring->unbind();
         // ----
      } else if (pacmanAlive == 0){
         pacDying->bind();
//         prog->bind();
         static float sec = 0.0;
         glUniformMatrix4fv(pacDying->getUniform("P"), 1, GL_FALSE, &P[0][0]);
         glUniformMatrix4fv(pacDying->getUniform("V"), 1, GL_FALSE, &V[0][0]);
         glUniform3fv(pacDying->getUniform("campos"), 1, &mycam.pos[0]);
         glBindVertexArray(VertexArrayID);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
         float triggerDone = totaltime * 4.f;
         int trigger_iDone = (int) triggerDone;
         int txDone = trigger_iDone % 2;
         int tyDone = trigger_iDone / 2;
         toff = vec2(txDone, tyDone);
         glUniform2fv(pacDying->getUniform("texoff"), 1, &toff.x);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, TexturePacDying);
         mat4 TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0.f, -5));
         mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 0.0f));
         float angle = 3.1415926 / 2.0f;
         mat4 RotateX = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
         M = TransZ * RotateX * RotateY * S;
         glUniformMatrix4fv(pacDying->getUniform("M"), 1, GL_FALSE, &M[0][0]);
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
//         glBindVertexArray(0);
         sec += 0.03;
         if (sec > 5.0){
            pacmanAlive = 1;
            sec = 0.0;
            if (pacLives <= -1)
               gameDone = 0;
            if (hasWon <= -1)
               gameDone = 0;
         }
         pacDying->unbind();
         if (hasWon != 0) {
            lives->bind();
            glUniformMatrix4fv(lives->getUniform("P"), 1, GL_FALSE, &P[0][0]);
            glUniformMatrix4fv(lives->getUniform("V"), 1, GL_FALSE, &V[0][0]);
            glBindVertexArray(VertexArrayID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
            toff = vec2(0, 0);
            glUniform2fv(lives->getUniform("texoff"), 1, &toff.x);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, TextureLives);
            TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-4, 0.f, -7));
            S = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 0.0f));
            angle = 3.1415926 / 2.0f;
            RotateX = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
            mat4 RotateZ = glm::rotate(glm::mat4(1.0f), angle * 2, glm::vec3(0.0f, 0.0f, 1.0f));
            M = TransZ * RotateX * RotateZ * S;
            if (pacLives == 3) {
               TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-4, 0.f, -7));
               M = TransZ * RotateX * RotateZ * S;
               glUniformMatrix4fv(lives->getUniform("M"), 1, GL_FALSE, &M[0][0]);
               glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
               
               TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0.f, -7));
               M = TransZ * RotateX * RotateZ * S;
               glUniformMatrix4fv(lives->getUniform("M"), 1, GL_FALSE, &M[0][0]);
               glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
               
               TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.f, -7));
               M = TransZ * RotateX * RotateZ * S;
               glUniformMatrix4fv(lives->getUniform("M"), 1, GL_FALSE, &M[0][0]);
               glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
               
            } else if (pacLives == 2) {
               TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-3, 0.f, -7));
               M = TransZ * RotateX * RotateZ * S;
               glUniformMatrix4fv(lives->getUniform("M"), 1, GL_FALSE, &M[0][0]);
               glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
               
               TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0.f, -7));
               M = TransZ * RotateX * RotateZ * S;
               glUniformMatrix4fv(lives->getUniform("M"), 1, GL_FALSE, &M[0][0]);
               glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
               
            } else if (pacLives == 1) {
               TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0.f, -7));
               M = TransZ * RotateX * RotateZ * S;
               glUniformMatrix4fv(lives->getUniform("M"), 1, GL_FALSE, &M[0][0]);
               glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
            }
            else if (pacLives <= 0){
               gameOver->bind();
               glUniformMatrix4fv(gameOver->getUniform("P"), 1, GL_FALSE, &P[0][0]);
               glUniformMatrix4fv(gameOver->getUniform("V"), 1, GL_FALSE, &V[0][0]);
               glBindVertexArray(VertexArrayID);
               glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
               toff = vec2(0, 0);
               glUniform2fv(gameOver->getUniform("texoff"), 1, &toff.x);
               glActiveTexture(GL_TEXTURE0);
               glBindTexture(GL_TEXTURE_2D, TexturGameOver);
               TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0.f, -9));
               S = glm::scale(glm::mat4(1.0f), glm::vec3(5.5f, 5.5f, 0.0f));
               angle = 3.1415926 / 2.0f;
               RotateX = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
               mat4 RotateZ = glm::rotate(glm::mat4(1.0f), angle * 2, glm::vec3(0.0f, 0.0f, 1.0f));
               mat4 Rotatey = glm::rotate(glm::mat4(1.0f), angle * 2, glm::vec3(0.0f, 1.0f, 0.0f));
               M = TransZ * RotateX * Rotatey * RotateZ * S;
               glUniformMatrix4fv(lives->getUniform("M"), 1, GL_FALSE, &M[0][0]);
               glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
               gameOver->unbind();
               pacLives--;

            }
            glBindVertexArray(0);
            lives->unbind();
         } else if (hasWon == 0) {
            won->bind();
            glUniformMatrix4fv(won->getUniform("P"), 1, GL_FALSE, &P[0][0]);
            glUniformMatrix4fv(won->getUniform("V"), 1, GL_FALSE, &V[0][0]);
            glBindVertexArray(VertexArrayID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
            toff = vec2(0, 0);
            glUniform2fv(won->getUniform("texoff"), 1, &toff.x);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, TextureWon);
            TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0.f, -9));
            S = glm::scale(glm::mat4(1.0f), glm::vec3(10.f, 10.f, 0.0f));
            angle = 3.1415926 / 2.0f;
            RotateX = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
            mat4 RotateZ = glm::rotate(glm::mat4(1.0f), angle * 2, glm::vec3(0.0f, 0.0f, 1.0f));
            mat4 Rotatey = glm::rotate(glm::mat4(1.0f), angle * 2, glm::vec3(0.0f, 1.0f, 0.0f));
            M = TransZ * RotateX * Rotatey * RotateZ * S;
            glUniformMatrix4fv(won->getUniform("M"), 1, GL_FALSE, &M[0][0]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
            hasWon--;
            won->unbind();
            
         }
      }
   }

};
//******************************************************************************************
int main(int argc, char **argv)
{
	std::string resourceDir = "../../resources"; // Where the resources are loaded from
	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	/* your main will always include a similar set up to establish your window
		and GL context, etc. */
	WindowManager * windowManager = new WindowManager();
	windowManager->init(1920, 1080);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	/* This is the code that will likely change program to program as you
		may need to initialize or set up different data and state */
	// Initialize scene.
	application->init(resourceDir);
	application->initGeom();

	// Loop until the user closes the window.
	while(! glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.
		application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
      if (gameDone == 0){
         break;
      }
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
