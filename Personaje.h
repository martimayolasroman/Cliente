#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <map>
#include "Mapa.h"




class Personaje
{

public:
	enum Estado {
		Idle,
		Caminando,
		Saltando,
		Disparando

	};

	Personaje(sf::Texture& texturaIdle, sf::Texture& texturaCorrer, sf::Texture& texturaSaltar, sf::Vector2f posicionInicial);
	void Mover(float deltaX, float deltaY);
	void Saltar();
	void ActualizarAnimacion(float deltaTime);
	void Actualizar(float deltaTime, const Mapa& mapa);
	void ActualizarBorde();
	void RecibirDano(int cantidad);
	void Respawn();
	void Dibujar(sf::RenderWindow& ventana, bool esLocal);
	void CambiarEstado(Estado nuevoEstado);
	sf::Vector2f posicion; // Posición del personaje
	float velocidad;       // Velocidad de movimiento

	Estado estado;         // Estado actual del personaje
	int vidas;             // Número de vidas
	int salud;             // Puntos de salud
	sf::RectangleShape borde; 
	void AplicarGravedad(float deltaTime);
	void VerificarColisionSuelo(const Mapa& mapa);

	float velocidadY = 0.0f;
	bool enSuelo = false;
	const float GRAVEDAD = 900.0f;

	sf::Sprite sprite;     // Sprite del personaje
	std::map<Estado, sf::Texture*> texturas; // Texturas para cada estado

	std::map<Estado, std::vector<sf::IntRect>> animaciones; // Animaciones del personaje
	int frameActual = 0;   // Frame actual de la animación
	float tiempoAnimacion = 0.0f; // Tiempo acumulado para la animación
	float tiempoFrame = 0.1f; // Tiempo entre frames (en segundos)


	

};

