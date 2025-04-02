#ifndef MAIN_WORLD_H
#define MAIN_WORLD_H
#include "../commun.h"
#include "../engine/entities/mobs/boss_slime.h"
#include "../engine/entities/tilesEntity.h"
#include "../ui/hud.h"

t_scene* createMainWord(t_context* context, t_salle* salle, t_joueur** player, t_grid** level);
t_scene* createMarchantMap(t_context* context, t_salle* salle, t_joueur** player, t_grid** level);
t_scene* createBossMap(t_context* context, t_salle* salle, t_joueur** player, t_grid** level);
t_scene* attente(t_context* context);

t_scene* createMapWord(t_context* context, t_salle** salle, SDL_Rect* rectcord, t_joueur* player, int nbSalle);
void CreateNiveau(t_context* context, int* nbSalle, t_joueur** joueur);
void CreateNiveauWrapper(t_fonctionParam* f);
#endif