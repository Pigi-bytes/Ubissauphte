#include "player.h"

float lerpAngle(float a, float b, float t) {
    float diff = b - a;
    // Si la différence est proche de 2pi (ou -2pi), on effectue une interpolation linéaire sur 2pi
    if (fabsf(diff - 2.0f * M_PI) < EPSILON || fabsf(diff + 2.0f * M_PI) < EPSILON) {
        return a + 2.0f * M_PI * t;
    }
    float delta = fmodf(diff + M_PI, 2.0f * M_PI) - M_PI;
    return a + delta * t;
}

float smoothStepf(float step) {
    return step * step * (3.0f - 2.0f * step);
}

t_joueur* createPlayer(t_control* control, SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset) {
    t_joueur* joueur = (t_joueur*)malloc(sizeof(t_joueur));

    joueur->control = control;
    joueur->entity.texture = texture;
    joueur->entity.displayRect = rect;
    joueur->entity.flip = SDL_FLIP_NONE;
    joueur->entity.animationController = initAnimationController();
    joueur->entity.debug = SDL_FALSE;

    joueur->entity.useCircleCollision = SDL_TRUE;
    joueur->entity.collisionCircle.x = rect.x + rect.w / 2;
    joueur->entity.collisionCircle.y = rect.y + rect.h / 2;
    joueur->entity.collisionCircle.radius = fminf(rect.w, rect.h) / 2;

    joueur->entity.physics = (t_physics){.velocity = {0, 0}, .mass = 10.0f, .friction = 0.02f, .restitution = 0.02};

    joueur->currentWeapon = NULL;
    joueur->aimAngle = 0.0f;
    joueur->attack = (t_attack){.isActive = SDL_FALSE, .progress = 0.0f, .hitBox = {{0, 0}, 0.0f, 0.0f, 0.0f}, .nbHits = 0};

    addAnimation(joueur->entity.animationController, createAnimation(tileset, (int[]){1, 2}, 2, 240, SDL_TRUE, "idle"));
    addAnimation(joueur->entity.animationController, createAnimation(tileset, (int[]){3, 4}, 2, 240, SDL_TRUE, "walk"));
    setAnimation(joueur->entity.animationController, "idle");

    return joueur;
}

// void draw_slash(SDL_Renderer* renderer, SDL_FPoint center, float angle, float range, float arc_width, float line_width) {
//     const int segments = 50;                 // Nombre de segments pour l'arc (plus élevé = plus lisse)
//     const int point_size = (int)line_width;  // Taille des points (épaisseur du slash)

//     float start_angle = angle - arc_width / 2;  // Angle de départ du slash
//     float end_angle = angle + arc_width / 2;    // Angle de fin du slash

//     // Couleur jaune/blanc du slash
//     SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);

//     // Dessiner des gros points le long de l'arc
//     for (int i = 0; i <= segments; i++) {
//         float t = (float)i / segments;                                      // Progression le long de l'arc (0.0 à 1.0)
//         float current_angle = start_angle + t * (end_angle - start_angle);  // Angle actuel

//         // Point central de l'arc
//         SDL_FPoint arc_point = {
//             center.x + cosf(current_angle) * range,
//             center.y + sinf(current_angle) * range};

//         // Dessiner un gros point (carré de taille `point_size`)
//         for (int dx = -point_size / 2; dx <= point_size / 2; dx++) {
//             for (int dy = -point_size / 2; dy <= point_size / 2; dy++) {
//                 SDL_RenderDrawPoint(renderer, (int)(arc_point.x + dx), (int)(arc_point.y + dy));
//             }
//         }
//     }
// }

