#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "UI/Menu.h"

void iniciarJuego(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Cambiar el color del fondo a verde para simular el inicio del juego
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000); // Simulación de 3 segundos del inicio del juego
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        return -1; // Salir si SDL no se inicializa
    }

    if (TTF_Init() == -1) {
        std::cout << "Error al inicializar TTF: " << TTF_GetError() << std::endl;
        return -1; // Salir si TTF no se inicializa
    }

    SDL_Window* window = SDL_CreateWindow("Tank Attack - Menu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Menu menu(renderer);
    bool corriendo = true;
    SDL_Event evento;

    while (corriendo) {
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {
                corriendo = false;
            }

            menu.manejarEventos(&evento);

            // Si se ha clicado "Iniciar Juego", abrir la nueva ventana
            if (menu.iniciarJuegoSeleccionado()) {
                iniciarJuego(renderer); // Abre una nueva ventana simulada
            }

            // Si se ha clicado "Salir", salir del programa
            if (menu.salirSeleccionado()) {
                corriendo = false;
            }
        }

        // Limpiar la pantalla antes de renderizar
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fondo negro
        SDL_RenderClear(renderer);

        // Renderizar el menú
        menu.renderizar();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit(); // Finalizar TTF
    SDL_Quit();
    return 0;
}
