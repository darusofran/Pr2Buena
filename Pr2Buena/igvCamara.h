#ifndef __IGVCAMARA
#define __IGVCAMARA

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include "igvPunto3D.h"

enum tipoCamara
{  IGV_PARALELA
   , IGV_FRUSTUM
   , IGV_PERSPECTIVA
};

class igvCamara
{  private:
      tipoCamara tipo = IGV_PARALELA;
      GLdouble xwmin = -3.0, xwmax = 3.0;
      GLdouble ywmin = -3.0, ywmax = 3.0;
      GLdouble angulo = 60.0, raspecto = 1.0;
      GLdouble znear = 1.0, zfar = 200.0;
      igvPunto3D P0 = {3.0, 2.0, 4.0};
      igvPunto3D r = {0.0, 0.0, 0.0};
      igvPunto3D V = {0.0, 1.0, 0.0};

   public:
      igvCamara () = default;
      ~igvCamara () = default;
      igvCamara ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V );

      void set ( igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V );
      void set ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V
                 , double _xwmin, double _xwmax, double _ywmin, double _ywmax
                 , double _znear, double _zfar );
      void set ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V
                 , double _angulo, double _raspecto, double _znear, double _zfar );

      void aplicar ( GLdouble aspecto = 0.0 );
      void zoom ( double factor );
      void orbitar ( double grados );
      void cabecear ( double grados );
      void rotarY ( double grados );
      void moverZnear ( double cantidad );
      void moverZfar ( double cantidad );
      void setZnear ( GLdouble valor );
      void setZfar ( GLdouble valor );

      tipoCamara getTipo () const;
      GLdouble getXwmin () const;
      GLdouble getXwmax () const;
      GLdouble getYwmin () const;
      GLdouble getYwmax () const;
      GLdouble getAngulo () const;
      GLdouble getRaspecto () const;
      GLdouble getZnear () const;
      GLdouble getZfar () const;
      const igvPunto3D &getP0 () const;
      const igvPunto3D &getR () const;
      const igvPunto3D &getV () const;
};

#endif
