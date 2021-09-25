#ifndef SBRFC_INCLUDE_MI_H_
#define SBRFC_INCLUDE_MI_H_

#include "BaseConocimientos.h"
#include "BaseHechos.h"

void setPrioridades(int numPrio, int * prioridades);
Hecho* EncaminamientoAdelante(BaseConocimientos * BC , BaseHechos * BH ,string objetivo, ofstream *f);
void PrintReglasUsadas(BaseConocimientos * BC , ofstream *f);

#endif // SBRFC_INCLUDE_MI_H_
