#include "igvMallaTriangulos.h"

igvMallaTriangulos::igvMallaTriangulos ( long int _num_vertices, float *_vertices
                                         , long int _num_triangulos
                                         , unsigned int *_triangulos )
   : num_vertices ( _num_vertices ), num_triangulos ( _num_triangulos )
{  if ( num_vertices > 0 && _vertices != nullptr )
   {  vertices = new float[num_vertices * 3];
      for ( long int i = 0 ; i < num_vertices * 3 ; ++i )
      {  vertices[i] = _vertices[i];
      }
   }

   if ( num_triangulos > 0 && _triangulos != nullptr )
   {  triangulos = new unsigned int[num_triangulos * 3];
      for ( long int i = 0 ; i < num_triangulos * 3 ; ++i )
      {  triangulos[i] = _triangulos[i];
      }
   }
}

igvMallaTriangulos::~igvMallaTriangulos ()
{  delete [] vertices;
   delete [] normales;
   delete [] triangulos;
}

void igvMallaTriangulos::visualizar ( bool usar_normales, bool gouraud )
{  if ( num_vertices <= 0 || num_triangulos <= 0
       || vertices == nullptr || triangulos == nullptr )
   {  return;
   }

   glShadeModel ( gouraud ? GL_SMOOTH : GL_FLAT );
   glEnableClientState ( GL_VERTEX_ARRAY );
   glVertexPointer ( 3, GL_FLOAT, 0, vertices );

   if ( usar_normales && normales != nullptr )
   {  glEnableClientState ( GL_NORMAL_ARRAY );
      glNormalPointer ( GL_FLOAT, 0, normales );
   }

   glDrawElements ( GL_TRIANGLES, static_cast<GLsizei> ( num_triangulos * 3 )
                    , GL_UNSIGNED_INT, triangulos );

   if ( usar_normales && normales != nullptr )
   {  glDisableClientState ( GL_NORMAL_ARRAY );
   }
   glDisableClientState ( GL_VERTEX_ARRAY );
}
