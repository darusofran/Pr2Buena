#include "igvInterfaz.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>

namespace
{  enum OpcionesMenu
   {  MENU_SALIR = 1
      , MENU_VISTA_PANORAMICA
      , MENU_VISTA_PLANTA
      , MENU_VISTA_PERFIL
      , MENU_VISTA_ALZADO
      , MENU_MULTIVISTA
      , MENU_INTERACCION_TECLADO
      , MENU_INTERACCION_RATON
      , MENU_SOMBREADO_PLANO
      , MENU_SOMBREADO_GOURAUD
      , MENU_NORMALES
      , MENU_ANIMAR
      , MENU_AYUDA
   };

   bool estadoGL ( GLenum capacidad )
   {  return glIsEnabled ( capacidad ) == GL_TRUE;
   }
}

igvInterfaz *igvInterfaz::_instancia = nullptr;

igvInterfaz::igvInterfaz ()
{
}

igvInterfaz &igvInterfaz::getInstancia ()
{  if ( _instancia == nullptr )
   {  _instancia = new igvInterfaz;
   }
   return *_instancia;
}

void igvInterfaz::crear_mundo ()
{  const float aspecto = alto_ventana > 0
                           ? static_cast<float> ( ancho_ventana ) / alto_ventana : 1.0f;
   camara.set ( IGV_PERSPECTIVA, {6.0, 4.4, 7.0}, {0.0, 0.1, 0.0}
                , {0.0, 1.0, 0.0}, 48.0, aspecto, 0.1, 100.0 );
}

void igvInterfaz::configura_entorno ( int argc, char **argv, int _ancho_ventana
                                      , int _alto_ventana, int _pos_X, int _pos_Y
                                      , std::string _titulo )
{  ancho_ventana = _ancho_ventana;
   alto_ventana = _alto_ventana;

   glutInit ( &argc, argv );
   glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
   glutInitWindowSize ( ancho_ventana, alto_ventana );
   glutInitWindowPosition ( _pos_X, _pos_Y );
   glutCreateWindow ( _titulo.c_str () );

   glEnable ( GL_DEPTH_TEST );
   glClearColor ( 0.97f, 0.98f, 1.0f, 0.0f );
   glEnable ( GL_LIGHTING );
   glEnable ( GL_NORMALIZE );
   glEnable ( GL_COLOR_MATERIAL );
   glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

   const GLfloat luzDifusa[4] = {0.90f, 0.90f, 0.90f, 1.0f};
   const GLfloat luzAmbiente[4] = {0.24f, 0.24f, 0.26f, 1.0f};
   glLightfv ( GL_LIGHT0, GL_DIFFUSE, luzDifusa );
   glLightfv ( GL_LIGHT0, GL_AMBIENT, luzAmbiente );
   glEnable ( GL_LIGHT0 );

   crear_mundo ();
}

void igvInterfaz::inicia_bucle_visualizacion ()
{  glutMainLoop ();
}

void igvInterfaz::inicializa_callbacks ()
{  glutKeyboardFunc ( keyboardFunc );
   glutSpecialFunc ( specialKeyFunc );
   glutReshapeFunc ( reshapeFunc );
   glutDisplayFunc ( displayFunc );
   glutMouseFunc ( mouseFunc );
   glutMotionFunc ( motionFunc );
   crear_menu ();
   actualizarTitulo ();
}

