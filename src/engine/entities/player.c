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
    joueur->attack = (t_attack){.isActive = SDL_FALSE, .progress = 0.0f, .hitBox = {{0, 0}, 0.0f, 0.0f, 0.0f}, .nbHits = 0, .timeSlowFactor = 1.0f, .timeSlowDuration = 0.0f, .timeSlowRemaining = 0.0f};

    addAnimation(joueur->entity.animationController, createAnimation(tileset, (int[]){1, 2}, 2, 240, SDL_TRUE, "idle"));
    addAnimation(joueur->entity.animationController, createAnimation(tileset, (int[]){3, 4}, 2, 240, SDL_TRUE, "walk"));
    setAnimation(joueur->entity.animationController, "idle");

    return joueur;
}

void renderPlayer(SDL_Renderer* renderer, t_joueur* player, t_camera* camera) {
    if (player->attack.nbHits > 0) {
        // Calculer l'intensité du shake basée sur l'arme et les dégâts
        float impact_force = player->currentWeapon->damage * player->currentWeapon->mass / 10.0f;

        // Shake non-linéaire (exponential pour les gros impacts)
        float shake_intensity = 0.3f * powf(impact_force, 1.2f);

        // Facteur de distance (diminue si l'ennemi est touché loin du joueur)
        float distance_factor = 1.0f;
        if (player->attack.hit_distance > 0) {
            distance_factor = fmaxf(0.5f, 1.0f - (player->attack.hit_distance / (player->currentWeapon->range * 1.5f)));
        }

        // Durée du shake en fonction de la masse (armes lourdes = plus long)
        float shake_duration = 0.3f + (player->currentWeapon->mass * 0.05f);

        // Application du shake avec intensité modulée
        cameraAddShake(camera, shake_intensity * distance_factor, shake_duration);
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
    SDL_RendererFlip weaponFlip = SDL_FLIP_HORIZONTAL;

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
    float originalDeltaTime = *deltaTime;

    // Gestion du ralentissement temporel
    if (player->attack.timeSlowRemaining > 0) {
        player->attack.timeSlowRemaining -= originalDeltaTime;

        // Calculer le facteur de ralentissement actuel (transition douce)
        float progress = player->attack.timeSlowRemaining / player->attack.timeSlowDuration;
        float smoothProgress = smoothStepf(progress);
        float currentSlowFactor = 1.0f - (1.0f - player->attack.timeSlowFactor) * smoothProgress;

        // Appliquer le ralentissement au deltaTime
        *deltaTime *= currentSlowFactor;
    }

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

    // Variables pour stocker la dernière direction d'entrée
    float lastDirX = 0.0f;
    float lastDirY = 0.0f;
    float currentDirX = 0.0f;
    float currentDirY = 0.0f;

    // Détection des mouvements directionnels
    if (input->key[player->control->left]) {
        player->entity.physics.velocity.x -= force * *deltaTime;
        player->entity.flip = SDL_FLIP_HORIZONTAL;
        currentDirX = -1.0f;
    }
    if (input->key[player->control->right]) {
        player->entity.physics.velocity.x += force * *deltaTime;
        player->entity.flip = SDL_FLIP_NONE;
        currentDirX = 1.0f;
    }
    if (input->key[player->control->up]) {
        player->entity.physics.velocity.y -= force * *deltaTime;
        currentDirY = -1.0f;
    }
    if (input->key[player->control->down]) {
        player->entity.physics.velocity.y += force * *deltaTime;
        currentDirY = 1.0f;
    }

    // Mettre à jour la dernière direction si le joueur se déplace
    if (currentDirX != 0.0f || currentDirY != 0.0f) {
        lastDirX = currentDirX;
        lastDirY = currentDirY;

        // Normaliser la direction pour les mouvements diagonaux
        float length = sqrtf(lastDirX * lastDirX + lastDirY * lastDirY);
        if (length > 0.0f) {
            lastDirX /= length;
            lastDirY /= length;
        }
    }

    // Changement d'arme
    if (keyPressOnce(input, SDL_SCANCODE_T)) {
        switchToNextWeapon(player);
    }

    // Gestion du dash et de l'attaque
    if (input->mouseButtons[SDL_BUTTON_LEFT] || input->key[player->control->dash]) {
        // Pour le dash : utiliser la dernière direction de déplacement
        if (input->key[player->control->dash]) {
            // Vérifier si le joueur a une direction enregistrée
            if (lastDirX != 0.0f || lastDirY != 0.0f) {
                // Appliquer le dash dans la direction mémorisée
                player->entity.physics.velocity.x += lastDirX * force_dash * *deltaTime;
                player->entity.physics.velocity.y += lastDirY * force_dash * *deltaTime;
            }
        }

        // Pour l'attaque : conserver le comportement basé sur la souris
        if (input->mouseButtons[SDL_BUTTON_LEFT] && player->attack.cooldown <= 0) {
            float mouseWorldX = 0.0f, mouseWorldY = 0.0f;
            convertMouseToWorld(vp, input->mouseX, input->mouseY, &mouseWorldX, &mouseWorldY);
            float dx = mouseWorldX - player->entity.collisionCircle.x;
            float dy = mouseWorldY - player->entity.collisionCircle.y;

            player->aimAngle = atan2f(dy, dx);
            player->entity.physics.velocity.x *= 0.2f;
            player->entity.physics.velocity.y *= 0.2f;
            start_attack(player);
        }
    }

    // Animation selon la vitesse
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

    int previousHits = player->attack.nbHits;

    for (int i = 1; i < entities->count; i++) {
        t_entity* enemy = getObject(entities, i);

        if (((t_enemy*)enemy)->isDead) {
            continue;
        }

        SDL_FPoint position = {enemy->collisionCircle.x, enemy->collisionCircle.y};

        if (is_in_attack_sector(position, enemy->collisionCircle.radius, player->attack.hitBox.origin, current_angle, player->currentWeapon->range, player->currentWeapon->angleAttack)) {
            takeDamageAndCheckDeath((t_enemy*)enemy, player->currentWeapon->damage);
            float dx = position.x - player->attack.hitBox.origin.x;
            float dy = position.y - player->attack.hitBox.origin.y;

            float length = sqrtf(dx * dx + dy * dy);
            if (length > 0) {
                dx /= length;
                dy /= length;

                float knockback_base = player->currentWeapon->mass * 750.0f;                    // Augmenté de 600 à 750 pour plus d'impact
                float mass_ratio = player->currentWeapon->mass / (enemy->physics.mass + 0.5f);  // Diminution du dénominateur pour amplifier l'effet

                // Courbe exponentielle pour des knockbacks plus impactants
                float power_curve = powf(mass_ratio, 0.7f);  // Exposant <1 pour atténuer les différences extrêmes
                float effective_knockback = knockback_base * (0.15f + 0.85f * power_curve);

                effective_knockback = fmaxf(effective_knockback, knockback_base * 0.12f);
                effective_knockback = fminf(effective_knockback, knockback_base * 4.5f);

                // *** MODIFICATION: Tenir compte du deltaTime pour le knockback ***
                // Au lieu d'appliquer une impulsion instantanée, on applique une force continue
                // divisée par deltaTime pour maintenir une force constante quelle que soit la fréquence
                float knockback_impulse = effective_knockback * (*deltaTime);

                enemy->physics.velocity.x += dx * knockback_impulse;
                enemy->physics.velocity.y += dy * knockback_impulse;
            }
            player->attack.nbHits++;
        }
    }

    if (player->attack.nbHits > previousHits) {
        // Armes lourdes = ralentissement plus intense mais plus court
        float weight_factor = player->currentWeapon->mass / 5.0f;  // Normalisation (5.0f est une masse moyenne)

        // Facteur: armes lourdes ralentissent plus (jusqu'à 0.4x pour les plus lourdes)
        player->attack.timeSlowFactor = fmaxf(0.4f, 0.75f - (weight_factor * 0.1f));

        // Durée: armes légères = ralentissement plus long
        player->attack.timeSlowDuration = 0.15f + (0.3f / weight_factor);

        // Limites pour la stabilité
        player->attack.timeSlowDuration = fminf(player->attack.timeSlowDuration, 0.35f);

        player->attack.timeSlowRemaining = player->attack.timeSlowDuration;
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

void addWeaponToPlayer(t_joueur* player, t_arme* weapon) {
    if (!player || !weapon || player->weaponCount >= 10) return;

    // Ajouter l'arme au tableau
    player->weapons[player->weaponCount] = weapon;
    player->weaponCount++;

    // Si c'est la première arme, l'équiper automatiquement
    if (player->weaponCount == 1) {
        player->currentWeaponIndex = 0;
        player->currentWeapon = weapon;
    }
}

void switchToNextWeapon(t_joueur* player) {
    if (!player || player->weaponCount <= 1) return;

    // Passer à l'arme suivante (avec retour au début si nécessaire)
    player->currentWeaponIndex = (player->currentWeaponIndex + 1) % player->weaponCount;
    player->currentWeapon = player->weapons[player->currentWeaponIndex];

    // Effet visuel et sonore de changement d'arme (optionnel)
    // playSound("weapon_switch");

    // Reset du cooldown d'attaque pour éviter de spammer les attaques
    player->attack.cooldown = 0.2f;
}