#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>


class Mapa
{
public:
	Mapa();
	bool CargarMapadesdeArchivo(const std::string& ruta);
	void renderizar(sf::RenderWindow& ventana);
	bool VerificarColision(const sf::Vector2f& posicion) const;
	std::vector<std::vector<int>> mapa; // Matriz del mapa

	sf::Texture tileset; // Textura del tileset
	sf::Sprite tileSuelo; // Sprite para el suelo (tile 1)
	sf::Sprite tileBox; // Sprite para la cobertura (tile 2)
	sf::Sprite tileGun; // Sprite para la pistola (tile 3)
	int width, height; // Dimensiones del mapa


};

