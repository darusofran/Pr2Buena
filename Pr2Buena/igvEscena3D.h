#ifndef __IGVESCENA3D
#define __IGVESCENA3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include "igvCilindro.h"
#include "igvSuperficieRevolucion.h"

struct Transformaciones
{  float tx = 0.0f, ty = 0.0f, tz = 0.0f;
   float rx = 0.0f, ry = 0.0f, rz = 0.0f;
   float s = 1.0f;
};

enum ParteSatelite
{  PARTE_CUERPO = 1
   , PARTE_TORRETA = 2
   , PARTE_PANEL_IZQUIERDO = 3
   , PARTE_PANEL_DERECHO = 4
   , PARTE_SENSOR = 5
};

class igvEscena3D
{  private:
      bool ejes = true;
      igvCilindro *cuerpo = nullptr;
      igvSuperficieRevolucion *antena = nullptr;

      Transformaciones transformacion;
      float desplazamientoOrbital = 0.0f;
      float giroTorreta = 0.0f;
      float inclinacionPaneles = 18.0f;
      float orientacionSensor = 0.0f;
      int parteSeleccionada = PARTE_CUERPO;

   public:
      igvEscena3D ();
      ~igvEscena3D ();

      void visualizar ( bool modoSeleccion = false, bool usarNormales = true
                        , bool gouraud = false, bool animar = false
                        , float tiempo = 0.0f );

      bool get_ejes () const;
      void set_ejes ( bool _ejes );
      void seleccionarParte ( int parte );
      int getParteSeleccionada () const;

      void moverOrbita ( float incremento );
      void girarTorreta ( float incremento );
      void inclinarPaneles ( float incremento );
      void orientarSensor ( float incremento );
      void trasladarModelo ( float dx, float dy, float dz );
      void rotarModelo ( float dx, float dy, float dz );
      void escalarModelo ( float factor );

      float getCentroX () const;
      float getCentroY () const;
      float getCentroZ () const;

   private:
      void pintar_ejes ();
};

#endif
