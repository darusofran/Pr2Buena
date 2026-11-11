#include "igvSuperficieRevolucion.h"

#include <algorithm>
#include <cmath>

igvSuperficieRevolucion::igvSuperficieRevolucion ( float radio, float profundidad
                                                  , int divisionesRadiales
                                                  , int divisionesAngulares )
   : igvMallaTriangulos ()
{  const int nr = std::max ( 2, divisionesRadiales );
   const int nt = std::max ( 8, divisionesAngulares );
   const float R = std::max ( 0.001f, radio );
   const float h = std::max ( 0.001f, profundidad );
   const float foco = R * R / ( 4.0f * h );
   const float pi = 3.14159265358979323846f;

   num_vertices = 1L + static_cast<long int> ( nr ) * nt;
   num_triangulos = static_cast<long int> ( nt ) * ( 1 + 2 * ( nr - 1 ) );
   vertices = new float[num_vertices * 3];
   normales = new float[num_vertices * 3];
   triangulos = new unsigned int[num_triangulos * 3];

   vertices[0] = 0.0f;
   vertices[1] = 0.0f;
   vertices[2] = 0.0f;
   normales[0] = 0.0f;
   normales[1] = 0.0f;
   normales[2] = 1.0f;

   for ( int anillo = 1 ; anillo <= nr ; ++anillo )
   {  const float r = R * static_cast<float> ( anillo ) / nr;
      for ( int sector = 0 ; sector < nt ; ++sector )
      {  const float theta = 2.0f * pi * static_cast<float> ( sector ) / nt;
         const float x = r * std::cos ( theta );
         const float y = r * std::sin ( theta );
         const long int indice = 1L + static_cast<long int> ( anillo - 1 ) * nt + sector;
         const float nx = -x / ( 2.0f * foco );
         const float ny = -y / ( 2.0f * foco );
         const float longitud = std::sqrt ( nx * nx + ny * ny + 1.0f );

         vertices[indice * 3] = x;
         vertices[indice * 3 + 1] = y;
         vertices[indice * 3 + 2] = r * r / ( 4.0f * foco );
         normales[indice * 3] = nx / longitud;
         normales[indice * 3 + 1] = ny / longitud;
         normales[indice * 3 + 2] = 1.0f / longitud;
      }
   }

   long int t = 0;
   for ( int sector = 0 ; sector < nt ; ++sector )
   {  const unsigned int actual = static_cast<unsigned int> ( 1 + sector );
      const unsigned int siguiente = static_cast<unsigned int> ( 1 + ( sector + 1 ) % nt );
      triangulos[t++] = 0;
      triangulos[t++] = actual;
      triangulos[t++] = siguiente;
   }

   for ( int anillo = 1 ; anillo < nr ; ++anillo )
   {  for ( int sector = 0 ; sector < nt ; ++sector )
      {  const int siguienteSector = ( sector + 1 ) % nt;
         const unsigned int interior = static_cast<unsigned int> ( 1 + ( anillo - 1 ) * nt + sector );
         const unsigned int interiorSiguiente = static_cast<unsigned int> ( 1 + ( anillo - 1 ) * nt + siguienteSector );
         const unsigned int exterior = static_cast<unsigned int> ( 1 + anillo * nt + sector );
         const unsigned int exteriorSiguiente = static_cast<unsigned int> ( 1 + anillo * nt + siguienteSector );

         triangulos[t++] = interior;
         triangulos[t++] = exterior;
         triangulos[t++] = exteriorSiguiente;
         triangulos[t++] = interior;
         triangulos[t++] = exteriorSiguiente;
         triangulos[t++] = interiorSiguiente;
      }
   }
}
