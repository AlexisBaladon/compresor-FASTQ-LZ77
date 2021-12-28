/*

for each read menos el ultimo
    comprimir largos de reads
    escribir 1
comprimir ultimo largo
escribir 0

Si vale la pena reescribir el alfabeto (cantidad caracteres < 128)
    escribir 1
    escribir cantidad caracteres
    for each caracter
        escribir caracter crudo (se les asignara 0 al caracter 0, 1 al 1, ...)
else
    escribir 0

for 1:N
    comprimir caracter según alfabeto definido

for each tupla
    si es solo un caracter
        escribir 0
        escribir caracter según alfabeto
    si no
        escribir 1
        escribir largo crudo
        escribir offset crudo

*/

#ifndef COMPRESOR
#define COMPRESOR

#include "../include/lectura.h"
#include "../include/escritura.h"
#include "arregloCircular.h"

#include <list>
#include <string>
#include <vector>


class Compresor {
    private:
        int N;
        //int cantidadCerosBasura = 0;
        unsigned char ultimoCaracterComprimido = 0;
        int posicionBinaria = 1;
        int ** alfabeto;

    public:
        Compresor();
        Compresor(int N, int ** alfabeto);
        ~Compresor();
        void setN(int);
        void setAlfabeto(int ** alfabeto);
        int ** getAlfabeto();
        int getPosicionBinaria();
        void setPosicionBinaria(int);
        unsigned char getUltimoCaracterComprimido();
        void setUltimoCaracterComprimido(unsigned char);

        void escribirBinario(Escritura &, bool binario);
        //Separa cada segmento en diferentes archivos
        void organizarArchivos(Lectura & lecturaArchivo, Escritura & escrituraArchivoComprimido, Escritura & escrituraSegmento1, Escritura & escrituraSegmento2, Escritura & escrituraSegmento4);
        void comprimirElias(Escritura &, int num);
        void comprimirLargoReads(Lectura &, Escritura &, std::string direccionArchivoComprimido);
        void conseguirAlfabeto(Lectura &);
        void comprimirAlfabeto(Escritura &);
        void comprimirPrimerosN(Lectura &, Escritura &, ArregloCircular & primerosNCaracteres);
        void comprimirLZ77(Lectura &, Escritura &, ArregloCircular & anterioresNCaracteres);
        void comprimirCerosBasura(Escritura &);

        //Separa el archivo a comprimir en distintos archivos auxiliares
        void transferirSegmentos(Lectura &, Escritura &);
};

#endif