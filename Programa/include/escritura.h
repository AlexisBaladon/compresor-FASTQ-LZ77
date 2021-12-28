#ifndef ESCRITURA
#define ESCRITURA

#include <fstream>
#include <string>

class Escritura {
    private:
        std::ofstream output;
    public:
        Escritura();
        ~Escritura();
        void abrir(std::string archivo);
        void cerrar();
        bool estaAbierto();
        void escribir(unsigned char);
        void escribir(std::string);
};

#endif