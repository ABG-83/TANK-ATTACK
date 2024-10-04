// Menu.h

#ifndef MENU_H
#define MENU_H

#include <SDL_ttf.h>
#include <SDL2/SDL.h>
#include <vector>
#include <string>

// Clase Opcion para representar las diferentes opciones del menú (botones)
class Opcion {
public:
    std::string texto; // El texto de la opción
    SDL_Rect rect;     // Área del botón
    SDL_Color color;   // Color del botón

    Opcion(const std::string &texto, SDL_Rect rect, SDL_Color color)
        : texto(texto), rect(rect), color(color) {}
};

// Clase Menu para gestionar el menú principal
class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();

    bool iniciarJuegoSeleccionado() const { return iniciarJuego; }
    bool salirSeleccionado() const { return salir; }

    void manejarEventos(SDL_Event* evento);
    void renderizar();

private:
    SDL_Renderer* renderer;
    std::vector<Opcion> opciones; // Vector con las opciones del menú (botones)
    TTF_Font* fuente;
    bool iniciarJuego;
    bool salir;

    void agregarOpcion(const std::string& texto, int x, int y);
    void renderizarTexto(const std::string& texto, SDL_Rect& rect, SDL_Color color);
};

#endif
