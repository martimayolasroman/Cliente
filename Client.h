#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network/IpAddress.hpp>


#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <optional>
#include "Mapa.h"
#include <variant>  
#include "Personaje.h"
#include <thread>

class Client
{

	public:

		
		
	
		// Conexión al servidor de servicios
		bool connectToServer(); //Conecta al servidor_Servicios
		bool sendRequestServer(const std::string& data);// Envía datos al servidor_servicios para procesar LOGIN/REGISTER
		std::string receivePacket();// Recibe paquetes de respuesta del servidor de servicios
		
		


		// Juego y conexión con Servidor_Gameplay
		void solicitarPartida(const std::string& modo);// Envía solicitud de matchmaking
		void esperarPartida();// Espera a que se le asigne una partida
		void conectarAlGameplay(std::string ip, unsigned short port);// Conecta al servidor de gameplay
		void startGame(std::string ip, unsigned short port);// Inicia el loop del juego

		// Envío y recepción de datos durante la partida
		void iniciarRecepcion();// Iniciar hilo de recepción de datos
		void enviarDatosPersonaje(std::string ip, unsigned short port);// Enviar estado del personaje
		void recibirDatosPersonaje();// Recibir datos del personaje rival


		//Ip adress server_Servicios
		sf::IpAddress serverAddress = sf::IpAddress::LocalHost;// --LOCALHOST
		//std::optional<sf::IpAddress> serverAddress = sf::IpAddress::resolve("192.168.1.100");
		unsigned short serverPort = 54000;
		

		
		std::string nombreUsuario;


		// Métodos para manejar el personaje
		

		Personaje* jugador; // Personaje del jugador local
		Personaje* jugadorRemoto; // Personaje del jugador rival 

		//std::vector<Personaje> jugadoresRivales; 

		bool partidaTerminada = false;
		

	private:

		sf::UdpSocket UdpSocket;
	
};

