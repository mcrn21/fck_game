#include "noise.h"

#include <cmath>

#define X_NOISE_GEN 1619
#define Y_NOISE_GEN 31337
#define Z_NOISE_GEN 6971
#define SEED_NOISE_GEN 1013
#define SHIFT_NOISE_GEN 8

#define DEFAULT_PERLIN_FREQUENCY 1.0
#define DEFAULT_PERLIN_LACUNARITY 2.0
#define DEFAULT_PERLIN_OCTAVE_COUNT 6
#define DEFAULT_PERLIN_PERSISTENCE 0.5
#define DEFAULT_PERLIN_QUALITY noise::QUALITY_STD

#define DEFAULT_SIMPLEX_FREQUENCY 1.0
#define DEFAULT_SIMPLEX_AMPLITUDE 1.0
#define DEFAULT_SIMPLEX_LACUNARITY 2.0
#define DEFAULT_SIMPLEX_OCTAVE_COUNT 6
#define DEFAULT_SIMPLEXN_PERSISTENCE 0.5

#define DEFAULT_RIGEDMULTI_FREQUENCY 1.0
#define DEFAULT_RIGEDMULTI_LACUNARITY 2.0
#define DEFAULT_RIGEDMULTI_OCTAVE_COUNT 6
#define DEFAULT_RIGEDMULTI_QUALITY noise::QUALITY_STD

#define DEFAULT_VORONOI_DISPLACEMENT 1.0
#define DEFAULT_VORONOI_DISTANCE false
#define DEFAULT_VORONOI_FREQUENCY 1.0

