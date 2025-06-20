#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include "Usuarios.h"

using namespace std;

class HashAbierto {
private:
  vector<vector<Usuarios*>> bucketsID;
  vector<vector<Usuarios*>> bucketsSN;
  size_t capacidad;

  uint64_t murmurHash(const string& clave, uint32_t semilla);
  size_t hash(const string& clave, uint32_t semilla);

public:
  HashAbierto(size_t numElementos);

  size_t capacidadA();

//  void insertar(Usuarios* u);

  void insertarPorID(Usuarios* u);
  void insertarPorSN(Usuarios* u);

  Usuarios* buscarPorID(int id);
  Usuarios* buscarPorScreenName(const string& name);

  bool eliminarPorID(int id);
  bool eliminarPorScreenName(const string& name);
};

