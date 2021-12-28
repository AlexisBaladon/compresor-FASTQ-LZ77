#ifndef DESCOMPRESOR
#define DESCOMPRESOR

#include "../include/lectura.h"
#include "../include/escritura.h"
#include "../include/arregloCircular.h"

#include <list>
#include <string>
#include <vector>

class Descompresor {
    private:
        ArregloCircular ultimosNCaracteresDescomprimidos;
        int posicionBinaria = 0; //Pertenece a [1,8], ayuda a saber cuál fue el último bit leido
        int N;
        bool finDescompresion = 0;
        unsigned char ultimoCaracterLeido;
        std::vector<unsigned char> alfabeto;
    public:
        Descompresor(int N);
        ~Descompresor();
        void setAlfabeto(std::vector<unsigned char>);
        bool getFinDescompresion();
        void setUltimoCaracterLeido(unsigned char);
        unsigned char getUltimoCaracterLeido();
        void setPosicionBinaria(int);
        int getPosicionBinaria();

        //Actualiza los ultimos N caracteres leidos (ventana) según la decodificación LZ77
        void agregarCaracterDescomprimido(unsigned char);

        //Simula la lectura bit por bit leyendo caracteres
        //Hace que posicionBinaria avance 1 lugar y se mantenga entre 1 y 8 (si supera el 8 vuelve al 1)
        //Además si la posición estaba en 1 asigna un nuevo caracter a "nuevoCaracterLeido"
        void avanzarPosicionBinaria(Lectura &);

        //Descomprime los largos de los Reads codificados con el método de Elias-Gamma
        //Estos son almacenados en "largoReads"
        void descomprimirLargoReads(Lectura &, std::list<int> * largoReads);

        //Recorre el archivo para conocer su alfabeto
        //Convierte a alfabeto en uno nuevo si la cantidad de caracteres distintos en el archivo es menor o igual a 128
        //Si no es el caso convierte al alfabeto en el alfabeto ASCII usual
        void descomprimirAlfabeto(Lectura &);

        void descomprimirPrimerosN(Lectura & lecturaArchivo, Escritura & escrituraArchivo);

        //Descomprime una sola tupla codificada por LZ77
        int descomprimirLZ77(Lectura & lecturaArchivo, Escritura & escrituraArchivo);

        //Transfiere finalmente los archivos ya descomprimidos
        void transferirTextos(Lectura &, Lectura &, Lectura &, Escritura &, std::list<int> * largoReads);
};

#endif