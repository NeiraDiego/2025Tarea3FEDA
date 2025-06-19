#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Usuarios.h"

using namespace std;

Usuarios procesLinea(const string& linea) {
  Usuarios user;
  stringstream lineaStream(linea);
  string campo;

  return user;
}

int main() {
  ifstream archivo("data.csv");
  string linea;
  vector<Usuarios> usuarios;

  // Saltar header
    getline(archivo, linea);

  cout << linea << endl;

  getline(archivo, linea);
  cout << linea << endl;
  linea = linea.substr(1, linea.size() - 3);
  cout << linea << endl;
  
}
