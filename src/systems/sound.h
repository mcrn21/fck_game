#ifndef SOUND_UVHPRTOHNRHM_H
#define SOUND_UVHPRTOHNRHM_H

#include "../components/components.h"
#include "../fck/system.h"

namespace fck::system
{

class Sound : public System<component::Sound>
{
public:
    Sound();
    ~Sound() = default;

public: //slots
    void onEntityMoved(const Entity &entity, const sf::Vector2f &offset);
};

} // namespace fck::system

#endif // SOUND_UVHPRTOHNRHM_H