void igvInterfaz::crear_menu ()
{  const int menuVistas = glutCreateMenu ( menuHandle );
   glutAddMenuEntry ( "Panoramica", MENU_VISTA_PANORAMICA );
   glutAddMenuEntry ( "Planta", MENU_VISTA_PLANTA );
   glutAddMenuEntry ( "Perfil", MENU_VISTA_PERFIL );
   glutAddMenuEntry ( "Alzado", MENU_VISTA_ALZADO );
   glutAddMenuEntry ( "Cuatro vistas", MENU_MULTIVISTA );

   const int menuInteraccion = glutCreateMenu ( menuHandle );
   glutAddMenuEntry ( "Articulaciones por teclado", MENU_INTERACCION_TECLADO );
   glutAddMenuEntry ( "Articulaciones por raton", MENU_INTERACCION_RATON );

   const int menuSombreado = glutCreateMenu ( menuHandle );
   glutAddMenuEntry ( "Plano", MENU_SOMBREADO_PLANO );
   glutAddMenuEntry ( "Gouraud", MENU_SOMBREADO_GOURAUD );
   glutAddMenuEntry ( "Alternar normales", MENU_NORMALES );

   glutCreateMenu ( menuHandle );
   glutAddSubMenu ( "Vista", menuVistas );
   glutAddSubMenu ( "Interaccion", menuInteraccion );
   glutAddSubMenu ( "Sombreado", menuSombreado );
   glutAddMenuEntry ( "Animacion: activar / detener", MENU_ANIMAR );
   glutAddMenuEntry ( "Ayuda de teclado", MENU_AYUDA );
   glutAddMenuEntry ( "Salir", MENU_SALIR );
   glutAttachMenu ( GLUT_RIGHT_BUTTON );
}

void igvInterfaz::actualizarTitulo ()
{  if ( glutGetWindow () == 0 )
   {  return;
   }

   const char *nombreVista = vista == 0 ? "panoramica"
                             : vista == 1 ? "planta"
                             : vista == 2 ? "perfil" : "alzado";
   std::string titulo = "IGV - Sonda exploradora | Articulaciones: ";
   titulo += modoRaton ? "raton" : "teclado";
   titulo += " | Vista: ";
   titulo += multivista ? "4 vistas" : nombreVista;
   if ( modoCamara )
   {  titulo += " | Camara activa";
   }
   if ( animar )
   {  titulo += " | Animacion";
   }
   glutSetWindowTitle ( titulo.c_str () );
}

void igvInterfaz::imprimirAyuda ()
{  std::cout << "\n=== Sonda espacial: controles ===\n"
             << "o/O: desplazamiento orbital X +/-\n"
             << "r/R: giro de torreta +/-\n"
             << "k/K: inclinacion de paneles +/-\n"
             << "d/D: orientacion del sensor +/-\n"
             << "1..5: seleccionar cuerpo, torreta, panel izq., panel der. o sensor\n"
             << "x/X, y/Y, z/Z: rotacion global del satelite\n"
             << "Flechas: trasladar modelo; c: activar camara, flechas para orbitar/cabecear\n"
             << "+/-: zoom; p: cambiar proyeccion; f/F y b/B: recorte near/far\n"
             << "v: cambiar vista; V: cuatro vistas; e: ejes; n: normales; g: Gouraud\n"
             << "t: alternar articulacion por teclado/raton; arrastrar una pieza en modo raton\n"
             << "a: animacion automatica; s/S: escala; h: ayuda; ESC: salir\n"
             << "Menu contextual: boton derecho del raton.\n\n";
}

