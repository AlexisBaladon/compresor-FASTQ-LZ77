#include "../include/arregloCircular.h"

ArregloCircular::ArregloCircular(int N) {
    caracter = new unsigned char[N];
    tamanio = N;
    indiceInicio = 0;
    cantidadElementos = 0;
}

ArregloCircular::~ArregloCircular() {
    delete [] caracter;
}

int ArregloCircular::size() {
    return cantidadElementos;
}

void ArregloCircular::push_back(unsigned char elem) {
    caracter[(indiceInicio + cantidadElementos) % tamanio] = elem;
    cantidadElementos++;
}

void ArregloCircular::pop_front() {
    indiceInicio = (indiceInicio + 1)%tamanio;
    cantidadElementos --;
}

unsigned char ArregloCircular::getElemento(int pos) {
    return caracter[(pos + indiceInicio)%tamanio];
}
