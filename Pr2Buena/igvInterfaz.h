#ifndef __IGVINTERFAZ
#define __IGVINTERFAZ

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include <string>

#include "igvEscena3D.h"
#include "igvCamara.h"

class igvInterfaz
{  private:
      int ancho_ventana = 0;
      int alto_ventana = 0;
      igvEscena3D escena;
      igvCamara camara;
      igvCamara camaraSuperior;
      igvCamara camaraPerfil;
      igvCamara camaraAlzado;

      bool modoCamara = false;
      bool modoRaton = false;
      bool multivista = false;
      bool usarNormales = true;
      bool gouraud = false;
      bool animar = false;
      bool botonIzquierdo = false;
      int cursorX = 0;
      int cursorY = 0;
      int vista = 0;
      float tiempoAnimacion = 0.0f;

      static igvInterfaz *_instancia;
      igvInterfaz ();

      void crear_menu ();
      void aplicarVista ( int indice, int ancho, int alto );
      void dibujarVista ( int indice, int x, int y, int ancho, int alto );
      int seleccionarParte ( int x, int y );
      void actualizarTitulo ();
      void imprimirAyuda ();

   public:
      static igvInterfaz &getInstancia ();
      ~igvInterfaz () = default;

      static void keyboardFunc ( unsigned char key, int x, int y );
      static void specialKeyFunc ( int key, int x, int y );
      static void reshapeFunc ( int w, int h );
      static void displayFunc ();
      static void mouseFunc ( int boton, int estado, int x, int y );
      static void motionFunc ( int x, int y );
      static void idleFunc ();
      static void menuHandle ( int valor );

      void crear_mundo ();
      void configura_entorno ( int argc, char **argv, int _ancho_ventana, int _alto_ventana
                               , int _pos_X, int _pos_Y, std::string _titulo );
      void inicializa_callbacks ();
      void inicia_bucle_visualizacion ();

      int get_ancho_ventana () const;
      int get_alto_ventana () const;
      void set_ancho_ventana ( int _ancho_ventana );
      void set_alto_ventana ( int _alto_ventana );
};

#endif
