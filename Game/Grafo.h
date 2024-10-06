
//
// Created by aldo-bagliano on 4/10/24.
//

#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <SDL2/SDL.h>

class Grafo {
public:
    Grafo(int ancho, int alto, int tamañoCeldaAncho, int tamañoCeldaAlto);
    void generarObstaculos();
    void dibujarGrafo(SDL_Renderer* renderer);
    bool esCeldaLibre(int x, int y); // Nueva función para verificar si la celda está libre

    int getNumColumnas() const;
    int getNumFilas() const;
    bool esCeldaAccesible(int x, int y) const;

private:
    int ancho, alto;
    int tamañoCeldaAncho, tamañoCeldaAlto; // Tamaño de las celdas
    std::vector<std::vector<bool>> mapa;
    std::vector<std::vector<int>> matrizAdyacencia;

    SDL_Color colorFondo = { 220, 220, 220, 255 }; // Color de fondo
    SDL_Color colorObstaculo = { 139, 69, 19, 255 }; // Color de obstáculos
    SDL_Color colorLibre = { 255, 215, 0, 255 }; // Color para celdas libres

    bool dfs(int x, int y, std::vector<std::vector<bool>>& visitado);
    bool esAccesible();
};

#endif // GRAFO_H
