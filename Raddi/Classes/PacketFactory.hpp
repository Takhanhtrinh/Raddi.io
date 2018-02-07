//
//  PacketFactory.hpp
//  Raddi
//
//  Created by Trinh Ta on 2/20/17.
//
//

#ifndef PacketFactory_hpp
#define PacketFactory_hpp

#include <iostream>
#include "Packet.hpp"
class PacketFactory{
public:
    static Packet* createPacket(int type,int len);
};
#endif /* PacketFactory_hpp */
