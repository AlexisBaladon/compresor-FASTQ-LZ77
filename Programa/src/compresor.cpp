#include "../include/compresor.h"
#include "../include/lectura.h"
#include "../include/escritura.h"
#include "../include/binarios.h"
#include "../include/arregloCircular.h"

#include <list>
#include <iterator>
#include <string>
#include <cmath>

//Setters, getters, creador y destructor.

Compresor::Compresor() {

}

Compresor::Compresor(int N, int ** alfabeto) {
    this->N = N;
    this->alfabeto = alfabeto;
}

Compresor::~Compresor() {

}

void Compresor::setN(int N) {
    this->N = N;
}

void Compresor::setAlfabeto(int ** alfabeto) {
    this->alfabeto = alfabeto;
}

int ** Compresor::getAlfabeto() {
    return alfabeto;
}

int Compresor::getPosicionBinaria() {
    return posicionBinaria;
}

void Compresor::setPosicionBinaria(int posicionBinaria) {
    this->posicionBinaria = posicionBinaria;
}

unsigned char Compresor::getUltimoCaracterComprimido() {
    return ultimoCaracterComprimido;
}

void Compresor::setUltimoCaracterComprimido(unsigned char ultimoCaracterComprimido) {
    this->ultimoCaracterComprimido = ultimoCaracterComprimido;
}

//Auxiliares

void Compresor::escribirBinario(Escritura & escrituraArchivo, bool binario) {
    if(binario == 1) hacerBit1(posicionBinaria,ultimoCaracterComprimido);
    if(posicionBinaria == 8) {
        escrituraArchivo.escribir(ultimoCaracterComprimido);
        ultimoCaracterComprimido = 0;
    }
    posicionBinaria = posicionBinaria%8 + 1;
}

void Compresor::comprimirElias(Escritura & escrituraArchivo, int num) {
        int cont = 0;
        while(bitEnPosicion(cont + 1,num) == 0) {
            cont++;
        }
        for(int i = 1; i < 32 - cont; i++) {
            escribirBinario(escrituraArchivo, 0);
        }
        for(int i = cont + 1; i <= 32; i++) {
            escribirBinario(escrituraArchivo,bitEnPosicion(i,num));
        }
    }

//Operaciones importantes

void Compresor::organizarArchivos(Lectura & lecturaArchivo, Escritura & escrituraArchivoComprimido, Escritura & escrituraSegmento1, Escritura & escrituraSegmento2, Escritura & escrituraSegmento4) {
    std::list<int> largoReads;

    while(lecturaArchivo.peek() != EOF) {
        //fila1
        lecturaArchivo.leer(); // @
        escrituraSegmento4.escribir(lecturaArchivo.getLine());
        escrituraSegmento4.escribir("\n");
        //fila2
        std::string fila1 = lecturaArchivo.getLine();
        largoReads.push_back(fila1.size());
        if(largoReads.front() != fila1.size()) {
            escribirBinario(escrituraArchivoComprimido,1);
            comprimirElias(escrituraArchivoComprimido,largoReads.size() - 1);
            comprimirElias(escrituraArchivoComprimido,largoReads.front());
            largoReads.clear();
            largoReads.push_back(fila1.size());
        }
        escrituraSegmento1.escribir(fila1);
        //fila3
        lecturaArchivo.leer(); // +
        escrituraSegmento4.escribir(lecturaArchivo.getLine());
        //fila4
        escrituraSegmento2.escribir(lecturaArchivo.getLine());
        if(lecturaArchivo.peek() != EOF) escrituraSegmento4.escribir("\n");
    }

    if(!largoReads.empty()) {
        escribirBinario(escrituraArchivoComprimido,1);
        comprimirElias(escrituraArchivoComprimido,largoReads.size());
        comprimirElias(escrituraArchivoComprimido,largoReads.front());
    }
    escribirBinario(escrituraArchivoComprimido,0);
}

void Compresor::conseguirAlfabeto(Lectura & lecturaArchivo) {
    //precon: alfabeto no tiene memoria asignada

    //Inicializar alfabeto en NULL
    alfabeto = new int *[257]; // alfabeto[256] = cardinalAlfabeto
    for (int i = 0; i < 256; i++) {
        (alfabeto[i] = NULL);
    }

    //Llenar alfabeto
    do {
        unsigned char ultimoLeido = lecturaArchivo.leer();
        if(alfabeto[ultimoLeido] == NULL) {
            alfabeto[ultimoLeido] = new int;
        }
    } while(lecturaArchivo.peek() != EOF);
    int cont = 0;
    for(int i = 0; i < 256; i++) {
        if(alfabeto[i] != NULL) {
            *alfabeto[i] = cont;
            cont++;
        }
    }

    //Asignar cardinal de alfabeto en alfabeto[256]
    if(cont > 128) {
        for(int i = 0; i < 256; i++) {
            if(alfabeto[i] == NULL) alfabeto[i] = new int;
            *alfabeto[i] = i;
            cont = 256;
        }
    }
    alfabeto[256] = new int;
    *alfabeto[256] = cont;
}
    
