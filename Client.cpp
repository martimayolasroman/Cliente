#include "Client.h"

#include <optional>
#include <mutex>


std::mutex mutexJugadorRemoto;



void Client::iniciarRecepcion() {

   
    UdpSocket.setBlocking(false); // Configurar el socket en modo no bloqueante

    while (true) {
       
        recibirDatosPersonaje(); // Llamar a la función de recepción
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); 
    }
    
}



bool Client::connectToServer()
{
	
    if (UdpSocket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done) {
        // Error handling
        std::cerr << "Error al enlazar el cocket UDP" << std::endl;
        return false;
    }

    std::cout << "Conectado al servidor (UDP). Puerto local: " << UdpSocket.getLocalPort() << std::endl;
    return true;
   
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

    sf::Socket::Status status = UdpSocket.send(packet,serverAddress,serverPort);

    if (status != sf::Socket::Status::Done) {
        std::cerr << "Error al enviar paquete. " << std::endl;
        return false;
    }

    
    std::cout << "Paquete enviado con éxito: " << data << std::endl;
    return true;
}



std::string Client::receivePacket()
{
    sf::Packet packet;
    std::optional<sf::IpAddress> sender;
    unsigned short port;
 
    sf::Socket::Status status = UdpSocket.receive(packet, sender, port);


    // Recibir datos con opcionales
    if (status != sf::Socket::Status::Done) {
        return "ERROR RECEIVING";
    }


    // Verificar si la dirección es válida
    if (!sender.has_value()) {
        return "INVALID_SENDER";
    }



    // Mostrar si el paquete tiene contenido
    std::cout << " Recibido paquete del servidor. Verificando contenido..." << std::endl;

    std::string response;
    if (!(packet >> response)) {
        std::cerr << "Error al extraer datos del paquete recibido." << std::endl;
        return "ERROR PACKET";
    }


   
    std::cout << "Respuesta del servidor: " << response << std::endl;
    return response;
}





void Client::startGame(std::string ip, unsigned short port)
{
    sf::RenderWindow ventana(sf::VideoMode({ 800, 600 }), "Shooter 2D - Gameplay");

    // Cargar texturas del personaje
    sf::Texture texturaIdle, texturaCorrer, texturaSaltar;
    if (!texturaIdle.loadFromFile("tilesets/idle.png") || !texturaCorrer.loadFromFile("tilesets/run.png") || !texturaSaltar.loadFromFile("tilesets/jump.png")) {
        std::cerr << "Error al cargar las texturas del personaje." << std::endl;
        return;
    }

    

    // Crear personaje del jugador local
    jugador = new Personaje(texturaIdle, texturaCorrer, texturaSaltar, sf::Vector2f(1.f, 1.0f));
    jugadorRemoto = new Personaje(texturaIdle, texturaCorrer, texturaSaltar, sf::Vector2f(1.f, 1.0f));

    Mapa mapa;
    if (!mapa.CargarMapadesdeArchivo("Maps/mapa.txt")) {
        std::cerr << "Error al cargar el mapa." << std::endl;
        return;
    }

    sf::Clock reloj;
    
    while (ventana.isOpen()) {

        float deltaTime = reloj.restart().asSeconds();


        std::optional<sf::Event> evento ;
       

        while (evento = ventana.pollEvent()) {

            if (!evento.has_value()) break; //Evita acceder a `evento` vacío

           

            if (evento->is<sf::Event::Closed>()) {
                std::cout << "Evento de cierre detectado. Cerrando ventana..." << std::endl;
                ventana.close();
            }

           
      
            //// Manejar entradas del usuario

            if (auto keyEvent = evento->getIf<sf::Event::KeyPressed>()) {
                

                if (keyEvent->code == sf::Keyboard::Key::D) {
                    jugador->Mover(0.5f, 0.0f); // Mover a la derecha
                  
                }
                if (keyEvent->code == sf::Keyboard::Key::A) {
                    
                    jugador->Mover(-0.5f, 0.0f); // Mover a la izquierda
                }
                if (keyEvent->code == sf::Keyboard::Key::W) {
                    
                    jugador->Saltar(); // Saltar
                }
                if (keyEvent->code == sf::Keyboard::Key::Space) {

                   
                    // Verificar colisión
                    sf::FloatRect boundsLocal = jugador->sprite.getGlobalBounds();
                    sf::FloatRect boundsRemoto = jugadorRemoto->sprite.getGlobalBounds();
                    if (boundsLocal.findIntersection(boundsRemoto)) {
                        std::cout << "VICTORIA: " + nombreUsuario;
                        // Enviar victoria al servidor
                        sf::Packet packetVictoria;
                        std::string mensajeVictoria = "VICTORIA:" + nombreUsuario + ":10";
                        packetVictoria << mensajeVictoria;
                      
                        std::cout << "Enviando paquete: " << mensajeVictoria << std::endl;
                    
                
                        if (UdpSocket.send(packetVictoria, serverAddress, serverPort) == sf::Socket::Status::Done) {
                            std::cout << "Victoria registrada!" << std::endl;
                            partidaTerminada = true;
                           // ventana.close();
                        }
                    }
                }

               
                
            }
        }

        // Actualizar animaciones
      //  jugador->ActualizarAnimacion(deltaTime);
       // jugadorRemoto->ActualizarAnimacion(deltaTime);

        jugador->AplicarGravedad(deltaTime);
        jugador->VerificarColisionSuelo(mapa);
        // Enviar datos del personaje al servidor
       
            enviarDatosPersonaje(ip,port);

        // Recibir datos de los personajes rivales
            recibirDatosPersonaje();

        ventana.clear();
        jugador->Dibujar(ventana,true); // Dibujar al personaje local
        jugadorRemoto->Dibujar(ventana,false);
        mapa.renderizar(ventana); // Renderizar el mapa
        
        ventana.display();
    }
    delete jugador; // Liberar memoria
}

