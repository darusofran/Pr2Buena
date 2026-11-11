#ifndef __IGVSUPERFICIEREVOLUCION
#define __IGVSUPERFICIEREVOLUCION

#include "igvMallaTriangulos.h"

/**
 * Superficie parabolica de revolucion alrededor del eje Z.
 * La parabola z = r^2 / (4f) se muestrea en anillos y sectores.
 */
class igvSuperficieRevolucion : public igvMallaTriangulos
{  public:
      igvSuperficieRevolucion ( float radio, float profundidad
                                , int divisionesRadiales, int divisionesAngulares );
      ~igvSuperficieRevolucion () override = default;
};

#endif
