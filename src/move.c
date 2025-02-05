#include "./move.h"

SDL_bool checkCollision(SDL_Rect* rect1, SDL_Rect* rect2) {
    return (SDL_bool)((rect1->x < rect2->x + rect2->w &&
                       rect1->x + rect1->w > rect2->x &&
                       rect1->y < rect2->y + rect2->h &&
                       rect1->y + rect1->h > rect2->y));
}

void movePlayer(t_joueur* player, int dx, int dy, t_obstacle obstacles[], int num_obstacles) {
    SDL_Rect temp = player->entity.rect;
    temp.x += dx;
    temp.y += dy;

    for (int i = 0; i < num_obstacles; i++) {
        if (checkCollision(&temp, &obstacles[i].entity.rect)) {
            return;
        }
    }
    // Vérification des limites de la map
    if (temp.x < 0) temp.x = 0;
    if (temp.y < 0) temp.y = 0;
    if (temp.x + temp.w > WORLD_WIDTH) temp.x = WORLD_WIDTH - temp.w;
    if (temp.y + temp.h > WORLD_HEIGHT) temp.y = WORLD_HEIGHT - temp.h;

    player->entity.rect.x = temp.x;
    player->entity.rect.y = temp.y;
}

void handleInputPlayer(t_joueur* player, t_obstacle obstacles[], int num_obstacles, t_camera* camera, t_input* input) {
    float speed = 5;
    float dx = 0;
    float dy = 0;

    if (input->key[player->control->left]) dx = -speed;
    if (input->key[player->control->right]) dx = speed;
    if (input->key[player->control->up]) dy = -speed;
    if (input->key[player->control->down]) dy = speed;

    if (dx && dy) {
        dx *= (1.0 / sqrt(2));
        dy *= (1.0 / sqrt(2));
    }

    movePlayer(player, dx, dy, obstacles, num_obstacles);
    cameraCenterOnRect(camera, &player->entity.rect);
}

// void renderRect(SDL_Renderer* renderer, SDL_Rect* rect, SDL_Color color) {
//     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
//     SDL_RenderFillRect(renderer, rect);
// }

// void renderPlayer(SDL_Renderer* renderer, t_joueur* object) {
//     SDL_RenderCopy(renderer, object->entity.texture, NULL, &object->entity.rect);
// }

t_joueur* createplayer(t_control* control, SDL_Texture* texture, SDL_Rect rect) {
    t_joueur* joueur = (t_joueur*)malloc(sizeof(t_joueur));

    joueur->control = control;
    joueur->entity.texture = texture;
    joueur->entity.rect = rect;

    return joueur;
}

void freePlayer(t_joueur* player) {
    SDL_DestroyTexture(player->entity.texture);
    free(player);
}

SDL_Texture* creerTextureDepuisRect(SDL_Renderer* renderer, SDL_Color couleur, SDL_Rect rect) {
    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_TARGET,
                                             rect.w, rect.h);

    SDL_Texture* ancienCible = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, couleur.r, couleur.g, couleur.b, couleur.a);
    SDL_RenderFillRect(renderer, &(SDL_Rect){0, 0, rect.w, rect.h});

    SDL_SetRenderTarget(renderer, ancienCible);

    return texture;
}

void cameraCenterOnRect(t_camera* camera, SDL_Rect* cible) {
    camera->x = cible->x + cible->w / 2 - camera->w / 2;
    camera->y = cible->y + cible->h / 2 - camera->h / 2;

    if (camera->x < 0) camera->x = 0;
    if (camera->y < 0) camera->y = 0;
    if (camera->x > camera->worldWidth - camera->w) camera->x = camera->worldWidth - camera->w;
    if (camera->y > camera->worldHeight - camera->h) camera->y = camera->worldHeight - camera->h;
}

SDL_Rect cameraTransformWorld2Camera(t_camera* camera, SDL_Rect* rectWordCoord) {
    return (SDL_Rect){rectWordCoord->x - camera->x, rectWordCoord->y - camera->y, rectWordCoord->w, rectWordCoord->h};
}

SDL_bool entityOnCamera(t_camera* camera, SDL_Rect* rectWordCoord) {
    return !(rectWordCoord->x + rectWordCoord->w <= camera->x ||  // Trop à gauche
             rectWordCoord->x >= camera->x + camera->w ||         // Trop à droite
             rectWordCoord->y + rectWordCoord->h <= camera->y ||  // Trop en haut
             rectWordCoord->y >= camera->y + camera->h);          // Trop en bas
}

void renderEntity(SDL_Renderer* renderer, t_camera* camera, t_entity* entity) {
    if (!entityOnCamera(camera, &entity->rect)) {
        return;
    }

    SDL_Rect screen_rect = cameraTransformWorld2Camera(camera, &entity->rect);
    if (entity->texture) {
        SDL_RenderCopy(renderer, entity->texture, NULL, &screen_rect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &screen_rect);
    }
}