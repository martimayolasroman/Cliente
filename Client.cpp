#include "Client.h"



bool Client::connectToServer()
{
	
    sf::Socket::Status status = socket.connect(sf::IpAddress::LocalHost, 54000);
    return status == sf::Socket::Status::Done;
   
}

void Client::disconnectServer()
{
    socket.disconnect();
}

bool Client::sendRequestServer(const std::string& data)
{


    sf::Packet packet;
    packet << data;

    //Verificar que el paquete tiene contenido antes de enviarlo

    if (packet.getDataSize() == 0) {
        std::cerr << "Error: paquete vacio. No se enivara" << std::endl;
        return false;
    }

    sf::Socket::Status status = socket.send(packet);

    if (status != sf::Socket::Status::Done) {
        std::cerr << "Error al enviar paquete. " << std::endl;
        return false;
    }



   // socket.send(packet);
    std::cout << "Paquete enviado con éxito: " << data << std::endl;
    return true;
}



std::string Client::receivePacket()
{
    sf::Packet packet;
    std::string response;

    sf::Socket::Status status = socket.receive(packet);

    if (status != sf::Socket::Status::Done) {
        std::cerr << "Error al recibir paquete del servidor." << std::endl;
        return "ERROR RECEIVING";
    }

    // Mostrar si el paquete tiene contenido
    std::cout << " Recibido paquete del servidor. Verificando contenido..." << std::endl;

    if (!(packet >> response)) {
        std::cerr << "Error al extraer datos del paquete recibido." << std::endl;
        return "ERROR RECEIVING";
    }

    std::cout << "Respuesta del servidor: " << response << std::endl;


    return response;
}
 