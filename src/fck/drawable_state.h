#ifndef DRAWABLESTATE_ORYBISWUJKUL_H
#define DRAWABLESTATE_ORYBISWUJKUL_H

#include <string>

namespace fck
{

class DrawableState
{
public:
    DrawableState();
    virtual ~DrawableState() = default;

    virtual std::string getCurrentState() const = 0;
    virtual void setCurrentState(const std::string &state_name) = 0;
};

} // namespace fck

#endif // DRAWABLESTATE_ORYBISWUJKUL_H
