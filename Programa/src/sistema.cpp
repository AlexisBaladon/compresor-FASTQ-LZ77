#include "../include/sistema.h"
#include "../include/compresor.h"
#include "../include/descompresor.h"
#include "../include/arregloCircular.h"

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <iterator>
#include <cstdio>

Sistema::Sistema () {
    //inicializar todo
}

Sistema::~Sistema() {
    //liberar memoria
}

void Sistema::comprimir(std::string nombreArchivo, int N, std::string formato) {

    Compresor compresor;
    compresor.setN(N);
    Escritura escritura;
    Lectura lectura;
    std::string direccionArchivoOriginal = "archivosDeTexto/" + nombreArchivo + "." + formato;
    std::string direccionArchivoComprimido = "archivosDeTexto/" + nombreArchivo + "Comprimido.bin";
    ArregloCircular anterioresN(2*N);

    if(formato != "fastq") {
        lectura.abrir(direccionArchivoOriginal);
        compresor.conseguirAlfabeto(lectura);
        lectura.cerrar();
        std::cout << "Alfabeto conseguido! [1/4]" << std::endl;
        escritura.abrir(direccionArchivoComprimido);
        compresor.comprimirAlfabeto(escritura);
        std::cout << "Alfabeto comprimido! [2/4]" << std::endl;
        lectura.abrir(direccionArchivoOriginal);
        compresor.comprimirPrimerosN(lectura,escritura,anterioresN);
        std::cout << "Primeros N caracteres comprimidos! [3/4]" << std::endl;
        compresor.comprimirLZ77(lectura,escritura,anterioresN);
        for(int i = 1; i < 8; i++) {
            compresor.escribirBinario(escritura,0);
        }
        std::cout << "Todas las tuplas comprimidas! [4/4]" << std::endl;
        escritura.cerrar();
        lectura.cerrar();
    }
    else {
        Escritura escrituraSegmento1;
        Escritura escrituraSegmento2;
        Escritura escrituraSegmento4;
        Compresor compresorSegmento1;
        compresorSegmento1.setN(N);
        Compresor compresorSegmento2;
        compresorSegmento2.setN(N);
        Compresor compresorSegmento4;
        compresorSegmento4.setN(N);

        //Inicializaciรณn de alfabetos

        int ** alfabetoSegmento1 = new int *[257];
        alfabetoSegmento1[256] = new int;
        *alfabetoSegmento1[256] = 6;
        for(int i = 0; i < 256; i++) {
            alfabetoSegmento1[i] = NULL;
        }
        alfabetoSegmento1[10] = new int;
        *alfabetoSegmento1[10] = 0;
        alfabetoSegmento1['C'] = new int;
        *alfabetoSegmento1['C'] = 1;
        alfabetoSegmento1['A'] = new int;
        *alfabetoSegmento1['A'] = 2;
        alfabetoSegmento1['T'] = new int;
        *alfabetoSegmento1['T'] = 3;
        alfabetoSegmento1['G'] = new int;
        *alfabetoSegmento1['G'] = 4;
        alfabetoSegmento1['N'] = new int;
        *alfabetoSegmento1['N'] = 5;
        compresorSegmento1.setAlfabeto(alfabetoSegmento1);

        int ** alfabetoSegmento2 = new int *[257];
        alfabetoSegmento2[256] = new int;
        *alfabetoSegmento2[256] = 95;
        for(int i = 0; i < 256; i++) {
            alfabetoSegmento2[i] = NULL;
        }
        alfabetoSegmento2[10] = new int;
        *alfabetoSegmento2[10] = 0;
        for(int i = 33; i < 126; i++) {
            alfabetoSegmento2[i] = new int;
            *alfabetoSegmento2[i] = i - 32;
        }
        compresorSegmento2.setAlfabeto(alfabetoSegmento2);

        //Inicio de descompresiรณn
        
        lectura.abrir(direccionArchivoOriginal);
        escritura.abrir(direccionArchivoComprimido);
        escrituraSegmento1.abrir("archivosDeTexto/auxiliares/segmento1Original.txt");
        escrituraSegmento2.abrir("archivosDeTexto/auxiliares/segmento2Original.txt");
        escrituraSegmento4.abrir("archivosDeTexto/auxiliares/segmento4Original.txt");
        compresor.organizarArchivos(lectura,escritura,escrituraSegmento1,escrituraSegmento2,escrituraSegmento4);
        lectura.cerrar();
        escrituraSegmento1.cerrar();
        escrituraSegmento2.cerrar();
        escrituraSegmento4.cerrar();
        std::cout << "Segmento 3 comprimido y archivos reorganizados! [1/4]" << std::endl;

        Lectura lecturaSegmento1;
        Lectura lecturaSegmento2;
        Lectura lecturaSegmento4;
        lecturaSegmento1.abrir("archivosDeTexto/auxiliares/segmento1Original.txt");
        compresorSegmento1.setPosicionBinaria(compresor.getPosicionBinaria()); //Transferencia de datos de compresor anterior
        compresorSegmento1.setUltimoCaracterComprimido(compresor.getUltimoCaracterComprimido()); //Transferencia de datos de compresor anterior
        compresorSegmento1.comprimirPrimerosN(lecturaSegmento1,escritura,anterioresN);
        compresorSegmento1.comprimirLZ77(lecturaSegmento1,escritura,anterioresN);
        lecturaSegmento1.cerrar();
        std::cout << "Segmento 1 comprimido! [2/4]" << std::endl;

        ArregloCircular anterioresN2(2*N);
        lecturaSegmento2.abrir("archivosDeTexto/auxiliares/segmento2Original.txt");
        compresorSegmento2.setPosicionBinaria(compresorSegmento1.getPosicionBinaria()); //Transferencia de datos de compresor anterior
        compresorSegmento2.setUltimoCaracterComprimido(compresorSegmento1.getUltimoCaracterComprimido()); //Transferencia de datos de compresor anterior
        compresorSegmento2.comprimirPrimerosN(lecturaSegmento2,escritura,anterioresN2);
        compresorSegmento2.comprimirLZ77(lecturaSegmento2,escritura,anterioresN2);
        lecturaSegmento2.cerrar();
        std::cout << "Segmento 2 comprimido! [3/4]" << std::endl;

        lecturaSegmento4.abrir("archivosDeTexto/auxiliares/segmento4Original.txt");
        compresorSegmento4.setPosicionBinaria(compresorSegmento2.getPosicionBinaria()); //Transferencia de datos de compresor anterior
        compresorSegmento4.setUltimoCaracterComprimido(compresorSegmento2.getUltimoCaracterComprimido()); //Transferencia de datos de compresor anterior
        compresorSegmento4.conseguirAlfabeto(lecturaSegmento4);
        lecturaSegmento4.cerrar();
        compresorSegmento4.comprimirAlfabeto(escritura);
        ArregloCircular anterioresN4(2*N);
        lecturaSegmento4.abrir("archivosDeTexto/auxiliares/segmento4Original.txt");
        compresorSegmento4.comprimirPrimerosN(lecturaSegmento4,escritura,anterioresN4);
        compresorSegmento4.comprimirLZ77(lecturaSegmento4,escritura,anterioresN4);
        
        for(int i = 1; i < 8; i++) { //Ceros basura para no perder informaciรณn
            compresorSegmento4.escribirBinario(escritura,0);
        }
        escritura.cerrar();
        lecturaSegmento4.cerrar();
        std::cout << "Segmento 4 comprimido! [4/4]" << std::endl;
        remove("archivosDeTexto/auxiliares/segmento1Original.txt");
        remove("archivosDeTexto/auxiliares/segmento2Original.txt");
        remove("archivosDeTexto/auxiliares/segmento4Original.txt");
    }
    
    std::cout << "Compresion lista! Su archivo se encuentra en: " << direccionArchivoComprimido << std::endl;
    std::cout << "No olvide quitarlo de la carpeta en donde se encuentra, en otro caso" << std::endl;
    std::cout << "el archivo podria ser sobreescrito por el mismo programa" << std::endl;
}

