using namespace std;

#include "Interfaces/Game.h"

int main() {
  const auto game =  new Game;
  game->start();
  game->run();
};