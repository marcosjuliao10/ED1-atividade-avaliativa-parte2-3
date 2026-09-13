#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"

#define largura_janela 800
#define altura_janela 600

typedef enum {
    moeda_bronze,
    moeda_prata,
    moeda_ouro,
    moeda_diamante
} tipo_moeda;

typedef struct {
    Vector2 pos;
    float raio;
    tipo_moeda tipo;
    int valor;
    bool coletada;
} moeda;

int valor_moeda(tipo_moeda tipo) {
    switch (tipo) {
        case moeda_bronze: return 5;
        case moeda_prata: return 10;
        case moeda_ouro: return 25;
        case moeda_diamante: return 50;
        default: return 0;
    }
}

Color cor_moeda(tipo_moeda tipo) {
    switch (tipo) {
        case moeda_bronze: return (Color){160, 90, 40, 255};
        case moeda_prata: return (Color){190, 190, 190, 255};
        case moeda_ouro: return GOLD;
        case moeda_diamante: return SKYBLUE;
        default: return WHITE;
    }
}

moeda *criar_moedas(int quantidade) {
    moeda *moedas = malloc(quantidade * sizeof(moeda));

    for (int i = 0; i < quantidade; i++) {
        moeda *m = moedas + i;

        m->pos = (Vector2){
            GetRandomValue(30, largura_janela - 30),
            GetRandomValue(30, altura_janela - 30)
        };

        m->raio = 10;

        int sorteio = GetRandomValue(0, 9);

        if (sorteio == 0)
            m->tipo = moeda_diamante;
        else
            m->tipo = GetRandomValue(moeda_bronze, moeda_ouro);

        m->valor = valor_moeda(m->tipo);
        m->coletada = false;
    }

    return moedas;
}

bool tentar_coletar(moeda *m, Vector2 jogador, float raio_jogador) {
    if (m->coletada) return false;

    float dx = m->pos.x - jogador.x;
    float dy = m->pos.y - jogador.y;
    float distancia = dx * dx + dy * dy;
    float soma = (m->raio + raio_jogador) * (m->raio + raio_jogador);

    if (distancia <= soma) {
        m->coletada = true;
        return true;
    }

    return false;
}

int main() {
    InitWindow(largura_janela, altura_janela, "atividade 2 - exercicio 2");
    SetTargetFPS(60);

    int quantidade = 15;
    moeda *moedas = criar_moedas(quantidade);

    Vector2 jogador = {400, 300};
    float raio_jogador = 15;
    int pontos = 0;

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_RIGHT)) jogador.x += 4;
        if (IsKeyDown(KEY_LEFT)) jogador.x -= 4;
        if (IsKeyDown(KEY_UP)) jogador.y -= 4;
        if (IsKeyDown(KEY_DOWN)) jogador.y += 4;

        for (int i = 0; i < quantidade; i++) {
            if (tentar_coletar(moedas + i, jogador, raio_jogador))
                pontos += moedas[i].valor;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < quantidade; i++) {
            if (!moedas[i].coletada)
                DrawCircleV(moedas[i].pos, moedas[i].raio,
                            cor_moeda(moedas[i].tipo));
        }

        DrawCircleV(jogador, raio_jogador, BLUE);
        DrawText(TextFormat("pontos: %d", pontos), 10, 10, 20, BLACK);

        EndDrawing();
    }

    free(moedas);
    CloseWindow();
    return 0;
}
