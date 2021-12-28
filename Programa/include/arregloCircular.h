#ifndef ALFABETOCIRCULAR
#define ALFABETOCIRCULAR

class ArregloCircular {
    private:
        unsigned char * caracter;
        int tamanio;
        int indiceInicio;
        int cantidadElementos;
    public:
        ArregloCircular(int N);
        ~ArregloCircular();
        int size();
        unsigned char front();
        void push_back(unsigned char);
        void pop_front();
        unsigned char getElemento(int pos);
};

#endif