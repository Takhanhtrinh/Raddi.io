all: testBit

BitPack.o: BitPack.cpp
	g++ -c BitPack.cpp -o BitPack.o
testBit.o: testBit.cpp
	g++ -c testBit.cpp -o testBit.o
testBit: BitPack.o Packet.o PacketManager.o testBit.o
	g++ -std=c++11 testBit.o -o testBit BitPack.o PacketManager.o Packet.o
Packet.o: Packet.cpp
	g++ -c Packet.cpp -o Packet.o
PacketManager.o: PacketManager.cpp
	g++ -c PacketManager.cpp -o PacketManager.o
clean:
	rm BitPack.o testBit testBit.o PacketManager.o Packet.o