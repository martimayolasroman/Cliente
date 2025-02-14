#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>



class Client
{
	public:
		//Client(const std::string& serverIp, unsigned short serverPort);
		bool connectToServer();
		void disconnectServer();
		bool sendRequestServer(const std::string& data);
		std::string receivePacket();
	private:
		sf::TcpSocket socket;
	
};

