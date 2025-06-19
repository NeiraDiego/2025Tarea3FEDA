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

  // Campo 1: id
  try {
    getline(lineaStream, campo, ',');
    user.id = stoi(campo);
  }
  catch (const std::exception& e) {
    cerr << "error en la linea" << campo << endl;
  }

  // Campo 2: screenName
  getline(lineaStream, campo, ',');
  if (campo.front() == '"') campo = campo.substr(1, campo.size() - 2);
  user.screenName = campo;

  // Campo 3: tags
  getline(lineaStream, campo, ']'); // hasta el cierre de lista
  auto ini = campo.find("[");
  if (ini != string::npos) {
    string tagsStr = campo.substr(ini + 1);
    stringstream tagsStream(tagsStr);
    string tag;
    while (getline(tagsStream, tag, ',')) {
      tag.erase(remove(tag.begin(), tag.end(), '"'), tag.end());
      tag.erase(remove(tag.begin(), tag.end(), ' '), tag.end());
      if (!tag.empty()) user.tags.push_back(tag);
    }
  }
  lineaStream.ignore(); // coma después del campo

  // Campo 4: avatar
  getline(lineaStream, campo, ',');
  if (campo.front() == '"') campo = campo.substr(1, campo.size() - 2);
  user.avatar = campo;

  // Campo 5: followersCount
  getline(lineaStream, campo, ',');
  user.followersCount = stoi(campo);

  // Campo 6: friendsCount
  getline(lineaStream, campo, ',');
  user.friendsCount = stoi(campo);

  // Campo 7: lang
  getline(lineaStream, campo, ',');
  if (campo.front() == '"') campo = campo.substr(1, campo.size() - 2);
  user.lang = campo;

  // Campo 8: lastSeen
  getline(lineaStream, campo, ',');
  user.lastSeen = stol(campo);

  // Campo 9: tweetId
  getline(lineaStream, campo, ',');
  if (campo.front() == '"') campo = campo.substr(1, campo.size() - 2);
  user.tweetId = stol(campo);

  // Campo 10: friends
  getline(lineaStream, campo); // hasta fin de línea
  auto f_ini = campo.find("[");
  if (f_ini != string::npos) {
    string friendsStr = campo.substr(f_ini + 1);
    friendsStr.erase(remove(friendsStr.begin(), friendsStr.end(), ']'), friendsStr.end());
    stringstream friendsStream(friendsStr);
    string fid;
    while (getline(friendsStream, fid, ',')) {
      fid.erase(remove(fid.begin(), fid.end(), '"'), fid.end());
      fid.erase(remove(fid.begin(), fid.end(), ' '), fid.end());
      if (!fid.empty()) user.friends.push_back(stol(fid));
    }
  }

  return user;
}

  
