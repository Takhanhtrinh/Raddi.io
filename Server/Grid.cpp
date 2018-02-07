
#include "protocol/common.h"
#include <iostream>
#include "Grid.h"
#include <assert.h>



Grid::Grid(int width, int height, int size ):
m_size(size),
m_width(width),
m_height(height),
EntityCount(0) {
    m_col = m_width / size + 1;
    m_row = m_height / size + 1;
    m_cell.resize(m_row);
    for (int i =0; i < m_row; i++) {
        m_cell[i].resize(m_col);
        
    }
    
}
std::unique_ptr<Grid> Grid::createGrid(int width, int height, int size){
    auto grid = std::unique_ptr<Grid>(new Grid(width,height,size));
    return grid;
}
void Grid::add( const Entity& data) {
    
    Point point;
    int key;
    assert(data.getPositionX() >= 0 || data.getPositionY() >= 0 || data.getPositionX() <= m_width || data.getPositionY() <= m_height);
    point = findCell(data.getPositionX(), data.getPositionY());
    if (point.m_x == -1 || point.m_y == -1) return;
    
    Cell *cell = getCell(point.m_x,point.m_y);
    
    
    if (data.getType() == FOOD_TYPE){
        
        const Food *food = dynamic_cast<const Food*>(&data);
        if (food != nullptr){
        cell->m_data.insert(std::make_pair(food->getID(),std::unique_ptr<Entity>(new Food(*food))));
        // printf("insert food success\n");

        }
        EntityCount++;    
        
    }
    else if (data.getType()  == PLAYER_TYPE)
    {
    key = data.getID();
    if (cell->m_data.count(key)  == 1) {
        std::cout << "<Grid> duplicate data " << std::endl;
        return;
    }
    const Player *player = dynamic_cast<const Player*> (&data);
    if(!player) {
        std::cout << "add player cast faild <Grid>" << std::endl;
        return;
    }
   
    cell->m_data.insert(std::make_pair(key, std::unique_ptr<Entity>(new Player(*player))));
    // std::cout << "add Player ID: " << player->getID() << "  success" << std::endl;
    // printf("add player sucess\n");
        }
    else if (data.getType() == BULLET_TYPE) {
        key = data.getID();
        if (cell->m_data.count(key)  == 1) {
            cell->m_data.erase(key);
            
        }
        const Bullet *bullet = dynamic_cast<const Bullet*>(&data);
       
        if (!bullet){
            std::cout << "add Bullet cast faild<Grid> " << std::endl;
            return;
        }
        cell->m_data.insert(std::make_pair(key, std::unique_ptr<Entity>(new Bullet(*bullet))));
        // GAMELOG("Grid update", "sucees add bullet");
        // printf("bullet id: %d x: %f y: %f prex: %f prey: %f\n", bullet->getID(), bullet->getPositionX(), bullet->getPositionY(), bullet->getLastX(), bullet->getLastY());
            
    
    }
   


        
}
//cell->m_data.push_back(std::unique_ptr<Entity>(new Entity(data)));


Point Grid:: findCellBoudingBox(int x,int y, const Point &size) const{
    Point rt = Point(-1,-1);
    if ( x < 0 )
        rt.m_x = 0;
    else if (x > m_width)
        rt.m_x = m_col -1 ;
    else
        rt.m_x = x / m_size;
    if ( y < 0 )
        rt.m_y = 0;
    else if (y > m_height)
        rt.m_y = m_row - 1;
    else
        rt.m_y = y / m_size;
    return rt;
    
}
Point Grid:: findMovementCell(const MovementEntity &data) const{
    float dx = ceil(data.getLastX());
    float dy = ceil(data.getLastY());
    Point point(-1,-1);
       assert(dx >= 0 &&dy>= 0);
       if (dx >= 0 && dx <= m_width && dy >=0 && dy <= m_height){
        point.m_x= dx / m_size;
        point.m_y = dy / m_size;
        
    }
    else 
        printf("error find cell\n");
    
    return point;

}
Point Grid::findCell(float x, float y) const {
    int dx = (int ) x;
    float dy = (int ) y;
    Point point(-1,-1);
    // assert(dx >= 0 &&dy>= 0);
    if (dx >= 0 && dx <= m_width && dy >=0 && dy <= m_height){
        point.m_x= dx / m_size;
        point.m_y = dy / m_size;
        
    }
    else 
        printf("error find cell\n");
    
    return point;
    
}
void Grid:: findCell(const Point &point, int array[]) const {
    findCell(point.m_x, point.m_y);
}

Cell* Grid::getCell(int x, int y) {
    if (x < 0 || y < 0) return &m_cell[0][0];
    if (x > m_width || y > m_height) return &m_cell[m_row][m_col];
    return &m_cell[y][x];
}

