#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

#include "Client.h"

//Configuracion de la ventana

const int WIDTH = 400;
const int HEIGHT = 300;

int main() {

	Client client;
	std::cout << "=== Shooter 2D Online ===" << std::endl;

	//Conectar servidor
	if (!client.connectToServer()) {
		std::cout << "Error: No se pudo conectar al servidor.\n";
		return 1;
	}

	while (true) {
		std::cout << "\n 1.Iniciar Sesion \n 2.Registrarse \n Salir \n Eleccion: ";
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
			std::cout << "¡Inicio de sesión exitoso!\n";
			break; // Iniciar juego
		}
		else if (response == "REGISTER_OK") {
			std::cout << "¡Registro exitoso!\n";
			// Iniciar juego
		}
		else {
			std::cout << "Error: " << response << "\n";
		}

		

	}
	//client.disconnectServer();
	return 0;
}
	

