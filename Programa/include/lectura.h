#ifndef LECTURA
#define LECTURA

#include <fstream>
#include <string>

class Lectura {
    private:
        std::ifstream input;
    public:
        Lectura();
        ~Lectura();
        void abrir(std::string archivo);
        void cerrar();
        bool estaAbierto();
        unsigned char leer();
        int peek();
        std::string getLine();
};

#endif