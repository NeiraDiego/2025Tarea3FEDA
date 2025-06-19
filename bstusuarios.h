#pragma once
#include "Usuarios.h"

using namespace std;

struct NodoBST_ID {
  Usuarios* dato;
  NodoBST_ID* izq;
  NodoBST_ID* der;
  NodoBST_ID(Usuarios* u);
};

struct NodoBST_SN {
  Usuarios* dato;
  NodoBST_SN* izq;
  NodoBST_SN* der;
  NodoBST_SN(Usuarios* u);
};

// Inserción
void insertarPorID(NodoBST_ID*& raiz, Usuarios* nuevo);
void insertarPorScreenName(NodoBST_SN*& raiz, Usuarios* nuevo);

// Búsqueda
Usuarios* buscarPorID(NodoBST_ID* raiz, int id);
Usuarios* buscarPorScreenName(NodoBST_SN* raiz, const string& name);

// Eliminación
NodoBST_ID* eliminarPorID(NodoBST_ID* raiz, int id);
NodoBST_SN* eliminarPorScreenName(NodoBST_SN* raiz, const string& name);

// Recorridos
void inordenID(NodoBST_ID* nodo);
void inordenScreenName(NodoBST_SN* nodo);

