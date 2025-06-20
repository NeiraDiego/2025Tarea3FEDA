#include <iostream>
#include "Usuarios.h"
#include <string>

using namespace std;

// Nodo BST por ID
struct NodoBST_ID {
    Usuarios* dato;
    NodoBST_ID* izq;
    NodoBST_ID* der;
    NodoBST_ID(Usuarios* u) : dato(u), izq(nullptr), der(nullptr) {}
};

// Nodo BST por screenName
struct NodoBST_SN {
    Usuarios* dato;
    NodoBST_SN* izq;
    NodoBST_SN* der;
    NodoBST_SN(Usuarios* u) : dato(u), izq(nullptr), der(nullptr) {}
};

// Funcion de inseratar un nuevo nodo comenzando desde la raiz
// elemento que definirá el arbol a la hora de "llamarlo"
void insertarPorID(NodoBST_ID*& raiz, Usuarios* nuevo) {
    if (!raiz)
        raiz = new NodoBST_ID(nuevo);
    else if (nuevo->id < raiz->dato->id)
        insertarPorID(raiz->izq, nuevo);
    else
        insertarPorID(raiz->der, nuevo);
}

void insertarPorScreenName(NodoBST_SN*& raiz, Usuarios* nuevo) {
    if (!raiz)
        raiz = new NodoBST_SN(nuevo);
    else if (nuevo->screenName < raiz->dato->screenName)
        insertarPorScreenName(raiz->izq, nuevo);
    else
        insertarPorScreenName(raiz->der, nuevo);
}

// Búsquedas
Usuarios* buscarPorID(NodoBST_ID* raiz, int id) {
    if (!raiz) return nullptr;
    if (id == raiz->dato->id) return raiz->dato;
    else if (id < raiz->dato->id) return buscarPorID(raiz->izq, id);
    else return buscarPorID(raiz->der, id);
}

Usuarios* buscarPorScreenName(NodoBST_SN* raiz, const string& name) {
    if (!raiz) return nullptr;
    if (name == raiz->dato->screenName) return raiz->dato;
    else if (name < raiz->dato->screenName) return buscarPorScreenName(raiz->izq, name);
    else return buscarPorScreenName(raiz->der, name);
}

// Eliminaciones
NodoBST_ID* encontrarMin(NodoBST_ID* nodo) {
    while (nodo && nodo->izq) nodo = nodo->izq;
    return nodo;
}

NodoBST_ID* eliminarPorID(NodoBST_ID* raiz, int id) {
    if (!raiz) return nullptr;

    if (id < raiz->dato->id)
        raiz->izq = eliminarPorID(raiz->izq, id);
    else if (id > raiz->dato->id)
        raiz->der = eliminarPorID(raiz->der, id);
    else {
        if (!raiz->izq) {
            NodoBST_ID* temp = raiz->der;
            delete raiz;
            return temp;
        } else if (!raiz->der) {
            NodoBST_ID* temp = raiz->izq;
            delete raiz;
            return temp;
        } else {
            NodoBST_ID* sucesor = encontrarMin(raiz->der);
            raiz->dato = sucesor->dato;
            raiz->der = eliminarPorID(raiz->der, sucesor->dato->id);
        }
    }
    return raiz;
}

NodoBST_SN* encontrarMin(NodoBST_SN* nodo) {
    while (nodo && nodo->izq) nodo = nodo->izq;
    return nodo;
}

NodoBST_SN* eliminarPorScreenName(NodoBST_SN* raiz, const string& name) {
    if (!raiz) return nullptr;

    if (name < raiz->dato->screenName)
        raiz->izq = eliminarPorScreenName(raiz->izq, name);
    else if (name > raiz->dato->screenName)
        raiz->der = eliminarPorScreenName(raiz->der, name);
    else {
        if (!raiz->izq) {
            NodoBST_SN* temp = raiz->der;
            delete raiz;
            return temp;
        } else if (!raiz->der) {
            NodoBST_SN* temp = raiz->izq;
            delete raiz;
            return temp;
        } else {
            NodoBST_SN* sucesor = encontrarMin(raiz->der);
            raiz->dato = sucesor->dato;
            raiz->der = eliminarPorScreenName(raiz->der, sucesor->dato->screenName);
        }
    }
    return raiz;
}
