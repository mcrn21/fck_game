#ifndef LEVEL_H
#define LEVEL_H

#include "fck/entity.h"
#include "fck/world.h"

#include <unordered_map>
#include <vector>

namespace fck
{

class Level
{
public:
    struct Section
    {
        std::vector<Entity> entities;
    };

    Level();
    ~Level() = default;

    bool loadFromFile(const std::string &file_name);

    void enable(const std::string &section_name = std::string());
    void disable();

private:
    std::unordered_map<std::string, Section> m_sections;
};

} // namespace fck

#endif // LEVEL_H
