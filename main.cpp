#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include "Client.h"
#include "Mapa.h"




int main() {

	Client client;
	
	std::cout << "=== Shooter 2D Online ===" << std::endl;

	//Conectar servidor
	if (!client.connectToServer()) {
		std::cout << "Error: No se pudo conectar al servidor.\n";
		return 1;
	}

	while (true) {

		std::cout << "\n 1.Iniciar Sesion \n 2.Registrarse \n 3.Salir \n Eleccion: ";
		int opcion;
		std::cin >> opcion;
		std::cin.ignore();

		if (opcion == 3) break;

		std::string user, pass;
		std::cout << "Usuario: ";
		std::getline(std::cin, user);
		std::cout << "Contraseña: ";
		std::getline(std::cin, pass);

		

		// Enviar acción (LOGIN/REGISTER)
		std::string action = (opcion == 1) ? "LOGIN" : "REGISTER";
		if (!client.sendRequestServer(action + ":" + user + ":" + pass)) {
			std::cout << "Error: Fallo al enviar datos.\n";
			continue;
		}

		// Recibir respuesta
		std::string response = client.receivePacket();

		if (response.empty()) {
			std::cout << "Error: No se recibió respuesta.\n";
			continue;
		}

		// Procesar respuesta
		if (response == "LOGIN_OK") {
			client.nombreUsuario = user; // Almacenar el nombre del usuario
			std::cout << "¡Inicio de sesión exitoso!\n";
			
		}
		else if (response == "REGISTER_OK") {
			client.nombreUsuario = user; // Almacenar el nombre del usuario
			std::cout << "¡Registro exitoso!\n";
			
		}
		else {
			std::cout << "Error: " << response << "\n";
		}

		//Matchmaking

		std::string modo;
		std::cout << "Selecciona modo (AMISTOSO/COMPETITIVO): ";
		std::getline(std::cin, modo);
		

		client.solicitarPartida(modo);
		std::cout << "Buscando partida..." << std::endl;

		// Esperar notificación de partida
		client.esperarPartida();
		
		

	}
	
	return 0;
}
	

