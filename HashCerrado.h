#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include "Usuarios.h"

using namespace std;

struct Entry {
  Usuarios* dato = nullptr;
  bool ocupado = false;
  bool borrado = false;
};

class HashCerrado {
private:
  vector<Entry> tabla;
  size_t capacidad;
  size_t n;

  uint64_t murmurHash(const string& clave, uint32_t semilla);
  size_t hash(const string& clave, int intento, uint32_t semilla);

public:
  HashCerrado(size_t numElementos);
  
  size_t capacidadC() ;

  bool insertarPorID(Usuarios* u);
  bool insertarPorScreenName(Usuarios* u);

  Usuarios* buscarPorID(int id);
  Usuarios* buscarPorScreenName(const string& name);

  bool eliminarPorID(int id);
  bool eliminarPorScreenName(const string& name);
};

