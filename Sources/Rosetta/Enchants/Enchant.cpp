// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Rosetta/Enchants/Enchant.hpp>

namespace RosettaStone
{
Enchant::Enchant(Effect& effect)
{
    effects.emplace_back(effect);
}

Enchant::Enchant(std::vector<Effect>& effects)
{
    effects = effects;
}

void Enchant::ActivateTo(Character* character)
{
    for (auto& effect : effects)
    {
        effect.Apply(character);
    }
}
}  // namespace RosettaStone