void Grid:: getPointsBoudingBox(const AABB & aabb, std::unordered_map<int,Entity*> &data){
    data.clear();
    Point min;
    Point max;
    min.m_x = aabb.m_center.m_x - aabb.m_halfDim.m_x;
    min.m_y = aabb.m_center.m_y + aabb.m_halfDim.m_y;
    max.m_x = aabb.m_center.m_x + aabb.m_halfDim.m_x;
    max.m_y = aabb.m_center.m_y - aabb.m_halfDim.m_y;
    if (min.m_x == - 1 || min.m_y ==-1 || max.m_x ==-1 || max.m_y == -1 ) return;
    Point leftTop = findCellBoudingBox(min.m_x, min.m_y, aabb.m_halfDim);
    Point rightBottom = findCellBoudingBox(max.m_x, max.m_y,aabb.m_halfDim);
    for (int i = rightBottom.m_y; i <= leftTop.m_y; i++) {
        for (int j = leftTop.m_x; j<= rightBottom.m_x; j++) {
            Cell *cell = getCell(j,i);
            for (auto it = cell->m_data.cbegin();it != cell->m_data.cend(); it++) {
                //std::cout << it->first << std::endl;
                    if (aabb.intersects(it->second->getBox()))
                        data.insert(std::make_pair(it->first, it->second.get()));
                    
                }
                // for (int k =0; k < cell->m_data.size(); k++) {
                //     data.push_back(cell->m_data[k].get());
            }
        }
    }



void Grid::deleteNode(const Entity &data) {
    if (data.getType() == PLAYER_TYPE)
        return;
    
    
    Point point = findCell(data.getPositionX(), data.getPositionY());
    if (point.m_x ==  -1 || point.m_y == -1) return;
    //   std::cout << "key ne " << key << std::endl;
    Cell *cell = getCell(point.m_x, point.m_y);
    if (cell == nullptr ) return;
    std::unordered_map<int,std::unique_ptr<Entity> > :: iterator node;
    node = cell->m_data.find(data.getID());
    if (node == cell->m_data.end()) {
//         std::cout<< " not found " << std::endl;
    }
    else {
//         std::cout<< "  found " << std::endl;
        cell->m_data.erase(data.getID());
        
    }
}
void Grid::empty() {
    for (int i =0; i < m_row; i++) {
        for (int j =0; j < m_col; j++) {
            Cell *cell = getCell(i,j);
            cell->empty();
        }
    }
}
Player* Grid::getPlayer(const Entity &player) {
    int id = player.getID();
    //std::cout << "iddd ne: " << player.getID() << std::endl;
    Player *returnPlayer = nullptr;
    if (id >= 0 ) {
        
        
        //     std:: cout <<"x ne : " <<  player.getPositionX() << " y ne: " << player.getPositionY() << std::endl;
        Point point = findCell(player.getPositionX(), player.getPositionY());
        //        std::cout << point.m_x << "  " << point.m_y << std::endl;
        if (point.m_x ==  -1 || point.m_y == -1) return nullptr;
        //   std::cout << "key ne " << key << std::endl;
        Cell *cell = getCell(point.m_x, point.m_y);
        
        
        if (cell->m_data.count(id) == 1) {
            returnPlayer = dynamic_cast<Player*> (cell->m_data.at(id).get());
            if (returnPlayer)
                return returnPlayer;
        }
        else{
            //            std::cout << "cant find " << std::endl;
            return nullptr;
        }
        
        
    }
    return returnPlayer;
}
void Grid::update( const MovementEntity &data){
    //get last position and deleted it and then update current position of player to the grid
      
   if (deleteMovementEntity(data)) 
        add(data);

    
}
 bool Grid:: deleteMovementEntity(const MovementEntity &data){
   
    Point point = findCell(data.getLastX(), data.getLastY());
   assert(point.m_x >=0 || point.m_y >= 0 );
    Cell *cell = getCell(point.m_x, point.m_y);
    if (cell == nullptr ) return false;
    std::unordered_map<int,std::unique_ptr<Entity> > ::iterator it;
    it = cell->m_data.find(data.getID());
    // assert(it != cell->m_data.end());
    if(it == cell->m_data.end()) {
        
    }
    else{
        cell->m_data.erase(data.getID());
        // std::cout << "delete player sucess" << std::endl;
        return true;
    }
    
    
    return false;
 }
bool Grid::deletePlayer(const Player &data){
    
    //    Point point = findCell((data.getPosition().x), (data.getPosition().y));
    Point point = findCell(data.getLastX(),data.getLastY());
   assert(point.m_x >=0 && point.m_y >= 0);
    Cell *cell = getCell(point.m_x, point.m_y);
    if (cell == nullptr ) return false;
    std::unordered_map<int,std::unique_ptr<Entity> > ::iterator it;
    it = cell->m_data.find(data.getID());
    if(it == cell->m_data.end()) {
        std::cout << "not found player" << std::endl;
    }
    else{
        cell->m_data.erase(data.getID());
        // std::cout << "delete player sucess" << std::endl;
        return true;
    }
    
    
    return false;
}


Grid::~Grid() {
    empty();
}
