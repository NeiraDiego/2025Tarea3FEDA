#pragma once
#include <iostream>
#include <vector>
#include <string.h>

using namespace std;


struct Usuarios {
  long id;
  string screenName;
  string tags;
  string avatar;
  int followersCount;
  int friendsCount;
  string lang;
  long lastSeen;
  long tweetId;
  string friends;
};