void Client::solicitarPartida(const std::string& modo)
{
    sf::Packet packet;
    //packet << "MATCHMAKING:" + modo;
    packet << "MATCHMAKING:" + nombreUsuario + ":" + modo; // Enviar nombre y modo

    std::string MatchmakingModeCheck;
    packet >> MatchmakingModeCheck;
 

    if (UdpSocket.send(packet, serverAddress, serverPort) != sf::Socket::Status::Done) {
        std::cerr << "Error al solicitar partida." << std::endl;
    }


}

void Client::esperarPartida()
{

    while (true) {

        std::string notificacion = receivePacket();

        std::cout << "----------Notificacion esperar partida: " << notificacion << std::endl;

        if (notificacion.find("MATCH_FOUND:") == 0) {
            //Extraer direccion y puerto del Servidor_Gameplay
            size_t pos1 = notificacion.find(":");
            size_t pos2 = notificacion.find(":", pos1 + 1);
            std::string ip = notificacion.substr(pos1 + 1, pos2 - pos1 - 1);
            unsigned short port = std::stoi(notificacion.substr(pos2 + 1));
            //sf::IpAddress ServerGameplayIP = ip;

            std::cout << "Partida encontrada. Conectando al Servidor_Gameplay..." << std::endl;
            conectarAlGameplay(ip, port); // Conectar al Servidor_Gameplay
            break;
        }



    }



}

void Client::conectarAlGameplay(std::string ip, unsigned short port)
{

   
    std::optional<sf::IpAddress> serverGameplayIP = sf::IpAddress::resolve(ip);

    if (!serverGameplayIP.has_value()) {
        std::cerr << "Error: No se pudo resolver la dirección IP del servidor de gameplay." << std::endl;
        return;
    }


    std::cout << "Conectado al Servidor_Gameplay. Iniciando juego..." << std::endl;

    // Enviar un mensaje de confirmación al Servidor_Gameplay
    sf::Packet packet;
    packet << "CONNECTED:" + nombreUsuario;
    if (UdpSocket.send(packet, serverGameplayIP.value(), port) != sf::Socket::Status::Done) {
        std::cerr << "Error al enviar mensaje al Servidor_Gameplay." << std::endl;
        return;
    }

    

    std::thread hiloRecepcion([this]() { this->iniciarRecepcion(); });
    hiloRecepcion.detach();
    startGame(ip, port); // Abrir la ventana del gameplay
    

}

void Client::enviarDatosPersonaje(std::string ip, unsigned short port)
{
    sf::Packet packet;
    std::optional<sf::IpAddress> serverGameplayIP = sf::IpAddress::resolve(ip);

    packet << "UPDATE:" << nombreUsuario << jugador->posicion.x << jugador->posicion.y << static_cast<int>(jugador->estado);
    
    // Enviar el paquete al servidor
   
        if (UdpSocket.send(packet, serverGameplayIP.value(), port) != sf::Socket::Status::Done) {
            std::cerr << "Error al enviar datos del personaje." << std::endl;
        }
   
    
}

void Client::recibirDatosPersonaje()
{

    
    sf::Packet packet;
    std::optional<sf::IpAddress> sender;
    unsigned short port;
   
    sf::Clock relojLatencias;

         sf::Socket::Status status = UdpSocket.receive(packet, sender, port);
        if (status == sf::Socket::Status::Done) {

           

            if (sender.has_value()) { // Verificar si el sender tiene un valor

                
                std::string mensaje, nombre;
                float posX, posY;
                int estado;

              
                if (packet >> mensaje >>nombre >> posX >> posY >> estado) { // Verificar si se extrajeron los datos correctamente
                   
                  
                        // Actualizar personajes rivales
                        if (mensaje.find("UPDATE") == 0) {
                          
                           std::lock_guard<std::mutex> lock(mutexJugadorRemoto);
                           jugadorRemoto->posicion = sf::Vector2f(posX, posY);
                           jugadorRemoto->estado = static_cast<Personaje::Estado>(estado);
                           
                        
                        }
                    
                    
                }
                else {
                    std::cerr << "Error al extraer datos del paquete recibido." << std::endl;
                }
            }
            else {
                std::cerr << "Error: sender no tiene un valor válido." << std::endl;
            }
        }
        else if (status != sf::Socket::Status::NotReady) {
            
            std::cerr << "-----------------(recibirDatosPersonaje)Error en receive(): NOT READY " << static_cast<int>(status) << std::endl;
        }
        else {
            std::cerr << "-----------------(recibirDatosPersonaje)Error en receive(): " << static_cast<int>(status) << std::endl;
        }
    

       
    }







 