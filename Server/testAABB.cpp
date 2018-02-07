#include <iostream>
#include <vector>
#include "Grid.h"
#include "Food.h"
#include "Player.h"
#include <event2/event.h>
#include "Server.h"

//class Player;
int main() {
   
 GameServer server(5454);
 server.run();
    return 0;

}
