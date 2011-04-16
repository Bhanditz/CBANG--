#include <SFML/Network.hpp>

#include <iostream>
#include <string>

#include "../common.hpp"
#include "../engine.hpp"
#include "../player.hpp"

using namespace std;

class _server {
	public:
    sf::SocketTCP socket;
	sf::IPAddress addr;
} server;

/*
struct _buf {
	std::size_t size;
	char data[BUFFER_SIZE];
} buf;*/

sf::Packet packetSend, packetReceive;

vector<player> players;
int turn = 0, myId = 0;

string str;
int main (int argc, char **argv) {
 	if (argc<2) {
		cout << "Käyttö: client <<Serverin osoite>>!" << endl;
		return ERRNO_TOO_LESS_ARGS;
	} else {
		server.addr = sf::IPAddress(argv[1]);
		if (!server.addr.IsValid()) {
			cout << "Osoite ei ole kelvollinen." << endl;
			return ERRNO_NOT_VALID_ADDR;
		}
	}

    if (server.socket.Connect(PORT, server.addr) != sf::Socket::Done)
		return ERRNO_CANT_CONNECT;

	bool Connected = true;

	packetReceive.Clear();
	packetSend.Clear();
    cout << "Yhdistetty serverille " << server.addr << endl;
	string str;
	if (server.socket.Receive(packetReceive) != sf::Socket::Done)
		Connected = false;
	packetReceive >> str;
	OUTPUT("WELCOME", str);
	cout << "< Message: \"" <<  str<< "\"" << endl;

	/* PING */
	packetReceive.Clear();
	packetSend.Clear();
	if (server.socket.Receive(packetReceive) != sf::Socket::Done)
		Connected = false;
	packetReceive >> str;
	cout << "< Ping:    \"" <<  str<< "\"" << endl;
	packetSend << str;
	if (server.socket.Send(packetSend) != sf::Socket::Done)
		Connected = false;
	/* /PING */

	msg tmp;

	// Get Id
	packetReceive.Clear();
	packetSend.Clear();
	if (server.socket.Receive(packetReceive) != sf::Socket::Done)
		Connected = false;
	packetReceive >> str;
	DEBUG(str);
	tmp = Engine.Parse(str);
	DEBUG(tmp.target);
	myId = (int)tmp.target;
	cout << "< Id:      \"" << myId << "\"" << endl;

	while (Connected) {

		packetSend.Clear();
		packetReceive.Clear();

		if (turn == myId) {
			cout << myId << ": > ";
			std::getline(std::cin, str);
			str = string("M")+(char)(48+myId)+'9'+str;

			packetSend << str;
			if (!str.compare("shutdown")) {

				str = 'S';
				Connected = false;
			} else {
				Connected = (server.socket.Send(packetSend) == sf::Socket::Done);
			}
		}

		// wait for instructions from the server
        server.socket.Receive(packetReceive);
        packetReceive >> str;
        tmp = Engine.Parse(str);

		// Server sent empty line -> ignore it
        if (str.empty())
			continue;


        //cout << myId << ": < \"" << str<< "\"" << endl;
		switch(tmp.type) {
			case SHUTDOWN:
				Connected = false;
				break;
			case TURN:
				turn = tmp.target;
				std::cout << "Vuoro vaihtui: " << turn << std::endl;
				break;
			case PING:
				packetSend.Clear();
				packetSend << str;
				server.socket.Send(packetSend);
				break;
			case TEXT:
				if (tmp.sender != myId)
					std::cout << "< " << tmp.data << std::endl;
			default:
				std::cout << "Ominaisuus ei ole vielä valmis! ^^" << std::endl;
		}
	}
	cout << "Sammutetaan client" << endl;
    server.socket.Close();

	return 0;
}
