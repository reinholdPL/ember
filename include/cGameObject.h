#pragma once

#include "cTransform.h"
class cGameObject
{
private:
    cTransform _transform;

public:
//get transofm ref
    cTransform &getTransform() { return _transform; }
    void setTransform(cTransform transform) { _transform = transform; }
    cGameObject() {}
    cGameObject(cTransform transform) : _transform(transform) {}
    ~cGameObject() {}
};