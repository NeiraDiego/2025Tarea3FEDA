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

// esta funcion determina el tamaño en bytes de los usuarios
// en un vector, comienza con el tamaño de la propia estructura
// vector y luego le suma iterativamente de cada parte del 
// mismo. Usamos capacity para los string y enteros ya 
// que size() no corresponde al numero de bytes 
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

// Esta funcion genera un vector de indices aleatorios para
// utilizarlos para generar la lista de IDs y ScreenNames
// que se usarán en los experimentos
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
  ifstream file("data.csv"); // cargamos el archivo 
  string linea;
  getline(file, linea); // saltar header
  int CuentaAPantalla = 0; // esta variable es para ver el avance en pantalla 
  // cargamos todos los datos en un vector para acceder despues a ellos
  // asi hacemos un solo "vaciado" desde el archivo
  vector<Usuarios> todos;
  cout << "cargando usuarios..." << endl;
    while (getline(file, linea)) {
      todos.push_back(procesLinea(linea));
      CuentaAPantalla++;
      // cout << CuentaAPantalla << endl; // esta es la linea que imprime a pantalla
    }
  cout << "...usuarios cargados" << endl;
  ofstream out("resultados_insercion.csv");
  cout << "realizando experimentos de inserción..." << endl;
  out << "Cantidad,t_HC_ID,t_HC_SN,t_HA_ID,t_HA_SN,t_BST_ID,t_BTS_SN,s_Vec,s_HC_ID,s_HC_SN,s_HA_ID,s_HA_SN,s_BST_ID,s_BTS_SN\n";
  for(int i = 0; i < 50; ++i){
    for (int cantidad : {1000, 5000, 10000, 15000, 20000, 30000}) {
      vector<Usuarios> datos(todos.begin(), todos.begin() + cantidad);

      // ----------------------
      // Hash Cerrado
      // ----------------------
      HashCerrado hashCerradoID(cantidad);
      auto inicioHCID = high_resolution_clock::now();
      for (auto& u : datos) hashCerradoID.insertarPorID(&u);
      auto finHCID = high_resolution_clock::now();
      auto durHCID = duration_cast<microseconds>(finHCID - inicioHCID).count();

      HashCerrado hashCerradoSN(cantidad);
      auto inicioHCSN = high_resolution_clock::now();
      for (auto& u : datos) hashCerradoSN.insertarPorScreenName(&u);
      auto finHCSN = high_resolution_clock::now();
      auto durHCSN = duration_cast<microseconds>(finHCSN - inicioHCSN).count();

      // ----------------------
      // Hash Abierto
      // ----------------------
      HashAbierto hashAbiertoID(cantidad);
      auto inicioHAID = high_resolution_clock::now();
      for (auto& u : datos) hashAbiertoID.insertarPorID(&u);
      auto finHAID = high_resolution_clock::now();
      auto durHAID = duration_cast<microseconds>(finHAID - inicioHAID).count();

      HashAbierto hashAbiertoSN(cantidad);
      auto inicioHASN = high_resolution_clock::now();
      for (auto& u : datos) hashAbiertoSN.insertarPorSN(&u);
      auto finHASN = high_resolution_clock::now();
      auto durHASN = duration_cast<microseconds>(finHASN - inicioHASN).count();

      // ----------------------
      // BST
      // ----------------------
      NodoBST_ID* bstID = nullptr;
      auto inicioBSTID = high_resolution_clock::now();
      for (auto& u : datos) insertarPorID(bstID, &u);
      auto finBSTID = high_resolution_clock::now();
      auto durBSTID = duration_cast<microseconds>(finBSTID - inicioBSTID).count();

      NodoBST_SN* bstSN = nullptr;
      auto inicioBSTSN = high_resolution_clock::now();
      for (auto& u : datos) insertarPorID(bstID, &u);
      auto finBSTSN = high_resolution_clock::now();
      auto durBSTSN = duration_cast<microseconds>(finBSTSN - inicioBSTSN).count();


      out << cantidad << "," << durHCID << "," << durHCSN << ",";
      out << durHAID << "," << durHASN << "," << durBSTID << "," << durBSTSN << ",";

    


      // ----------------------
      // Medición de memoria
      // ----------------------
      size_t memVec = tamanoUsuarios(datos);
      size_t memHCID = hashCerradoID.capacidadC() * sizeof(Entry);
      size_t memHCSN = hashCerradoSN.capacidadC() * sizeof(Entry);
      size_t memHAID = hashAbiertoID.capacidadA() * sizeof(vector<Usuarios*>);
      size_t memHASN = hashAbiertoSN.capacidadA() * sizeof(vector<Usuarios*>);
      size_t memBSTID = cantidad * sizeof(NodoBST_ID);
      size_t memBSTSN = cantidad * sizeof(NodoBST_SN);

      out << memVec << "," << memHCID << "," << memHCSN << "," << memHAID << ",";
      out << memHASN << "," << memBSTID << "," << memBSTSN << "\n";

    }
  }
  cout << "...experimentos de inserción terminados" << endl;
  out.close();

  // -------------------------------
  // BÚSQUEDAS EXITOSAS Y FALLIDAS
  // -------------------------------
  HashCerrado hcerradoID(40000);
  HashCerrado hcerradoSN(40000);
  HashAbierto habiertoID(40000);
  HashAbierto habiertoSN(40000);
  NodoBST_ID* bstID = nullptr; // cambiar bst a bstID y crear bstNAME  ww
  NodoBST_SN* bstSN = nullptr; // cambiar bst a bstID y crear bstNAME  ww

  for (auto& u : todos) {
    hcerradoID.insertarPorID(&u);
    hcerradoSN.insertarPorScreenName(&u);
    habiertoID.insertarPorID(&u);
    habiertoSN.insertarPorSN(&u);
    insertarPorID(bstID, &u);
    insertarPorScreenName(bstSN, &u);
  }

  vector<int> indices = generarIndicesAleatorios(10000, 100);
  vector<long> IdUsuariosABuscar;
  for (int idx : indices){
    IdUsuariosABuscar.push_back(todos[idx].id); 
  }
  vector<string> NombresABuscar;
  for (int idx : indices){
    NombresABuscar.push_back(todos[idx].screenName); 
  }

  ofstream out2("resultados_busqueda.csv");
    out2 << "HC_ID_si(us),HC_SN_si(us),HA_ID_si(us),HA_SN_si(us),BST_ID_si(us),BST_SN_si(us)";
    out2 << ",HC_ID_no(us),HC_SN_no(us),HA_ID_no(us),HA_SN_no(us),BST_ID_no(us),BST_SN_no(us)" << endl;
  cout << "realizando experimentos de búsqueda..." << endl;
  for(int i = 0; i < 50; ++i){
    // Búsquedas exitosas
    auto ini = high_resolution_clock::now();
    for (int ind : IdUsuariosABuscar) hcerradoID.buscarPorID(ind);
    auto fin = high_resolution_clock::now();
    out2 << duration_cast<microseconds>(fin - ini).count() << ",";
    //out2 << "Búsqueda exitosa HashCerrado por ID: " << duration_cast<microseconds>(fin - ini).count() << " us\n";

    ini = high_resolution_clock::now();
    for (string nombr : NombresABuscar) hcerradoSN.buscarPorScreenName(nombr);
    fin = high_resolution_clock::now();
    out2 << duration_cast<microseconds>(fin - ini).count() << ",";
    //out2 << "Búsqueda exitosa HashCerrado por ScreenName: " << duration_cast<microseconds>(fin - ini).count() << " us\n";
  
    ini = high_resolution_clock::now();
    for (int ind : IdUsuariosABuscar) habiertoID.buscarPorID(ind);
    fin = high_resolution_clock::now();
    out2 << duration_cast<microseconds>(fin - ini).count() << ",";
    //out2 << "Búsqueda exitosa HashAbierto por ID: " << duration_cast<microseconds>(fin - ini).count() << " us\n";

    ini = high_resolution_clock::now();
    for (string nombr : NombresABuscar) habiertoSN.buscarPorScreenName(nombr);
    fin = high_resolution_clock::now();
    out2 << duration_cast<microseconds>(fin - ini).count() << ",";
    //out2 << "Búsqueda exitosa HashAbierto por ScreenName: " << duration_cast<microseconds>(fin - ini).count() << " us\n";
  
    ini = high_resolution_clock::now();
    for (int ind : IdUsuariosABuscar) buscarPorID(bstID, ind);
    fin = high_resolution_clock::now();
    out2 << duration_cast<microseconds>(fin - ini).count() << ",";
    //out2 << "Búsqueda exitosa BST por ID: " << duration_cast<microseconds>(fin - ini).count() << " us\n";

    ini = high_resolution_clock::now();
    for (string nombr : NombresABuscar) buscarPorScreenName(bstSN, nombr);
    fin = high_resolution_clock::now();
    out2 << duration_cast<microseconds>(fin - ini).count() << ",";
    //out2 << "Búsqueda exitosa BST por ScreenName: " << duration_cast<microseconds>(fin - ini).count() << " us\n";

    // Búsquedas fallidas
    ini = high_resolution_clock::now();
    for (int i = 0; i < 100; i++) hcerradoID.buscarPorID(i);
    fin = high_resolution_clock::now();
    out2 << duration_cast<microseconds>(fin - ini).count() << ",";
    //out2 << "Búsqueda fallida Hashabierto por ID: " << duration_cast<microseconds>(fin - ini).count() << " us\n";

    ini = high_resolution_clock::now();
    for (int i = 0; i < 100; i++) hcerradoSN.buscarPorScreenName("NoExiste@" + to_string(i));
    fin = high_resolution_clock::now();
    out2 << duration_cast<microseconds>(fin - ini).count() << ",";
    //out2 << "Búsqueda fallida HashAbierto por ScreenName: " << duration_cast<microseconds>(fin - ini).count() << " us\n";

    ini = high_resolution_clock::now();
    for (int i = 0; i < 100; i++) habiertoID.buscarPorID(i);
    fin = high_resolution_clock::now();
    out2 << duration_cast<microseconds>(fin - ini).count() << ",";
    //out2 << "Búsqueda fallida Hashabierto por ID: " << duration_cast<microseconds>(fin - ini).count() << " us\n";

    ini = high_resolution_clock::now();
    for (int i = 0; i < 100; i++) habiertoSN.buscarPorScreenName("NoExiste@" + to_string(i));
    fin = high_resolution_clock::now();
    out2 << duration_cast<microseconds>(fin - ini).count() << ",";
    //out2 << "Búsqueda fallida HashAbierto por ScreenName: " << duration_cast<microseconds>(fin - ini).count() << " us\n";

    ini = high_resolution_clock::now();
    for (int i = 0; i < 100; i++) buscarPorID(bstID, i);
    fin = high_resolution_clock::now();
    out2 << duration_cast<microseconds>(fin - ini).count() << ",";
    //out2 << "Búsqueda fallida BTS por ID: " << duration_cast<microseconds>(fin - ini).count() << " us\n";

    ini = high_resolution_clock::now();
    for (int i = 0; i < 100; i++) buscarPorScreenName(bstSN, "NoExiste@" + to_string(i));
    fin = high_resolution_clock::now();
    out2 << duration_cast<microseconds>(fin - ini).count();
    //out2 << "Búsqueda fallida BTS por ScreenName: " << duration_cast<microseconds>(fin - ini).count() << " us\n";
    out2 << endl;
  }
  cout << "...experimentos de busqueda terminados" << endl;

  cout << "listo" << endl;
  out.close();
  return 0;
}