void igvInterfaz::keyboardFunc ( unsigned char key, int, int )
{  igvInterfaz &app = getInstancia ();
   switch ( key )
   {  case 27:
         std::exit ( 0 );
         break;
      case 'e':
      case 'E':
         app.escena.set_ejes ( !app.escena.get_ejes () );
         break;
      case 'o':
         if ( !app.modoRaton ) app.escena.moverOrbita ( 0.12f );
         break;
      case 'O':
         if ( !app.modoRaton ) app.escena.moverOrbita ( -0.12f );
         break;
      case 'r':
         if ( !app.modoRaton ) app.escena.girarTorreta ( 5.0f );
         break;
      case 'R':
         if ( !app.modoRaton ) app.escena.girarTorreta ( -5.0f );
         break;
      case 'k':
         if ( !app.modoRaton ) app.escena.inclinarPaneles ( 5.0f );
         break;
      case 'K':
         if ( !app.modoRaton ) app.escena.inclinarPaneles ( -5.0f );
         break;
      case 'd':
         if ( !app.modoRaton ) app.escena.orientarSensor ( 5.0f );
         break;
      case 'D':
         if ( !app.modoRaton ) app.escena.orientarSensor ( -5.0f );
         break;
      case '1': app.escena.seleccionarParte ( PARTE_CUERPO ); break;
      case '2': app.escena.seleccionarParte ( PARTE_TORRETA ); break;
      case '3': app.escena.seleccionarParte ( PARTE_PANEL_IZQUIERDO ); break;
      case '4': app.escena.seleccionarParte ( PARTE_PANEL_DERECHO ); break;
      case '5': app.escena.seleccionarParte ( PARTE_SENSOR ); break;
      case 'x': app.escena.rotarModelo ( 5.0f, 0.0f, 0.0f ); break;
      case 'X': app.escena.rotarModelo ( -5.0f, 0.0f, 0.0f ); break;
      case 'y': app.escena.rotarModelo ( 0.0f, 5.0f, 0.0f ); break;
      case 'Y': app.escena.rotarModelo ( 0.0f, -5.0f, 0.0f ); break;
      case 'z': app.escena.rotarModelo ( 0.0f, 0.0f, 5.0f ); break;
      case 'Z': app.escena.rotarModelo ( 0.0f, 0.0f, -5.0f ); break;
      case 's': app.escena.escalarModelo ( 1.1f ); break;
      case 'S': app.escena.escalarModelo ( 1.0f / 1.1f ); break;
      case 'c':
      case 'C':
         app.modoCamara = !app.modoCamara;
         if ( app.modoCamara && !app.multivista )
         {  app.vista = 0;
         }
         break;
      case 'p':
      case 'P':
         if ( app.camara.getTipo () == IGV_PERSPECTIVA )
         {  app.camara.set ( IGV_PARALELA, app.camara.getP0 (), app.camara.getR ()
                              , app.camara.getV (), -4.5, 4.5, -4.5, 4.5
                              , -30.0, 100.0 );
         }
         else
         {  app.camara.set ( IGV_PERSPECTIVA, app.camara.getP0 (), app.camara.getR ()
                              , app.camara.getV (), 48.0
                              , static_cast<double> ( app.ancho_ventana ) / std::max ( 1, app.alto_ventana )
                              , 0.1, 100.0 );
         }
         break;
      case '+': app.camara.zoom ( -10.0 ); break;
      case '-': app.camara.zoom ( 10.0 ); break;
      case 'f': app.camara.moverZnear ( -0.1 ); break;
      case 'F': app.camara.moverZnear ( 0.1 ); break;
      case 'b': app.camara.moverZfar ( 1.0 ); break;
      case 'B': app.camara.moverZfar ( -1.0 ); break;
      case 'v':
         app.vista = ( app.vista + 1 ) % 4;
         break;
      case 'V':
         app.multivista = !app.multivista;
         break;
      case 't':
      case 'T':
         app.modoRaton = !app.modoRaton;
         app.botonIzquierdo = false;
         break;
      case 'n':
      case 'N':
         app.usarNormales = !app.usarNormales;
         break;
      case 'g':
      case 'G':
         app.gouraud = !app.gouraud;
         break;
      case 'a':
      case 'A':
         app.animar = !app.animar;
         glutIdleFunc ( app.animar ? idleFunc : nullptr );
         break;
      case 'h':
      case 'H':
         app.imprimirAyuda ();
         break;
      default:
         break;
   }

   app.actualizarTitulo ();
   glutPostRedisplay ();
}

void igvInterfaz::specialKeyFunc ( int key, int, int )
{  igvInterfaz &app = getInstancia ();
   if ( app.modoCamara )
   {  switch ( key )
      {  case GLUT_KEY_LEFT: app.camara.orbitar ( -5.0 ); break;
         case GLUT_KEY_RIGHT: app.camara.orbitar ( 5.0 ); break;
         case GLUT_KEY_UP: app.camara.cabecear ( 5.0 ); break;
         case GLUT_KEY_DOWN: app.camara.cabecear ( -5.0 ); break;
         default: break;
      }
   }
   else
   {  switch ( key )
      {  case GLUT_KEY_LEFT: app.escena.trasladarModelo ( -0.12f, 0.0f, 0.0f ); break;
         case GLUT_KEY_RIGHT: app.escena.trasladarModelo ( 0.12f, 0.0f, 0.0f ); break;
         case GLUT_KEY_UP: app.escena.trasladarModelo ( 0.0f, 0.0f, -0.12f ); break;
         case GLUT_KEY_DOWN: app.escena.trasladarModelo ( 0.0f, 0.0f, 0.12f ); break;
         default: break;
      }
   }
   glutPostRedisplay ();
}

