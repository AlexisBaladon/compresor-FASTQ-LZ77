#ifndef SISTEMA
#define SISTEMA

#include <string>

class Sistema {
    private:
        
    public:
        Sistema();
        ~Sistema();
        void comprimir(std::string archivo, int N, std::string formato);
        void descomprimir(std::string archivo, int N, std::string formato);
};

#endif