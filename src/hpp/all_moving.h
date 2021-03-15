#pragma once

#include "basic.h"
#include "defensive.h"
#include "joker.h"

enum ATTACKERTYPE
{
    BASIC,
    DEFENSIVE,
    JOKER,
    ATTACKER_STOPPER ///<indicator for amount of attackers that exist
};

/**
 * to add another type of attacker
 * 
 * include the file of type of attacker
 * add type into enum ATTACKERTYPE before ATTACKERTYPE::ATTACKER_STOPPER
 * add reading from file in CMap::GenerateNewMap(std::ostream &)
 * add summoning in CGate::Summon(ATTACKERTYPE)
 * add type and name display in CGameState::DisplayChoice
 * */