void igvInterfaz::aplicarVista ( int indice, int ancho, int alto )
{  const GLdouble aspecto = static_cast<GLdouble> ( ancho ) / std::max ( 1, alto );
   if ( indice == 0 )
   {  camara.aplicar ( aspecto );
      return;
   }

   const igvPunto3D objetivo ( escena.getCentroX (), escena.getCentroY (), escena.getCentroZ () );
   if ( indice == 1 )
   {  camaraSuperior.set ( IGV_PARALELA
                           , { objetivo[X], objetivo[Y] + 10.0, objetivo[Z] }
                           , objetivo, {0.0, 0.0, -1.0}
                           , -4.0, 4.0, -4.0, 4.0, -30.0, 50.0 );
      camaraSuperior.aplicar ( aspecto );
   }
   else if ( indice == 2 )
   {  camaraPerfil.set ( IGV_PARALELA
                        , { objetivo[X] + 10.0, objetivo[Y] + 0.8, objetivo[Z] }
                        , objetivo, {0.0, 1.0, 0.0}
                        , -4.0, 4.0, -4.0, 4.0, -30.0, 50.0 );
      camaraPerfil.aplicar ( aspecto );
   }
   else
   {  camaraAlzado.set ( IGV_PARALELA
                         , { objetivo[X], objetivo[Y] + 0.8, objetivo[Z] + 10.0 }
                         , objetivo, {0.0, 1.0, 0.0}
                         , -4.0, 4.0, -4.0, 4.0, -30.0, 50.0 );
      camaraAlzado.aplicar ( aspecto );
   }
}

void igvInterfaz::dibujarVista ( int indice, int x, int y, int ancho, int alto )
{  if ( ancho <= 0 || alto <= 0 )
   {  return;
   }
   glViewport ( x, y, ancho, alto );
   aplicarVista ( indice, ancho, alto );
   escena.visualizar ( false, usarNormales, gouraud, animar, tiempoAnimacion );
}

/*void igvInterfaz::displayFunc ()
{  igvInterfaz &app = getInstancia ();
   glDrawBuffer ( GL_BACK );
   glClearColor ( 0.97f, 0.98f, 1.0f, 0.0f );
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   if ( app.multivista )
   {  const int mitadAncho = app.ancho_ventana / 2;
      const int mitadAlto = app.alto_ventana / 2;
      app.dibujarVista ( 0, 0, mitadAlto, mitadAncho, app.alto_ventana - mitadAlto );
      glClear ( GL_DEPTH_BUFFER_BIT );
      app.dibujarVista ( 1, mitadAncho, mitadAlto
                         , app.ancho_ventana - mitadAncho, app.alto_ventana - mitadAlto );
      glClear ( GL_DEPTH_BUFFER_BIT );
      app.dibujarVista ( 2, 0, 0, mitadAncho, mitadAlto );
      glClear ( GL_DEPTH_BUFFER_BIT );
      app.dibujarVista ( 3, mitadAncho, 0
                         , app.ancho_ventana - mitadAncho, mitadAlto );
   }
   else
   {  app.dibujarVista ( app.vista, 0, 0, app.ancho_ventana, app.alto_ventana );
   }

   // VIEWPORT SUPERIOR

   int anchoSuperior = _instancia->ancho_ventana / 4;

   int altoSuperior = _instancia->alto_ventana / 4;


   // Lo ponemos arriba a la derecha
   glViewport(
       _instancia->ancho_ventana - anchoSuperior - 10,
       _instancia->alto_ventana - altoSuperior - 10,
       anchoSuperior,
       altoSuperior
   );

   // Cámara superior
   _instancia->camaraSuperior.aplicar();

   // Dibujamos la misma escena
   _instancia->escena.visualizar();


   glutSwapBuffers ();
}*/

