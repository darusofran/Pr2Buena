#ifndef __IGVMALLATRIANGULOS
#define __IGVMALLATRIANGULOS

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

/**
 * Malla indexada de triangulos con vertices y normales.
 */
class igvMallaTriangulos
{  protected:
      long int num_vertices = 0;
      float *vertices = nullptr;
      float *normales = nullptr;
      long int num_triangulos = 0;
      unsigned int *triangulos = nullptr;

   public:
      igvMallaTriangulos () = default;
      igvMallaTriangulos ( long int _num_vertices, float *_vertices
                           , long int _num_triangulos, unsigned int *_triangulos );
      virtual ~igvMallaTriangulos ();

      void visualizar ( bool usar_normales = true, bool gouraud = false );
};

#endif
