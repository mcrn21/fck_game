#ifndef NOISE_YIHYEIOAYKSK_H
#define NOISE_YIHYEIOAYKSK_H

#include <cstdint>
#include <vector>

namespace fck
{

namespace noise
{

enum Quality
{
    QUALITY_FAST = 0,
    QUALITY_STD = 1,
    QUALITY_BEST = 2
};

class Module
{
public:
    Module();
    virtual ~Module();

    int32_t seed() const;
    void setSeed(int32_t seed);

    virtual float value(double x, double y, double z)
    {
        return 0;
    }

private:
    int32_t m_seed;
};

class Perlin : public Module
{
public:
    Perlin();
    ~Perlin();

    float value(double x, double y, double z);

    double frequency() const;
    void setFrequency(double frequency);

    double lacunarity() const;
    void setLacunarity(double lacunarity);

    Quality noiseQuality() const;
    void setNoiseQuality(Quality noise_quality);

    int32_t octaveCount() const;
    void setOctaveCount(int32_t octave_count);

    double persistence() const;
    void setPersistence(double persistence);

private:
    double m_frequency;
    double m_lacunarity;
    Quality m_noise_quality;
    int32_t m_octave_count;
    double m_persistence;
};

class Simplex : public Module
{
public:
    Simplex();
    ~Simplex();

    float value(double x, double y, double z);

    double frequency() const;
    void setFrequency(double frequency);

    double amplitude() const;
    void setAmplitude(double amplitude);

    double lacunarity() const;
    void setLacunarity(double lacunarity);

    int32_t octaveCount() const;
    void setOctaveCount(int32_t octave_count);

    double persistence() const;
    void setPersistence(double persistence);

private:
    float noise(float x, float y);
    float fractal(int32_t octave_count, float x, float y);

private:
    double m_frequency;
    double m_amplitude;
    double m_lacunarity;
    int32_t m_octave_count;
    double m_persistence;
};

class RigedMulti : public Module
{
public:
    RigedMulti();
    ~RigedMulti();

    float value(double x, double y, double z);

    double frequency() const;
    void setFrequency(double frequency);

    double lacunarity() const;
    void setLacunarity(double lacunarity);

    Quality noiseQuality() const;
    void setNoiseQuality(Quality noise_quality);

    int32_t octaveCount() const;
    void setOctaveCount(int32_t octave_count);

private:
    void calcSpectralWeights();

private:
    double m_frequency;
    double m_lacunarity;
    Quality m_noise_quality;
    int32_t m_octave_count;
    double m_spectral_weights[30];
};

class Voronoi : public Module
{
public:
    Voronoi();
    ~Voronoi();

    float value(double x, double y, double z);

    double displacement() const;
    void setDisplacement(double displacement);

    bool distance() const;
    void setDistance(bool distance);

    double frequency() const;
    void setFrequency(double frequency);

private:
    double m_displacement;
    bool m_distance;
    double m_frequency;
};

class CoordModify : public Module
{
public:
    CoordModify();
    ~CoordModify();

    float value(double x, double y, double z);

    void setModificator(Module *modificator);

    Module *xModificator() const;
    void setXModificator(Module *x_modificator);

    Module *yModificator() const;
    void setYModificator(Module *y_modificator);

    Module *zModificator() const;
    void setZModificator(Module *z_modificator);

    Module *source() const;
    void setSource(Module *source);

    float power() const;
    void setPower(float power);

private:
    Module *m_x_modificator;
    Module *m_y_modificator;
    Module *m_z_modificator;
    Module *m_source;

    float m_power;
};

//! Наложение маски
class Mask : public Module
{
public:
    Mask();
    ~Mask();

    float value(double x, double y, double z);

    Module *mask() const;
    void setMask(Module *mask);

    Module *source() const;
    void setSource(Module *source);

    float moreThen() const;
    void setMoreThen(float more_then);

private:
    Module *m_mask;
    Module *m_source;

    float m_more_then;
};

class Map
{
public:
    Map();
    ~Map();

    void setSize(int32_t width, int32_t height);

    float value(int32_t x, int32_t y) const;
    void setValue(int32_t x, int32_t y, float value);

private:
    int32_t m_width;
    int32_t m_height;
    std::vector<float> m_data;
};

class MapGenerator
{
public:
    MapGenerator();
    ~MapGenerator();

    Map *map() const;
    void setMap(Map *map);

    Module *module() const;
    void setModule(Module *module);

    void setDestSize(int32_t width, int32_t height);
    void setBounds(int32_t x1, int32_t x2, int32_t y1, int32_t y2);

    bool seamless() const;
    void setSeamless(bool seamless);

    float blend() const;
    void setBlend(float blend);

    void generate();

private:
    Map *m_map;
    Module *m_module;

    int32_t m_dest_width;
    int32_t m_dest_height;

    int32_t m_x1;
    int32_t m_x2;

    int32_t m_y1;
    int32_t m_y2;

    bool m_seamless;
    float m_blend;
};

namespace funcs
{

double linearInterp(double n0, double n1, double a);
double sCurve3(double a);
double sCurve5(double a);

double gradientCoherentNoise3D(
    double x, double y, double z, int32_t seed = 0, Quality quality = QUALITY_STD);
double gradientNoise3D(
    double fx, double fy, double fz, int32_t ix, int32_t iy, int32_t iz, int32_t seed = 0);
int32_t intValueNoise3D(int32_t x, int32_t y, int32_t z, int32_t seed = 0);
double valueNoise3D(int32_t x, int32_t y, int32_t z, int32_t seed = 0);

double makeInt32Range(double n);
float grad(int32_t permutation, float x, float y);
int32_t fastfloor(float f);

} // namespace funcs

} // namespace noise

} // namespace fck

#endif // NOISE_YIHYEIOAYKSK_H
