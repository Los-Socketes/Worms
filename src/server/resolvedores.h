#ifndef RESOLVEDORES_H
#define RESOLVEDORES_H

#include <box2d/box2d.h>

#include <vector>
#include <atomic>


class ResolvedorColisiones : public b2ContactListener {
public:
    std::atomic<bool> finPartida;
    void BeginContact(b2Contact* contact);
 
    void EndContact(b2Contact* contact);
};

class ResolvedorQuery : public b2QueryCallback {
public:
    std::vector<b2Body*> foundBodies;
      
    bool ReportFixture(b2Fixture* fixture);
};

#endif
