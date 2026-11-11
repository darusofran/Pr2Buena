#include "igvCilindro.h"

#include <algorithm>
#include <cmath>

igvCilindro::igvCilindro () : igvMallaTriangulos ()
{
}

igvCilindro::igvCilindro ( float r, float a, int divU, int divV )
   : igvMallaTriangulos ()
{  const int divisionesU = std::max ( 3, divU );
   const int divisionesV = std::max ( 1, divV );
   const float radio = std::max ( 0.001f, r );
   const float altura = std::max ( 0.001f, a );
   const float pi = 3.14159265358979323846f;

   num_vertices = static_cast<long int> ( divisionesU ) * ( divisionesV + 1 );
   num_triangulos = 2L * divisionesU * divisionesV;
   vertices = new float[num_vertices * 3];
   normales = new float[num_vertices * 3];
   triangulos = new unsigned int[num_triangulos * 3];

   for ( int j = 0 ; j <= divisionesV ; ++j )
   {  for ( int i = 0 ; i < divisionesU ; ++i )
      {  const float angulo = 2.0f * pi * static_cast<float> ( i ) / divisionesU;
         const long int indice = static_cast<long int> ( j ) * divisionesU + i;
         vertices[indice * 3] = radio * std::cos ( angulo );
         vertices[indice * 3 + 1] = altura * static_cast<float> ( j ) / divisionesV;
         vertices[indice * 3 + 2] = radio * std::sin ( angulo );
         normales[indice * 3] = std::cos ( angulo );
         normales[indice * 3 + 1] = 0.0f;
         normales[indice * 3 + 2] = std::sin ( angulo );
      }
   }

   long int t = 0;
   for ( int j = 0 ; j < divisionesV ; ++j )
   {  for ( int i = 0 ; i < divisionesU ; ++i )
      {  const int siguiente = ( i + 1 ) % divisionesU;
         const unsigned int inferior = static_cast<unsigned int> ( j * divisionesU + i );
         const unsigned int inferiorSiguiente = static_cast<unsigned int> ( j * divisionesU + siguiente );
         const unsigned int superior = static_cast<unsigned int> ( ( j + 1 ) * divisionesU + i );
         const unsigned int superiorSiguiente = static_cast<unsigned int> ( ( j + 1 ) * divisionesU + siguiente );

         triangulos[t++] = inferior;
         triangulos[t++] = superior;
         triangulos[t++] = inferiorSiguiente;
         triangulos[t++] = inferiorSiguiente;
         triangulos[t++] = superior;
         triangulos[t++] = superiorSiguiente;
      }
   }
}

igvCilindro::~igvCilindro ()
{
}
