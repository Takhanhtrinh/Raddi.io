CC = g++
CCFLAGS = -Wall -g
CCVER= -std=c++11 -stdlib=libc++
INC = -I/usr/local/Cellar/libevent/2.0.22/include/
LIBPATH = -L/usr/local/Cellar/libevent/2.0.22/lib/
LIB =-levent
all: main
BitPack.o: protocol/BitPack.cpp
	g++ -c protocol/BitPack.cpp -o BitPack.o
testBit.o: testBit.cpp
	g++ -c testBit.cpp -o testBit.o
testBit: BitPack.o testBit.o
	g++ testBit.o -o testBit BitPack.o
Packet.o: protocol/Packet.cpp
	g++ -c protocol/Packet.cpp -o Packet.o
PacketManager.o: protocol/PacketManager.cpp
	g++ -c protocol/PacketManager.cpp -o PacketManager.o
AABB.o: AABB.cpp
	g++ -c $(CCVER) AABB.cpp -o AABB.o
common.o: protocol/common.cpp
	g++ -c $(CCVER) protocol/common.cpp -o common.o
Entity.o: Entity.cpp
	g++ -c $(CCVER) Entity.cpp -o Entity.o
MovementEntity.o: MovementEntity.cpp
	g++ -c $(CCVER) MovementEntity.cpp -o MovementEntity.o
Food.o: Food.cpp
	g++ -c $(CCVER) Food.cpp -o Food.o
Player.o: Player.cpp
	g++ -c $(CCVER) Player.cpp -o Player.o
Bullet.o: Bullet.cpp
	g++ -c $(CCVER) Bullet.cpp -o Bullet.o
Item.o: Item.cpp
	g++ -c $(CCVER) Item.cpp -o Item.o
Grid.o: Grid.cpp
	g++ -c $(CCVER) Grid.cpp -o Grid.o
MyBuffer.o: MyBuffer.cpp
	g++ -c $(CCVER) MyBuffer.cpp -o MyBuffer.o

main.o: testAABB.cpp
	g++ -c $(CCVER) testAABB.cpp -o main.o
Server.o: Server.cpp
	g++ -c $(CCVER) Server.cpp -o Server.o 

main: common.o MyBuffer.o AABB.o Entity.o MovementEntity.o Item.o Bullet.o Food.o Player.o Grid.o BitPack.o Packet.o  PacketManager.o Server.o main.o 
	g++ $(CCVER) -o main $(INC) $(LIBPATH) $(LIB) main.o common.o MyBuffer.o Entity.o MovementEntity.o Food.o Player.o Item.o Bullet.o Grid.o Server.o BitPack.o Packet.o PacketManager.o

clean:
	-rm AABB.o Entity.o MovementEntity.o Food.o Player.o Grid.o main.o common.o Server.o main Message.o Packet.o PacketManager.o Bullet.o Item.o BitPack.o MyBuffer.o