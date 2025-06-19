#include <iostream>
#include <cstdint>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include "Usuarios.h"
#include "procesamiento.cpp"
#include "BSTUsuarios.h"
#include "HashCerrado.h"
#include "HashAbierto.h"

using namespace std;
using namespace chrono;

size_t tamanoUsuarios(const vector<Usuarios>& vec) {
  size_t total = sizeof(vec);
  for (const auto& u : vec) {
    total += sizeof(u);
    total += u.screenName.capacity();
    total += u.tags.capacity();
    total += u.avatar.capacity();
    total += u.lang.capacity();
    total += u.friends.capacity();
  }
  return total;
}

vector<int> generarIndicesAleatorios(int total, int cantidad) {
  vector<int> indices;
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, total - 1);
  for (int i = 0; i < cantidad; ++i) {
    indices.push_back(dis(gen));
  }
  return indices;
}

int main() {
  ifstream file("data.csv");
  string linea;
  getline(file, linea); // saltar header
  int CuentaAPantalla = 0;
  vector<Usuarios> todos;
  while (getline(file, linea)) {
    todos.push_back(procesLinea(linea));
    CuentaAPantalla++;
    cout << CuentaAPantalla << endl;
  }

  ofstream out("resultados_experimento.csv");
  out << "Cantidad,TiempoBST,TiempoHashCerrado,TiempoHashAbierto,TamVector,TamBST,TamHashCerrado,TamHashAbierto\n";

  for (int cantidad : {1000, 5000, 10000, 15000, 20000, 30000}) {
    vector<Usuarios> datos(todos.begin(), todos.begin() + cantidad);

    // ----------------------
    // BST
    // ----------------------
    NodoBST_ID* bstID = nullptr;
    auto inicioBST = high_resolution_clock::now();
    for (auto& u : datos) insertarPorID(bstID, &u);
    auto finBST = high_resolution_clock::now();
    auto durBST = duration_cast<microseconds>(finBST - inicioBST).count();

    // ----------------------
    // Hash Cerrado
    // ----------------------
    HashCerrado hashCerrado(cantidad);
    auto inicioHC = high_resolution_clock::now();
    for (auto& u : datos) hashCerrado.insertarPorID(&u);
    auto finHC = high_resolution_clock::now();
    auto durHC = duration_cast<microseconds>(finHC - inicioHC).count();

    // ----------------------
    // Hash Abierto
    // ----------------------
    HashAbierto hashAbierto(cantidad);
    auto inicioHA = high_resolution_clock::now();
    for (auto& u : datos) hashAbierto.insertar(&u);
    auto finHA = high_resolution_clock::now();
    auto durHA = duration_cast<microseconds>(finHA - inicioHA).count();

    // ----------------------
    // Medición de memoria
    // ----------------------
    size_t memVec = tamanoUsuarios(datos);
    size_t memBST = cantidad * sizeof(NodoBST_ID);
    size_t memHC = hashCerrado.capacidadC() * sizeof(Entry);
    size_t memHA = hashAbierto.capacidadA() * sizeof(vector<Usuarios*>);

    out << cantidad << "," << durBST << "," << durHC << "," << durHA << ",";
    out << memVec << "," << memBST << "," << memHC << "," << memHA << "\n";
  }

  // -------------------------------
  // BÚSQUEDAS EXITOSAS Y FALLIDAS
  // -------------------------------
  vector<Usuarios> muestra(todos.begin(), todos.begin() + 10000);
  HashCerrado hcerrado(10000);
  HashAbierto habierto(10000);
  NodoBST_ID* bst = nullptr;

  for (auto& u : muestra) {
    hcerrado.insertarPorID(&u);
    hcerrado.insertarPorScreenName(&u);
    habierto.insertar(&u);
    insertarPorID(bst, &u);
  }

  vector<int> indices = generarIndicesAleatorios(10000, 100);

  // Búsquedas exitosas
  auto ini = high_resolution_clock::now();
  for (int idx : indices) hcerrado.buscarPorID(muestra[idx].id);
  auto fin = high_resolution_clock::now();
  cout << "Búsqueda exitosa HashCerrado por ID: " << duration_cast<microseconds>(fin - ini).count() << " us\n";

  ini = high_resolution_clock::now();
  for (int idx : indices) habierto.buscarPorScreenName(muestra[idx].screenName);
  fin = high_resolution_clock::now();
  cout << "Búsqueda exitosa HashAbierto por ScreenName: " << duration_cast<microseconds>(fin - ini).count() << " us\n";

  // Búsquedas fallidas
  ini = high_resolution_clock::now();
  for (int i = 0; i < 100; i++) hcerrado.buscarPorID(999999999 + i);
  fin = high_resolution_clock::now();
  cout << "Búsqueda fallida HashCerrado por ID: " << duration_cast<microseconds>(fin - ini).count() << " us\n";

  ini = high_resolution_clock::now();
  for (int i = 0; i < 100; i++) habierto.buscarPorScreenName("NoExiste_" + to_string(i));
  fin = high_resolution_clock::now();
  cout << "Búsqueda fallida HashAbierto por ScreenName: " << duration_cast<microseconds>(fin - ini).count() << " us\n";

  out.close();
  return 0;
}
