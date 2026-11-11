# Practica 2 - Sonda espacial articulada

Proyecto basado en pr2a_conan de Pra2a (https://github.com/darusofran/Pra2a). Usa C++14, GLUT/FreeGLUT, GLU y el pipeline fijo de OpenGL de las practicas. No usa GLM ni dependencias graficas adicionales.

## Modelo

La escena implementa un satelite con transformaciones jerarquicas y pila de matrices:

- Cuerpo central cilindrico como raiz.
- Dos paneles solares con bisagra, que comparten su angulo de inclinacion.
- Torreta de comunicaciones con giro panoramico.
- Brazo robotico solidario a la torreta.
- Sensor terminal con una antena parabolica propia, construida como superficie de revolucion de z = r^2 / (4f) con malla indexada, normales y triangulos.

Las cuatro articulaciones tienen limites para evitar posturas imposibles. El grupo raiz admite ademas las transformaciones globales de la Practica 1.

## Teclas

| Tecla | Accion |
| --- | --- |
| o / O | Trasladar la raiz orbitalmente en X |
| r / R | Girar la torreta alrededor de Y |
| k / K | Inclinar los dos paneles solares |
| d / D | Orientar el sensor del extremo del brazo |
| 1 a 5 | Seleccionar cuerpo, torreta, panel izquierdo, panel derecho o sensor |
| Flechas | Trasladar el modelo completo en X/Z |
| x/X, y/Y, z/Z | Rotar el modelo completo |
| s / S | Escalar el modelo completo |
| c | Activar o desactivar el modo de camara; flechas orbitan y cabecean |
| + / - | Zoom |
| p | Alternar perspectiva y proyeccion paralela |
| f/F, b/B | Ajustar los planos de recorte cercano y lejano |
| v | Cambiar entre panoramica, planta, perfil y alzado |
| V | Activar/desactivar la vista simultanea de cuatro viewports |
| e | Mostrar/ocultar ejes |
| n | Activar/desactivar normales |
| g | Alternar sombreado plano y Gouraud |
| t | Alternar articulacion por teclado/raton |
| a | Activar/desactivar la animacion automatica |
| h | Mostrar esta ayuda en la consola |
| ESC | Salir |

En modo raton, haz clic en una pieza y arrastrala para mover su articulacion. El boton derecho abre el menu de vistas, interaccion, sombreado, animacion y ayuda. La seleccion se resuelve mediante una pasada de colores unicos al buffer de color.

## Compilacion

Abre esta carpeta (pr2a_conan) con CLion o CMake, usando el perfil de Conan del repositorio. Las dependencias del proyecto base son FreeGLUT y OpenGL; GLU se usa para dibujar los eslabones y las primitivas auxiliares.