namespace fck
{

namespace noise
{

const double rand_vec[256 * 4]
    = {-0.763874,   -0.596439,   -0.246489,  0.0, 0.396055,    0.904518,   -0.158073,   0.0,
       -0.499004,   -0.8665,     -0.0131631, 0.0, 0.468724,    -0.824756,  0.316346,    0.0,
       0.829598,    0.43195,     0.353816,   0.0, -0.454473,   0.629497,   -0.630228,   0.0,
       -0.162349,   -0.869962,   -0.465628,  0.0, 0.932805,    0.253451,   0.256198,    0.0,
       -0.345419,   0.927299,    -0.144227,  0.0, -0.715026,   -0.293698,  -0.634413,   0.0,
       -0.245997,   0.717467,    -0.651711,  0.0, -0.967409,   -0.250435,  -0.037451,   0.0,
       0.901729,    0.397108,    -0.170852,  0.0, 0.892657,    -0.0720622, -0.444938,   0.0,
       0.0260084,   -0.0361701,  0.999007,   0.0, 0.949107,    -0.19486,   0.247439,    0.0,
       0.471803,    -0.807064,   -0.355036,  0.0, 0.879737,    0.141845,   0.453809,    0.0,
       0.570747,    0.696415,    0.435033,   0.0, -0.141751,   -0.988233,  -0.0574584,  0.0,
       -0.58219,    -0.0303005,  0.812488,   0.0, -0.60922,    0.239482,   -0.755975,   0.0,
       0.299394,    -0.197066,   -0.933557,  0.0, -0.851615,   -0.220702,  -0.47544,    0.0,
       0.848886,    0.341829,    -0.403169,  0.0, -0.156129,   -0.687241,  0.709453,    0.0,
       -0.665651,   0.626724,    0.405124,   0.0, 0.595914,    -0.674582,  0.43569,     0.0,
       0.171025,    -0.509292,   0.843428,   0.0, 0.78605,     0.536414,   -0.307222,   0.0,
       0.18905,     -0.791613,   0.581042,   0.0, -0.294916,   0.844994,   0.446105,    0.0,
       0.342031,    -0.58736,    -0.7335,    0.0, 0.57155,     0.7869,     0.232635,    0.0,
       0.885026,    -0.408223,   0.223791,   0.0, -0.789518,   0.571645,   0.223347,    0.0,
       0.774571,    0.31566,     0.548087,   0.0, -0.79695,    -0.0433603, -0.602487,   0.0,
       -0.142425,   -0.473249,   -0.869339,  0.0, -0.0698838,  0.170442,   0.982886,    0.0,
       0.687815,    -0.484748,   0.540306,   0.0, 0.543703,    -0.534446,  -0.647112,   0.0,
       0.97186,     0.184391,    -0.146588,  0.0, 0.707084,    0.485713,   -0.513921,   0.0,
       0.942302,    0.331945,    0.043348,   0.0, 0.499084,    0.599922,   0.625307,    0.0,
       -0.289203,   0.211107,    0.9337,     0.0, 0.412433,    -0.71667,   -0.56239,    0.0,
       0.87721,     -0.082816,   0.47291,    0.0, -0.420685,   -0.214278,  0.881538,    0.0,
       0.752558,    -0.0391579,  0.657361,   0.0, 0.0765725,   -0.996789,  0.0234082,   0.0,
       -0.544312,   -0.309435,   -0.779727,  0.0, -0.455358,   -0.415572,  0.787368,    0.0,
       -0.874586,   0.483746,    0.0330131,  0.0, 0.245172,    -0.0838623, 0.965846,    0.0,
       0.382293,    -0.432813,   0.81641,    0.0, -0.287735,   -0.905514,  0.311853,    0.0,
       -0.667704,   0.704955,    -0.239186,  0.0, 0.717885,    -0.464002,  -0.518983,   0.0,
       0.976342,    -0.214895,   0.0240053,  0.0, -0.0733096,  -0.921136,  0.382276,    0.0,
       -0.986284,   0.151224,    -0.0661379, 0.0, -0.899319,   -0.429671,  0.0812908,   0.0,
       0.652102,    -0.724625,   0.222893,   0.0, 0.203761,    0.458023,   -0.865272,   0.0,
       -0.030396,   0.698724,    -0.714745,  0.0, -0.460232,   0.839138,   0.289887,    0.0,
       -0.0898602,  0.837894,    0.538386,   0.0, -0.731595,   0.0793784,  0.677102,    0.0,
       -0.447236,   -0.788397,   0.422386,   0.0, 0.186481,    0.645855,   -0.740335,   0.0,
       -0.259006,   0.935463,    0.240467,   0.0, 0.445839,    0.819655,   -0.359712,   0.0,
       0.349962,    0.755022,    -0.554499,  0.0, -0.997078,   -0.0359577, 0.0673977,   0.0,
       -0.431163,   -0.147516,   -0.890133,  0.0, 0.299648,    -0.63914,   0.708316,    0.0,
       0.397043,    0.566526,    -0.722084,  0.0, -0.502489,   0.438308,   -0.745246,   0.0,
       0.0687235,   0.354097,    0.93268,    0.0, -0.0476651,  -0.462597,  0.885286,    0.0,
       -0.221934,   0.900739,    -0.373383,  0.0, -0.956107,   -0.225676,  0.186893,    0.0,
       -0.187627,   0.391487,    -0.900852,  0.0, -0.224209,   -0.315405,  0.92209,     0.0,
       -0.730807,   -0.537068,   0.421283,   0.0, -0.0353135,  -0.816748,  0.575913,    0.0,
       -0.941391,   0.176991,    -0.287153,  0.0, -0.154174,   0.390458,   0.90762,     0.0,
       -0.283847,   0.533842,    0.796519,   0.0, -0.482737,   -0.850448,  0.209052,    0.0,
       -0.649175,   0.477748,    0.591886,   0.0, 0.885373,    -0.405387,  -0.227543,   0.0,
       -0.147261,   0.181623,    -0.972279,  0.0, 0.0959236,   -0.115847,  -0.988624,   0.0,
       -0.89724,    -0.191348,   0.397928,   0.0, 0.903553,    -0.428461,  -0.00350461, 0.0,
       0.849072,    -0.295807,   -0.437693,  0.0, 0.65551,     0.741754,   -0.141804,   0.0,
       0.61598,     -0.178669,   0.767232,   0.0, 0.0112967,   0.932256,   -0.361623,   0.0,
       -0.793031,   0.258012,    0.551845,   0.0, 0.421933,    0.454311,   0.784585,    0.0,
       -0.319993,   0.0401618,   -0.946568,  0.0, -0.81571,    0.551307,   -0.175151,   0.0,
       -0.377644,   0.00322313,  0.925945,   0.0, 0.129759,    -0.666581,  -0.734052,   0.0,
       0.601901,    -0.654237,   -0.457919,  0.0, -0.927463,   -0.0343576, -0.372334,   0.0,
       -0.438663,   -0.868301,   -0.231578,  0.0, -0.648845,   -0.749138,  -0.133387,   0.0,
       0.507393,    -0.588294,   0.629653,   0.0, 0.726958,    0.623665,   0.287358,    0.0,
       0.411159,    0.367614,    -0.834151,  0.0, 0.806333,    0.585117,   -0.0864016,  0.0,
       0.263935,    -0.880876,   0.392932,   0.0, 0.421546,    -0.201336,  0.884174,    0.0,
       -0.683198,   -0.569557,   -0.456996,  0.0, -0.117116,   -0.0406654, -0.992285,   0.0,
       -0.643679,   -0.109196,   -0.757465,  0.0, -0.561559,   -0.62989,   0.536554,    0.0,
       0.0628422,   0.104677,    -0.992519,  0.0, 0.480759,    -0.2867,    -0.828658,   0.0,
       -0.228559,   -0.228965,   -0.946222,  0.0, -0.10194,    -0.65706,   -0.746914,   0.0,
       0.0689193,   -0.678236,   0.731605,   0.0, 0.401019,    -0.754026,  0.52022,     0.0,
       -0.742141,   0.547083,    -0.387203,  0.0, -0.00210603, -0.796417,  -0.604745,   0.0,
       0.296725,    -0.409909,   -0.862513,  0.0, -0.260932,   -0.798201,  0.542945,    0.0,
       -0.641628,   0.742379,    0.192838,   0.0, -0.186009,   -0.101514,  0.97729,     0.0,
       0.106711,    -0.962067,   0.251079,   0.0, -0.743499,   0.30988,    -0.592607,   0.0,
       -0.795853,   -0.605066,   -0.0226607, 0.0, -0.828661,   -0.419471,  -0.370628,   0.0,
       0.0847218,   -0.489815,   -0.8677,    0.0, -0.381405,   0.788019,   -0.483276,   0.0,
       0.282042,    -0.953394,   0.107205,   0.0, 0.530774,    0.847413,   0.0130696,   0.0,
       0.0515397,   0.922524,    0.382484,   0.0, -0.631467,   -0.709046,  0.313852,    0.0,
       0.688248,    0.517273,    0.508668,   0.0, 0.646689,    -0.333782,  -0.685845,   0.0,
       -0.932528,   -0.247532,   -0.262906,  0.0, 0.630609,    0.68757,    -0.359973,   0.0,
       0.577805,    -0.394189,   0.714673,   0.0, -0.887833,   -0.437301,  -0.14325,    0.0,
       0.690982,    0.174003,    0.701617,   0.0, -0.866701,   0.0118182,  0.498689,    0.0,
       -0.482876,   0.727143,    0.487949,   0.0, -0.577567,   0.682593,   -0.447752,   0.0,
       0.373768,    0.0982991,   0.922299,   0.0, 0.170744,    0.964243,   -0.202687,   0.0,
       0.993654,    -0.035791,   -0.106632,  0.0, 0.587065,    0.4143,     -0.695493,   0.0,
       -0.396509,   0.26509,     -0.878924,  0.0, -0.0866853,  0.83553,    -0.542563,   0.0,
       0.923193,    0.133398,    -0.360443,  0.0, 0.00379108,  -0.258618,  0.965972,    0.0,
       0.239144,    0.245154,    -0.939526,  0.0, 0.758731,    -0.555871,  0.33961,     0.0,
       0.295355,    0.309513,    0.903862,   0.0, 0.0531222,   -0.91003,   -0.411124,   0.0,
       0.270452,    0.0229439,   -0.96246,   0.0, 0.563634,    0.0324352,  0.825387,    0.0,
       0.156326,    0.147392,    0.976646,   0.0, -0.0410141,  0.981824,   0.185309,    0.0,
       -0.385562,   -0.576343,   -0.720535,  0.0, 0.388281,    0.904441,   0.176702,    0.0,
       0.945561,    -0.192859,   -0.262146,  0.0, 0.844504,    0.520193,   0.127325,    0.0,
       0.0330893,   0.999121,    -0.0257505, 0.0, -0.592616,   -0.482475,  -0.644999,   0.0,
       0.539471,    0.631024,    -0.557476,  0.0, 0.655851,    -0.027319,  -0.754396,   0.0,
       0.274465,    0.887659,    0.369772,   0.0, -0.123419,   0.975177,   -0.183842,   0.0,
       -0.223429,   0.708045,    0.66989,    0.0, -0.908654,   0.196302,   0.368528,    0.0,
       -0.95759,    -0.00863708, 0.288005,   0.0, 0.960535,    0.030592,   0.276472,    0.0,
       -0.413146,   0.907537,    0.0754161,  0.0, -0.847992,   0.350849,   -0.397259,   0.0,
       0.614736,    0.395841,    0.68221,    0.0, -0.503504,   -0.666128,  -0.550234,   0.0,
       -0.268833,   -0.738524,   -0.618314,  0.0, 0.792737,    -0.60001,   -0.107502,   0.0,
       -0.637582,   0.508144,    -0.579032,  0.0, 0.750105,    0.282165,   -0.598101,   0.0,
       -0.351199,   -0.392294,   -0.850155,  0.0, 0.250126,    -0.960993,  -0.118025,   0.0,
       -0.732341,   0.680909,    -0.0063274, 0.0, -0.760674,   -0.141009,  0.633634,    0.0,
       0.222823,    -0.304012,   0.926243,   0.0, 0.209178,    0.505671,   0.836984,    0.0,
       0.757914,    -0.56629,    -0.323857,  0.0, -0.782926,   -0.339196,  0.52151,     0.0,
       -0.462952,   0.585565,    0.665424,   0.0, 0.61879,     0.194119,   -0.761194,   0.0,
       0.741388,    -0.276743,   0.611357,   0.0, 0.707571,    0.702621,   0.0752872,   0.0,
       0.156562,    0.819977,    0.550569,   0.0, -0.793606,   0.440216,   0.42,        0.0,
       0.234547,    0.885309,    -0.401517,  0.0, 0.132598,    0.80115,    -0.58359,    0.0,
       -0.377899,   -0.639179,   0.669808,   0.0, -0.865993,   -0.396465,  0.304748,    0.0,
       -0.624815,   -0.44283,    0.643046,   0.0, -0.485705,   0.825614,   -0.287146,   0.0,
       -0.971788,   0.175535,    0.157529,   0.0, -0.456027,   0.392629,   0.798675,    0.0,
       -0.0104443,  0.521623,    -0.853112,  0.0, -0.660575,   -0.74519,   0.091282,    0.0,
       -0.0157698,  -0.307475,   -0.951425,  0.0, -0.603467,   -0.250192,  0.757121,    0.0,
       0.506876,    0.25006,     0.824952,   0.0, 0.255404,    0.966794,   0.00884498,  0.0,
       0.466764,    -0.874228,   -0.133625,  0.0, 0.475077,    -0.0682351, -0.877295,   0.0,
       -0.224967,   -0.938972,   -0.260233,  0.0, -0.377929,   -0.814757,  -0.439705,   0.0,
       -0.305847,   0.542333,    -0.782517,  0.0, 0.26658,     -0.902905,  -0.337191,   0.0,
       0.0275773,   0.322158,    -0.946284,  0.0, 0.0185422,   0.716349,   0.697496,    0.0,
       -0.20483,    0.978416,    0.0273371,  0.0, -0.898276,   0.373969,   0.230752,    0.0,
       -0.00909378, 0.546594,    0.837349,   0.0, 0.6602,      -0.751089,  0.000959236, 0.0,
       0.855301,    -0.303056,   0.420259,   0.0, 0.797138,    0.0623013,  -0.600574,   0.0,
       0.48947,     -0.866813,   0.0951509,  0.0, 0.251142,    0.674531,   0.694216,    0.0,
       -0.578422,   -0.737373,   -0.348867,  0.0, -0.254689,   -0.514807,  0.818601,    0.0,
       0.374972,    0.761612,    0.528529,   0.0, 0.640303,    -0.734271,  -0.225517,   0.0,
       -0.638076,   0.285527,    0.715075,   0.0, 0.772956,    -0.15984,   -0.613995,   0.0,
       0.798217,    -0.590628,   0.118356,   0.0, -0.986276,   -0.0578337, -0.154644,   0.0,
       -0.312988,   -0.94549,    0.0899272,  0.0, -0.497338,   0.178325,   0.849032,    0.0,
       -0.101136,   -0.981014,   0.165477,   0.0, -0.521688,   0.0553434,  -0.851339,   0.0,
       -0.786182,   -0.583814,   0.202678,   0.0, -0.565191,   0.821858,   -0.0714658,  0.0,
       0.437895,    0.152598,    -0.885981,  0.0, -0.92394,    0.353436,   -0.14635,    0.0,
       0.212189,    -0.815162,   -0.538969,  0.0, -0.859262,   0.143405,   -0.491024,   0.0,
       0.991353,    0.112814,    0.0670273,  0.0, 0.0337884,   -0.979891,  -0.196654,   0.0};

const uint8_t permutation_table[256] = {
    151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,  103,
    30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,
    197, 62,  94,  252, 219, 203, 117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174,
    20,  125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158, 231,
    83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,  244, 102, 143,
    54,  65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169, 200, 196,
    135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124,
    123, 5,   202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,
    182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,  154, 163, 70,  221, 153, 101,
    155, 167, 43,  172, 9,   129, 22,  39,  253, 19,  98,  108, 110, 79,  113, 224, 232, 178, 185,
    112, 104, 218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,
    51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184, 84,  204, 176,
    115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243,
    141, 128, 195, 78,  66,  215, 61,  156, 180};

uint8_t permutation(int32_t i)
{
    return permutation_table[static_cast<uint8_t>(i)];
}

Module::Module() : m_seed(0)
{
}

Module::~Module()
{
}

int32_t Module::getSeed() const
{
    return m_seed;
}

void Module::setSeed(int32_t seed)
{
    m_seed = seed;
}

Perlin::Perlin()
    : Module(),
      m_frequency(DEFAULT_PERLIN_FREQUENCY),
      m_lacunarity(DEFAULT_PERLIN_LACUNARITY),
      m_noise_quality(DEFAULT_PERLIN_QUALITY),
      m_octave_count(DEFAULT_PERLIN_OCTAVE_COUNT),
      m_persistence(DEFAULT_PERLIN_PERSISTENCE)
{
}

Perlin::~Perlin()
{
}

float Perlin::getValue(double x, double y, double z)
{
    double value = 0.0;
    double signal = 0.0;
    double cur_persistence = 1.0;
    double nx;
    double ny;
    double nz;
    int32_t seed_;

    x *= m_frequency;
    y *= m_frequency;
    z *= m_frequency;

    for (int32_t cur_octave = 0; cur_octave < m_octave_count; ++cur_octave)
    {
        nx = funcs::makeInt32Range(x);
        ny = funcs::makeInt32Range(y);
        nz = funcs::makeInt32Range(z);

        seed_ = (getSeed() + cur_octave) & 0xffffffff;

        signal = funcs::gradientCoherentNoise3D(nx, ny, nz, seed_, m_noise_quality);
        value += signal * cur_persistence;

        x *= m_lacunarity;
        y *= m_lacunarity;
        z *= m_lacunarity;

        cur_persistence *= m_persistence;
    }

    return value;
}

double Perlin::getFrequency() const
{
    return m_frequency;
}

void Perlin::setFrequency(double frequency)
{
    m_frequency = frequency;
}

double Perlin::getLacunarity() const
{
    return m_lacunarity;
}

void Perlin::setLacunarity(double lacunarity)
{
    m_lacunarity = lacunarity;
}

Quality Perlin::getNoiseQuality() const
{
    return m_noise_quality;
}

void Perlin::setNoiseQuality(Quality noise_quality)
{
    m_noise_quality = noise_quality;
}

int32_t Perlin::getOctaveCount() const
{
    return m_octave_count;
}

void Perlin::setOctaveCount(int32_t octave_count)
{
    m_octave_count = octave_count;
}

double Perlin::getPersistence() const
{
    return m_persistence;
}

void Perlin::setPersistence(double persistence)
{
    m_persistence = persistence;
}

Simplex::Simplex()
    : m_frequency(DEFAULT_SIMPLEX_FREQUENCY),
      m_amplitude(DEFAULT_SIMPLEX_AMPLITUDE),
      m_lacunarity(DEFAULT_SIMPLEX_LACUNARITY),
      m_octave_count(DEFAULT_SIMPLEX_OCTAVE_COUNT),
      m_persistence(DEFAULT_SIMPLEXN_PERSISTENCE)
{
}

Simplex::~Simplex()
{
}

float Simplex::getValue(double x, double y, double z)
{
    return getFractal(m_octave_count, x, z);
}

double Simplex::getFrequency() const
{
    return m_frequency;
}

void Simplex::setFrequency(double frequency)
{
    m_frequency = frequency;
}

double Simplex::getAmplitude() const
{
    return m_amplitude;
}

void Simplex::setAmplitude(double amplitude)
{
    m_amplitude = amplitude;
}

double Simplex::getLacunarity() const
{
    return m_lacunarity;
}

void Simplex::setLacunarity(double lacunarity)
{
    m_lacunarity = lacunarity;
}

int32_t Simplex::getOctaveCount() const
{
    return m_octave_count;
}

void Simplex::setOctaveCount(int32_t octave_count)
{
    m_octave_count = octave_count;
}

double Simplex::getPersistence() const
{
    return m_persistence;
}

void Simplex::setPersistence(double persistence)
{
    m_persistence = persistence;
}

float Simplex::getNoise(float x, float y)
{
    float n0, n1, n2;

    const float F2 = 0.366025403f;
    const float G2 = 0.211324865f;

    float s = (x + y) * F2;
    float xs = x + s;
    float ys = y + s;
    int32_t i = funcs::fastfloor(xs);
    int32_t j = funcs::fastfloor(ys);

    float t = static_cast<float>(i + j) * G2;
    float X0 = i - t;
    float Y0 = j - t;
    float x0 = x - X0;
    float y0 = y - Y0;

    int32_t i1, j1;
    if (x0 > y0)
    {
        i1 = 1;
        j1 = 0;
    }
    else
    {
        i1 = 0;
        j1 = 1;
    }

    float x1 = x0 - i1 + G2;
    float y1 = y0 - j1 + G2;
    float x2 = x0 - 1.0f + 2.0f * G2;
    float y2 = y0 - 1.0f + 2.0f * G2;

    float t0 = 0.5f - x0 * x0 - y0 * y0;
    if (t0 < 0.0f)
    {
        n0 = 0.0f;
    }
    else
    {
        t0 *= t0;
        n0 = t0 * t0 * funcs::grad(permutation(i + permutation(j)), x0, y0);
    }

    float t1 = 0.5f - x1 * x1 - y1 * y1;
    if (t1 < 0.0f)
    {
        n1 = 0.0f;
    }
    else
    {
        t1 *= t1;
        n1 = t1 * t1 * funcs::grad(permutation(i + i1 + permutation(j + j1)), x1, y1);
    }

    float t2 = 0.5f - x2 * x2 - y2 * y2;
    if (t2 < 0.0f)
    {
        n2 = 0.0f;
    }
    else
    {
        t2 *= t2;
        n2 = t2 * t2 * funcs::grad(permutation(i + 1 + permutation(j + 1)), x2, y2);
    }

    return 45.23065f * (n0 + n1 + n2);
}

float Simplex::getFractal(int32_t octave_count, float x, float y)
{
    float output = 0;
    float denom = 0;
    float frequency = m_frequency;
    float amplitude = m_amplitude;
    int32_t seed_ = 0;

    for (int32_t cur_octave = 0; cur_octave < octave_count; ++cur_octave)
    {
        output += (amplitude * getNoise(x * frequency, y * frequency));
        denom += amplitude;
        frequency *= m_lacunarity;
        amplitude *= m_persistence;
    }

    return output / denom;
}

RigedMulti::RigedMulti()
    : Module(),
      m_frequency(DEFAULT_RIGEDMULTI_FREQUENCY),
      m_lacunarity(DEFAULT_RIGEDMULTI_LACUNARITY),
      m_noise_quality(DEFAULT_RIGEDMULTI_QUALITY),
      m_octave_count(DEFAULT_RIGEDMULTI_OCTAVE_COUNT)

{
    calcSpectralWeights();
}

RigedMulti::~RigedMulti()
{
}

float RigedMulti::getValue(double x, double y, double z)
{
    x *= m_frequency;
    y *= m_frequency;
    z *= m_frequency;

    double signal = 0.0;
    double value = 0.0;
    double weight = 1.0;

    double offset = 1.0;
    double gain = 2.0;

    for (int32_t cur_octave = 0; cur_octave < m_octave_count; ++cur_octave)
    {
        double nx = funcs::makeInt32Range(x);
        double ny = funcs::makeInt32Range(y);
        double nz = funcs::makeInt32Range(z);

        int32_t seed_ = (getSeed() + cur_octave) & 0x7fffffff;
        signal = funcs::gradientCoherentNoise3D(nx, ny, nz, seed_, m_noise_quality);

        signal = fabs(signal);
        signal = offset - signal;

        signal *= signal;
        signal *= weight;

        weight = signal * gain;
        if (weight > 1.0)
        {
            weight = 1.0;
        }
        if (weight < 0.0)
        {
            weight = 0.0;
        }

        value += (signal * m_spectral_weights[cur_octave]);
        x *= m_lacunarity;
        y *= m_lacunarity;
        z *= m_lacunarity;
    }

    return (value * 1.25) - 1.0;
}

double RigedMulti::getFrequency() const
{
    return m_frequency;
}

void RigedMulti::setFrequency(double frequency)
{
    m_frequency = frequency;
}

double RigedMulti::getLacunarity() const
{
    return m_lacunarity;
}

void RigedMulti::setLacunarity(double lacunarity)
{
    m_lacunarity = lacunarity;
}

Quality RigedMulti::getNoiseQuality() const
{
    return m_noise_quality;
}

void RigedMulti::setNoiseQuality(Quality noise_quality)
{
    m_noise_quality = noise_quality;
}

int32_t RigedMulti::getOctaveCount() const
{
    return m_octave_count;
}

void RigedMulti::setOctaveCount(int32_t octave_count)
{
    m_octave_count = octave_count;
}

void RigedMulti::calcSpectralWeights()
{
    double h = 1.0;
    double frequency = 1.0;
    for (int32_t i = 0; i < 30; i++)
    {
        m_spectral_weights[i] = pow(frequency, -h);
        frequency *= m_lacunarity;
    }
}

Voronoi::Voronoi()
    : Module(),
      m_displacement(DEFAULT_VORONOI_DISPLACEMENT),
      m_distance(DEFAULT_VORONOI_DISTANCE),
      m_frequency(DEFAULT_VORONOI_FREQUENCY)
{
}

Voronoi::~Voronoi()
{
}

float Voronoi::getValue(double x, double y, double z)
{
    x *= m_frequency;
    y *= m_frequency;
    z *= m_frequency;

    int32_t x_int = x > 0.0 ? (int)x : (int)x - 1;
    int32_t y_int = y > 0.0 ? (int)y : (int)y - 1;
    int32_t z_int = z > 0.0 ? (int)z : (int)z - 1;

    double min_dist = 2147483647.0;
    double x_candidate = 0;
    double y_candidate = 0;
    double z_candidate = 0;

    for (int32_t z_cur = z_int - 2; z_cur <= z_int + 2; ++z_cur)
    {
        for (int32_t y_cur = y_int - 2; y_cur <= y_int + 2; ++y_cur)
        {
            for (int32_t x_cur = x_int - 2; x_cur <= x_int + 2; ++x_cur)
            {
                double x_pos = x_cur + funcs::valueNoise3D(x_cur, y_cur, z_cur, getSeed());
                double y_pos = y_cur + funcs::valueNoise3D(x_cur, y_cur, z_cur, getSeed() + 1);
                double z_pos = z_cur + funcs::valueNoise3D(x_cur, y_cur, z_cur, getSeed() + 2);
                double x_dist = x_pos - x;
                double y_dist = y_pos - y;
                double z_dist = z_pos - z;
                double dist = x_dist * x_dist + y_dist * y_dist + z_dist * z_dist;

                if (dist < min_dist)
                {
                    min_dist = dist;
                    x_candidate = x_pos;
                    y_candidate = y_pos;
                    z_candidate = z_pos;
                }
            }
        }
    }

    double value;
    if (m_distance)
    {
        double x_dist = x_candidate - x;
        double y_dist = y_candidate - y;
        double z_dist = z_candidate - z;
        value = sqrt(x_dist * x_dist + y_dist * y_dist + z_dist * z_dist) * 1.7320508075688772935
            - 1.0;
    }
    else
    {
        value = 0.0;
    }

    return value
        + (m_displacement
           * (double)funcs::valueNoise3D(
               (int)floor(x_candidate), (int)floor(y_candidate), (int)floor(z_candidate)));
}

double Voronoi::getDisplacement() const
{
    return m_displacement;
}

void Voronoi::setDisplacement(double displacement)
{
    m_displacement = displacement;
}

bool Voronoi::getDistance() const
{
    return m_distance;
}

void Voronoi::setDistance(bool distance)
{
    m_distance = distance;
}

double Voronoi::getFrequency() const
{
    return m_frequency;
}

void Voronoi::setFrequency(double frequency)
{
    m_frequency = frequency;
}

CoordModify::CoordModify()
    : Module(),
      m_x_modificator(nullptr),
      m_y_modificator(nullptr),
      m_z_modificator(nullptr),
      m_source(nullptr),
      m_power(1)
{
}

CoordModify::~CoordModify()
{
}

float CoordModify::getValue(double x, double y, double z)
{
    if (!m_source)
        return -999;

    if (!m_x_modificator && !m_y_modificator && !m_z_modificator)
        return m_source->getValue(x, y, x);

    float xv = 0;
    float yv = 0;
    float zv = 0;

    if ((m_x_modificator == m_y_modificator) && (m_y_modificator == m_z_modificator)
        && (m_x_modificator == m_z_modificator))
    {
        xv = yv = zv = m_x_modificator->getValue(x, y, z) * m_power;
    }
    else
    {
        if (m_x_modificator)
            xv = m_x_modificator->getValue(x, y, z) * m_power;
        if (m_y_modificator)
            yv = m_y_modificator->getValue(x, y, z) * m_power;
        if (m_z_modificator)
            zv = m_z_modificator->getValue(x, y, z) * m_power;
    }

    return m_source->getValue(x + xv, y + yv, z + zv);
}

void CoordModify::setModificator(Module *modificator)
{
    m_x_modificator = m_y_modificator = m_z_modificator = modificator;
}

Module *CoordModify::getXModificator() const
{
    return m_x_modificator;
}

void CoordModify::setXModificator(Module *x_modificator)
{
    m_x_modificator = x_modificator;
}

Module *CoordModify::getYModificator() const
{
    return m_y_modificator;
}

void CoordModify::setYModificator(Module *y_modificator)
{
    m_y_modificator = y_modificator;
}

Module *CoordModify::getZModificator() const
{
    return m_z_modificator;
}

void CoordModify::setZModificator(Module *z_modificator)
{
    m_z_modificator = z_modificator;
}

Module *CoordModify::getSource() const
{
    return m_source;
}

void CoordModify::setSource(Module *source)
{
    m_source = source;
}

float CoordModify::getPower() const
{
    return m_power;
}

void CoordModify::setPower(float power)
{
    m_power = power;
}

Mask::Mask() : Module(), m_mask(nullptr), m_source(nullptr), m_more_then(-999)
{
}

Mask::~Mask()
{
}

float Mask::getValue(double x, double y, double z)
{
    if (!m_mask && !m_source)
        return -999;

    float source_value = m_source->getValue(x, y, z);
    float mask_value = m_mask->getValue(x, y, z);

    return (source_value >= mask_value) ? source_value : -999;
}

Module *Mask::getMask() const
{
    return m_mask;
}

void Mask::setMask(Module *mask)
{
    m_mask = mask;
}

Module *Mask::getSource() const
{
    return m_source;
}

void Mask::setSource(Module *source)
{
    m_source = source;
}

float Mask::getMoreThen() const
{
    return m_more_then;
}

void Mask::setMoreThen(float more_then)
{
    m_more_then = more_then;
}

Map::Map() : m_width(0), m_height(0)
{
}

Map::~Map()
{
}

void Map::setSize(int32_t width, int32_t height)
{
    if (m_width < 0 || m_height < 0)
        return;

    m_width = width;
    m_height = height;

    m_data.clear();
    m_data.resize(m_width * m_height);
}

float Map::getValue(int32_t x, int32_t y) const
{
    if (x > m_width || y > m_width || x < 0 || y < 0)
        return 0;
    return m_data[m_width * y + x];
}

void Map::setValue(int32_t x, int32_t y, float value)
{
    if (x > m_width || y > m_width || x < 0 || y < 0)
        return;
    m_data[m_width * y + x] = value;
}

MapGenerator::MapGenerator()
    : m_dest_width(0),
      m_dest_height(0),
      m_x1(0),
      m_x2(0),
      m_y1(0),
      m_y2(0),
      m_seamless(true),
      m_blend(1)
{
}

MapGenerator::~MapGenerator()
{
}

Map *MapGenerator::getMap() const
{
    return m_map;
}

void MapGenerator::setMap(Map *map)
{
    m_map = map;
}

Module *MapGenerator::getModule() const
{
    return m_module;
}

void MapGenerator::setModule(Module *module)
{
    m_module = module;
}

void MapGenerator::setDestSize(int32_t width, int32_t height)
{
    if (width < 0 || height < 0)
        return;

    m_dest_width = width;
    m_dest_height = height;
}

void MapGenerator::setBounds(int32_t x1, int32_t x2, int32_t y1, int32_t y2)
{
    if (x1 > x2 || y1 > y2)
        return;

    m_x1 = x1;
    m_x2 = x2;
    m_y1 = y1;
    m_y2 = y2;
}

bool MapGenerator::getSeamless() const
{
    return m_seamless;
}

void MapGenerator::setSeamless(bool seamless)
{
    m_seamless = seamless;
}

float MapGenerator::getBlend() const
{
    return m_blend;
}

void MapGenerator::setBlend(float blend)
{
    m_blend = blend;
}

void MapGenerator::generate()
{
    if (m_x1 == m_x2 || m_y1 == m_y2 || m_dest_width == 0 || m_dest_height == 0 || m_map == nullptr
        || m_module == nullptr)
        return;

    m_map->setSize(m_dest_width, m_dest_height);

    double x_extent = m_x2 - m_x1;
    double z_extent = m_y2 - m_y1;
    double x_delta = x_extent / (double)m_dest_width;
    double z_delta = z_extent / (double)m_dest_height;
    double x_cur = m_x1;
    double z_cur = m_y1;

    for (int32_t z = 0; z < m_dest_height; ++z)
    {
        float value = 0;
        x_cur = m_x1;
        for (int32_t x = 0; x < m_dest_width; ++x)
        {
            if (!m_seamless)
            {
                value = m_module->getValue(x_cur, 0, z_cur);
            }
            else
            {
                double sw_value = m_module->getValue(x_cur, 0, z_cur);
                double se_value = m_module->getValue(x_cur + x_extent, 0, z_cur);
                double nw_value = m_module->getValue(x_cur, 0, z_cur + z_extent);
                double ne_value = m_module->getValue(x_cur + x_extent, 0, z_cur + z_extent);

                double z0 = funcs::linearInterp(sw_value, se_value, m_blend);
                double z1 = funcs::linearInterp(nw_value, ne_value, m_blend);

                value = funcs::linearInterp(z0, z1, m_blend);
            }
            x_cur += x_delta;
            m_map->setValue(x, z, value);
        }
        z_cur += z_delta;
    }
}

double funcs::linearInterp(double n0, double n1, double a)
{
    return ((1.0 - a) * n0) + (a * n1);
}

double funcs::sCurve3(double a)
{
    return (a * a * (3.0 - 2.0 * a));
}

double funcs::sCurve5(double a)
{
    double a3 = a * a * a;
    double a4 = a3 * a;
    double a5 = a4 * a;
    return (6.0 * a5) - (15.0 * a4) + (10.0 * a3);
}

double funcs::gradientCoherentNoise3D(double x, double y, double z, int32_t seed, Quality quality)
{
    int32_t x0 = (x > 0.0 ? (int)x : (int)x - 1);
    int32_t x1 = x0 + 1;
    int32_t y0 = (y > 0.0 ? (int)y : (int)y - 1);
    int32_t y1 = y0 + 1;
    int32_t z0 = (z > 0.0 ? (int)z : (int)z - 1);
    int32_t z1 = z0 + 1;

    double xs = 0, ys = 0, zs = 0;
    switch (quality)
    {
    case QUALITY_FAST:
        xs = (x - (double)x0);
        ys = (y - (double)y0);
        zs = (z - (double)z0);
        break;
    case QUALITY_STD:
        xs = funcs::sCurve3(x - (double)x0);
        ys = funcs::sCurve3(y - (double)y0);
        zs = funcs::sCurve3(z - (double)z0);
        break;
    case QUALITY_BEST:
        xs = funcs::sCurve5(x - (double)x0);
        ys = funcs::sCurve5(y - (double)y0);
        zs = funcs::sCurve5(z - (double)z0);
        break;
    }

    double n0, n1, ix0, ix1, iy0, iy1;

    n0 = funcs::gradientNoise3D(x, y, z, x0, y0, z0, seed);
    n1 = funcs::gradientNoise3D(x, y, z, x1, y0, z0, seed);
    ix0 = funcs::linearInterp(n0, n1, xs);

    n0 = funcs::gradientNoise3D(x, y, z, x0, y1, z0, seed);
    n1 = funcs::gradientNoise3D(x, y, z, x1, y1, z0, seed);
    ix1 = funcs::linearInterp(n0, n1, xs);
    iy0 = funcs::linearInterp(ix0, ix1, ys);

    n0 = funcs::gradientNoise3D(x, y, z, x0, y0, z1, seed);
    n1 = funcs::gradientNoise3D(x, y, z, x1, y0, z1, seed);
    ix0 = funcs::linearInterp(n0, n1, xs);

    n0 = funcs::gradientNoise3D(x, y, z, x0, y1, z1, seed);
    n1 = funcs::gradientNoise3D(x, y, z, x1, y1, z1, seed);
    ix1 = funcs::linearInterp(n0, n1, xs);
    iy1 = funcs::linearInterp(ix0, ix1, ys);

    return funcs::linearInterp(iy0, iy1, zs);
}

double funcs::gradientNoise3D(
    double fx, double fy, double fz, int32_t ix, int32_t iy, int32_t iz, int32_t seed)
{
    int32_t vector_index
        = (X_NOISE_GEN * ix + Y_NOISE_GEN * iy + Z_NOISE_GEN * iz + SEED_NOISE_GEN * seed)
        & 0xffffffff;
    vector_index ^= (vector_index >> SHIFT_NOISE_GEN);
    vector_index &= 0xff;

    double xv_gradient = rand_vec[(vector_index << 2)];
    double yv_gradient = rand_vec[(vector_index << 2) + 1];
    double zv_gradient = rand_vec[(vector_index << 2) + 2];

    double xv_point = (fx - (double)ix);
    double yv_point = (fy - (double)iy);
    double zv_point = (fz - (double)iz);

    return ((xv_gradient * xv_point) + (yv_gradient * yv_point) + (zv_gradient * zv_point)) * 2.12;
}

int32_t funcs::intValueNoise3D(int32_t x, int32_t y, int32_t z, int32_t seed)
{
    int32_t n = (X_NOISE_GEN * x + Y_NOISE_GEN * y + Z_NOISE_GEN * z + SEED_NOISE_GEN * seed)
        & 0x7fffffff;
    n = (n >> 13) ^ n;
    return (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
}

double funcs::valueNoise3D(int32_t x, int32_t y, int32_t z, int32_t seed)
{
    return 1.0 - ((double)funcs::intValueNoise3D(x, y, z, seed) / 1073741824.0);
}

double funcs::makeInt32Range(double n)
{
    if (n >= 1073741824.0)
    {
        return (2.0 * fmod(n, 1073741824.0)) - 1073741824.0;
    }
    else if (n <= -1073741824.0)
    {
        return (2.0 * fmod(n, 1073741824.0)) + 1073741824.0;
    }
    else
    {
        return n;
    }
}

float funcs::grad(int32_t permutation, float x, float y)
{
    int32_t h = permutation & 0x0F;
    float grad = 1.0f + (h & 7);
    if ((h & 8) != 0)
        grad = -grad;
    return (grad * x);
}

int32_t funcs::fastfloor(float f)
{
    int32_t i = (int)f;
    return (f < i) ? (i - 1) : (i);
}

} // namespace noise

} // namespace fck