void igvInterfaz::displayFunc()
{  igvInterfaz &app = getInstancia ();
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Vista principal seleccionada por el usuario.
   app.dibujarVista ( app.vista, 0, 0, app.ancho_ventana, app.alto_ventana );

   // Viewport auxiliar con la vista en planta, como en Pr1.
   const int anchoSuperior = std::max ( 1, app.ancho_ventana / 4 );
   const int altoSuperior = std::max ( 1, app.alto_ventana / 4 );
   const int xSuperior = app.ancho_ventana - anchoSuperior - 10;
   const int ySuperior = app.alto_ventana - altoSuperior - 10;

   glEnable ( GL_SCISSOR_TEST );
   glScissor ( xSuperior, ySuperior, anchoSuperior, altoSuperior );
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   app.dibujarVista ( 1, xSuperior, ySuperior, anchoSuperior, altoSuperior );
   glDisable ( GL_SCISSOR_TEST );

   glutSwapBuffers ();
}

void igvInterfaz::reshapeFunc ( int w, int h )
{  igvInterfaz &app = getInstancia ();
   app.ancho_ventana = std::max ( 1, w );
   app.alto_ventana = std::max ( 1, h );
   glutPostRedisplay ();
}

int igvInterfaz::seleccionarParte ( int x, int y )
{  if ( ancho_ventana <= 0 || alto_ventana <= 0 )
   {  return 0;
   }

   int indiceVista = vista;
   int vx = 0;
   int vy = 0;
   int vw = ancho_ventana;
   int vh = alto_ventana;
   const int yOpenGL = alto_ventana - 1 - y;

   if ( multivista )
   {  const int mitadAncho = ancho_ventana / 2;
      const int mitadAlto = alto_ventana / 2;
      const bool izquierda = x < mitadAncho;
      const bool arriba = yOpenGL >= mitadAlto;
      indiceVista = arriba ? ( izquierda ? 0 : 1 ) : ( izquierda ? 2 : 3 );
      vx = izquierda ? 0 : mitadAncho;
      vy = arriba ? mitadAlto : 0;
      vw = izquierda ? mitadAncho : ancho_ventana - mitadAncho;
      vh = arriba ? alto_ventana - mitadAlto : mitadAlto;
   }

   const GLboolean iluminacionAnterior = estadoGL ( GL_LIGHTING );
   const GLboolean ditherAnterior = estadoGL ( GL_DITHER );
   const GLboolean blendAnterior = estadoGL ( GL_BLEND );
   GLint bufferDibujoAnterior = GL_BACK;
   GLint bufferLecturaAnterior = GL_BACK;
   GLfloat colorBorradoAnterior[4];
   glGetIntegerv ( GL_DRAW_BUFFER, &bufferDibujoAnterior );
   glGetIntegerv ( GL_READ_BUFFER, &bufferLecturaAnterior );
   glGetFloatv ( GL_COLOR_CLEAR_VALUE, colorBorradoAnterior );

#ifdef GL_MULTISAMPLE
   const GLboolean multisampleAnterior = estadoGL ( GL_MULTISAMPLE );
   glDisable ( GL_MULTISAMPLE );
#endif
   glDisable ( GL_LIGHTING );
   glDisable ( GL_DITHER );
   glDisable ( GL_BLEND );
   glDrawBuffer ( GL_BACK );
   glReadBuffer ( GL_BACK );
   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glViewport ( vx, vy, vw, vh );
   aplicarVista ( indiceVista, vw, vh );
   escena.visualizar ( true, false, false, false, 0.0f );
   glFlush ();

   GLubyte pixel[3] = {0, 0, 0};
   if ( x >= vx && x < vx + vw && yOpenGL >= vy && yOpenGL < vy + vh )
   {  glReadPixels ( x, yOpenGL, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel );
   }

   glClearColor ( colorBorradoAnterior[0], colorBorradoAnterior[1]
                  , colorBorradoAnterior[2], colorBorradoAnterior[3] );
   glDrawBuffer ( static_cast<GLenum> ( bufferDibujoAnterior ) );
   glReadBuffer ( static_cast<GLenum> ( bufferLecturaAnterior ) );
   if ( iluminacionAnterior ) glEnable ( GL_LIGHTING ); else glDisable ( GL_LIGHTING );
   if ( ditherAnterior ) glEnable ( GL_DITHER ); else glDisable ( GL_DITHER );
   if ( blendAnterior ) glEnable ( GL_BLEND ); else glDisable ( GL_BLEND );
#ifdef GL_MULTISAMPLE
   if ( multisampleAnterior ) glEnable ( GL_MULTISAMPLE ); else glDisable ( GL_MULTISAMPLE );
#endif

   return pixel[0] >= PARTE_CUERPO && pixel[0] <= PARTE_SENSOR ? pixel[0] : 0;
}

