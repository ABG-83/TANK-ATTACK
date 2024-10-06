#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include "Grafo.h"
#include <SDL2/SDL_image.h> // Para cargar imagenes

// Estructura para manejar los tanques
struct Tanque {
    int x, y;
    int ancho, alto;
    SDL_Texture* textura;

    Tanque(int x, int y, int ancho, int alto, SDL_Texture* textura)
        : x(x), y(y), ancho(ancho), alto(alto), textura(textura) {}
};


class Game {
public:
    Game(SDL_Renderer* renderer);
    ~Game();

    void manejarEventos(SDL_Event* evento);
    void actualizar();
    void renderizar();
    void iniciar();

private:
    SDL_Renderer* renderer;
    bool salir;

    Grafo* grafo;

    int ancho, alto; // Tamaño de las celdas

    // Variables relacionadas a los tanques
    SDL_Texture* tanqueRojo;
    SDL_Texture* tanqueAzul;
    SDL_Texture* tanqueAmarillo;
    SDL_Texture* tanqueCeleste;

    std::vector<Tanque> tanques; // Almacena los tanques
    Tanque* tanqueSeleccionado; // Para seleccionar un tanque

    void inicializarTanques();
};

#endif // GAME_H
