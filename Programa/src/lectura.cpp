#include "../include/lectura.h"
#include <fstream>
#include <string>
#include <iostream>

Lectura::Lectura() {

}

Lectura::~Lectura() {
    
}

void Lectura::abrir(std::string archivo) {
    input.open(archivo, std::ios::in | std::ios::binary);
}

void Lectura::cerrar() {
    input.close();
}

bool Lectura::estaAbierto() {
    return input.is_open();
}

unsigned char Lectura::leer() {
    unsigned char leido;
    input.read(reinterpret_cast<char*>(&leido),sizeof(unsigned char));
    return leido;
}

int Lectura::peek() {
    return input.peek();
}

std::string Lectura::getLine() {
    std::string res;
    getline(input,res);
    return res;
}

