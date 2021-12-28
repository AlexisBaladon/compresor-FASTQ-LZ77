#include "../include/escritura.h"
#include <fstream>
#include <string>

Escritura::Escritura() {

}

Escritura::~Escritura() {

}

void Escritura::abrir(std::string archivo) {
    output.open(archivo, std::ios::out | std::ios::binary);
}

void Escritura::cerrar() {
    output.close();
}

bool Escritura::estaAbierto() {
    return output.is_open();
}

void Escritura::escribir(unsigned char aEscribir) {
    output.write(reinterpret_cast<char*>(&aEscribir),1);
}

void Escritura::escribir(std::string aEscribir) {
    output << aEscribir;
}