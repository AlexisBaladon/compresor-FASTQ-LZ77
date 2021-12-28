#ifndef binarios
#define binarios

//Devuelve el bit de la posición "posicion", en orden de bits más significativos a menos (b7 está en la posición 1 y b0 en la 8).
bool bitEnPosicion(int posicion, unsigned char);
bool bitEnPosicion(int posicion, int);

//Transforma el bit en la posicion "posicion" en 1
void hacerBit1(int posicion, int & numeroModificado);
void hacerBit1(int posicion, unsigned char & caracterModificado);

#endif