void igvInterfaz::mouseFunc ( int boton, int estado, int x, int y )
{  igvInterfaz &app = getInstancia ();
   if ( boton == GLUT_LEFT_BUTTON )
   {  if ( estado == GLUT_DOWN )
      {  const int parte = app.seleccionarParte ( x, y );
         if ( parte != 0 )
         {  app.escena.seleccionarParte ( parte );
         }
         app.botonIzquierdo = true;
         app.cursorX = x;
         app.cursorY = y;
      }
      else
      {  app.botonIzquierdo = false;
      }
      app.actualizarTitulo ();
      glutPostRedisplay ();
   }
}

void igvInterfaz::motionFunc ( int x, int y )
{  igvInterfaz &app = getInstancia ();
   if ( app.botonIzquierdo && app.modoRaton )
   {  const int dx = x - app.cursorX;
      const int dy = y - app.cursorY;
      switch ( app.escena.getParteSeleccionada () )
      {  case PARTE_CUERPO:
            app.escena.moverOrbita ( dx * 0.012f );
            break;
         case PARTE_TORRETA:
            app.escena.girarTorreta ( dx * 0.8f );
            break;
         case PARTE_PANEL_IZQUIERDO:
         case PARTE_PANEL_DERECHO:
            app.escena.inclinarPaneles ( -dy * 0.55f );
            break;
         case PARTE_SENSOR:
            app.escena.orientarSensor ( dx * 0.65f - dy * 0.2f );
            break;
         default:
            break;
      }
   }
   app.cursorX = x;
   app.cursorY = y;
   glutPostRedisplay ();
}

void igvInterfaz::idleFunc ()
{  igvInterfaz &app = getInstancia ();
   if ( app.animar )
   {  app.tiempoAnimacion = static_cast<float> ( glutGet ( GLUT_ELAPSED_TIME ) ) / 1000.0f;
      glutPostRedisplay ();
   }
}

void igvInterfaz::menuHandle ( int valor )
{  igvInterfaz &app = getInstancia ();
   switch ( valor )
   {  case MENU_SALIR:
         std::exit ( 0 );
         break;
      case MENU_VISTA_PANORAMICA: app.vista = 0; app.multivista = false; break;
      case MENU_VISTA_PLANTA: app.vista = 1; app.multivista = false; break;
      case MENU_VISTA_PERFIL: app.vista = 2; app.multivista = false; break;
      case MENU_VISTA_ALZADO: app.vista = 3; app.multivista = false; break;
      case MENU_MULTIVISTA: app.multivista = !app.multivista; break;
      case MENU_INTERACCION_TECLADO: app.modoRaton = false; break;
      case MENU_INTERACCION_RATON: app.modoRaton = true; break;
      case MENU_SOMBREADO_PLANO: app.gouraud = false; break;
      case MENU_SOMBREADO_GOURAUD: app.gouraud = true; break;
      case MENU_NORMALES: app.usarNormales = !app.usarNormales; break;
      case MENU_ANIMAR:
         app.animar = !app.animar;
         glutIdleFunc ( app.animar ? idleFunc : nullptr );
         break;
      case MENU_AYUDA: app.imprimirAyuda (); break;
      default: break;
   }
   app.actualizarTitulo ();
   glutPostRedisplay ();
}

int igvInterfaz::get_ancho_ventana () const { return ancho_ventana; }
int igvInterfaz::get_alto_ventana () const { return alto_ventana; }
void igvInterfaz::set_ancho_ventana ( int _ancho_ventana ) { ancho_ventana = _ancho_ventana; }
void igvInterfaz::set_alto_ventana ( int _alto_ventana ) { alto_ventana = _alto_ventana; }
