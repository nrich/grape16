#ifndef __COMMON_SHARED_H__
#define __COMMON_SHARED_H__

#include <cassert>
#include <cstdint>
#include <string>
#include <cmath>

#ifdef __MACOSX__
#include <gl.h>
//#include <glu.h>
#else
#include <GL/gl.h>
#endif

#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif

#define ANG2RAD (M_PI/180.0)

#define mathPiDiv180 (M_PI/180)
#define mathDoublePi (M_PI*2)

#define DEF_EPSILON 0.0001

#define PITCH                           0
#define YAW                             1
#define ROLL                            2

typedef float vecf;
typedef vecf vec2f[2];
typedef vecf vec3f[3];
typedef vecf vec4f[4];

#include <random>
typedef std::mt19937 RngT;

#endif //__COMMON_SHARED_H__
