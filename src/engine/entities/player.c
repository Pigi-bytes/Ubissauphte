#include "player.h"

#include "enemy.h"

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

    joueur->dash = (t_dash){
        .isActive = SDL_FALSE,
        .duration = 150,  // Durée du dash en millisecondes
        .speedMultiplier = 8.0f,
        .cooldownTimer = initTimer(),
        .cooldownTime = 5000,         // Cooldown de 2 secondes
        .trailSpawnInterval = 0.01f,  // Créer une traînée toutes les 20ms
        .timeSinceLastTrail = 0};

    for (int i = 0; i < DASH_TRAIL_COUNT; i++) {
        joueur->dash.trails[i].active = SDL_FALSE;
        joueur->dash.trails[i].opacity = 0.0f;
    }

    startTimer(joueur->dash.cooldownTimer);

    initHealthSystem(&joueur->health, 100);
    joueur->level = 1;
    joueur->xp = 0;
    joueur->xpToNextLevel = 100;
    joueur->gold = 0;

    addAnimation(joueur->entity.animationController, createAnimation(tileset, (int[]){1, 2}, 2, 240, SDL_TRUE, "idle"));
    addAnimation(joueur->entity.animationController, createAnimation(tileset, (int[]){3, 4}, 2, 240, SDL_TRUE, "walk"));
    setAnimation(joueur->entity.animationController, "idle");

    joueur->particleEmitter = createParticleEmitter();

    joueur->inventaire = createInventaire();
    joueur->baseStats.health.additive = 10;
    joueur->baseStats.health.multiplicative = 1;

    joueur->baseStats.healthMax.additive = 10;
    joueur->baseStats.healthMax.multiplicative = 1;

    joueur->baseStats.mana.additive = 10;
    joueur->baseStats.mana.multiplicative = 1;

    joueur->baseStats.manaMax.additive = 10;
    joueur->baseStats.manaMax.multiplicative = 1;

    joueur->baseStats.attack.additive = 10;
    joueur->baseStats.attack.multiplicative = 1;

    joueur->baseStats.defense.additive = 10;
    joueur->baseStats.defense.multiplicative = 1;

    joueur->baseStats.speed.additive = 10;
    joueur->baseStats.speed.multiplicative = 1;

    joueur->calculatedStats.health.additive = 0;
    joueur->calculatedStats.health.multiplicative = 1;

    joueur->calculatedStats.healthMax.additive = 0;
    joueur->calculatedStats.healthMax.multiplicative = 1;

    joueur->calculatedStats.mana.additive = 0;
    joueur->calculatedStats.mana.multiplicative = 1;

    joueur->calculatedStats.manaMax.additive = 0;
    joueur->calculatedStats.manaMax.multiplicative = 1;

    joueur->calculatedStats.attack.additive = 0;
    joueur->calculatedStats.attack.multiplicative = 1;

    joueur->calculatedStats.defense.additive = 0;
    joueur->calculatedStats.defense.multiplicative = 1;

    joueur->calculatedStats.speed.additive = 0;
    joueur->calculatedStats.speed.multiplicative = 1;

    joueur->level = 1;
    joueur->gold = 5000;
    joueur->xp = 0;

    joueur->equipement[SLOT_ARME].stack = NULL;
    joueur->equipement[SLOT_ARMURE].stack = NULL;
    joueur->equipement[SLOT_ACTIVABLE1].stack = NULL;
    joueur->equipement[SLOT_ACTIVABLE2].stack = NULL;

    return joueur;
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

