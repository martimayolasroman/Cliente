#include "Mapa.h"
#include <fstream>
#include <sstream>
#include <iostream>


Mapa::Mapa(): width(0),height(0),tileSuelo(tileset), tileBox(tileset), tileGun(tileset)
{
   
    // Cargar la textura del tileset

    if (!tileset.loadFromFile("tilesets/blocksTexture.png")) {
        std::cerr << "Error al cargar el tileset." << std::endl;
    }
   

    

     tileSuelo.setTexture(tileset);
     tileSuelo.setTextureRect(sf::IntRect({ 0, 0 }, { 32, 32 }));


     tileBox.setTexture(tileset);
     tileBox.setTextureRect(sf::IntRect({ 32, 0 }, { 32, 32 }));

     tileGun.setTexture(tileset);
     tileGun.setTextureRect(sf::IntRect({ 64, 0 }, { 32, 32 }));

  
}

bool Mapa::CargarMapadesdeArchivo(const std::string& ruta)
{
    std::ifstream archivo(ruta);
    if (!archivo) {
        std::cerr << "Error al abrir el archivo de mapa." << std::endl;
        return false;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::vector<int> fila;
        std::stringstream ss(linea);
        std::string valor;

        while (std::getline(ss, valor, ',')) {
            fila.push_back(std::stoi(valor));
        }

        mapa.push_back(fila);
    }

    //// Establecer dimensiones del mapa
    height = mapa.size();
    if (height > 0) {
        width = mapa[0].size();
    }



    return true;
}

void Mapa::renderizar(sf::RenderWindow& ventana)
{
    const int TILE_SIZE = 32; // Tamaño de cada tile 


    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (mapa[y][x] == 1) { // Solo renderizar suelo (tile 1)
                
                tileSuelo.setPosition({ static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE) });
                
                ventana.draw(tileSuelo);
            }
            if (mapa[y][x] == 2) { // Solo renderizar COBERTURA (tile 2)

                tileBox.setPosition({ static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE) });

                ventana.draw(tileBox);
            }
            if (mapa[y][x] == 3) { // Solo renderizar arma (tile 3)

                tileGun.setPosition({ static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE) });

                ventana.draw(tileGun);
            }
        }
    }
}

bool Mapa::VerificarColision(const sf::Vector2f& posicion) const
{
    const float TILE_SIZE = 32.0f;
    int tileX = static_cast<int>(posicion.x / TILE_SIZE);
    int tileY = static_cast<int>((posicion.y + 32) / TILE_SIZE); 

    if (tileY >= 0 && tileY < mapa.size() && tileX >= 0 && tileX < mapa[tileY].size()) {
        if(mapa[tileY][tileX] == 1 || mapa[tileY][tileX] == 2)
        return true;
    }
    return false;
}
