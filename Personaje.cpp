#include "Personaje.h"
#include <iostream>

Personaje::Personaje(sf::Texture& texturaIdle, sf::Texture& texturaCorrer, sf::Texture& texturaSaltar, 
sf::Vector2f posicionInicial) : posicion(posicionInicial), 
velocidad(100.0f),  estado(Idle), vidas(3), salud(5),sprite(texturaIdle)
{

    // Asignar texturas a los estados
    texturas[Idle] = &texturaIdle;
    texturas[Caminando] = &texturaCorrer;
    texturas[Saltando] = &texturaSaltar;

    // Inicializar sprite con la textura de idle
    sprite.setTexture(*texturas[Idle]);
    sprite.setPosition(posicion);



    animaciones[Idle] = std::vector<sf::IntRect>{ sf::IntRect({0, 0}, {32, 32}) };
    animaciones[Caminando] = std::vector<sf::IntRect>{
    sf::IntRect({0, 0}, {32, 32}),
    sf::IntRect({32, 0}, {32, 32}),
    sf::IntRect({64, 0}, {32, 32})
    };
    animaciones[Saltando] = std::vector<sf::IntRect>{ sf::IntRect({0, 0}, {32, 32}) };
    borde.setFillColor(sf::Color::Transparent);
    borde.setOutlineThickness(3.0f); 
    

    


    
    
}

void Personaje::Mover(float deltaX, float deltaY)
{
    posicion.x += deltaX * velocidad;
    posicion.y += deltaY * velocidad;
    sprite.setPosition(posicion);


    // Cambiar al estado "Caminando" si se está moviendo
    if (deltaX != 0 || deltaY != 0) {
        CambiarEstado(Caminando);
    }
    else if (estado != Saltando && estado != Disparando) {
        CambiarEstado(Idle);
    }
}

void Personaje::Saltar()
{
    if (enSuelo) {
        if (estado != Saltando) {
            CambiarEstado(Saltando);
            // Aplicar velocidad vertical (simular salto)
            velocidadY = -400.0f;
            enSuelo = false;
        }
    }
}


void Personaje::ActualizarAnimacion(float deltaTime)
{
    tiempoAnimacion += deltaTime;

    // Cambiar el frame de la animación según el tiempo
    if (tiempoAnimacion >= tiempoFrame) {
        frameActual = (frameActual + 1) % animaciones[estado].size();
        sprite.setTextureRect(animaciones[estado][frameActual]);
        tiempoAnimacion = 0.0f;
    }
}

void Personaje::Actualizar(float deltaTime, const Mapa& mapa)
{
    // Aplicar gravedad y colisiones solo en el cliente
        if (!enSuelo) {
            velocidadY += GRAVEDAD * deltaTime;
            posicion.y += velocidadY * deltaTime;
        }

    // Verificar colisión con el suelo
    VerificarColisionSuelo(mapa);

}

void Personaje::ActualizarBorde()
{

    // Obtener el rectángulo global del sprite
    sf::FloatRect bounds = sprite.getGlobalBounds();
    // Configurar posición y tamaño del borde
    borde.setPosition(bounds.position);
    borde.setSize(sf::Vector2f(bounds.size.x, bounds.size.y));

    
}

void Personaje::RecibirDano(int cantidad)
{
    salud -= cantidad;
    if (salud <= 0) {
        vidas--;
        Respawn();
    }
}

void Personaje::Respawn()
{
    posicion = sf::Vector2f(1, 1); // Centro del mapa
    salud = 5; // Restablecer salud
    sprite.setPosition(posicion);
}

void Personaje::Dibujar(sf::RenderWindow& ventana,bool esLocal)
{
    
    ActualizarBorde();

    // Color del borde según el tipo de jugador
    if (esLocal) {
        borde.setOutlineColor(sf::Color(0, 100, 255)); // Azul
    }
    else {
        borde.setOutlineColor(sf::Color(255, 50, 50)); // Rojo
    }
    sprite.setPosition(posicion);

   
    
    ventana.draw(sprite);
    ventana.draw(borde);
}

void Personaje::CambiarEstado(Estado nuevoEstado)
{
    if (estado != nuevoEstado) {
        estado = nuevoEstado;
        sprite.setTexture(*texturas[estado]); // Cambiar la textura
        frameActual = 0; // Reiniciar el frame de la animación
        tiempoAnimacion = 0.0f; // Reiniciar el tiempo de la animación
    }
}



void Personaje::AplicarGravedad(float deltaTime)
{
    if (!enSuelo) {
        velocidadY += GRAVEDAD * deltaTime;
        posicion.y += velocidadY * deltaTime;
        sprite.setPosition(posicion);
    }
}

void Personaje::VerificarColisionSuelo(const Mapa& mapa)
{
    // Lógica de colisión local (sin dependencia del servidor)
    const float TILE_SIZE = 32.0f;
    sf::FloatRect bounds = sprite.getGlobalBounds();
    int tileY = static_cast<int>((posicion.y + bounds.size.y) / TILE_SIZE);
    int tileX = static_cast<int>((posicion.x + bounds.size.x / 2) / TILE_SIZE);

    if (tileY >= 0 && tileY < mapa.mapa.size() && tileX < mapa.mapa[tileY].size()) {
        enSuelo = (mapa.mapa[tileY][tileX] == 1 || mapa.mapa[tileY][tileX] == 2);
        if (enSuelo) {
            velocidadY = 0;
            posicion.y = tileY * TILE_SIZE - bounds.size.y;
        }
    }

}
