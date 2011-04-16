//
// game.h
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2011 Miro Nieminen (leonarven@gmail.com) ja sante ^^
//

//PROTOCOL:
//sender
//destination
//data = string

#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Network.hpp>
#include <vector>
#include <cstdlib>
#include <cstring>
#include "common.hpp"

class _Engine
{
public:
	msg Parse(std::string buf)
	{
		msg message;
		message.buf = (char *)buf.c_str();
		message.type = (MSG_TYPE)buf[0];

		DEBUG((int)buf[1])
		DEBUG((int)buf[2])

		message.sender = buf[1] - 48;
		message.target = buf[2] - 48;

		DEBUG(buf)
		DEBUG(buf.length())

		if (buf.length() >= 4)
			message.data = buf.substr(3);


        return message;
    }

/*
	std::string sImplode(MSG_TYPE, unsigned sender, unsigned target, std::string data)
	{

    }
	sf::Packet sImplode(MSG_TYPE, unsigned sender, unsigned target, std::string data)
	{

    }*/




} Engine;

#endif // GAME_HPP