void renderPlayer(SDL_Renderer* renderer, t_joueur* player, t_camera* camera) {
    if (player->attack.nbHits != 0) {
        cameraAddShake(camera, 3.0f, 0.3f);
    }

    SDL_FPoint origin = {player->entity.collisionCircle.x, player->entity.collisionCircle.y};
    float range = player->currentWeapon->range;
    float angle = player->aimAngle;
    float arc = player->currentWeapon->angleAttack;

    // Afficher le cône d'attaque (debug)
    SDL_FPoint start = {origin.x + cosf(angle - arc / 2) * range, origin.y + sinf(angle - arc / 2) * range};
    SDL_FPoint end = {origin.x + cosf(angle + arc / 2) * range, origin.y + sinf(angle + arc / 2) * range};

    Debug_PushLine(origin.x, origin.y, start.x, start.y, 2, SDL_COLOR_CYAN);
    Debug_PushLine(origin.x, origin.y, end.x, end.y, 2, SDL_COLOR_CYAN);

    int segments = 20;
    float angle_step = arc / segments;
    SDL_FPoint prev_point = start;

    for (int i = 1; i <= segments; i++) {
        float current_angle = angle - arc / 2 + angle_step * i;
        SDL_FPoint current_point = {origin.x + cosf(current_angle) * range, origin.y + sinf(current_angle) * range};

        Debug_PushLine(prev_point.x, prev_point.y, current_point.x, current_point.y, 1, SDL_COLOR_CYAN);
        prev_point = current_point;
    }

    // Scale calculations optimisé pour atteindre le bord de la hitbox
    // Maintenant, l'échelle est calculée pour que l'épée atteigne exactement le bord de la hitbox
    float baseScale = 1.0f;
    float idealRangeScale = range / (player->currentWeapon->displayRect.h * 0.8f);  // 0.8 car le pivot est à 80% de la hauteur
    float minScale = 0.1f;
    float maxScale = 3.0f;
    float scaleY = fmaxf(minScale, fminf(maxScale, idealRangeScale));
    float scaleX = scaleY;  // Garde les proportions

    int scaledWidth = (int)(player->currentWeapon->displayRect.w);
    int scaledHeight = (int)(player->currentWeapon->displayRect.h * scaleY);

    // Point de pivot fixe sur l'arme
    float pivotRatioX = 0.5f;  // Milieu horizontal de l'arme
    float pivotRatioY = 0.8f;  // Près du bas de l'arme (position du manche)

    SDL_Point pivotPoint = {(int)(scaledWidth * pivotRatioX), (int)(scaledHeight * pivotRatioY)};

    // Position et rotation de l'arme selon l'état (attaque ou non)
    float rotationAngle;
    SDL_Rect displayRect;
    SDL_RendererFlip weaponFlip = SDL_FLIP_NONE;

    if (player->attack.isActive) {
        // PENDANT L'ATTAQUE: on déplace l'arme pour que le point de pivot coïncide avec le joueur

        // Calcul de l'angle actuel pour l'animation d'attaque
        float current_angle = lerpAngle(player->attack.hitBox.startAngle, player->attack.hitBox.endAngle, smoothStepf(player->attack.progress));

        // Position de l'arme: on place l'arme pour que son pivot coïncide avec le centre du joueur
        displayRect = (SDL_Rect){(int)(origin.x - pivotPoint.x), (int)(origin.y - pivotPoint.y), scaledWidth, scaledHeight};

        // Angle de rotation pour suivre exactement les lignes rouges
        rotationAngle = current_angle * 180.0f / M_PI + 90;

        // Arme rendue derrière le joueur
        SDL_RenderCopyEx(renderer, player->currentWeapon->texture, NULL, &displayRect, rotationAngle, &pivotPoint, weaponFlip);

        // Dessiner le point de pivot pour le debug
        SDL_FPoint visualPivot = {
            displayRect.x + pivotPoint.x,
            displayRect.y + pivotPoint.y};

        Debug_PushLine(visualPivot.x - 3, visualPivot.y, visualPivot.x + 3, visualPivot.y, 2, SDL_COLOR_RED);
        Debug_PushLine(visualPivot.x, visualPivot.y - 3, visualPivot.x, visualPivot.y + 3, 2, SDL_COLOR_RED);

        // Dessiner un point à l'extrémité de l'arme pour vérifier qu'elle atteint bien le bord de la hitbox
        // Calculer la position de la pointe de l'arme
        float weaponLength = scaledHeight * (1.0f - pivotRatioY);  // Distance du pivot à la pointe de l'arme
        SDL_FPoint weaponTip = {
            origin.x + cosf(current_angle) * weaponLength,
            origin.y + sinf(current_angle) * weaponLength};

        // Dessiner un point jaune à la pointe de l'arme
        Debug_PushLine(weaponTip.x - 3, weaponTip.y, weaponTip.x + 3, weaponTip.y, 3, SDL_COLOR_YELLOW);
        Debug_PushLine(weaponTip.x, weaponTip.y - 3, weaponTip.x, weaponTip.y + 3, 3, SDL_COLOR_YELLOW);

        // Joueur rendu par-dessus l'arme
        renderEntity(renderer, &player->entity, camera);
    } else {
        // SANS ATTAQUE: arme à côté du joueur avec un offset simple
        renderEntity(renderer, &player->entity, camera);

        // Positionnement simple de l'arme avec un offset (à droite ou à gauche selon orientation)
        float offsetX = 8.0f;
        float offsetY = 4.0f;

        // Ajustement pour l'orientation du joueur
        if (player->entity.flip == SDL_FLIP_HORIZONTAL) {
            offsetX = -offsetX;
            weaponFlip = SDL_FLIP_HORIZONTAL;
        }

        // Position de l'arme avec offset par rapport au joueur
        displayRect = (SDL_Rect){
            (int)(origin.x - pivotPoint.x + offsetX),
            (int)(origin.y - pivotPoint.y + offsetY),
            scaledWidth,
            scaledHeight};

        // Angle 0 pour l'arme au repos (orientée à 12h)
        rotationAngle = 0.0f;

        // Afficher l'arme
        SDL_RenderCopyEx(renderer,
                         player->currentWeapon->texture,
                         NULL,
                         &displayRect,
                         rotationAngle,
                         &pivotPoint,
                         weaponFlip);

        // Dessiner le point de pivot pour le debug
        SDL_FPoint visualPivot = {
            displayRect.x + pivotPoint.x,
            displayRect.y + pivotPoint.y};

        Debug_PushLine(visualPivot.x - 3, visualPivot.y, visualPivot.x + 3, visualPivot.y, 2, SDL_COLOR_RED);
        Debug_PushLine(visualPivot.x, visualPivot.y - 3, visualPivot.x, visualPivot.y + 3, 2, SDL_COLOR_RED);
    }
}

