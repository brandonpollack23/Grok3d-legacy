#ifndef __THREEDEEGEOMETRY__H
#define __THREEDEEGEOMETRY__H

#include <string>

namespace Grok3d { namespace Geometry 
{
    class Vector3
    {
    public:
        Vector3(float xnew, float ynew, float znew = 0.0);

        static const Vector3 back;
        static const Vector3 down;
        static const Vector3 forward;
        static const Vector3 left;
        static const Vector3 negativeInfinity;
        static const Vector3 one;
        static const Vector3 positiveInfinity;
        static const Vector3 right;
        static const Vector3 up;
        static const Vector3 zero;

        float GetMagnitude();
        float GetSquareMagnitude();
        Vector3 Normalized();
        void Set(float xnew, float ynew, float znew);
        std::string ToString();

        float operator[](unsigned int index);
        float operator==(Vector3& other);

        float x, y, z;
    };

} /*Geometry*/ } /*Grok3d*/

#endif