void Sistema::descomprimir(std::string nombreArchivo, int N, std::string formato) {

    Descompresor descompresor(N);
    Lectura lectura;
    Escritura escritura;
    std::string direccionArchivoOriginal = "archivosDeTexto/" + nombreArchivo + ".bin";
    std::string direccionArchivoDescomprimido = "archivosDeTexto/" + nombreArchivo + "Descomprimido." + formato;

	if(formato != "fastq") {
        lectura.abrir(direccionArchivoOriginal);
        descompresor.avanzarPosicionBinaria(lectura);
        descompresor.descomprimirAlfabeto(lectura);
        std::cout << "Alfabeto descomprimido! [1/3]" << std::endl;
        escritura.abrir(direccionArchivoDescomprimido);
        descompresor.descomprimirPrimerosN(lectura,escritura);
        std::cout << "Primeros N caracteres descomprimidos! [2/3]" << std::endl;
        while(descompresor.getFinDescompresion() == 0) {
            descompresor.descomprimirLZ77(lectura,escritura);
        }
        std::cout << "Todas las tuplas descomprimidas! [3/3]" << std::endl;
        escritura.cerrar();
        lectura.cerrar();
    }
    else {
        Lectura lecturaSegmento1;
        Lectura lecturaSegmento2;
        Lectura lecturaSegmento4;
        Escritura escrituraSegmento1;
        Escritura escrituraSegmento2;
        Escritura escrituraSegmento4;
        Descompresor descompresorSegmento1(N);
        Descompresor descompresorSegmento2(N);
        Descompresor descompresorSegmento4(N);

        //Inicializaciรณn de alfabetos

        std::vector<unsigned char> alfabetoSegmento1;
        alfabetoSegmento1.resize(6);
        alfabetoSegmento1[0] = 10;
        alfabetoSegmento1[1] = 'C';
        alfabetoSegmento1[2] = 'A';
        alfabetoSegmento1[3] = 'T';
        alfabetoSegmento1[4] = 'G';
        alfabetoSegmento1[5] = 'N';
        descompresorSegmento1.setAlfabeto(alfabetoSegmento1);

        std::vector<unsigned char> alfabetoSegmento2;
        alfabetoSegmento2.resize(95);
        alfabetoSegmento2[0] = 10;
        for(int i = 1; i < 95; i++) {
            alfabetoSegmento2[i] = i + 32;
        }
        descompresorSegmento2.setAlfabeto(alfabetoSegmento2);

        //Inicio de descompresiรณn

        std::list<int> * largoReadsSegmento1 = new std::list<int>;
        lectura.abrir(direccionArchivoOriginal);
        descompresor.descomprimirLargoReads(lectura,largoReadsSegmento1);
        int numIteraciones = 0;
        for(std::list<int>::const_iterator it = largoReadsSegmento1->begin(); it != largoReadsSegmento1->end(); it++) {
            int cant = (*it);
            it++;
            int largo = (*it);
            numIteraciones += cant*largo;
        }
        numIteraciones -= N;
        std::cout << "Segmento 3 descomprimido! [1/5]" << std::endl;

        escrituraSegmento1.abrir("archivosDeTexto/auxiliares/segmento1Descomprimido.txt");
        descompresorSegmento1.setPosicionBinaria(descompresor.getPosicionBinaria()); //Transferencia de datos de descompresor anterior
        descompresorSegmento1.setUltimoCaracterLeido(descompresor.getUltimoCaracterLeido()); //Transferencia de datos de descompresor anterior
        descompresorSegmento1.descomprimirPrimerosN(lectura,escrituraSegmento1);
        int numIteracionesCopia = numIteraciones;
        while(numIteraciones > 0) {
            numIteraciones -= descompresorSegmento1.descomprimirLZ77(lectura,escrituraSegmento1);
        }
        escrituraSegmento1.cerrar();
        std::cout << "Segmento 1 descomprimido! [2/5]" << std::endl;

        escrituraSegmento2.abrir("archivosDeTexto/auxiliares/segmento2Descomprimido.txt");
        descompresorSegmento2.setPosicionBinaria(descompresorSegmento1.getPosicionBinaria()); //Transferencia de datos de descompresor anterior
        descompresorSegmento2.setUltimoCaracterLeido(descompresorSegmento1.getUltimoCaracterLeido()); //Transferencia de datos de descompresor anterior
        descompresorSegmento2.descomprimirPrimerosN(lectura,escrituraSegmento2);
        while(numIteracionesCopia > 0) {
            numIteracionesCopia -= descompresorSegmento2.descomprimirLZ77(lectura,escrituraSegmento2);
        }
        //delete largoReadsSegmento2;
        escrituraSegmento2.cerrar();
        std::cout << "Segmento 2 descomprimido! [3/5]" << std::endl;
        
        descompresorSegmento4.setPosicionBinaria(descompresorSegmento2.getPosicionBinaria()); //Transferencia de datos de descompresor anterior
        descompresorSegmento4.setUltimoCaracterLeido(descompresorSegmento2.getUltimoCaracterLeido()); //Transferencia de datos de descompresor anterior
        descompresorSegmento4.descomprimirAlfabeto(lectura);
        escrituraSegmento4.abrir("archivosDeTexto/auxiliares/segmento4Descomprimido.txt");
        descompresorSegmento4.descomprimirPrimerosN(lectura,escrituraSegmento4);
        while(descompresorSegmento4.getFinDescompresion() == 0) {
            descompresorSegmento4.descomprimirLZ77(lectura,escrituraSegmento4);
        }
        escrituraSegmento4.cerrar();
        lectura.cerrar();
        std::cout << "Segmento 4 descomprimido! [4/5]" << std::endl;

        lecturaSegmento1.abrir("archivosDeTexto/auxiliares/segmento1Descomprimido.txt");
        lecturaSegmento2.abrir("archivosDeTexto/auxiliares/segmento2Descomprimido.txt");
        lecturaSegmento4.abrir("archivosDeTexto/auxiliares/segmento4Descomprimido.txt");
        escritura.abrir(direccionArchivoDescomprimido);
        descompresor.transferirTextos(lecturaSegmento1,lecturaSegmento2,lecturaSegmento4,escritura,largoReadsSegmento1);
        escritura.cerrar();
        lecturaSegmento1.cerrar();
        lecturaSegmento2.cerrar();
        lecturaSegmento4.cerrar();
        delete largoReadsSegmento1;
        std::cout << "Todos los segmentos transferidos! [5/5]" << std::endl;
        remove("archivosDeTexto/auxiliares/segmento1Descomprimido.txt");
        remove("archivosDeTexto/auxiliares/segmento2Descomprimido.txt");
        remove("archivosDeTexto/auxiliares/segmento4Descomprimido.txt");
    }

    std::cout << "Descompresion lista! Su archivo se encuentra en: " << direccionArchivoDescomprimido << std::endl;
    std::cout << "No olvide quitarlo de la carpeta en donde se encuentra, en otro caso" << std::endl;
    std::cout << "el archivo podria ser sobreescrito por el mismo programa" << std::endl;
}
