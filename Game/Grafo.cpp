#include "Grafo.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
Grafo::Grafo(int ancho, int alto, int tamañoCeldaAncho, int tamañoCeldaAlto)
    : ancho(ancho), alto(alto), tamañoCeldaAncho(tamañoCeldaAncho), tamañoCeldaAlto(tamañoCeldaAlto) {
    // Inicializar la cuadrícula y la matriz de adyacencia
    mapa.resize(ancho, std::vector<bool>(alto, true)); // Todas las celdas inicialmente accesibles
    matrizAdyacencia.resize(ancho * alto, std::vector<int>(ancho * alto, 0));

    // Crear las conexiones entre nodos adyacentes en la matriz de adyacencia
    for (int x = 0; x < ancho; x++) {
        for (int y = 0; y < alto; y++) {
            int nodoActual = x * alto + y;

            if (x > 0) { // Conectar con el nodo de la izquierda
                int nodoIzq = (x - 1) * alto + y;
                matrizAdyacencia[nodoActual][nodoIzq] = 1;
                matrizAdyacencia[nodoIzq][nodoActual] = 1;
            }
            if (y > 0) { // Conectar con el nodo de arriba
                int nodoArriba = x * alto + (y - 1);
                matrizAdyacencia[nodoActual][nodoArriba] = 1;
                matrizAdyacencia[nodoArriba][nodoActual] = 1;
            }
        }
    }
}

void Grafo::generarObstaculos() {
    std::srand(std::time(nullptr)); // Semilla para la aleatoriedad
    for (int x = 0; x < ancho; x++) {
        for (int y = 0; y < alto; y++) {
            if (std::rand() % 5 == 0) { // Probabilidad de 1/5 de ser un obstáculo
                mapa[x][y] = false;
            } else {
                mapa[x][y] = true; // Ver que las celdas que no son obstaculos sean verdaderas
            }
        }
    }

    // Asegurar accesibilidad
    while (!esAccesible()) {
        // Regenerar obstáculos hasta que todos los puntos sean accesibles
        std::fill(mapa.begin(), mapa.end(), std::vector<bool>(alto, true)); // Reiniciar mapa
        generarObstaculos();
    }
}

bool Grafo::dfs(int x, int y, std::vector<std::vector<bool>>& visitado) {
    if (x < 0 || x >= ancho || y < 0 || y >= alto || !mapa[x][y] || visitado[x][y]) {
        return false;
    }

    visitado[x][y] = true;
    // Explorar en las 4 direcciones
    dfs(x + 1, y, visitado);
    dfs(x - 1, y, visitado);
    dfs(x, y + 1, visitado);
    dfs(x, y - 1, visitado);
    return true;
}

bool Grafo::esAccesible() {
    std::vector<std::vector<bool>> visitado(ancho, std::vector<bool>(alto, false));
    dfs(0, 0, visitado); // Comenzar desde la celda (0, 0)

    // Verificar que todas las celdas accesibles estén visitadas
    for (int x = 0; x < ancho; x++) {
        for (int y = 0; y < alto; y++) {
            if (mapa[x][y] && !visitado[x][y]) {
                return false; // Hay celdas no accesibles
            }
        }
    }
    return true;
}

void Grafo::dibujarGrafo(SDL_Renderer* renderer) {
    for (int x = 0; x < ancho; x++) {
        for (int y = 0; y < alto; y++) {
            SDL_Rect celda = { x * tamañoCeldaAncho, y * tamañoCeldaAlto, tamañoCeldaAncho, tamañoCeldaAlto };

            if (mapa[x][y]) {
                SDL_SetRenderDrawColor(renderer, 165, 123, 63, 255); // Celdas libres con color específico (el cafe)
            } else {
                SDL_SetRenderDrawColor(renderer, 196, 187, 172, 255); // Obstáculos con color específico (gris)
            }
            SDL_RenderFillRect(renderer, &celda); // Dibujar la celda
        }
    }
}
