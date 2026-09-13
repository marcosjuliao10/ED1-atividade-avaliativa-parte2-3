#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"

#define largura_janela 800
#define altura_janela 600

typedef struct {
    float dano;
    int alcance;
} dados_arma;

typedef struct {
    int cura;
} dados_pocao;

typedef struct {
    int absorcao;
} dados_escudo;

typedef union {
    dados_arma arma;
    dados_pocao pocao;
    dados_escudo escudo;
} dados_item;

typedef enum {
    item_arma,
    item_pocao,
    item_escudo
} tipo_item;

typedef struct {
    Vector2 pos;
    float raio;
    tipo_item tipo;
    dados_item dados;
    bool coletado;
} item;

typedef struct {
    Vector2 pos;
    float raio;
    int vida;
    float dano;
    int armadura;
} jogador;

item *criar_itens(int quantidade) {
    item *itens = malloc(quantidade * sizeof(item));

    for (int i = 0; i < quantidade; i++) {
        item *it = itens + i;

        it->pos = (Vector2){
            GetRandomValue(30, largura_janela - 30),
            GetRandomValue(30, altura_janela - 30)
        };

        it->raio = 12;
        it->coletado = false;
        it->tipo = GetRandomValue(item_arma, item_escudo);

        if (it->tipo == item_arma) {
            it->dados.arma.dano = GetRandomValue(2, 8);
            it->dados.arma.alcance = GetRandomValue(1, 3);
        } else if (it->tipo == item_pocao) {
            it->dados.pocao.cura = GetRandomValue(10, 30);
        } else {
            it->dados.escudo.absorcao = GetRandomValue(5, 15);
        }
    }

    return itens;
}

void aplicar_item(jogador *j, item *it) {
    switch (it->tipo) {
        case item_arma:
            j->dano += it->dados.arma.dano;
            break;

        case item_pocao:
            j->vida += it->dados.pocao.cura;
            break;

        case item_escudo:
            j->armadura += it->dados.escudo.absorcao;
            break;
    }

    it->coletado = true;
}

int main() {
    InitWindow(largura_janela, altura_janela, "atividade 3 - exercicio 1");
    SetTargetFPS(60);

    int quantidade = 15;
    item *itens = criar_itens(quantidade);

    jogador j;
    j.pos = (Vector2){400, 300};
    j.raio = 15;
    j.vida = 100;
    j.dano = 10;
    j.armadura = 0;

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_RIGHT)) j.pos.x += 4;
        if (IsKeyDown(KEY_LEFT)) j.pos.x -= 4;
        if (IsKeyDown(KEY_UP)) j.pos.y -= 4;
        if (IsKeyDown(KEY_DOWN)) j.pos.y += 4;

        for (int i = 0; i < quantidade; i++) {
            item *it = itens + i;

            if (!it->coletado) {
                float dx = it->pos.x - j.pos.x;
                float dy = it->pos.y - j.pos.y;
                float distancia = dx * dx + dy * dy;
                float soma = (it->raio + j.raio) * (it->raio + j.raio);

                if (distancia <= soma) {
                    aplicar_item(&j, it);
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < quantidade; i++) {
            item *it = itens + i;

            if (!it->coletado) {
                Color cor = WHITE;

                if (it->tipo == item_arma) cor = RED;
                if (it->tipo == item_pocao) cor = GREEN;
                if (it->tipo == item_escudo) cor = BLUE;

                DrawCircleV(it->pos, it->raio, cor);
            }
        }

        DrawCircleV(j.pos, j.raio, BLACK);
        DrawText(TextFormat("vida: %d", j.vida), 10, 10, 20, BLACK);
        DrawText(TextFormat("dano: %.0f", j.dano), 10, 35, 20, BLACK);
        DrawText(TextFormat("armadura: %d", j.armadura), 10, 60, 20, BLACK);

        EndDrawing();
    }

    free(itens);
    CloseWindow();

    return 0;
}
