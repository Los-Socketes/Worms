#ifndef TSLIST_H_
#define TSLIST_H_

#include <mutex>
#include <vector>

/*
  La idea de esta clase es crear un monitor a una lista generica de
  tipos/clases que sea thread safe.
  Por temas de c++ y templates la implementacion y declaracion esta
  toda en un unico header file
  La idea es que sea una clase con los usos minimos e indispensables.
  No es plan que sea un wrapper a *la totalidad* de std::list.
  Vamos a tratar de hacer algo pequeno y util.

  "Bonitos y gorditos muchachos"
  			- Skipper
*/
template <typename T>

class TSList {
private:
    std::vector<T> lista;

    std::mutex mtx;

public:
    void append(T nuevoItem) {
        //Lockeamos porque appendear es una RC
        std::unique_lock<std::mutex> lck(mtx);

        this->lista.push_back(nuevoItem);
    }

    // Este metodo se llama at y no get porque devuelve una referencia,
    // no una copia. Yo le hubiese puesto "get", pero _etimologicamente_
    // get implica que vos lo obtenes (llamese, te pasan copia) y eso
    // no es lo que hace.
    [[nodiscard]] T& at(int pos) {
        //Lockeamos porque obtener es una RC*
        //Entendo que es una race condition, porque si alguien esta
        //modificando la lista; vos podrias llegar a ver una vision
        //distorcionada de lo que hay ahi. Puede que este equivocado
        std::unique_lock<std::mutex> lck(mtx);

        T& valor =  this->lista.at(pos);

        return valor;
    }

    void remove(int pos) {
        std::unique_lock<std::mutex> lck(mtx);

        //Esto del lista.begin es de c++ que no se que corno del
        //iterator del bla bla bla. No se, horrible. Es lo que hay.
        //Mientras que usemos esta clase, solo hay que pasar la
        //posicion y listo
        this->lista.erase(this->lista.begin() + pos);
    }

    [[nodiscard]] int size() {
        std::unique_lock<std::mutex> lck(mtx);

        int longitud;
        longitud = this->lista.size();

        return longitud;
    }


};


#endif
