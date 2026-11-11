#include "igvEscena3D.h"

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace
{  const float PI = 3.14159265358979323846f;

   void colorPieza ( int parte, bool seleccion, int parteActiva
                     , const GLfloat color[3] )
   {  if ( seleccion )
      {  glColor3ub ( static_cast<GLubyte> ( parte ), 0, 0 );
      }
      else
      {  const GLfloat emisionSeleccionada[4] = { 0.16f, 0.13f, 0.025f, 1.0f };
         const GLfloat sinEmision[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
         glColor3fv ( color );
         glMaterialfv ( GL_FRONT_AND_BACK, GL_EMISSION
                        , parte == parteActiva ? emisionSeleccionada : sinEmision );
      }
   }

   void caja ( float sx, float sy, float sz )
   {  glPushMatrix ();
      glScalef ( sx, sy, sz );
      glutSolidCube ( 1.0 );
      glPopMatrix ();
   }

   void cilindroZ ( float radio, float longitud )
   {  GLUquadric *q = gluNewQuadric ();
      if ( q != nullptr )
      {  gluQuadricNormals ( q, GLU_SMOOTH );
         gluCylinder ( q, radio, radio, longitud, 16, 2 );
         gluDeleteQuadric ( q );
      }
   }

   void lineasPanel ( float signo )
   {  const GLfloat rejilla[3] = { 0.40f, 0.76f, 0.94f };
      glColor3fv ( rejilla );
      glBegin ( GL_LINES );
      for ( int i = 1 ; i < 4 ; ++i )
      {  const float x = signo * ( 0.55f * i );
         glVertex3f ( x, 0.055f, -0.57f );
         glVertex3f ( x, 0.055f, 0.57f );
      }
      for ( int j = 1 ; j < 3 ; ++j )
      {  const float z = -0.57f + 1.14f * j / 3.0f;
         glVertex3f ( signo * 0.55f, 0.055f, z );
         glVertex3f ( signo * 2.20f, 0.055f, z );
      }
      glEnd ();
   }
}

igvEscena3D::igvEscena3D ()
{  cuerpo = new igvCilindro ( 0.43f, 1.1f, 40, 6 );
   antena = new igvSuperficieRevolucion ( 0.62f, 0.17f, 18, 48 );
}

igvEscena3D::~igvEscena3D ()
{  delete cuerpo;
   cuerpo = nullptr;
   delete antena;
   antena = nullptr;
}

void igvEscena3D::pintar_ejes ()
{  const GLfloat rojo[3] = { 1.0f, 0.0f, 0.0f };
   const GLfloat verde[3] = { 0.0f, 1.0f, 0.0f };
   const GLfloat azul[3] = { 0.0f, 0.0f, 1.0f };
   glDisable ( GL_LIGHTING );

   glColor3fv ( rojo );
   glBegin ( GL_LINES );
   glVertex3f ( -5.0f, 0.0f, 0.0f );
   glVertex3f ( 5.0f, 0.0f, 0.0f );
   glEnd ();

   glColor3fv ( verde );
   glBegin ( GL_LINES );
   glVertex3f ( 0.0f, -5.0f, 0.0f );
   glVertex3f ( 0.0f, 5.0f, 0.0f );
   glEnd ();

   glColor3fv ( azul );
   glBegin ( GL_LINES );
   glVertex3f ( 0.0f, 0.0f, -5.0f );
   glVertex3f ( 0.0f, 0.0f, 5.0f );
   glEnd ();

   glEnable ( GL_LIGHTING );
}

void igvEscena3D::visualizar ( bool modoSeleccion, bool usarNormales
                               , bool gouraud, bool animar, float tiempo )
{  if ( !modoSeleccion )
   {  const GLfloat luz[4] = { 3.0f, 5.0f, 6.0f, 1.0f };
      glLightfv ( GL_LIGHT0, GL_POSITION, luz );
      glEnable ( GL_LIGHT0 );
      if ( ejes )
      {  pintar_ejes ();
      }
   }

   const GLfloat cuerpoColor[3] = { 0.72f, 0.76f, 0.79f };
   const GLfloat metalColor[3] = { 0.46f, 0.52f, 0.57f };
   const GLfloat panelColor[3] = { 0.045f, 0.16f, 0.36f };
   const GLfloat antenaColor[3] = { 0.83f, 0.69f, 0.39f };
   const GLfloat receptorColor[3] = { 0.88f, 0.32f, 0.09f };
   const GLfloat emisionSensor[4] = { 0.16f, 0.13f, 0.025f, 1.0f };
   const GLfloat emisionCero[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

   const float desplazamientoAnimado = animar ? 0.55f * std::sin ( tiempo * 0.55f ) : 0.0f;
   const float yawAnimado = animar ? tiempo * 22.0f : 0.0f;
   const float panelAnimado = animar ? 18.0f * std::sin ( tiempo * 1.2f ) : 0.0f;
   const float sensorAnimado = animar ? 16.0f * std::sin ( tiempo * 1.7f ) : 0.0f;
   const float panelActual = std::max ( 0.0f, std::min ( 80.0f, inclinacionPaneles + panelAnimado ) );
   const float sensorActual = std::max ( -75.0f, std::min ( 75.0f, orientacionSensor + sensorAnimado ) );

   glPushMatrix ();
   glTranslatef ( desplazamientoOrbital + desplazamientoAnimado + transformacion.tx
                  , transformacion.ty, transformacion.tz );
   glRotatef ( transformacion.rx, 1.0f, 0.0f, 0.0f );
   glRotatef ( transformacion.ry, 0.0f, 1.0f, 0.0f );
   glRotatef ( transformacion.rz, 0.0f, 0.0f, 1.0f );
   glScalef ( transformacion.s, transformacion.s, transformacion.s );

   // Nivel 1: cuerpo principal del satelite.
   colorPieza ( PARTE_CUERPO, modoSeleccion, parteSeleccionada, cuerpoColor );
   glPushMatrix ();
   glTranslatef ( 0.0f, -0.55f, 0.0f );
   if ( cuerpo != nullptr )
   {  cuerpo->visualizar ( usarNormales, gouraud );
   }
   glPopMatrix ();
   glPushMatrix ();
   glTranslatef ( 0.0f, 0.55f, 0.0f );
   glutSolidSphere ( 0.43, 24, 16 );
   glPopMatrix ();
   glPushMatrix ();
   glTranslatef ( 0.0f, -0.55f, 0.0f );
   glutSolidSphere ( 0.43, 24, 16 );
   glPopMatrix ();
   glPushMatrix ();
   glTranslatef ( 0.0f, 0.0f, 0.46f );
   caja ( 0.10f, 0.40f, 0.18f );
   glPopMatrix ();

   // Nivel 2, ramas A y B: soportes y paneles solares con bisagra compartida.
   for ( int lado = -1 ; lado <= 1 ; lado += 2 )
   {  const int partePanel = lado < 0 ? PARTE_PANEL_IZQUIERDO : PARTE_PANEL_DERECHO;
      colorPieza ( partePanel, modoSeleccion, parteSeleccionada, metalColor );
      glPushMatrix ();
      glTranslatef ( lado * 0.40f, 0.0f, 0.0f );
      glRotatef ( lado * panelActual, 0.0f, 0.0f, 1.0f );
      glPushMatrix ();
      glTranslatef ( lado * 0.40f, 0.0f, 0.0f );
      caja ( 0.85f, 0.07f, 0.07f );
      glPopMatrix ();

      colorPieza ( partePanel, modoSeleccion, parteSeleccionada, panelColor );
      glPushMatrix ();
      glTranslatef ( lado * 1.35f, 0.0f, 0.0f );
      caja ( 1.75f, 0.075f, 1.22f );
      glPopMatrix ();
      if ( !modoSeleccion )
      {  lineasPanel ( static_cast<float> ( lado ) );
      }
      glPopMatrix ();
   }

   // Nivel 2 y 3: torreta panoramica y brazo robotico.
   glPushMatrix ();
   glTranslatef ( 0.0f, 0.58f, 0.0f );
   glRotatef ( giroTorreta + yawAnimado, 0.0f, 1.0f, 0.0f );
   colorPieza ( PARTE_TORRETA, modoSeleccion, parteSeleccionada, metalColor );
   glPushMatrix ();
   glRotatef ( 90.0f, 1.0f, 0.0f, 0.0f );
   glutSolidTorus ( 0.07, 0.28, 12, 32 );
   glPopMatrix ();
   glPushMatrix ();
   glTranslatef ( 0.0f, 0.22f, 0.0f );
   caja ( 0.16f, 0.46f, 0.16f );
   glPopMatrix ();

   glPushMatrix ();
   glTranslatef ( 0.0f, 0.40f, 0.0f );
   caja ( 0.22f, 0.12f, 0.22f );
   glPushMatrix ();
   glTranslatef ( 0.0f, 0.0f, 0.10f );
   cilindroZ ( 0.065f, 0.62f );
   glTranslatef ( 0.0f, 0.0f, 0.62f );
   glutSolidSphere ( 0.10, 16, 12 );

   glRotatef ( -20.0f, 1.0f, 0.0f, 0.0f );
   glTranslatef ( 0.0f, 0.0f, 0.0f );
   cilindroZ ( 0.052f, 0.55f );

   glTranslatef ( 0.0f, 0.0f, 0.55f );
   colorPieza ( PARTE_SENSOR, modoSeleccion, parteSeleccionada, antenaColor );
   glRotatef ( sensorActual, 0.0f, 1.0f, 0.0f );

   if ( antena != nullptr )
   {  antena->visualizar ( usarNormales, gouraud );
   }
   if ( modoSeleccion )
   {  glColor3ub ( PARTE_SENSOR, 0, 0 );
   }
   else
   {  glMaterialfv ( GL_FRONT_AND_BACK, GL_EMISSION
                     , parteSeleccionada == PARTE_SENSOR ? emisionSensor : emisionCero );
   }
   glPushMatrix ();
   glTranslatef ( 0.0f, 0.0f, 0.17f );
   glutSolidTorus ( 0.018, 0.62, 10, 40 );
   glPopMatrix ();

   colorPieza ( PARTE_SENSOR, modoSeleccion, parteSeleccionada, receptorColor );
   glPushMatrix ();
   glTranslatef ( 0.0f, 0.0f, 0.17f );
   cilindroZ ( 0.035f, 0.34f );
   glPopMatrix ();
   glPushMatrix ();
   glTranslatef ( 0.0f, 0.0f, 0.58f );
   glutSolidSphere ( 0.075, 16, 12 );
   glPopMatrix ();
   glPopMatrix ();
   glPopMatrix ();

   glPopMatrix ();
   glPopMatrix ();
}

bool igvEscena3D::get_ejes () const
{  return ejes;
}

void igvEscena3D::set_ejes ( bool _ejes )
{  ejes = _ejes;
}

void igvEscena3D::seleccionarParte ( int parte )
{  if ( parte >= PARTE_CUERPO && parte <= PARTE_SENSOR )
   {  parteSeleccionada = parte;
   }
}

int igvEscena3D::getParteSeleccionada () const
{  return parteSeleccionada;
}

void igvEscena3D::moverOrbita ( float incremento )
{  desplazamientoOrbital = std::max ( -2.0f, std::min ( 2.0f, desplazamientoOrbital + incremento ) );
}

void igvEscena3D::girarTorreta ( float incremento )
{  giroTorreta += incremento;
   if ( giroTorreta >= 360.0f || giroTorreta <= -360.0f )
   {  giroTorreta = std::fmod ( giroTorreta, 360.0f );
   }
}

void igvEscena3D::inclinarPaneles ( float incremento )
{  inclinacionPaneles = std::max ( 0.0f, std::min ( 80.0f, inclinacionPaneles + incremento ) );
}

void igvEscena3D::orientarSensor ( float incremento )
{  orientacionSensor = std::max ( -75.0f, std::min ( 75.0f, orientacionSensor + incremento ) );
}

void igvEscena3D::trasladarModelo ( float dx, float dy, float dz )
{  transformacion.tx = std::max ( -4.0f, std::min ( 4.0f, transformacion.tx + dx ) );
   transformacion.ty = std::max ( -3.0f, std::min ( 3.0f, transformacion.ty + dy ) );
   transformacion.tz = std::max ( -4.0f, std::min ( 4.0f, transformacion.tz + dz ) );
}

void igvEscena3D::rotarModelo ( float dx, float dy, float dz )
{  transformacion.rx += dx;
   transformacion.ry += dy;
   transformacion.rz += dz;
}

void igvEscena3D::escalarModelo ( float factor )
{  transformacion.s = std::max ( 0.35f, std::min ( 2.5f, transformacion.s * factor ) );
}

float igvEscena3D::getCentroX () const
{  return transformacion.tx + desplazamientoOrbital;
}

float igvEscena3D::getCentroY () const
{  return transformacion.ty;
}

float igvEscena3D::getCentroZ () const
{  return transformacion.tz;
}
