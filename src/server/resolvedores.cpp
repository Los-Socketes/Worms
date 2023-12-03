#include "resolvedores.h"

#include "box2dDefs.h"

#include <iostream>

#include "gusano.h"
#include "provision.h"

void ResolvedorColisiones::BeginContact(b2Contact *contact) {
    // std::cout << "CONTACTO\n";
    
    b2Body* cuerpoA = contact->GetFixtureA()->GetBody();
    b2Body* cuerpoB = contact->GetFixtureB()->GetBody();

    Entidad *entidadA = (Entidad *) cuerpoA->GetUserData().pointer;
    Entidad *entidadB = (Entidad *) cuerpoB->GetUserData().pointer;

    if (entidadB->tipo == TipoEntidad::PROYECTIL
        &&
        entidadA->tipo == TipoEntidad::GUSANO) {
        b2Vec2 dir = cuerpoB->GetLinearVelocity();
        // cuerpoA->ApplyLinearImpulseToCenter(dir, true);
        // printf("A\n");
        entidadA->gusano->recibirDano(dir, entidadB);
        // abort();
    }

    else if (entidadA->tipo == TipoEntidad::PROYECTIL
        &&
        entidadB->tipo == TipoEntidad::GUSANO) {
        b2Vec2 dir = cuerpoA->GetLinearVelocity();
        entidadB->gusano->recibirDano(dir, entidadA);
    }

    else if(entidadA->tipo == TipoEntidad::VIGA
        &&
        entidadB->tipo == TipoEntidad::GUSANO) {
        std::cout << "Viga: " << cuerpoA->GetPosition().y << "\n";
        entidadB->gusano->recibirDanioCaida(cuerpoB->GetLinearVelocity());
    }

    else if(entidadB->tipo == TipoEntidad::VIGA 
        &&
        entidadA->tipo == TipoEntidad::GUSANO) {
        std::cout << "Viga: " << cuerpoB->GetPosition().y << "\n";
        entidadA->gusano->recibirDanioCaida(cuerpoA->GetLinearVelocity());
    }

    else if (entidadA->tipo == TipoEntidad::GUSANO
        &&
        entidadB->tipo == TipoEntidad::ARMA) {

        cuerpoA->ApplyLinearImpulseToCenter(b2Vec2(100.0f, 1000.0f), true);
  }
      
    else if (entidadA->tipo == TipoEntidad::OCEANO
        &&
        entidadB->tipo == TipoEntidad::GUSANO) {
        // b2Vec2 dir = cuerpoA->GetLinearVelocity();
        entidadB->gusano->setEstado(AHOGADO);
        printf("AHOGADO\n");
        // abort();
    }



    //TODO Cambiar estos dos a que no sea solo con viga, sino cualquiera
    else if (entidadA->tipo == TipoEntidad::PROYECTILREAL
	//    &&
	//    entidadB->tipo == TipoEntidad::VIGA
    ) {
        printf("PROYECTIL REAL A\n");
        entidadA->proyectilReal->enElAire = false;
        entidadA->proyectilReal->colisiono = true;
    }

    else if (entidadB->tipo == TipoEntidad::PROYECTILREAL
	//    &&
	//    entidadA->tipo == TipoEntidad::VIGA
    ) {
        printf("PROYECTIL REAL B\n");
        entidadB->proyectilReal->enElAire = false;
        entidadB->proyectilReal->colisiono = true;
    }


    else if (entidadA->tipo == TipoEntidad::PROVISION
        &&
        entidadB->tipo == TipoEntidad::VIGA) {

        entidadA->provision->estaEnElAire = false;
    }
      
    else if (entidadA->tipo == TipoEntidad::VIGA
        &&
        entidadB->tipo == TipoEntidad::PROVISION) {
        entidadB->provision->estaEnElAire = false;
    }

    else if (entidadA->tipo == TipoEntidad::PROVISION
        &&
        entidadB->tipo == TipoEntidad::GUSANO) {
        std::cout << "CACACACACACCACA\n";
        entidadA->provision->fueAgarrada = true;
        entidadA->provision->provisionar(entidadB->gusano);
    }
      
    else if (entidadA->tipo == TipoEntidad::GUSANO
        &&
        entidadB->tipo == TipoEntidad::PROVISION) {
        std::cout << "CACACACACACCACA\n";
        entidadB->provision->fueAgarrada = true;
        entidadB->provision->provisionar(entidadA->gusano);
    }
}

void ResolvedorColisiones::EndContact(b2Contact *contact) {
    //Esto nos evita reads invalidos en caso de que el programa haya
    //terminado
    if (this->finPartida == true)
        return;

    b2Body* cuerpoA = contact->GetFixtureA()->GetBody();
    b2Body* cuerpoB = contact->GetFixtureB()->GetBody();

    Entidad *entidadA = (Entidad *) cuerpoA->GetUserData().pointer;
    Entidad *entidadB = (Entidad *) cuerpoB->GetUserData().pointer;

    if (entidadB->tipo == TipoEntidad::PROYECTIL
        &&
        entidadA->tipo == TipoEntidad::GUSANO) {
        printf("A\n");
        // abort();
    }

    if (entidadA->tipo == TipoEntidad::PROYECTIL
        &&
        entidadB->tipo == TipoEntidad::GUSANO) {
        printf("B\n");
        // abort();
    }

    if(entidadA->tipo == TipoEntidad::VIGA
       &&
       entidadB->tipo == TipoEntidad::GUSANO) {
        if (entidadB->gusano->getEstado() != SALTANDO &&
            entidadB->gusano->getEstado() != HACE_PIRUETA &&
            entidadB->gusano->getEstado() != HERIDO) {
            entidadB->gusano->setEstado(CAYENDO);
        }
    }

    if(entidadB->tipo == TipoEntidad::VIGA
       &&
       entidadA->tipo == TipoEntidad::GUSANO) {
        if (entidadA->gusano->getEstado() != SALTANDO &&
            entidadA->gusano->getEstado() != HACE_PIRUETA &&
            entidadA->gusano->getEstado() != HERIDO) {
            entidadA->gusano->setEstado(CAYENDO);
        }
    }
    // std::cout << "FIN CONTACTO\n";
}

bool ResolvedorQuery::ReportFixture(b2Fixture* fixture) {
    foundBodies.push_back( fixture->GetBody() ); 
    return true;//keep going to find all fixtures in the query area
}
