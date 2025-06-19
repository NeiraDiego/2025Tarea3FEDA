#include "HashAbierto.h"
#include <cstdint>
#include "MurmurHash3.h"

using namespace std;

// este codigo está basado casi directamente en el laboratorio 17
// solo se le modifico para trabajar sobre los indices deseados


// se aumenta la aleatoriedad controlada y determinista de la
// 
uint64_t HashAbierto::murmurHash(const string& clave, uint32_t semilla) {
  uint64_t result[2];
  MurmurHash3_x64_128(clave.c_str(), clave.size(), semilla, result);
  return result[0];
}

size_t HashAbierto::hash(const string& clave, uint32_t semilla) {
  return murmurHash(clave, semilla) % capacidad;
}

HashAbierto::HashAbierto(size_t numElementos) {
  capacidad = static_cast<size_t>(numElementos / 0.6) + 1;
  bucketsID.resize(capacidad);
  bucketsSN.resize(capacidad);
}

void HashAbierto::insertar(Usuarios* u) {
  size_t idx_id = hash(to_string(u->id), 0x9747b28c);
  bucketsID[idx_id].push_back(u);

  size_t idx_sn = hash(u->screenName, 0x5bd1e995);
  bucketsSN[idx_sn].push_back(u);
}

void HashAbierto::insertarPorID(Usuarios* u) {
  size_t idx_id = hash(to_string(u->id), 0x9747b28c);
  bucketsID[idx_id].push_back(u);
}

void HashAbierto::insertarPorSN(Usuarios* u) {
  size_t idx_sn = hash(u->screenName, 0x5bd1e995);
  bucketsSN[idx_sn].push_back(u);
}

Usuarios* HashAbierto::buscarPorID(int id) {
  size_t idx = hash(to_string(id), 0x9747b28c);
  for (Usuarios* u : bucketsID[idx]) {
    if (u->id == id) return u;
  }
  return nullptr;
}

Usuarios* HashAbierto::buscarPorScreenName(const string& name) {
  size_t idx = hash(name, 0x5bd1e995);
  for (Usuarios* u : bucketsSN[idx]) {
    if (u->screenName == name) return u;
  }
  return nullptr;
}

bool HashAbierto::eliminarPorID(int id) {
  size_t idx = hash(to_string(id), 0x9747b28c);
  auto& bucket = bucketsID[idx];
  for (auto it = bucket.begin(); it != bucket.end(); ++it) {
    if ((*it)->id == id) {
      bucket.erase(it);
      return true;
    }
  }
  return false;
}

bool HashAbierto::eliminarPorScreenName(const string& name) {
  size_t idx = hash(name, 0x5bd1e995);
  auto& bucket = bucketsSN[idx];
  for (auto it = bucket.begin(); it != bucket.end(); ++it) {
    if ((*it)->screenName == name) {
      bucket.erase(it);
      return true;
    }
  }
  return false;
}

size_t HashAbierto::capacidadA() {
  return capacidad;
}
