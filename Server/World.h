#include "Grid.h"
#include "Food.h"
#include "Player.h"
#include <unordered_map>
#include <memory>

class World
{
    public:
     World(int width, int height, int size);
     void addPlayer(const Player & player);
     void deletePlayer(const Player &player);
     Player *getPlayer(int fd);
     void deletePlayer(int fd);
     void spawnFood();
     int getPlayerCount() const {return playerCount;}



     private:
     std::unique_ptr<Grid> grid;
     std::unordered_map<int, std::unique_ptr<Player> > m_player;
     int playerCount;
};