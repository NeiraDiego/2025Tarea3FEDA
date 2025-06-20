#include "HashCerrado.h"
#include <cstdint>
#include <string>
#include "MurmurHash3.h"

using namespace std;
// aumento de aleatoriedad
uint64_t HashCerrado::murmurHash(const string& clave, uint32_t semilla) {
  uint64_t result[2];
  MurmurHash3_x64_128(clave.c_str(), clave.size(), semilla, result);
  return result[0];
}

// funación hash para realizar todas las operaciones
size_t HashCerrado::hash(const string& clave, int intento, uint32_t semilla) {
  return (murmurHash(clave, semilla) + intento) % capacidad;
}

// creación del arreglo 
HashCerrado::HashCerrado(size_t numElementos) {
  capacidad = static_cast<size_t>(numElementos / 0.6) + 1;
  tabla.resize(capacidad);
  n = 0;
}

// Inserciones
// insercion por ID, el ciclo for es lo que 
// genera que sea O(alfa) ya que debe buscar donde insertarse
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
    // ########## OJO ################
    // esta linea la debí insertar luego de correr los experimentos
    // ya que estaba dando resultados ilogicamente grandes ya que 
    // no estaba verificando si encontraba un lugar desocupado
    // y recorría todo el arreglo
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
