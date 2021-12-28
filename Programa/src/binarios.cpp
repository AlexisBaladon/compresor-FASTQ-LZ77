#include "../include/binarios.h"

bool bitEnPosicion(int posicion, unsigned char caracter) {
    return caracter & (1<< (8-posicion));
}

bool bitEnPosicion(int posicion,int num) {
    return num & (1<< (32-posicion));
}

void hacerBit1(int posicion, int & numeroModificado) {
    numeroModificado = numeroModificado | (1 << (32 - posicion));
}

void hacerBit1(int posicion, unsigned char & caracterModificado) {
    caracterModificado = caracterModificado | (1 << (8 - posicion));
}