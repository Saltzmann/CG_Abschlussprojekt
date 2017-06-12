#ifndef COLLISIONRESPONSE_H
#define COLLISIONRESPONSE_H


#include "collisiondetectiondata.h"

namespace Physics
{
    class CollisionResponse
    {
    public:
        CollisionResponse(CollisionDetectionData& cdd)
            : m_CDDataRef(cdd)
        { }

        void response();

    private:
        CollisionDetectionData& m_CDDataRef;
    };
}
#endif // COLLISIONRESPONSE_H
