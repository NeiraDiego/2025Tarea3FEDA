#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Usuarios.h"

using namespace std;

Usuarios procesLinea(const string& linea2) {
  Usuarios user;
  // eimina un simbolo " que aparece al principio y final de la linea
  string linea;
  linea = linea2.substr(1, linea2.size() - 2);
  stringstream lineaStream(linea);
  string campo;

  // Campo 1: id
  getline(lineaStream, campo, ',');
  user.id = stol(campo);

  // Campo 2: screenName
  getline(lineaStream, campo, ',');
  campo = campo.substr(2, campo.size() - 4);
  user.screenName = campo;

  // Campo 3: tags
  getline(lineaStream, campo, ']');
  user.tags = campo; //campo.substr(2, campo.size() - 4);
  getline(lineaStream, campo, ',');

  // Campo 4: avatar
  getline(lineaStream, campo, ',');
  // campo = campo.substr(2, campo.size() - 4);
  user.avatar = campo;

  // Campo 5: followersCount
  getline(lineaStream, campo, ',');
  user.followersCount = stoi(campo);

  // Campo 6: friendsCount
  getline(lineaStream, campo, ',');
  user.friendsCount = stoi(campo);

  // Campo 7: lang
  getline(lineaStream, campo, ',');
  // campo = campo.substr(2, campo.size() - 4);
  user.lang = campo;

  // Campo 8: lastSeen
  getline(lineaStream, campo, ',');
  user.lastSeen = stol(campo);

  // Campo 9: tweetId
  getline(lineaStream, campo, ',');
  campo = campo.substr(2, campo.size() - 4);
  user.tweetId = stol(campo);


  // Campo 10: friends
  getline(lineaStream, campo, ']');
  user.friends = campo;//.substr(2, campo.size() - 4);

  return user;
}

  
