#include "HashCerrado.h"
#include <cstdint>
#include <string>
#include "MurmurHash3.h"

using namespace std;

uint64_t HashCerrado::murmurHash(const string& clave, uint32_t semilla) {
  uint64_t result[2];
  MurmurHash3_x64_128(clave.c_str(), clave.size(), semilla, result);
  return result[0];
}

size_t HashCerrado::hash(const string& clave, int intento, uint32_t semilla) {
  return (murmurHash(clave, semilla) + intento) % capacidad;
}

HashCerrado::HashCerrado(size_t numElementos) {
  capacidad = static_cast<size_t>(numElementos / 0.6) + 1;
  tabla.resize(capacidad);
  n = 0;
}

// Inserciones

bool HashCerrado::insertarPorID(Usuarios* u) {
  string clave = to_string(u->id);
  for (int i = 0; i < capacidad; ++i) {
    size_t idx = hash(clave, i, 0x9747b28c);
    if (!tabla[idx].ocupado || tabla[idx].borrado) {
      tabla[idx] = {u, true, false};
      n++;
      return true;
    }
  }
  return false;
}

bool HashCerrado::insertarPorScreenName(Usuarios* u) {
  string clave = u->screenName;
  for (int i = 0; i < capacidad; ++i) {
    size_t idx = hash(clave, i, 0x5bd1e995);
    if (!tabla[idx].ocupado || tabla[idx].borrado) {
      tabla[idx] = {u, true, false};
      n++;
      return true;
    }
  }
  return false;
}

// Búsquedas

Usuarios* HashCerrado::buscarPorID(int id) {
  string clave = to_string(id);
  for (int i = 0; i < capacidad; ++i) {
    size_t idx = hash(clave, i, 0x9747b28c);
    if (!tabla[idx].ocupado && !tabla[idx].borrado) return nullptr;
    if (tabla[idx].ocupado && !tabla[idx].borrado && tabla[idx].dato->id == id)
      return tabla[idx].dato;
  }
  return nullptr;
}

Usuarios* HashCerrado::buscarPorScreenName(const string& name) {
  for (int i = 0; i < capacidad; ++i) {
    size_t idx = hash(name, i, 0x5bd1e995);
    if (!tabla[idx].ocupado && !tabla[idx].borrado) return nullptr;
    if (tabla[idx].ocupado && !tabla[idx].borrado && tabla[idx].dato->screenName == name)
      return tabla[idx].dato;
  }
  return nullptr;
}

// Eliminaciones

bool HashCerrado::eliminarPorID(int id) {
  string clave = to_string(id);
  for (int i = 0; i < capacidad; ++i) {
    size_t idx = hash(clave, i, 0x9747b28c);
    if (tabla[idx].ocupado && !tabla[idx].borrado && tabla[idx].dato->id == id) {
      tabla[idx].borrado = true;
      return true;
    }
  }
  return false;
}

bool HashCerrado::eliminarPorScreenName(const string& name) {
  for (int i = 0; i < capacidad; ++i) {
    size_t idx = hash(name, i, 0x5bd1e995);
    if (tabla[idx].ocupado && !tabla[idx].borrado && tabla[idx].dato->screenName == name) {
      tabla[idx].borrado = true;
      return true;
    }
  }
  return false;
}

size_t HashCerrado::capacidadC() {
  return capacidad;
}