void Compresor::comprimirAlfabeto(Escritura & escrituraArchivo) {
    escribirBinario(escrituraArchivo,*alfabeto[256] <= 128); //Sirve Alfabeto
    if(*alfabeto[256] <= 128) {
        //Escribir largo de alfabeto
        for(int i = 32 - 8 + 1; i <= 32; i++) {
            escribirBinario(escrituraArchivo,bitEnPosicion(i,*alfabeto[256]));
        }

        //Escribir palabras del alfabeto en orden
        for (int i = 0 ; i < 256; i++) {
            if(alfabeto[i] != NULL) {
                for(int j = 32 - 8 + 1; j <= 32; j++) {
                escribirBinario(escrituraArchivo,bitEnPosicion(j,i));
                }
            }
        }
    }
}

void Compresor::comprimirPrimerosN(Lectura & lecturaArchivo, Escritura & escrituraArchivo, ArregloCircular & primerosNCaracteres) {
    for(int i = 1; i <= N; i++) {
        unsigned char ch = lecturaArchivo.leer();
        primerosNCaracteres.push_back(ch);
        for(int j = 32 - ceil(log2(*alfabeto[256])) + 1; j <= 32 ; j++) {
            escribirBinario(escrituraArchivo,bitEnPosicion(j,*alfabeto[ch]));
        }
    }
}

void Compresor::comprimirLZ77(Lectura & lecturaArchivo, Escritura & escrituraArchivo, ArregloCircular & buffer) {
    //precon: lecturaArchivo.abierto()
    //precon: escritura.abierto()

        buffer.push_back(lecturaArchivo.leer());
        int siguienteComienzoIt = N;
        for(int i = 1; (i < N) && (lecturaArchivo.peek() != EOF) ; i++) {
            buffer.push_back(lecturaArchivo.leer());
        }
        int anteriorItComienzoIndice = 0;
        while(buffer.size() != N) { // "buffer" contiene los primeros y prรณximos N elementos
            unsigned char primerCaracterLeido = buffer.getElemento(siguienteComienzoIt);
            int recorrido = buffer.size() - N;
            int mejorIndice = 1;
            int mejorCantidadMatches = 0;
            for(int i = 1; i <= recorrido; i++) {
                int anteriorIt = anteriorItComienzoIndice; 
                int siguienteIt = siguienteComienzoIt;
                int cantidadMatches = 0;
                for(cantidadMatches ;(cantidadMatches < recorrido) && (buffer.getElemento(anteriorIt) == buffer.getElemento(siguienteIt)); cantidadMatches++) {
                    siguienteIt++;
                    anteriorIt++;
                }
                anteriorItComienzoIndice++;
                if(cantidadMatches > mejorCantidadMatches) {
                    mejorCantidadMatches = cantidadMatches;
                    mejorIndice = i;
                    if(mejorCantidadMatches == recorrido) break;
                } //criterios de parada
            }

            escribirBinario(escrituraArchivo,mejorCantidadMatches > 1);

            if(mejorCantidadMatches > 1) { //es Tupla
                int largo = mejorCantidadMatches;
                int offset = N - mejorIndice + 1;
                for(int i = 32 - ceil(log2(N)) + 1; i <= 32; i++) {
                    escribirBinario(escrituraArchivo,bitEnPosicion(i,largo-2)); //c.v. a largo para representarlo con menos bits
                }
                for(int i = 32 - ceil(log2(N)) + 1; i <= 32; i++) {
                    escribirBinario(escrituraArchivo,bitEnPosicion(i,offset-1)); //c.v. a offset para representarlo con menos bits
                }
            }
            else { //es Caracter
                for(int i = 32 - ceil(log2(*alfabeto[256])) + 1; i <= 32; i++) {
                    escribirBinario(escrituraArchivo,bitEnPosicion(i,*alfabeto[primerCaracterLeido]));
                }
                mejorCantidadMatches = 1;
            }

            for(int i = 1; i <= mejorCantidadMatches; i++) {
                buffer.pop_front();
                if(lecturaArchivo.peek() != EOF) buffer.push_back(lecturaArchivo.leer());
            }
            anteriorItComienzoIndice = 0;
        }

    for(int i = 0; i < 257; i++) {
        delete alfabeto[i];
    }
    delete [] alfabeto;
}

