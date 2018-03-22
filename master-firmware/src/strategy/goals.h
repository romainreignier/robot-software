#ifndef STRATEGY_GOALS_H
#define STRATEGY_GOALS_H

#include <goap/goap.hpp>
#include "state.h"

struct InitGoal : goap::Goal<RobotState> {
    virtual int distance_to(const RobotState &state) const
    {
        return state.arms_are_deployed ? 0 : 1;
    }
};

struct TowerGoal : goap::Goal<RobotState> {
    virtual int distance_to(const RobotState &state) const
    {
        auto res = 0;
        res += state.arms_are_deployed ? 0 : 1;
        res += state.tower_built ? 0 : 1;
        return res;
    }
};

struct SwitchGoal : goap::Goal<RobotState> {
    bool is_reached(RobotState state)
    {
        return state.switch_on == true;
    }
};

struct GameGoal : goap::Goal<RobotState> {
    bool is_reached(RobotState state)
    {
        return state.arms_are_deployed == false
            && state.tower_built == true
            && state.switch_on == true;
    }
};

#endif /* STRATEGY_GOALS_H */
