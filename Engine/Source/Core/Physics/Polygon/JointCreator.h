#pragma once

#include "Core/../Core.h"
#include "Core/../CoreStructs/Vector.h"
#include "PolygonCreator/PolygonCursor.h"
class b2Body;
class Scene;

class GE_API JointCreator
{
public:
    JointCreator(Scene &scene);
    ~JointCreator()
    {
      
    }
    void SetBody(b2Body *body);

private:
    b2Body *firstBody;
    Scene &scene;
    b2Body *secondBody;
    class Cursor* cursor;

private:
    void OnInput(int input);
    void SetFirstBody(b2Body *firstBody);
    void SetSecondBody(b2Body *secondBody);
    void CreateDistanceJoint();
    void CreateRevoluteJoint();
};
