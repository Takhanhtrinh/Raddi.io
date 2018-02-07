#ifndef GRID_H
#define GRID_H
#pragma once
#include <vector>
#include "Entity.h"
#include <memory>
#include <unordered_map>
#include "Player.h"
#include "AABB.cpp"
#include "Bullet.h"
#include "Food.h"
#include "Item.h"
#include "MovementEntity.h"

const short MAXIMUM_NODE_PER_CELL = 20;
struct Cell {
    std::unordered_map<int,std::unique_ptr<Entity> > m_data;
    Cell() {
        m_data.reserve(MAXIMUM_NODE_PER_CELL);
        
    }
    void empty(){
        m_data.clear();
    }
};
class Grid {
public:
    static std::unique_ptr<Grid> createGrid(int width, int height, int size);
    Grid(int width, int height, int size);
    Cell *getCell(int x, int y);
    void add(const Entity &data) ;
    Point findCell(float  x, float y) const;
    void findCell(const Point &point, int array[]) const;
    Point findMovementCell(const MovementEntity &data) const;
    Point findCellBoudingBox(int x,int y, const Point &size) const;
    void getPointsBoudingBox(const AABB &aabb, std::unordered_map<int,Entity*> &data);
    int getCol() const {return m_col;}
    int getRow() const {return m_row;}
    int getSize() const {return m_size;}
    void deleteNode(const Entity &data);
    void update(  const MovementEntity &data);
    bool deleteMovementEntity(const MovementEntity &data);
    bool deletePlayer(const Player &data);
    
    Player *getPlayer(const Entity &player);
    int getCount() const {return EntityCount;}
    void empty();
    ~Grid();
private:
    int m_size;
    int m_width;
    int m_height;
    std::vector<std::vector<Cell> > m_cell;
    int m_col;
    int m_row;
    int EntityCount;
};
#endif
