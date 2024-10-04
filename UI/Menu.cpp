#include "Menu.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

Menu::Menu(SDL_Renderer* renderer)
    : renderer(renderer), iniciarJuego(false), salir(false) {

    // Iniciar el subsistema de fuentes TTF
    if (TTF_Init() == -1) {
        std::cout << "Error al inicializar TTF: " << TTF_GetError() << std::endl;
        return; // Salir si TTF no se inicializa
    }

    // Cargar una fuente (la ruta es de la fuente que trae ubuntu)
    fuente = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
    if (fuente == nullptr) {
        std::cout << "Error al cargar la fuente: " << TTF_GetError() << std::endl;
        return; // Salir si la fuente no se carga
    }

    // Definir colores para las opciones (botones)
    SDL_Color azul = {0, 0, 255, 255}; // Azul para los botones

    // Añadir los botones al menú
    agregarOpcion("Iniciar Juego", 300, 200); // Botón de "Iniciar Juego"
    agregarOpcion("Salir", 300, 300);         // Botón de "Salir"
}

Menu::~Menu() {
    // Limpiar la fuente
    if (fuente != nullptr) {
        TTF_CloseFont(fuente);
    }
    TTF_Quit(); // Asegúrate de finalizar TTF
}

void Menu::agregarOpcion(const std::string& texto, int x, int y) {
    SDL_Rect rect = {x, y, 200, 50}; // Tamaño y posición del botón
    SDL_Color color = {0, 0, 255, 255}; // Azul para el fondo del botón
    opciones.push_back(Opcion(texto, rect, color));
}

void Menu::renderizarTexto(const std::string& texto, SDL_Rect& rect, SDL_Color color) {
    if (fuente == nullptr) {
        std::cout << "Fuente no cargada. No se puede renderizar texto." << std::endl;
        return; // Evitar continuar si la fuente es nula
    }

    // Crear una superficie de texto
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fuente, texto.c_str(), color);
    if (surfaceMessage == nullptr) {
        std::cout << "Error al crear la superficie del texto: " << TTF_GetError() << std::endl;
        return;
    }

    // Convertir la superficie a una textura
    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage); // Liberar la superficie una vez convertida a textura

    // Renderizar el texto en el centro del rectángulo del botón
    int text_width = surfaceMessage->w;
    int text_height = surfaceMessage->h;
    SDL_Rect textRect = { rect.x + (rect.w - text_width) / 2, rect.y + (rect.h - text_height) / 2, text_width, text_height };

    SDL_RenderCopy(renderer, message, nullptr, &textRect); // Renderizar el texto
    SDL_DestroyTexture(message); // Limpiar la textura
}

void Menu::manejarEventos(SDL_Event* evento) {
    if (evento->type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        // Detectar si el clic fue sobre algún botón
        for (auto& opcion : opciones) {
            if (x >= opcion.rect.x && x <= (opcion.rect.x + opcion.rect.w) &&
                y >= opcion.rect.y && y <= (opcion.rect.y + opcion.rect.h)) {

                if (opcion.texto == "Iniciar Juego") {
                    iniciarJuego = true;
                    std::cout << "Iniciando juego..." << std::endl;
                } else if (opcion.texto == "Salir") {
                    salir = true;
                    std::cout << "Saliendo del juego..." << std::endl;
                }
            }
        }
    }
}

void Menu::renderizar() {
    // Dibujar botones
    for (auto& opcion : opciones) {
        SDL_SetRenderDrawColor(renderer, opcion.color.r, opcion.color.g, opcion.color.b, opcion.color.a);
        SDL_RenderFillRect(renderer, &opcion.rect); // Dibujar el botón

        // Renderizar texto dentro del botón
        SDL_Color blanco = {255, 255, 255, 255}; // Texto blanco
        renderizarTexto(opcion.texto, opcion.rect, blanco);
    }
}
