#include "../include/descompresor.h"
#include "../include/lectura.h"
#include "../include/escritura.h"
#include "../include/binarios.h"

#include <list>
#include <iterator>
#include <vector>
#include <cmath>
#include <iostream>

//Setters, getters, creador y destructor.

Descompresor::Descompresor(int N): ultimosNCaracteresDescomprimidos(N) {
    this->N = N;
}

Descompresor::~Descompresor() {
    alfabeto.clear();
}

void Descompresor::setAlfabeto(std::vector<unsigned char> alfabeto) {
    this->alfabeto = alfabeto;
}

bool Descompresor::getFinDescompresion() {
    return finDescompresion;
}

int Descompresor::getPosicionBinaria() {
    return posicionBinaria;
}

void Descompresor::setPosicionBinaria(int posicionBinaria) {
    this->posicionBinaria = posicionBinaria;
}

unsigned char Descompresor::getUltimoCaracterLeido() {
    return ultimoCaracterLeido;
}

void Descompresor::setUltimoCaracterLeido(unsigned char ultimoCaracterLeido) {
    this->ultimoCaracterLeido = ultimoCaracterLeido;
}

//Auxiliares

void Descompresor::agregarCaracterDescomprimido(unsigned char caracter) {
    if(ultimosNCaracteresDescomprimidos.size() == N) {
        ultimosNCaracteresDescomprimidos.pop_front();
    }
    ultimosNCaracteresDescomprimidos.push_back(caracter);
}

void Descompresor::avanzarPosicionBinaria(Lectura & lectura) {
    posicionBinaria = posicionBinaria%8 + 1;
    if((posicionBinaria == 1) && (lectura.peek() != EOF)) {
        ultimoCaracterLeido = lectura.leer();
    }
    else if(posicionBinaria == 1) {
        finDescompresion = 1;
    }
}

//Operaciones importantes

void Descompresor::descomprimirLargoReads(Lectura & lecturaArchivo, std::list<int> * largoReads) {

    //Descompresión de Elias-Gamma
    int cont = 0;
    int largoDecodificado = 0;
    int cantidadLargos = 0;
    avanzarPosicionBinaria(lecturaArchivo);
    while (bitEnPosicion(posicionBinaria, ultimoCaracterLeido) == 1) {
        //Cantidad de largos
        avanzarPosicionBinaria(lecturaArchivo);
        while(bitEnPosicion(posicionBinaria, ultimoCaracterLeido) == 0) {
            cont++;
            avanzarPosicionBinaria(lecturaArchivo);
        }
        hacerBit1(32-cont, cantidadLargos);
        avanzarPosicionBinaria(lecturaArchivo);
        for(int i = 32 - cont + 1; i <= 32; i++) {
            if(bitEnPosicion(posicionBinaria, ultimoCaracterLeido) == 1) hacerBit1(i,cantidadLargos);
            avanzarPosicionBinaria(lecturaArchivo);
        }
        largoReads->push_back(cantidadLargos);
        cantidadLargos = 0;
        cont = 0;

        //Largo de reads
        while(bitEnPosicion(posicionBinaria, ultimoCaracterLeido) == 0) {
            cont++;
            avanzarPosicionBinaria(lecturaArchivo);
        }
        hacerBit1(32-cont, largoDecodificado);
        avanzarPosicionBinaria(lecturaArchivo);
        for(int i = 32 - cont + 1; i <= 32; i++) {
            if(bitEnPosicion(posicionBinaria, ultimoCaracterLeido) == 1) hacerBit1(i,largoDecodificado);
            avanzarPosicionBinaria(lecturaArchivo);
        }
        largoReads->push_back(largoDecodificado);
        largoDecodificado = 0;
        cont = 0;
    }
    avanzarPosicionBinaria(lecturaArchivo);
}

void Descompresor::descomprimirAlfabeto(Lectura & lecturaArchivo) {
    bool sirveAlfabeto = bitEnPosicion(posicionBinaria, ultimoCaracterLeido) == 1;
    avanzarPosicionBinaria(lecturaArchivo);
    if(sirveAlfabeto) { //Vale la pena asignar un nuevo alfabeto
        
        //Se decodifica el cardinal del alfabeto
        unsigned char cardinal = 0;
        for(int i = 1; i <= 8; i++) {
            if(bitEnPosicion(posicionBinaria, ultimoCaracterLeido) == 1) hacerBit1(i,cardinal);
            avanzarPosicionBinaria(lecturaArchivo);
        }
        alfabeto.resize((int)cardinal);

        //Se decodifica el alfabeto
        for(int i = 0; i < (int)cardinal; i++) {
            unsigned char letraAlfabeto = 0;
            for(int j = 1; j<= 8; j++) {
                if(bitEnPosicion(posicionBinaria, ultimoCaracterLeido) == 1) hacerBit1(j,letraAlfabeto);
                avanzarPosicionBinaria(lecturaArchivo);
            }
            alfabeto[i] = letraAlfabeto;
        }
    }
    else { //El alfabeto tiene más de 128 elementos y no tendría sentido tratarlo por separado
        alfabeto.resize(256);
        for(int i = 0; i < 256; i++) {
            alfabeto[i] = (unsigned char)i; //alfabeto "común y silvestre"
        }
    }
}

