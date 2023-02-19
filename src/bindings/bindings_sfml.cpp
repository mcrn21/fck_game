#include "bindings.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

namespace fck
{

void bindSfmlToLua(sol::state &sol_state)
{
    sol::table sf_namespace = sol_state["sf"].get_or_create<sol::table>();

    sol::usertype<sf::Angle> angle_bind = sf_namespace.new_usertype<sf::Angle>(
        "Angle",
        sol::constructors<sf::Angle()>(),
        "asDegrees",
        &sf::Angle::asDegrees,
        "asRadians",
        &sf::Angle::asRadians,
        "wrapSigned",
        &sf::Angle::wrapSigned,
        "wrapUnsigned",
        &sf::Angle::wrapUnsigned);

    sf_namespace["degrees"] = &sf::degrees;
    sf_namespace["radians"] = &sf::radians;

    sol::usertype<sf::Vector2f> vector2f_bind = sf_namespace.new_usertype<sf::Vector2f>(
        "Vector2f",
        sol::constructors<
            sf::Vector2f(),
            sf::Vector2f(float, float),
            sf::Vector2f(float, sf::Angle)>(),
        "x",
        &sf::Vector2f::x,
        "y",
        &sf::Vector2f::y,
        "length",
        &sf::Vector2f::length,
        "lengthSq",
        &sf::Vector2f::lengthSq,
        "normalized",
        &sf::Vector2f::normalized,
        "angleTo",
        &sf::Vector2f::angleTo,
        "angle",
        &sf::Vector2f::angle,
        "rotatedBy",
        &sf::Vector2f::rotatedBy,
        "projectedOnto",
        &sf::Vector2f::projectedOnto,
        "perpendicular",
        &sf::Vector2f::perpendicular,
        "dot",
        &sf::Vector2f::dot,
        "cross",
        &sf::Vector2f::cross,
        "cwiseMul",
        &sf::Vector2f::cwiseMul,
        "cwiseDiv",
        &sf::Vector2f::cwiseDiv,
        sol::meta_function::addition,
        [](const sf::Vector2f &left, const sf::Vector2f &right) { return left + right; },
        sol::meta_function::subtraction,
        [](const sf::Vector2f &left, const sf::Vector2f &right) { return left - right; },
        sol::meta_function::multiplication,
        [](const sf::Vector2f &left, float right) { return left * right; },
        sol::meta_function::division,
        [](const sf::Vector2f &left, float right) { return left / right; },
        sol::meta_function::equal_to,
        [](const sf::Vector2f &left, const sf::Vector2f &right) { return left == right; });

    sol::usertype<sf::Vector2i> vector2i_bind = sf_namespace.new_usertype<sf::Vector2i>(
        "Vector2i",
        sol::constructors<sf::Vector2i(), sf::Vector2i(int32_t, int32_t)>(),
        "x",
        &sf::Vector2i::x,
        "y",
        &sf::Vector2i::y,
        "lengthSq",
        &sf::Vector2i::lengthSq,
        "perpendicular",
        &sf::Vector2i::perpendicular,
        "dot",
        &sf::Vector2i::dot,
        "cross",
        &sf::Vector2i::cross,
        "cwiseMul",
        &sf::Vector2i::cwiseMul,
        "cwiseDiv",
        &sf::Vector2i::cwiseDiv,
        sol::meta_function::addition,
        [](const sf::Vector2i &left, const sf::Vector2i &right) { return left + right; },
        sol::meta_function::subtraction,
        [](const sf::Vector2i &left, const sf::Vector2i &right) { return left - right; },
        sol::meta_function::multiplication,
        [](const sf::Vector2i &left, int32_t right) { return left * right; },
        sol::meta_function::division,
        [](const sf::Vector2i &left, int32_t right) { return left / right; },
        sol::meta_function::equal_to,
        [](const sf::Vector2i &left, const sf::Vector2i &right) { return left == right; });

    sol::usertype<sf::Vector2u> vector2u_bind = sf_namespace.new_usertype<sf::Vector2u>(
        "Vector2u",
        sol::constructors<sf::Vector2u(), sf::Vector2u(uint32_t, uint32_t)>(),
        "x",
        &sf::Vector2u::x,
        "y",
        &sf::Vector2u::y,
        "lengthSq",
        &sf::Vector2u::lengthSq,
        "perpendicular",
        &sf::Vector2u::perpendicular,
        "dot",
        &sf::Vector2u::dot,
        "cross",
        &sf::Vector2u::cross,
        "cwiseMul",
        &sf::Vector2u::cwiseMul,
        "cwiseDiv",
        &sf::Vector2u::cwiseDiv,
        sol::meta_function::addition,
        [](const sf::Vector2u &left, const sf::Vector2u &right) { return left + right; },
        sol::meta_function::subtraction,
        [](const sf::Vector2u &left, const sf::Vector2u &right) { return left - right; },
        sol::meta_function::multiplication,
        [](const sf::Vector2u &left, uint32_t right) { return left * right; },
        sol::meta_function::division,
        [](const sf::Vector2u &left, uint32_t right) { return left / right; },
        sol::meta_function::equal_to,
        [](const sf::Vector2u &left, const sf::Vector2u &right) { return left == right; });

    sol::usertype<sf::FloatRect> float_rect_bind = sf_namespace.new_usertype<sf::FloatRect>(
        "FloatRect",
        sol::constructors<
            sf::FloatRect(),
            sf::FloatRect(const sf::Vector2f &, const sf::Vector2f &)>(),
        "left",
        &sf::FloatRect::left,
        "top",
        &sf::FloatRect::top,
        "width",
        &sf::FloatRect::width,
        "height",
        &sf::FloatRect::height,
        "contains",
        &sf::FloatRect::contains,
        "findIntersection",
        &sf::FloatRect::findIntersection,
        "getPosition",
        &sf::FloatRect::getPosition,
        "getSize",
        &sf::FloatRect::getSize,
        sol::meta_function::equal_to,
        [](const sf::FloatRect &left, const sf::FloatRect &right) { return left == right; });

    sol::usertype<sf::IntRect> int_rect_bind = sf_namespace.new_usertype<sf::IntRect>(
        "IntRect",
        sol::constructors<sf::IntRect(), sf::IntRect(const sf::Vector2i &, const sf::Vector2i &)>(),
        "left",
        &sf::IntRect::left,
        "top",
        &sf::IntRect::top,
        "width",
        &sf::IntRect::width,
        "height",
        &sf::IntRect::height,
        "contains",
        &sf::IntRect::contains,
        "findIntersection",
        &sf::IntRect::findIntersection,
        "getPosition",
        &sf::IntRect::getPosition,
        "getSize",
        &sf::IntRect::getSize,
        sol::meta_function::equal_to,
        [](const sf::IntRect &left, const sf::IntRect &right) { return left == right; });

    sol::usertype<sf::Transform> transforma_bind = sf_namespace.new_usertype<sf::Transform>(
        "Transform",
        sol::constructors<sf::Transform(
            float, float, float, float, float, float, float, float, float)>(),
        "getMatrix",
        &sf::Transform::getMatrix,
        "getInverse",
        &sf::Transform::getInverse,
        "transformPoint",
        &sf::Transform::transformPoint,
        "transformRect",
        &sf::Transform::transformRect,
        "combine",
        &sf::Transform::combine,
        "translate",
        &sf::Transform::translate,
        "rotate",
        sol::overload(
            sol::resolve<sf::Transform &(sf::Angle)>(&sf::Transform::rotate),
            sol::resolve<sf::Transform &(sf::Angle, const sf::Vector2f &)>(&sf::Transform::rotate)),
        "scale",
        sol::overload(
            sol::resolve<sf::Transform &(const sf::Vector2f &)>(&sf::Transform::scale),
            sol::resolve<sf::Transform &(const sf::Vector2f &, const sf::Vector2f &)>(
                &sf::Transform::scale)),
        sol::meta_function::multiplication,
        sol::overload(
            [](const sf::Transform &left, const sf::Transform &right) { return left * right; },
            [](const sf::Transform &left, const sf::Vector2f &right) { return left * right; }),
        sol::meta_function::equal_to,
        [](const sf::Transform &left, const sf::Transform &right) { return left == right; });

    sol::usertype<sf::Transformable> transformable_bind
        = sf_namespace.new_usertype<sf::Transformable>(
            "Transformable",
            sol::constructors<sf::Transformable()>(),
            "setPosition",
            &sf::Transformable::setPosition,
            "setRotation",
            &sf::Transformable::setRotation,
            "setScale",
            &sf::Transformable::setScale,
            "setOrigin",
            &sf::Transformable::setOrigin,
            "getPosition",
            &sf::Transformable::getPosition,
            "getRotation",
            &sf::Transformable::getRotation,
            "getScale",
            &sf::Transformable::getScale,
            "getOrigin",
            &sf::Transformable::getOrigin,
            "move",
            &sf::Transformable::move,
            "rotate",
            &sf::Transformable::rotate,
            "scale",
            &sf::Transformable::scale,
            "getTransform",
            &sf::Transformable::getTransform,
            "getInverseTransform",
            &sf::Transformable::getInverseTransform);
}

} // namespace fck