void updatePlayer(t_joueur* player, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    player->attack.nbHits = 0;

    if (player->attack.isActive) {
        update_attack(player, deltaTime, entities);
    }

    if (player->attack.cooldown > 0) {
        player->attack.cooldown -= *deltaTime;
    }

    updatePhysicEntity(&player->entity, deltaTime, grid, entities);
}

void handleInputPlayer(t_input* input, t_joueur* player, t_grid* grid, t_viewPort* vp, float* deltaTime) {
    float force = 400.0f;
    float force_dash = force * 3.5;

    if (!player->attack.isActive) {
        if (input->key[player->control->left]) {
            player->entity.physics.velocity.x -= force * *deltaTime;
            player->entity.flip = SDL_FLIP_HORIZONTAL;
        }
        if (input->key[player->control->right]) {
            player->entity.physics.velocity.x += force * *deltaTime;
            player->entity.flip = SDL_FLIP_NONE;
        }
        if (input->key[player->control->up]) player->entity.physics.velocity.y -= force * *deltaTime;
        if (input->key[player->control->down]) player->entity.physics.velocity.y += force * *deltaTime;

        if (input->mouseButtons[SDL_BUTTON_LEFT] || input->key[player->control->dash]) {
            float mouseWorldX = 0.0f, mouseWorldY = 0.0f;
            convertMouseToWorld(vp, input->mouseX, input->mouseY, &mouseWorldX, &mouseWorldY);
            float dx = mouseWorldX - player->entity.collisionCircle.x;
            float dy = mouseWorldY - player->entity.collisionCircle.y;

            if (input->key[player->control->dash]) {
                float magnitude = sqrt(dx * dx + dy * dy);
                if (magnitude > 0.0f) {
                    dx /= magnitude;
                    dy /= magnitude;
                }
                player->entity.physics.velocity.x += dx * force_dash * *deltaTime;
                player->entity.physics.velocity.y += dy * force_dash * *deltaTime;
            }
            if (input->mouseButtons[SDL_BUTTON_LEFT] && player->attack.cooldown <= 0) {
                player->aimAngle = atan2f(dy, dx);
                start_attack(player);
            }
        }
    }

    if (player->entity.physics.velocity.x != 0.0f || player->entity.physics.velocity.y != 0.0f) {
        setAnimation(player->entity.animationController, "walk");
    } else {
        setAnimation(player->entity.animationController, "idle");
    }
}

void start_attack(t_joueur* player) {
    if (!player || !player->currentWeapon) return;

    t_arme* weapon = player->currentWeapon;
    player->attack.isActive = SDL_TRUE;
    player->attack.progress = 0.0f;
    player->attack.nbHits = 0;

    player->attack.hitBox.origin = (SDL_FPoint){player->entity.collisionCircle.x, player->entity.collisionCircle.y},
    player->attack.hitBox.radius = weapon->range;

    player->attack.cooldown = player->currentWeapon->attackCooldown;

    float halfArc = weapon->angleAttack / 2.0f;
    player->attack.hitBox.startAngle = player->aimAngle - halfArc;
    player->attack.hitBox.endAngle = player->aimAngle + halfArc;
}

