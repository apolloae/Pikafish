/*
  Extended UCI options (Xiangqi rules + strength limit).

  Registration lives here so engine.cpp only needs a single hook call.
  That keeps upstream merges of engine.cpp from conflicting on option lists.
*/

#ifndef XQOPTIONS_H_INCLUDED
#define XQOPTIONS_H_INCLUDED

#include <algorithm>

#include "search.h"
#include "xqconfig.h"

namespace Stockfish {

class OptionsMap;

void add_extended_options(OptionsMap& options);

namespace Search {

// Official Stockfish strength limit. Skill 0..19 covers CCRL Blitz Elo
// from 1320 to 3190 when UCI_LimitStrength is on. Level 20 is full strength.
struct Skill {
    constexpr static int LowestElo  = 1320;
    constexpr static int HighestElo = 3190;

    Skill(int skill_level, int uci_elo) {
        if (uci_elo)
        {
            double e = double(uci_elo - LowestElo) / (HighestElo - LowestElo);
            level = std::clamp((((37.2473 * e - 40.8525) * e + 22.2943) * e - 0.311438), 0.0, 19.0);
        }
        else
            level = double(skill_level);
    }

    bool enabled() const { return level < 20.0; }
    bool time_to_pick(Depth depth) const { return depth == 1 + int(level); }
    Move pick_best(const RootMoves& rootMoves, usize multiPV);

    double level;
    Move   best = Move::none();
};

}  // namespace Search

}  // namespace Stockfish

#endif  // #ifndef XQOPTIONS_H_INCLUDED
