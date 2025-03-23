#include "player.h"

t_joueur* createPlayer(t_control* control, SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset) {
    t_joueur* joueur = (t_joueur*)malloc(sizeof(t_joueur));

    joueur->control = control;
    joueur->entity.texture = texture;
    joueur->entity.displayRect = rect;
    joueur->entity.flip = SDL_FLIP_NONE;
    joueur->entity.animationController = initAnimationController();
    joueur->entity.debug = SDL_TRUE;

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

void renderWeaponRangeUI(SDL_Renderer* renderer, t_joueur* player) {
    if (!renderer || !player || !player->currentWeapon) return;

    SDL_FPoint origin = {player->entity.collisionCircle.x, player->entity.collisionCircle.y};
    float range = player->currentWeapon->range;
    float arc = player->currentWeapon->angleAttack;
    float angle = player->aimAngle;

    // Calcul du carre de la vitesse
    float velocitySq = player->entity.physics.velocity.x * player->entity.physics.velocity.x + player->entity.physics.velocity.y * player->entity.physics.velocity.y;

    // Calcul de l'opacité: plus le joueur va vite, plus l'indicateur est transparent
    // - Minimum: 35 (toujours légèrement visible)
    // - Maximum: 55 (quand le joueur est immobile)
    // Le facteur 8 ajuste la sensibilité au mouvement
    uint8_t maxOpacity = (uint8_t)fmaxf(35, 55 - sqrtf(velocitySq) * 8);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, maxOpacity);

    // Division de l'arc en segments pour un rendu lisse
    const int arcSegments = 10;               // Nombre de segments dans l'arc
    const float arcStep = arc / arcSegments;  // Taille angulaire de chaque segment
    float startAngle = angle - arc / 2;

    for (int i = 0; i < arcSegments; i++) {
        float a1 = startAngle + i * arcStep;
        float a2 = startAngle + (i + 1) * arcStep;

        // Conversion des coordonnées polaires en coordonnées cartésiennes
        // et tracé de la ligne entre les deux points
        SDL_RenderDrawLine(renderer, origin.x + cosf(a1) * range, origin.y + sinf(a1) * range, origin.x + cosf(a2) * range, origin.y + sinf(a2) * range);
    }

    // Ligne directrice (plus légère)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, maxOpacity / 2);
    float aimX = origin.x + cosf(angle) * range;
    float aimY = origin.y + sinf(angle) * range;
    SDL_RenderDrawLine(renderer, origin.x, origin.y, aimX, aimY);

    // Dessin d'un petit cercle plein au point ciblé
    // Rayon 1.8 pixels pour une taille visible mais discrète
    for (int w = 0; w < 1.8f * 2; w++) {
        for (int h = 0; h < 1.8f * 2; h++) {
            int dx = 1.8f - w;
            int dy = 1.8f - h;
            if ((dx * dx + dy * dy) <= (1.8f * 1.8f)) {
                SDL_RenderDrawPoint(renderer, aimX + dx, aimY + dy);
            }
        }
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void applyHitCameraShake(t_joueur* player, t_camera* camera) {
    if (player->attack.nbHits <= 0) return;

    // Force d'impact basée sur les caractéristiques de l'arme
    // Degats × Masse / 10.0f donne une valeur de base cohérente
    // Exemple: Arme (dégâts 20, masse 6) → impact_force = 12.0
    float impactForce = player->currentWeapon->damage * player->currentWeapon->mass / 5.0f;

    // Application d'une courbe de puissance pour accentuer les impacts forts
    // L'exposant 1.2 amplifie legerement les differences
    // 0.3f est un multiplicateur global pour ajuster l'intensite
    // Exemple: impactForce 12.0 → shakeIntensity ≈ 5.4
    float shakeIntensity = 0.3f * powf(impactForce, 1.2f);

    float distanceFactor = 1.0f;
    if (player->attack.hit_distance > 0) {
        // L'atténuation est linéaire avec la distance
        // Plus l'ennemi est loin, moins la caméra tremble
        // La portée × 1.5 définit la distance maximale d'effet
        // fmaxf garantit un minimum de 50% d'intensité même pour les impacts lointains
        distanceFactor = fmaxf(0.5f, 1.0f - (player->attack.hit_distance / (player->currentWeapon->range * 1.5f)));
    }

    // Multiplicateur basé sur le nombre d'ennemis touchés
    // Chaque ennemi supplémentaire ajoute 35% d'intensité
    float hitMultiplier = 1.0f + 0.35f * (player->attack.nbHits - 1);

    // Durée de base = 0.3 secondes
    // Les armes lourdes produisent un tremblement plus long
    // Chaque point de masse ajoute 0.05 secondes de durée
    // Exemple: Masse 6.0 → durée = 0.3 + 6×0.05 = 0.6 secondes
    float shakeDuration = 0.3f + (player->currentWeapon->mass * 0.05f);

    // Augmentation de la durée avec le nombre d'ennemis (plus modérée)
    float durationMultiplier = 1.0f + 0.15f * (player->attack.nbHits - 1);

    cameraAddShake(camera, shakeIntensity * distanceFactor * hitMultiplier, shakeDuration * durationMultiplier);
}

void renderAttackConeDebug(t_joueur* player) {
    if (!player->currentWeapon) return;

    SDL_FPoint origin = {player->entity.collisionCircle.x, player->entity.collisionCircle.y};
    float range = player->currentWeapon->range;
    float angle = player->aimAngle;
    float arc = player->currentWeapon->angleAttack;

    Debug_PushSector(origin.x, origin.y, angle - arc / 2, angle + arc / 2, range, 20, 2, SDL_COLOR_CYAN);

    SDL_FPoint directionPoint = {origin.x + cosf(angle) * range, origin.y + sinf(angle) * range};
    Debug_PushLine(origin.x, origin.y, directionPoint.x, directionPoint.y, 3, SDL_COLOR_MAGENTA);
}

float calculateWeaponScale(t_joueur* player) {
    // Calcul de l'échelle idéale basée sur la portée de l'arme
    // - La hauteur de l'arme est multipliée par 0.8 pour que la portée visuelle
    //   soit légèrement plus grande que la texture (meilleur retour visuel)
    // - Plus la portée est grande, plus l'arme apparaîtra grande à l'écran
    float idealRangeScale = player->currentWeapon->range / (player->currentWeapon->displayRect.h * 0.8f);

    // Limites pour éviter des échelles extrêmes qui nuiraient à l'expérience
    float minScale = 0.1f;
    float maxScale = 3.0f;

    // fmaxf et fminf assurent que le résultat est dans l'intervalle [minScale, maxScale]
    return fmaxf(minScale, fminf(maxScale, idealRangeScale));
}

void renderWeaponDuringAttack(SDL_Renderer* renderer, t_joueur* player, SDL_FPoint origin, SDL_Point pivotPoint, int scaledWidth, int scaledHeight, SDL_RendererFlip weaponFlip) {
    t_arme* weapon = player->currentWeapon;

    float rawProgress = player->attack.progress;
    float adjustedProgress;

    // Courbes de progression non-linéaires pour des animations plus naturelles
    // Les armes lourdes et légères ont des comportements d'animation différents
    if (weapon->mass >= 5.0f) {
        // Arme lourde: mouvement en 3 phases distinctes
        // 1. Préparation lente (wind-up): 0-25% de la durée
        // 2. Frappe rapide: 25-60% de la durée
        // 3. Récupération lente (wind-down): 60-100% de la durée
        if (rawProgress < 0.25f) {
            // Phase 1: Préparation lente (courbe exponentielle)
            // L'exposant 1.5 donne un démarrage encore plus lent
            adjustedProgress = 0.2f * powf(rawProgress / 0.25f, 1.5f);
        } else if (rawProgress < 0.6f) {
            // Phase 2: Frappe rapide (mouvement principal)
            // L'exposant 0.7 donne une accélération rapide puis ralentissement
            adjustedProgress = 0.2f + 0.6f * powf((rawProgress - 0.25f) / 0.35f, 0.7f);
        } else {
            // Phase 3: Récupération lente (linéaire)
            adjustedProgress = 0.8f + 0.2f * (rawProgress - 0.6f) / 0.4f;  // Récupération lente
        }
    } else {
        // Arme légère: mouvement en 2 phases plus simples
        // 1. Préparation rapide: 0-30% de la durée
        // 2. Frappe constante: 30-100% de la durée
        if (rawProgress < 0.3f) {
            // Phase 1: Préparation rapide (linéaire)
            adjustedProgress = 0.4f * (rawProgress / 0.3f);
        } else {
            // Phase 2: Frappe constante (légère courbe)
            // L'exposant 0.9 donne une vitesse presque constante
            adjustedProgress = 0.4f + 0.6f * powf((rawProgress - 0.3f) / 0.7f, 0.9f);
        }
    }

    // Interpolation entre l'angle de départ et de fin basée sur la progression ajustée
    float baseAngle = lerpAngle(player->attack.hitBox.startAngle, player->attack.hitBox.endAngle, adjustedProgress);

    // Ajout d'une légère oscillation (tremblement de l'arme)
    // 0.05f = amplitude de l'oscillation en radians (environ 3 degrés)
    float oscillation = sinf(rawProgress * M_PI * 2.0f) * 0.05f;

    // Angle final combinant l'interpolation et l'oscillation
    float currentAngle = baseAngle + oscillation;

    SDL_Rect displayRect = {(origin.x - pivotPoint.x), (origin.y - pivotPoint.y), scaledWidth, scaledHeight};

    if (rawProgress > 0.3f && rawProgress < 0.7f) {
        // Calcul de l'étirement avec une courbe sinus
        // Maximum d'étirement au milieu de la phase d'impact (50%)
        // 0.15f = 15% d'étirement maximum
        float stretch = sinf((rawProgress - 0.3f) / 0.4f * M_PI) * 0.15f;

        // Application de l'etirement à la hauteur de l'arme
        displayRect.h = (int)(scaledHeight * (1.0f + stretch));
    }

    // Conversion de l'angle en radians vers degrés pour SDL
    // Ajout de 90° car dans SDL, 0° = droite, alors que dans nos calculs, 0° = est

    float rotationDegrees = currentAngle * 180.0f / M_PI + 90;
    SDL_RenderCopyEx(renderer, weapon->texture, NULL, &displayRect, rotationDegrees, &pivotPoint, weaponFlip);
}

void renderWeaponIdle(SDL_Renderer* renderer, t_joueur* player, SDL_FPoint origin, SDL_Point pivotPoint, int scaledWidth, int scaledHeight, SDL_RendererFlip weaponFlip) {
    // Constantes de décalage pour positionner l'arme par rapport au joueur
    const float OFFSET_X = 8.0f;
    const float offsetY = 4.0f;

    // Si le joueur est retourné (regarde à gauche), on inverse le décalage
    float offsetX = player->entity.flip == SDL_FLIP_HORIZONTAL ? -OFFSET_X : OFFSET_X;
    // (assure que l'arme pointe dans la même direction que le regard du joueur)
    weaponFlip = player->entity.flip;

    SDL_Rect displayRect = {(origin.x - pivotPoint.x + offsetX), (origin.y - pivotPoint.y + offsetY), scaledWidth, scaledHeight};
    SDL_RenderCopyEx(renderer, player->currentWeapon->texture, NULL, &displayRect, 0, &pivotPoint, weaponFlip);

    SDL_FPoint visualPivot = {displayRect.x + pivotPoint.x, displayRect.y + pivotPoint.y};
    Debug_PushLine(visualPivot.x - 3, visualPivot.y, visualPivot.x + 3, visualPivot.y, 2, SDL_COLOR_RED);
    Debug_PushLine(visualPivot.x, visualPivot.y - 3, visualPivot.x, visualPivot.y + 3, 2, SDL_COLOR_RED);
}

void updateTimeSlowEffect(t_joueur* player, float* originalDeltaTime, float* modifiedDeltaTime) {
    if (player->attack.timeSlowRemaining <= 0) return;

    // Decrementation du temps de ralentissement restant (en utilisant le temps non ralenti)
    player->attack.timeSlowRemaining -= *originalDeltaTime;

    // Calculer le facteur de ralentissement actuel avec une transition douce
    // Le ratio progress représente ou nous en sommes dans la durée de l'effet (1.0 => 0.0)
    float progress = player->attack.timeSlowRemaining / player->attack.timeSlowDuration;

    // Application d'une courbe de lissage pour une transition plus naturelle
    // Cela transforme une progression linéaire en une courbe plus belle
    float smoothProgress = smoothStepf(progress);

    // Bonus de ralentissement basé sur le nombre d'ennemis touchés
    // Limité à 3 ennemis maximum pour éviter les abus
    int hitCount = fminf(player->attack.nbHits, 3);
    float multiHitBonus = 0.0f;

    if (hitCount > 1) {
        multiHitBonus = 0.1f * (hitCount - 1);
    }

    // Facteur de ralentissement amplifié par le nombre d'ennemis touchés
    float baseSlowFactor = player->attack.timeSlowFactor;
    float enhancedSlowFactor = fmaxf(0.2f, baseSlowFactor - multiHitBonus);

    // Interpolation entre facteur normal (1.0) et facteur ralenti
    // Exemple: si timeSlowFactor = 0.5, on passera progressivement de 0.5 à 1.0
    float currentSlowFactor = 1.0f - (1.0f - enhancedSlowFactor) * smoothProgress;

    // Application du ralentissement au deltaTime
    // Cela affecte tous les systèmes dépendant du temps qui utilisent cette valeur
    *modifiedDeltaTime *= currentSlowFactor;
}

void startAttack(t_joueur* player) {
    if (!player || !player->currentWeapon) return;

    t_arme* weapon = player->currentWeapon;

    player->attack.isActive = SDL_TRUE;
    player->attack.progress = 0.0f;
    player->attack.nbHits = 0;
    player->attack.cooldown = player->currentWeapon->attackCooldown;

    // Calcul de l'arc d'attaque
    // On divise l'angle total d'attaque par 2 pour obtenir le demi-angle
    float halfArc = weapon->angleAttack / 2.0f;

    // L'attaque commence a (angle visé - demi-arc) et se termine à (angle visé + demi-arc)
    // Cela crée un secteur centré sur l'angle de visée du joueur
    player->attack.hitBox.startAngle = player->aimAngle - halfArc;
    player->attack.hitBox.endAngle = player->aimAngle + halfArc;
    player->attack.hitBox.radius = weapon->range;
    player->attack.hitBox.origin = (SDL_FPoint){player->entity.collisionCircle.x, player->entity.collisionCircle.y};
}

void updateAttack(t_joueur* player, float* deltaTime, t_objectManager* entities) {
    if (!player->attack.isActive) return;

    t_arme* weapon = player->currentWeapon;

    // Mise a jour de la progression de l'attaque (0.0 à 1.0)
    // Vitesse de progression determiné par la durée d'attaque de l'arme
    player->attack.progress += *deltaTime / weapon->attackDuration;

    // L'origine de l'attaque suit la position du joueur (permet d'attaquer en mouvement)
    player->attack.hitBox.origin = (SDL_FPoint){player->entity.collisionCircle.x, player->entity.collisionCircle.y};

    // Calcul de l'angle actuel de l'attaque par interpolation
    // On utilise smoothStepf pour une progression plus naturelle (accélération/décélération)
    float currentAngle = lerpAngle(player->attack.hitBox.startAngle, player->attack.hitBox.endAngle, smoothStepf(player->attack.progress));

    int previousHits = player->attack.nbHits;

    for (int i = 1; i < entities->count; i++) {
        if (getObjectTypeId(entities, i) == getTypeIdByName(entities->registry, "TILE_ENTITY")) continue;
        t_entity* enemy = getObject(entities, i);

        // Ignorer les entités nulles ou les ennemis déjà morts
        if (!enemy || ((t_enemy*)enemy)->isDead) continue;

        SDL_FPoint enemyPos = {enemy->collisionCircle.x, enemy->collisionCircle.y};

        // Vérification si l'ennemi est dans le secteur d'attaque actuel
        if (cercleInSector(enemyPos, enemy->collisionCircle.radius, player->attack.hitBox.origin, currentAngle, weapon->range, weapon->angleAttack)) {
            takeDamageAndCheckDeath((t_enemy*)enemy, weapon->damage);

            // Vecteur direction depuis l'origine de l'attaque vers l'ennemi
            float dx = enemyPos.x - player->attack.hitBox.origin.x;
            float dy = enemyPos.y - player->attack.hitBox.origin.y;

            // Normalisation du vecteur direction (pour obtenir un vecteur unitaire)
            float length = sqrtf(dx * dx + dy * dy);
            if (length > 0) {
                dx /= length;
                dy /= length;

                // Force de base du recul : 750.0f = multiplicateur de base pour une sensation de jeu satisfaisante
                // Plus l'arme est lourde, plus le recul est puissant : Exemple: masse 2.0 → knockbackBase = 1500
                float knockbackBase = weapon->mass * 750.0f;

                // Rapport de masse: Arme / Ennemi
                // Plus ce ratio est eleve, plus l'ennemi sera projete loin
                // L'addition de 0.5f au dénominateur attenue les rapports extrêmes
                // Exemple: Arme de masse 3.0 contre ennemi de masse 1.0 → ratio = 2.0
                float ratioMass = weapon->mass / (enemy->physics.mass + 0.5f);

                // Application d'une courbe de puissance pour un effet plus réaliste
                // L'exposant 0.7 réduit les différences extreme entre armes légères et lourdes
                // Exemple: ratio 2.0 → powerCurve = 2.0^0.7 ≈ 1.62
                float powerCurve = powf(ratioMass, 0.7f);

                // Application de limites pour garantir un équilibre:
                // - Minimum: 12% de la force de base (même contre des ennemis très lourds)
                // - Maximum: 450% de la force de base (même contre des ennemis très légers)
                float knockbackEfficace = knockbackBase * (0.15f + 0.85f * powerCurve);
                knockbackEfficace = fmaxf(knockbackEfficace, knockbackBase * 0.12f);
                knockbackEfficace = fminf(knockbackEfficace, knockbackBase * 4.5f);

                // Application de la force avec prise en compte du deltaTime
                // Cela garantit une force constante indépendamment du framerate
                // Exemple: knockbackEfficace 2295 × deltaTime 0.016 ≈ 36.72 unités de force
                float forceKnockback = knockbackEfficace * (*deltaTime);
                enemy->physics.velocity.x += dx * forceKnockback;
                enemy->physics.velocity.y += dy * forceKnockback;
            }

            player->attack.nbHits++;
        }
    }

    if (player->attack.nbHits > previousHits) {
        // Normalisation du facteur de poids de l'arme
        // 5.0f représente une masse moyenne/référence
        // Exemple: Arme masse 8.0 → facteurMass = 1.6 (60% plus lourde que la moyenne)
        float facteurMass = weapon->mass / 5.0f;

        // Calcul du facteur de ralentissement temporel (entre 0.4 et 0.75)
        // - 0.4 = temps ralenti à 40% de sa vitesse normale (ralentissement intense)
        // - 0.75 = temps ralenti à 75% de sa vitesse normale (ralentissement léger)
        // Les armes lourdes ralentissent davantage le temps
        // Exemple: facteurMass 1.6 → timeSlowFactor = max(0.4, 0.75 - 1.6×0.1) = max(0.4, 0.59) = 0.59
        player->attack.timeSlowFactor = fmaxf(0.4f, 0.75f - (facteurMass * 0.1f));

        // Calcul de la durée du ralentissement (en secondes)
        // Les armes légères produisent un ralentissement plus long
        // Formule: 0.15s (base) + 0.3s/facteurMass (bonus inversement proportionnel)
        // Exemple: facteurMass 1.6 → durée = 0.15 + 0.3/1.6 ≈ 0.34 secondes
        player->attack.timeSlowDuration = fminf(0.15f + (0.3f / facteurMass), 0.35f);

        player->attack.timeSlowRemaining = player->attack.timeSlowDuration;
    }

    if (player->attack.progress >= 1.0f) {
        player->attack.isActive = SDL_FALSE;
    }
}

void handleInputPlayer(t_input* input, t_joueur* player, t_grid* grid, t_viewPort* vp, float* deltaTime, t_sceneController* sceneController) {
    float forceBase = 350.0f;              // Force de déplacement standard
    float forceSprint = forceBase * 1.5f;  // Force quand on court (x1.5)
    float forceDash = forceBase * 3.5f;    // Force du dash (x3.5)

    // Force utilisee (course ou marche normale)
    float force = input->key[SDL_SCANCODE_LSHIFT] ? forceSprint : forceBase;

    float lastDirX = 0.0f;     // Derniere direction horizontale
    float lastDirY = 0.0f;     // Derniere direction verticale
    float currentDirX = 0.0f;  // Direction horizontale actuelle
    float currentDirY = 0.0f;  // Direction verticale actuelle

    // gauche: -1 en X, 0  en Y
    // droite:  1 en X, 0  en Y
    // haut:    0 en X, -1 en Y
    // bas:     0 en X, 1  en Y
    int directions[4][3] = {{player->control->left, -1, 0}, {player->control->right, 1, 0}, {player->control->up, 0, -1}, {player->control->down, 0, 1}};

    for (int i = 0; i < 4; i++) {
        if (input->key[directions[i][0]]) {
            player->entity.physics.velocity.x += directions[i][1] * force * *deltaTime;
            player->entity.physics.velocity.y += directions[i][2] * force * *deltaTime;

            if (directions[i][1] != 0) currentDirX = (float)directions[i][1];
            if (directions[i][2] != 0) currentDirY = (float)directions[i][2];
        }
    }

    // Si le joueur se deplace, on memorise sa direction actuelle
    if (currentDirX != 0.0f || currentDirY != 0.0f) {
        lastDirX = currentDirX;
        lastDirY = currentDirY;

        // Normalisation du vecteur de direction pour les mouvements diagonaux
        // (evite que le deplacement en diagonale soit plus rapide)
        float length = sqrtf(lastDirX * lastDirX + lastDirY * lastDirY);
        if (length > 0.0f) {
            lastDirX /= length;
            lastDirY /= length;
        }
    }

    if (keyPressOnce(input, SDL_SCANCODE_T)) {
        switchToNextWeapon(player);
    }

    if (input->key[player->control->escape]) {
        setScene(sceneController, "menuPrincipal");
    }

    if (input->key[player->control->map]) {
        setScene(sceneController, "carte");
        input->key[player->control->map] = SDL_FALSE;
    }

    // Conversion des coord souris en coord monde
    float mouseWorldX = 0.0f, mouseWorldY = 0.0f;
    convertMouseToWorld(vp, input->mouseX, input->mouseY, &mouseWorldX, &mouseWorldY);

    // Calcul du vecteur joueur -> souris
    float dx = mouseWorldX - player->entity.collisionCircle.x;
    float dy = mouseWorldY - player->entity.collisionCircle.y;

    // Calcul de l'angle de visée (en radians)
    // atan2f nous donne l'angle du vecteur (dx,dy) par rapport a l'axe x
    player->aimAngle = atan2f(dy, dx);

    // Orientation du sprite en fonction de l'angle de visee
    // Si le joueur regarde vers la gauche (cosinus negatif), on retourne le sprite
    // cosf(angle) < 0 signifie que l'angle est entre pi/2 et 3pi/2 (donc côte gauche)
    if (cosf(player->aimAngle) < 0) {
        player->entity.flip = SDL_FLIP_HORIZONTAL;
    } else {
        player->entity.flip = SDL_FLIP_NONE;
    }

    // Dash
    if (input->key[player->control->dash]) {
        if (lastDirX != 0.0f || lastDirY != 0.0f) {
            player->entity.physics.velocity.x += lastDirX * forceDash * *deltaTime;
            player->entity.physics.velocity.y += lastDirY * forceDash * *deltaTime;
        }
    }

    // Attaque
    if (input->mouseButtons[SDL_BUTTON_LEFT] && player->attack.cooldown <= 0) {
        startAttack(player);
    }

    // Si le joueur est en mouvement, on utilise l'animation de marche, sinon, on utilise l'animation d'idle
    if (player->entity.physics.velocity.x >= EPSILON || player->entity.physics.velocity.y >= EPSILON) {
        setAnimation(player->entity.animationController, "walk");
    } else {
        setAnimation(player->entity.animationController, "idle");
    }
}

void updatePlayer(t_joueur* player, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    // On conserve le deltaTime original pour certains calculs qui ne doivent pas être affecté par le ralentissement
    float originalDeltaTime = *deltaTime;

    // Gestion du ralentissement temporel (effet bullet-time apres une attaque reussie)
    updateTimeSlowEffect(player, &originalDeltaTime, deltaTime);

    player->attack.nbHits = 0;

    if (player->attack.isActive) {
        updateAttack(player, deltaTime, entities);
    }

    if (player->attack.cooldown > 0) {
        player->attack.cooldown -= *deltaTime;
    }

    updatePhysicEntity(&player->entity, deltaTime, grid, entities);
}

void renderPlayer(SDL_Renderer* renderer, t_joueur* player, t_camera* camera) {
    // Applique un tremblement de caméra si le joueur vient de toucher un ennemi
    applyHitCameraShake(player, camera);

    // Affiche l'indicateur de portée de l'arme quand le joueur ne frappe pas
    if (!player->attack.isActive) {
        renderWeaponRangeUI(renderer, player);
    }

    renderAttackConeDebug(player);

    SDL_FPoint origin = {player->entity.collisionCircle.x, player->entity.collisionCircle.y};

    // Calcul de l'échelle adaptée à la portée de l'arme
    // Plus l'arme a une grande portée, plus elle apparaît grande à l'écran
    float scaleY = calculateWeaponScale(player);

    // Dimensions de l'arme après mise à l'échelle
    int scaledWidth = player->currentWeapon->displayRect.w;
    int scaledHeight = (player->currentWeapon->displayRect.h * scaleY);

    SDL_Point pivotPoint = {(scaledWidth * 0.5f), (scaledHeight * 0.8f)};

    if (player->attack.isActive) {
        renderWeaponDuringAttack(renderer, player, origin, pivotPoint, scaledWidth, scaledHeight, player->entity.flip);
        renderEntity(renderer, &player->entity, camera);
    } else {
        renderEntity(renderer, &player->entity, camera);
        renderWeaponIdle(renderer, player, origin, pivotPoint, scaledWidth, scaledHeight, player->entity.flip);
    }
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