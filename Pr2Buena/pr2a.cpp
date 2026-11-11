#include <cstdlib>

#include "igvInterfaz.h"

int main ( int argc, char **argv )
{  igvInterfaz::getInstancia ().configura_entorno ( argc, argv
                                                   , 1100, 800
                                                   , 80, 60
                                                   , "IGV: Sonda exploradora" );
   igvInterfaz::getInstancia ().inicializa_callbacks ();
   igvInterfaz::getInstancia ().inicia_bucle_visualizacion ();
   return EXIT_SUCCESS;
}
