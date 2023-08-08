//
// Created by Sebbie on 11/22/2022.
//

#ifndef PA3_GAMECOMMAND_H
#define PA3_GAMECOMMAND_H

#include "GameObject.h"
#include "Model.h"
#include "Point2D.h"
#include "View.h"

void DoMoveCommand(Model &model, int trainer_id, Point2D p1);

void DoMoveToCenterCommand(Model &model, int trainer_id, int center_id);

void DoMoveToGymCommand(Model &model, int trainer_id, int gym_id);

void DoStopCommand(Model &model, int trainer_id);

void DoBattleCommand(Model &model, int trainer_id, unsigned int battles);

void DoRecoverInCenterCommand(Model &model, int trainer_id, unsigned int
potions_needed);

void DoAdvanceCommand(Model &model, View &view);

void DoRunCommand(Model &model, View &view);

void NewObject(Model &model, char type, int id, int x, int y);

#endif //PA3_GAMECOMMAND_H
