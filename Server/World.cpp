#include "World.h"
World::World(int width, int height, int size):
    playerCount(0)
 {
    Grid = std::unique_ptr<Grid> (new Grid(width,height,size));

}
void World::addPlayer(const Player &player) {
    m_player.insert(std::make_pair(player.getID(),std::unique_ptr<Player> (new Player(player))));
    grid->add(player);
    playerCount++;
}
void World::deletePlayer(int fd) {
    Player *player = getPlayer(fd);
    grid.deleteNode(player);
    m_player.erase(id);
    playerCount--;
}
Player *World::getPlayer(int fd) {
    return m_player.at(fd).get();
}
void World::deletePlayer(const Player &player) {
    deletePlayer(player.getID());
}