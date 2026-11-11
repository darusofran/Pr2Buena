#include "igvCamara.h"

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <algorithm>
#include <cmath>

namespace
{  const double PI = 3.14159265358979323846;
   const double EPSILON = 0.000001;
}

igvCamara::igvCamara ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V )
   : tipo ( _tipo ), P0 ( _P0 ), r ( _r ), V ( _V )
{
}

void igvCamara::set ( igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V )
{  P0 = _P0;
   r = _r;
   V = _V;
}

void igvCamara::set ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V
                      , double _xwmin, double _xwmax, double _ywmin, double _ywmax
                      , double _znear, double _zfar )
{  tipo = _tipo;
   P0 = _P0;
   r = _r;
   V = _V;
   xwmin = _xwmin;
   xwmax = _xwmax;
   ywmin = _ywmin;
   ywmax = _ywmax;
   znear = _znear;
   zfar = _zfar;
}

void igvCamara::set ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V
                      , double _angulo, double _raspecto, double _znear, double _zfar )
{  tipo = _tipo;
   P0 = _P0;
   r = _r;
   V = _V;
   angulo = _angulo;
   raspecto = std::max ( 0.001, _raspecto );
   znear = _znear;
   zfar = _zfar;
}

void igvCamara::aplicar ( GLdouble aspecto )
{  glMatrixMode ( GL_PROJECTION );
   glLoadIdentity ();

   if ( tipo == IGV_PARALELA )
   {  if ( aspecto > 0.0 )
      {  const GLdouble centroX = ( xwmin + xwmax ) * 0.5;
         const GLdouble semialto = ( ywmax - ywmin ) * 0.5;
         const GLdouble semiancho = semialto * aspecto;
         glOrtho ( centroX - semiancho, centroX + semiancho, ywmin, ywmax, znear, zfar );
      }
      else
      {  glOrtho ( xwmin, xwmax, ywmin, ywmax, znear, zfar );
      }
   }
   else if ( tipo == IGV_FRUSTUM )
   {  glFrustum ( xwmin, xwmax, ywmin, ywmax, znear, zfar );
   }
   else
   {  glMatrixMode ( GL_PROJECTION );
      gluPerspective ( angulo, aspecto > 0.0 ? aspecto : raspecto, znear, zfar );
   }

   glMatrixMode ( GL_MODELVIEW );
   glLoadIdentity ();
   gluLookAt ( P0[X], P0[Y], P0[Z], r[X], r[Y], r[Z], V[X], V[Y], V[Z] );
}

void igvCamara::zoom ( double factor )
{  const double f = 1.0 + factor / 100.0;
   if ( f <= 0.0 )
   {  return;
   }

   if ( tipo == IGV_PARALELA || tipo == IGV_FRUSTUM )
   {  xwmin *= f;
      xwmax *= f;
      ywmin *= f;
      ywmax *= f;
   }
   else
   {  angulo = std::max ( 10.0, std::min ( 120.0, angulo * f ) );
   }
}

void igvCamara::orbitar ( double grados )
{  const double rad = grados * PI / 180.0;
   const double dx = P0[X] - r[X];
   const double dz = P0[Z] - r[Z];
   P0[X] = r[X] + dx * std::cos ( rad ) - dz * std::sin ( rad );
   P0[Z] = r[Z] + dx * std::sin ( rad ) + dz * std::cos ( rad );
}

void igvCamara::cabecear ( double grados )
{  const double rad = grados * PI / 180.0;
   const double dx = r[X] - P0[X];
   const double dy = r[Y] - P0[Y];
   const double dz = r[Z] - P0[Z];

   double ax = V[Y] * dz - V[Z] * dy;
   double ay = V[Z] * dx - V[X] * dz;
   double az = V[X] * dy - V[Y] * dx;
   const double longitud = std::sqrt ( ax * ax + ay * ay + az * az );
   if ( longitud < EPSILON )
   {  return;
   }
   ax /= longitud;
   ay /= longitud;
   az /= longitud;

   const double c = std::cos ( rad );
   const double s = std::sin ( rad );
   const double producto = ax * dx + ay * dy + az * dz;
   const double nx = dx * c + ( ay * dz - az * dy ) * s + ax * producto * ( 1.0 - c );
   const double ny = dy * c + ( az * dx - ax * dz ) * s + ay * producto * ( 1.0 - c );
   const double nz = dz * c + ( ax * dy - ay * dx ) * s + az * producto * ( 1.0 - c );
   r[X] = P0[X] + nx;
   r[Y] = P0[Y] + ny;
   r[Z] = P0[Z] + nz;
}

void igvCamara::rotarY ( double grados )
{  const double rad = grados * PI / 180.0;
   const double dx = r[X] - P0[X];
   const double dz = r[Z] - P0[Z];
   r[X] = P0[X] + dx * std::cos ( rad ) - dz * std::sin ( rad );
   r[Z] = P0[Z] + dx * std::sin ( rad ) + dz * std::cos ( rad );
}

void igvCamara::moverZnear ( double cantidad )
{  setZnear ( znear + cantidad );
}

void igvCamara::moverZfar ( double cantidad )
{  setZfar ( zfar + cantidad );
}

void igvCamara::setZnear ( GLdouble valor )
{  znear = std::max ( 0.1, std::min ( valor, zfar - 0.1 ) );
}

void igvCamara::setZfar ( GLdouble valor )
{  zfar = std::max ( znear + 0.1, valor );
}

tipoCamara igvCamara::getTipo () const { return tipo; }
GLdouble igvCamara::getXwmin () const { return xwmin; }
GLdouble igvCamara::getXwmax () const { return xwmax; }
GLdouble igvCamara::getYwmin () const { return ywmin; }
GLdouble igvCamara::getYwmax () const { return ywmax; }
GLdouble igvCamara::getAngulo () const { return angulo; }
GLdouble igvCamara::getRaspecto () const { return raspecto; }
GLdouble igvCamara::getZnear () const { return znear; }
GLdouble igvCamara::getZfar () const { return zfar; }
const igvPunto3D &igvCamara::getP0 () const { return P0; }
const igvPunto3D &igvCamara::getR () const { return r; }
const igvPunto3D &igvCamara::getV () const { return V; }