void Descompresor::descomprimirPrimerosN(Lectura & lecturaArchivo, Escritura & escrituraArchivo) {
    int cardinal = alfabeto.size();
    int minBits = ceil(log2(cardinal));
    for(int i = 1; i <= N; i++) {
        unsigned char nuevoCaracter = 0;
        for(int j = 8 - minBits + 1; j<= 8; j++) {
            if(bitEnPosicion(posicionBinaria, ultimoCaracterLeido) == 1) hacerBit1(j,nuevoCaracter);
            avanzarPosicionBinaria(lecturaArchivo);
        }
        escrituraArchivo.escribir(alfabeto[nuevoCaracter]);
        agregarCaracterDescomprimido(alfabeto[nuevoCaracter]);
    }
}

int Descompresor::descomprimirLZ77(Lectura & lecturaArchivo, Escritura & escrituraArchivo) {

        int cardinal = alfabeto.size();
        int minBits = ceil(log2(cardinal));
        bool primeroEsTupla = bitEnPosicion(posicionBinaria, ultimoCaracterLeido);
        avanzarPosicionBinaria(lecturaArchivo);
        if(primeroEsTupla) { //Hay una tupla de largo > 2

            //Largo acotado por log2(N)
            int largo = 0;
            for(int i = 32 - ceil(log2(N)) + 1; i <= 32; i++) {
                if(bitEnPosicion(posicionBinaria, ultimoCaracterLeido) == 1) hacerBit1(i,largo);
                avanzarPosicionBinaria(lecturaArchivo);
            }

            largo = largo + 2; //deshago c.v. hecho en compresión

            //Offset acotado por log2(N)
            int offset = 0;
            for(int i = 32 - ceil(log2(N)) + 1; i <= 32; i++) {
                if(bitEnPosicion(posicionBinaria, ultimoCaracterLeido) == 1) hacerBit1(i,offset);
                avanzarPosicionBinaria(lecturaArchivo);
            }

            offset = offset + 1; //deshago c.v. hecho en compresión

            //Actualizar ultimos N
            for(int i = N - offset; i < N - offset + largo; i++) {
                escrituraArchivo.escribir(ultimosNCaracteresDescomprimidos.getElemento(N - offset));
                agregarCaracterDescomprimido(ultimosNCaracteresDescomprimidos.getElemento(N - offset));
            }
            return largo;
        }
        else { //Es solo un caracter
            unsigned char caracter = 0;
            for(int i = 8 - ceil(log2(alfabeto.size())) + 1; i <= 8; i++) {
                if(bitEnPosicion(posicionBinaria, ultimoCaracterLeido) == 1) hacerBit1(i,caracter);
                avanzarPosicionBinaria(lecturaArchivo);
            }
            if(!finDescompresion) {
                escrituraArchivo.escribir(alfabeto[caracter]);
                agregarCaracterDescomprimido(alfabeto[caracter]);
            }
            return 1;
        }
}

void Descompresor::transferirTextos(Lectura & lecturaSegmento1, Lectura & lecturaSegmento2, Lectura & lecturaSegmento4, Escritura & escrituraArchivo, std::list<int> * largoReads) {
    std::list<int>::const_iterator itCant = largoReads->begin();
    std::list<int>::const_iterator itLargo = largoReads->begin();
    itLargo++;
    int cont = 1;
    while(lecturaSegmento1.peek() != EOF) {

        //Linea 1
        escrituraArchivo.escribir('@');
        escrituraArchivo.escribir(lecturaSegmento4.getLine());
        escrituraArchivo.escribir("\n");

        //Linea 2
        int cant = *itCant;
        int largo = *itLargo;
        for(int i = 1; i <= largo; i++) {
            escrituraArchivo.escribir(lecturaSegmento1.leer());
        }
        if(cont == *itCant) {
            itCant++;
            itCant++;
            itLargo++;
            itLargo++;
            cont = 1;
        }
        else {
            cont++;
        }
        escrituraArchivo.escribir("\n");

        //Linea 3
        escrituraArchivo.escribir('+');
        escrituraArchivo.escribir(lecturaSegmento4.getLine());
        escrituraArchivo.escribir("\n");

        //Linea 4
        for(int i = 1; i <= largo; i++) {
            escrituraArchivo.escribir(lecturaSegmento2.leer());
        }
        if (lecturaSegmento1.peek() != EOF) escrituraArchivo.escribir("\n");
    }
}
