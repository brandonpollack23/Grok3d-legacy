#include "DataStructures/ThreeDeeGeometry.h"
#include <cmath>
#include <limits>
#include <sstream>

using namespace Grok3d;
using namespace Grok3d::Geometry;

const Vector3 Vector3::back(0.0, 0.0, -1.0);
const Vector3 Vector3::down(0.0, -1.0, 0);
const Vector3 Vector3::forward(0.0, 0.0, 1.0);
const Vector3 Vector3::left(-1.0, 0.0, 0.0);
const Vector3 Vector3::negativeInfinity(-INFINITY, -INFINITY, -INFINITY);
const Vector3 Vector3::one(1.0, 1.0, 1.0);
const Vector3 Vector3::positiveInfinity(INFINITY, INFINITY, INFINITY);
const Vector3 Vector3::right(1.0, 0.0, 0.0);
const Vector3 Vector3::up(0.0, 1.0, 1.0);
const Vector3 Vector3::zero(0.0, 0.0, 0.0);

Vector3::Vector3(float xnew, float ynew, float znew = 0.0) : x(xnew), y(ynew), z(znew)
{
}

float Vector3::GetMagnitude()
{
    return sqrt(GetSquareMagnitude());
}

float Vector3::GetSquareMagnitude()
{
    return x*x + y*y + z*z;
}

Vector3 Vector3::Normalized()
{
    float mag = GetMagnitude();
    return Vector3(x/mag, y/mag, z/mag);
}

float Vector3::operator[](unsigned int index)
{
    if(index == 0) return x;
    else if(index == 1) return y;
    else if(index == 2) return z;
}

float Vector3::operator==(Vector3& other)
{
    return other.x == x && other.y == y && other.z == z;
}

void Vector3::Set(float xnew, float ynew, float znew)
{
    x = xnew;
    y = ynew;
    z = znew;
}

std::string Vector3::ToString()
{
    std::ostringstream strstr;
    strstr << "Vector3(" << x << ", " << y << ", " << z << ")";
    return strstr.str();
}