void update_attack(t_joueur* player, float* deltaTime, t_objectManager* entities) {
    if (!player->attack.isActive) return;

    player->attack.progress += *deltaTime / player->currentWeapon->attackDuration;
    player->attack.hitBox.origin.x = player->entity.collisionCircle.x;
    player->attack.hitBox.origin.y = player->entity.collisionCircle.y;

    float current_angle = lerpAngle(player->attack.hitBox.startAngle, player->attack.hitBox.endAngle, smoothStepf(player->attack.progress));

    // Calculer l'extrémité du slash pour le dessiner plus tard
    float slash_range = player->currentWeapon->range;
    float slash_width = M_PI / 12;

    float start_angle = current_angle - slash_width / 2;
    float end_angle = current_angle + slash_width / 2;
    SDL_FPoint edge1 = {player->attack.hitBox.origin.x + slash_range * cosf(start_angle), player->attack.hitBox.origin.y + slash_range * sinf(start_angle)};
    SDL_FPoint edge2 = {player->attack.hitBox.origin.x + slash_range * cosf(end_angle), player->attack.hitBox.origin.y + slash_range * sinf(end_angle)};

    Debug_PushLine(player->attack.hitBox.origin.x, player->attack.hitBox.origin.y, edge1.x, edge1.y, 2, SDL_COLOR_RED);
    Debug_PushLine(player->attack.hitBox.origin.x, player->attack.hitBox.origin.y, edge2.x, edge2.y, 2, SDL_COLOR_RED);

    // Base de la force de knockback provient de l'arme
    float base_knockback_force = player->currentWeapon->mass * 10.0f;  // Facteur de 15 pour une bonne sensation

    for (int i = 1; i < entities->count; i++) {
        t_entity* enemy = getObject(entities, i);
        SDL_FPoint position = {enemy->collisionCircle.x, enemy->collisionCircle.y};

        if (is_in_attack_sector(position, enemy->collisionCircle.radius, player->attack.hitBox.origin, current_angle, player->currentWeapon->range, player->currentWeapon->angleAttack)) {
            takeDamageAndCheckDeath((t_enemy*)enemy, player->currentWeapon->damage);
            float dx = position.x - player->attack.hitBox.origin.x;
            float dy = position.y - player->attack.hitBox.origin.y;

            float length = sqrtf(dx * dx + dy * dy);
            if (length > 0) {
                dx /= length;
                dy /= length;

                // Calcul du knockback en fonction du rapport des masses (F = ma)
                // Formule : Force = (masse de l'arme / masse de l'ennemi) * force de base
                // On ajoute une constante pour éviter une division par zéro et pour que les ennemis
                // très lourds ressentent quand même un peu de knockback

                float mass_ratio = player->currentWeapon->mass / (enemy->physics.mass + 1.0f);

                // On utilise une courbe logarithmique pour avoir un effet plus naturel
                // Les ennemis très légers sont fortement projetés, les lourds résistent mieux
                float effective_knockback = base_knockback_force * (0.2f + 0.8f * mass_ratio);

                // Application d'un facteur minimum pour garantir un effet visuel même sur les ennemis lourds
                effective_knockback = fmaxf(effective_knockback, base_knockback_force * 0.1f);

                // Limite supérieure pour éviter des effets trop extrêmes sur des ennemis très légers
                effective_knockback = fminf(effective_knockback, base_knockback_force * 3.0f);

                enemy->physics.velocity.x += dx * effective_knockback;
                enemy->physics.velocity.y += dy * effective_knockback;
            }
            player->attack.nbHits++;
        }
    }

    if (player->attack.progress >= 1.0f) {
        player->attack.isActive = SDL_FALSE;
    }
}

SDL_bool is_in_attack_sector(SDL_FPoint target, float target_radius, SDL_FPoint origin, float current_angle, float range, float arc) {
    // Calcul de la distance au carré pour éviter la racine carrée
    SDL_FPoint diff = {target.x - origin.x, target.y - origin.y};
    float dist_sq = diff.x * diff.x + diff.y * diff.y;

    float max_distance = range + target_radius;
    // Vérification si la cible est dans le rayon de l'attaque
    if (dist_sq > max_distance * max_distance) return SDL_FALSE;

    float distance = sqrtf(dist_sq);
    if (distance <= target_radius) {
        return SDL_TRUE;
    }

    // Calcul de l'angle de la cible par rapport à l'origine
    float target_angle = atan2f(diff.y, diff.x);

    // Normalisation de la différence d'angle
    float angle_diff = fmodf(target_angle - current_angle + M_PI, 2 * M_PI) - M_PI;

    float theta = asinf(target_radius / distance);
    float allowed_angle = (arc / 2) + theta;

    // Vérification si l'angle de la cible est dans l'arc d'attaque
    return fabsf(angle_diff) <= allowed_angle;
}

void freePlayer(void* object) {
    t_joueur* player = (t_joueur*)object;
    SDL_DestroyTexture(player->entity.texture);
    free(player);
}