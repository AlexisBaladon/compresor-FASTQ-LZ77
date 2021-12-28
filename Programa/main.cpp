#include "include/sistema.h"

#include <list>
#include <iterator>
#include <iostream>
#include <string>

int main() {

    std::cout << std::endl << "Alexis Baladon, 5574612-4\n" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;

    std::cout <<
            
    " __         ________      ________  ________               \n"
    "/\\ \\       /\\_____  \\    /\\_____  \\/\\_____  \\      \n"
    "\\ \\ \\      \\/____//'/'   \\/___//'/'\\/___//'/'        \n"
    " \\ \\ \\  __      //'/'        /' /'     /' /'            \n"
    "  \\ \\ \\L\\ \\    //'/'___     /' /'     /' /'           \n"
    "   \\ \\____/   /\\_______\\   /\\_/      /\\_/            \n"
    "    \\/___/    \\/_______/   \\//       \\//               \n"
                                                
    << std::endl;

    std::cout << "----------------------------------------------\n" << std::endl;
    std::cout << "Bienvenido al programa de compresion/descompresion LZ77!" << std::endl;

    bool enUso = 1;
    do {
        std::cout << "Escriba el numero de la funcionalidad que desee:" << std::endl << std::endl;
        std::cout << "1): Descomprimir" << std::endl;
        std::cout << "2): Comprimir" << std::endl << std::endl;
        

        char comprimirDescomprimir;
        std::cin >> comprimirDescomprimir;
        std::cout << std::endl;

        std::cout << "Excelente opcion!" << std::endl;
        std::cout << "Ahora, ingrese el parametro N," << std::endl;
        std::cout << "el cual determinara el largo de la ventana," << std::endl;
        std::cout << "al igual que el de las coincidencias encontradas:" << std::endl << std::endl;

        int N;
        std::cin >> N;
        std::cout << std::endl;

        std::cout << "Perfecto!" << std::endl;
        std::cout << "Ahora ingrese el nombre del archivo de texto que desea utilizar" << std::endl;
        std::cout << "(Ejemplo: ArchivoDeEjemplo)" << std::endl;
        std::cout << "[Se asume que este archivo se encuentra dentro de la carpeta indicada en las instrucciones]" << std::endl << std::endl;
        
        std::string direccionArchivo;
        std::cin >> direccionArchivo;
        std::cout << std::endl;

        std::cout << "Sugerencia: Este programa es mas eficiente con archivos de formato FastQ." << std::endl;
        std::cout << "Ingrese el formato de su archivo:" << std::endl;
        std::cout << "(Ejemplo: fastq o txt)" << std::endl;

        std::string formato;
        std::cin >> formato;
        std::cout << std::endl;

        Sistema sistema;
        if(comprimirDescomprimir == '1') { // Descompresion
            sistema.descomprimir(direccionArchivo,N,formato);
        }
        else { //Compresion
            sistema.comprimir(direccionArchivo,N,formato);
        }

        std::cout << std::endl << "Desea continuar?" << std::endl << std::endl;
        std::cout << "1) Claro!" << std::endl;
        std::cout << "2) Fue suficiente por ahora" << std::endl << std::endl;

        char continua;
        std::cin >> continua;
        std::cout << std::endl;
        enUso = continua == '1';

    } while(enUso);
    
    std::cout << "Gracias por usar el programa!" << std::endl << std::endl;
    std::cin.get();
    return 0;
}