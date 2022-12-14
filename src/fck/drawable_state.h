#ifndef DRAWABLESTATE_ORYBISWUJKUL_H
#define DRAWABLESTATE_ORYBISWUJKUL_H

#include <string>
#include <vector>

namespace fck
{

class DrawableState
{
public:
    DrawableState();
    virtual ~DrawableState() = default;

    virtual std::string getCurrentState() const = 0;
    virtual void setCurrentState(const std::string &state_name) = 0;

    virtual std::vector<std::string> getStates() const = 0;
};

} // namespace fck

#endif // DRAWABLESTATE_ORYBISWUJKUL_H
