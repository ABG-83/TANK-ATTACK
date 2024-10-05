#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "Grafo.h"

class Game {
public:
    Game(SDL_Renderer* renderer);
    ~Game();

    void manejarEventos(SDL_Event* evento);
    void actualizar();
    void renderizar();
    bool salirJuego() const { return salir; }
    void iniciar();

private:
    SDL_Renderer* renderer;
    Grafo* grafo;  // Objeto grafo para gestionar el mapa
    bool salir;

    // cosas para el tamaño
    int ancho;
    int alto;

};

#endif
