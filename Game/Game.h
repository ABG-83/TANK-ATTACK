#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include "Grafo.h"
#include <SDL2/SDL_image.h> // Para cargar imágenes

// Estructura para manejar los tanques
struct Tanque {
    int x, y;
    int ancho, alto;
    SDL_Texture* textura;

    Tanque(int x, int y, int ancho, int alto, SDL_Texture* textura)
        : x(x), y(y), ancho(ancho), alto(alto), textura(textura) {}
};

// Clase para manejar las balas disparadas
class Bala {
public:
    int x, y;
    int dx, dy; // Dirección del disparo
    SDL_Texture* textura;

    Bala(int x, int y, int dx, int dy, SDL_Texture* textura)
        : x(x), y(y), dx(dx), dy(dy), textura(textura) {}

    void mover() {
        x += dx;
        y += dy;
    }

    void renderizar(SDL_Renderer* renderer, int ancho, int alto) const {
        SDL_Rect dstRect = { x * ancho, y * alto, ancho, alto };
        SDL_RenderCopy(renderer, textura, nullptr, &dstRect);
    }
};

class Game {
public:
    Game(SDL_Renderer* renderer);
    ~Game();

    void manejarEventos(SDL_Event* evento);
    void actualizar();
    void renderizar();
    void iniciar();
    void determinarGanador();

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
    Tanque* tanqueSeleccionado;  // Para seleccionar un tanque

    // Variables para el sistema de disparo
    std::vector<Bala> balas;     // Almacena las balas disparadas
    SDL_Texture* balaTextura;    // Textura de la bala

    // Variables para el sistema de turnos
    int turnoActual;             // 0 para el jugador 1, 1 para el jugador 2
    Uint32 tiempoInicio;         // Momento en que comenzó el juego
    int tiempoRestante;          // Tiempo restante en segundos (máximo 300 segundos que son los 5min)

    void inicializarTanques();
    void alternarTurno();        // Cambiar entre los turnos de los jugadores
    void renderizarUI();         // Renderizar la interfaz de usuario

};

#endif // GAME_H
