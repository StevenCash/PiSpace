#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include <Box2D/Box2D.h>
#include <iostream>
#include "GenericDrawing.h"

class DebugDraw : public b2Draw
{
 private:
    GenericDrawing drawingObject;
 public:
  /// Draw the bounding box
    virtual void DrawPolygon(const b2Vec2* /*vertices*/, int32 /*vertexCount*/, const b2Color& /*color*/)
  {
      //Do nothing
//      drawingObject.Draw(vertices,vertexCount);

  }
  
  /// Draw a solid closed polygon provided in CCW order.
  virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
  {
      drawingObject.Draw(vertices,vertexCount, color);
  }
  
  /// Draw a circle.
    virtual void DrawCircle(const b2Vec2& /*center*/, float32 /*radius*/, const b2Color& /*color*/)
  {
    //do nothing
  }
  
  /// Draw a solid circle.
    virtual void DrawSolidCircle(const b2Vec2& /*center*/, float32 /*radius*/, const b2Vec2& /*axis*/, const b2Color& /*color*/)
  {
    //do nothing
  }
  
  /// Draw a line segment.
  virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
  {
      b2Vec2 vertices[2];
      vertices[0] = p1;
      vertices[1] = p2;
      drawingObject.Draw(vertices,2,color);
  }
  
  /// Draw a transform. Choose your own length scale.
  /// @param xf a transform.
    virtual void DrawTransform(const b2Transform& /* xf */)
  {
    //do nothing
  }
  
};

#endif