void updateAttack(t_joueur* player, float* deltaTime, t_objectManager* entities, t_context* context) {
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
        if (getObjectTypeId(entities, i) != getTypeIdByName(entities->registry, "ENEMY")) continue;
        t_entity* enemy = getObject(entities, i);

        // Ignorer les entités nulles ou les ennemis déjà morts
        if (!enemy || ((t_enemy*)enemy)->health.isDead) continue;

        SDL_FPoint enemyPos = {enemy->collisionCircle.x, enemy->collisionCircle.y};

        // Vérification si l'ennemi est dans le secteur d'attaque actuel
        if (cercleInSector(enemyPos, enemy->collisionCircle.radius, player->attack.hitBox.origin, currentAngle, weapon->range, weapon->angleAttack)) {
            // Vecteur direction depuis l'origine de l'attaque vers l'ennemi
            float dx = enemyPos.x - player->attack.hitBox.origin.x;
            float dy = enemyPos.y - player->attack.hitBox.origin.y;

            // Normalisation du vecteur direction (pour obtenir un vecteur unitaire)
            float length = sqrtf(dx * dx + dy * dy);
            if (length > 0) {
                dx /= length;
                dy /= length;

                takeDamageFromPlayer((t_enemy*)enemy, weapon->damage, player, context, (SDL_FPoint){dx, dy});

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
    float forceBase = 400.0f;              // Force de déplacement standard
    float forceSprint = forceBase * 1.5f;  // Force quand on court (x1.5f)
    float forceDash = forceBase * 3.5f;    // Force du dash (x3.5f)

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

 

    if (keyPressOnce(input, SDL_SCANCODE_N)) {
        applyDamage(&player->health, 10, &player->entity, NULL);
    }

    if (input->key[player->control->escape]) {
        sceneController->lastMap = getObject(sceneController->scene, sceneController->currentScene);
        setScene(sceneController, "retourOption");
    }
    if (input->key[player->control->map]) {
        sceneController->lastMap = getObject(sceneController->scene, sceneController->currentScene);
        setScene(sceneController, "carte");
        input->key[player->control->map] = SDL_FALSE;
    }
    if (input->key[player->control->inventaire]) {
        sceneController->lastMap = getObject(sceneController->scene, sceneController->currentScene);
        setScene(sceneController, "mainInv");
        input->key[player->control->inventaire] = SDL_FALSE;
    }

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

    if (input->key[player->control->dash] && !player->dash.isActive && getTicks(player->dash.cooldownTimer) >= player->dash.cooldownTime) {
        player->dash.isActive = SDL_TRUE;
        startTimer(player->dash.cooldownTimer);

        for (int i = 0; i < DASH_TRAIL_COUNT; i++) {
            player->dash.trails[i].active = SDL_FALSE;
            player->dash.trails[i].opacity = 0.0f;
        }

        player->dash.timeSinceLastTrail = player->dash.trailSpawnInterval;  // Force la capture immédiate

        // Ajouter un petit boost initial pour que le dash démarre plus vite
        player->entity.physics.velocity.x += lastDirX * forceDash * 0.5f * (*deltaTime);
        player->entity.physics.velocity.y += lastDirY * forceDash * 0.5f * (*deltaTime);

        // Réduire temporairement la friction pour un dash fluide
        player->entity.physics.friction *= 0.5f;
    }

    if (player->dash.isActive) {
        // Accélération progressive au début du dash
        float dashProgress = 1.0f - (player->dash.duration / 150.0f);  // 0 au début, 1 à la fin
        float dashFactor = 0.8f + 0.4f * sinf(dashProgress * M_PI);    // Courbe en cloche (accélération puis décélération)

        player->entity.physics.velocity.x += lastDirX * player->dash.speedMultiplier * forceBase * dashFactor * (*deltaTime);
        player->entity.physics.velocity.y += lastDirY * player->dash.speedMultiplier * forceBase * dashFactor * (*deltaTime);

        player->dash.duration -= *deltaTime * 1000;
        if (player->dash.duration <= 0.0f) {
            player->dash.isActive = SDL_FALSE;
            player->dash.duration = 150;              // Réinitialiser la durée
            player->entity.physics.friction *= 2.0f;  // Restaurer la friction normale
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

void updatePlayer(t_joueur* player, float* deltaTime, t_salle* salle, t_objectManager* entities, t_context* context) {
    // On conserve le deltaTime original pour certains calculs qui ne doivent pas être affecté par le ralentissement
    float originalDeltaTime = *deltaTime;

    updateHealthSystem(&player->health, *deltaTime);

    // Gestion du ralentissement temporel (effet bullet-time apres une attaque reussie)
    updateTimeSlowEffect(player, &originalDeltaTime, deltaTime);

    player->attack.nbHits = 0;

    if (player->attack.isActive) {
        updateAttack(player, deltaTime, entities, context);
    }

    if (player->attack.cooldown > 0) {
        player->attack.cooldown -= *deltaTime;
    }

    if (player->dash.isActive) {
        player->dash.timeSinceLastTrail += *deltaTime;

        if (player->dash.timeSinceLastTrail >= player->dash.trailSpawnInterval) {
            for (int i = DASH_TRAIL_COUNT - 1; i > 0; i--) {
                player->dash.trails[i] = player->dash.trails[i - 1];
            }

            player->dash.trails[0].position.x = (int)player->entity.collisionCircle.x;
            player->dash.trails[0].position.y = (int)player->entity.collisionCircle.y;
            player->dash.trails[0].active = SDL_TRUE;
            player->dash.trails[0].opacity = 1.0f;

            player->dash.timeSinceLastTrail = 0;
        }
    } else {
        player->dash.timeSinceLastTrail += *deltaTime;

        if (player->dash.timeSinceLastTrail >= player->dash.trailSpawnInterval) {
            SDL_bool allTrailsInactive = SDL_TRUE;

            for (int i = 0; i < DASH_TRAIL_COUNT; i++) {
                if (player->dash.trails[i].active) {
                    player->dash.trails[i].opacity -= 0.1f;

                    if (player->dash.trails[i].opacity <= 0.0f) {
                        player->dash.trails[i].active = SDL_FALSE;
                    } else {
                        allTrailsInactive = SDL_FALSE;
                    }
                }
            }

            if (!allTrailsInactive) {
                player->dash.timeSinceLastTrail = 0;
            }
        }
    }

    SDL_FPoint velocity = player->entity.physics.velocity;
    float speedSq = velocity.x * velocity.x + velocity.y * velocity.y;

    if (speedSq > 2.0f) {
        static float stepTimer = 0;
        stepTimer += *deltaTime;

        if (stepTimer > 0.25f) {
            emitMovementParticles(player->particleEmitter, player->entity.collisionCircle.x, player->entity.collisionCircle.y + player->entity.collisionCircle.radius, SDL_COLOR_BLACK);
            stepTimer = 0;
        }
    }

    updateEntityParticles(player->particleEmitter, *deltaTime);
    updatePhysicEntity(&player->entity, deltaTime, salle->grille, entities);
}

// Fonction renderDashTrail optimisée
void renderDashTrail(SDL_Renderer* renderer, t_joueur* player, t_camera* camera) {
    // Vérification rapide s'il y a des traînées à afficher
    SDL_bool hasActiveTrail = SDL_FALSE;
    for (int i = 0; i < DASH_TRAIL_COUNT; i++) {
        if (player->dash.trails[i].active) {
            hasActiveTrail = SDL_TRUE;
            break;
        }
    }

    if (!hasActiveTrail) return;

    // Configuration du mode de fusion
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    uint8_t r, g, b, a;
    SDL_GetTextureColorMod(player->entity.texture, &r, &g, &b);
    SDL_GetTextureAlphaMod(player->entity.texture, &a);

    for (int i = 0; i < DASH_TRAIL_COUNT; i++) {
        if (!player->dash.trails[i].active) continue;

        float distanceFactor = (float)i / DASH_TRAIL_COUNT;
        float scaleFactor = 0.9f - (0.06f * i);
        uint8_t alpha = (uint8_t)(150 * player->dash.trails[i].opacity);

        SDL_SetTextureColorMod(player->entity.texture, (uint8_t)(180 * (1.0f - distanceFactor)), (uint8_t)(220 * (1.0f - distanceFactor * 0.3f)), 255);
        SDL_SetTextureAlphaMod(player->entity.texture, alpha);

        int width = player->entity.displayRect.w;
        int height = player->entity.displayRect.h;
        int ghostWidth = (int)(width * scaleFactor);
        int ghostHeight = (int)(height * scaleFactor);

        SDL_Rect destRect = {player->dash.trails[i].position.x - (ghostWidth / 2), player->dash.trails[i].position.y - (ghostHeight / 2), ghostWidth, ghostHeight};

        SDL_RenderCopyEx(renderer, player->entity.texture, NULL, &destRect, 0, NULL, player->entity.flip);
    }

    SDL_SetTextureColorMod(player->entity.texture, r, g, b);
    SDL_SetTextureAlphaMod(player->entity.texture, a);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void renderPlayer(SDL_Renderer* renderer, t_joueur* player, t_camera* camera) {
    // Applique un tremblement de caméra si le joueur vient de toucher un ennemi
    applyHitCameraShake(player, camera);

    renderDashTrail(renderer, player, camera);

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

    renderEntityParticles(renderer, player->particleEmitter);

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

void addPlayerXP(t_joueur* player, int xpAmount) {
    player->xp += xpAmount;
    checkAndProcessLevelUp(player);
    printf("Joueur: +%d XP (Total: %d/%d)\n", xpAmount, player->xp, player->xpToNextLevel);
}

float getPlayerXPProgress(t_joueur* player) {
    if (!player || player->xpToNextLevel <= 0) return 0.0f;

    float progress = (float)player->xp / (float)player->xpToNextLevel;

    return fmaxf(0.0f, fminf(progress, 1.0f));
}

SDL_bool checkAndProcessLevelUp(t_joueur* player) {
    if (player->xp < player->xpToNextLevel) return SDL_FALSE;

    player->level += 1;
    player->xp = player->xp - player->xpToNextLevel;

    player->xpToNextLevel = (int)(player->xpToNextLevel * 1.2f);

    printf("Player leveled up to level %d!\n", player->level);

    if (player->xp >= player->xpToNextLevel) {
        return checkAndProcessLevelUp(player);  // Récursion pour gérer plusieurs level up d'un coup
    }

    return SDL_TRUE;
}