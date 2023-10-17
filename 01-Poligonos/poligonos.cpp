#include <iostream>
#include <cassert>
#include <array>
using std::array;
using std::cin;
using std::cout;

struct Punto{ 
    double x, y;    // Puntos plano (x,y);
    bool IsEqual(Punto punto){ return punto.x == x and punto.y == y; }; // cambiar a comparación de doubles.
};    

struct Poligono{
    array<Punto, 100> puntos;        // Poligno esta formado por un conjunto de puntos
    unsigned size;                  // Para determianr cuantos puntos fueron agregados en el array
    void printPoligono(){
        cout<< "El poligono esta conformado por:" << '\n';
        for(unsigned i{}; i < puntos.size(); i++)
            cout<< i << " - ( " << puntos.at(i).x << ", " << puntos.at(i).y << ")\n";
        cout<< "Las primeras  " << size << " posiciones del poligono son válidas (0 - " << size - 1 << ")\n";
    }
};

/*
    Push:   Poligono x Punto -> Poligono x bool
    Pop:    Poligono -> Poligono x Punto x bool
    Top:    Poligono -> Punto
    Add:    Poligono x Punto x N -> Poligono
    Remove: Poligono x Punto -> Poligono x bool
    Get:    Poligono x N -> Punto x bool
    Set:    Poligono x Punto x Punto -> Poligono x bool
    SetByIndex:     Poligono x N x Punto -> Poligono x bool
    GetIndex        Poligono x Punto -> N
    RemoveByIndex:  Poligono x N -> Poligono x Punto x bool

    Tener en cuenta que al armar las funciones, algunas los tipos devueltos son por referencia pero se escriben igual matematicamente
*/

// Se puede hacer overload de funcionews para que matcheen distintos parametros.
Poligono& push(Poligono&, Punto);              
Poligono& push(Poligono&, Punto, bool&);        // Devuelvo referencia a poligono para poder hacer fluido de no se que verga
Poligono& pop(Poligono&);
Poligono& pop(Poligono&, bool&);
Poligono& pop(Poligono&, Punto&);
Poligono& pop(Poligono&, Punto&, bool&);        // Lo mismo, pero tambien devuelvo el punto por referencia por si les interesa tener el valor devuelto
Poligono& add(Poligono&, Punto, unsigned);      // Devuelvo referencia a poligono para poder hacer fluido de no se que verga
Punto top(const Poligono&);                     // No se modifica el poligono y se devuelve el ultimo de la lista
bool remove(Poligono&, Punto);                  // Asumo que normalmente vamos a querer eliminar un valor en especifico, no volver a llamar reiteredas veces
bool get(const Poligono&, unsigned, Punto&);    // No modifico poliugono de entrada, y el punto lo devuelvo por referencia para poder avisar si lo devuelto es valido o no con un bool
bool set(Poligono&, Punto, Punto);              // Para confirmar que se puedo setear el valor pedido
bool setByIndex(Poligono&, unsigned, Punto);    // Lo mismo que el anterior pero seteando por indice
int getIndex(Poligono&, Punto);                 // Devuelvo un int nomas, ya que al buscar el indice, si mandamos -1 ya sabemos que no se encontro o lo que sea.
bool removeByIndex(Poligono&, unsigned, Punto&);// Lo mismo que remove pero, pasando indice en vez de punto.

int main () {
    Poligono p{ {{ {2,3}, {3,3} }}, 2};
    Poligono full { {{ {5,6}, {61,23} }}, 100};
    bool valid;
    
    Punto punto{2, 3};
    assert(punto.IsEqual({2, 3}));

    push(p, { 4, 3});
    assert(p.size == 3);
    assert(p.puntos.at(2).IsEqual({4, 3}));

    push(p, {5, 3}, valid);
    assert(valid && p.size == 4);
    assert(p.puntos.at(3).IsEqual({5, 3}));

    push(full, {5, 3}, valid);
    assert(!valid && full.size == 100);

    push(push(p, {5, 6}), {2, 7});
    assert(p.size == 6);
    assert(p.puntos.at(4).IsEqual({5, 6}));
    assert(p.puntos.at(5).IsEqual({2, 7}));

    pop(p);
    assert(p.size == 5);

    pop(pop(p));
    assert(p.size == 3);

    pop(p, valid);
    assert(p.size == 2 && valid);

    pop(p, punto);
    assert(p.size == 1);
    assert(punto.IsEqual({3, 3}));

    pop(p, punto, valid);
    assert(p.size == 0);
    assert(punto.IsEqual({2, 3}) && valid);

    pop(p, punto, valid);
    assert(p.size == 0 && !valid);
    assert(punto.IsEqual({2, 3}));

    add(p, {7, 3}, 0);
    assert(p.size == 1);
    assert(p.puntos.at(0).IsEqual({7, 3}));

    add(p, {-3, -9}, 49);
    assert(p.size == 50);
    assert(p.puntos.at(49).IsEqual({-3, -9}));

    add(p, {-5, -5}, 24);
    assert(p.size == 51);
    assert(p.puntos.at(50).IsEqual({-3, -9}));
    assert(p.puntos.at(24).IsEqual({-5, -5}));

    add(p, {-1, -1}, 0);
    assert(p.size == 52);
    assert(p.puntos.at(0).IsEqual({-1, -1}));
    assert(p.puntos.at(1).IsEqual({7, 3}));
    assert(p.puntos.at(51).IsEqual({-3, -9}));
    assert(p.puntos.at(25).IsEqual({-5, -5}));

    assert(p.puntos.at(p.size - 1).IsEqual(top(p)));

    assert(remove(p, {7, 3}));
    assert(p.size == 51);
    assert(p.puntos.at(0).IsEqual({-1, -1}));
    assert(p.puntos.at(1).IsEqual({3, 3}));
    assert(p.puntos.at(50).IsEqual({-3, -9}));
    assert(p.puntos.at(24).IsEqual({-5, -5}));

    assert(!remove(p, {70, 33}));
    assert(p.size == 51);
    assert(p.puntos.at(0).IsEqual({-1, -1}));
    assert(p.puntos.at(1).IsEqual({3, 3}));
    assert(p.puntos.at(50).IsEqual({-3, -9}));
    assert(p.puntos.at(24).IsEqual({-5, -5}));

    assert(remove(p, {-3, -9}));
    assert(p.size == 50);
    assert(p.puntos.at(0).IsEqual({-1, -1}));
    assert(p.puntos.at(1).IsEqual({3, 3}));
    assert(p.puntos.at(24).IsEqual({-5, -5}));

    assert(get(p, 1, punto));
    assert(p.puntos.at(1).IsEqual(punto));

    assert(!get(p, 101, punto));
    assert(!get(p, 50, punto));
    assert(p.puntos.at(1).IsEqual(punto));

    assert(set(p, punto, {34, 23}));
    assert(p.puntos.at(1).IsEqual({34,23}));

    assert(!set(p, punto, {23, 34}));
    assert(p.puntos.at(1).IsEqual({34,23}));

    assert(!setByIndex(p, 55, punto));
    assert(setByIndex(p, 24, {-23, -23}));
    assert(p.puntos.at(24).IsEqual({-23, -23}));
    
    assert(getIndex(p, {-23, -23}) == 24);
    assert(getIndex(p, {-230, -23}) == -1);

    assert(removeByIndex(p, 0, punto));
    assert(p.size == 49);
    assert(punto.IsEqual({-1, -1}));
    assert(p.puntos.at(0).IsEqual({34, 23}));
    assert(p.puntos.at(23).IsEqual({-23, -23}));
    
    assert(!removeByIndex(p, 49, punto));
    assert(!removeByIndex(p, 103, punto));
    assert(p.size == 49);
    assert(punto.IsEqual({-1, -1}));
    assert(p.puntos.at(0).IsEqual({34, 23}));
    assert(p.puntos.at(23).IsEqual({-23, -23}));

    p.printPoligono();
}

Poligono& push(Poligono& p, Punto nuevoPunto){
    if(p.size < p.puntos.size()) {
        p.puntos.at(p.size) = nuevoPunto;
        p.size++;
    }

    return p;
}

Poligono& push(Poligono& p, Punto nuevoPunto, bool& isValid){
    if(p.size < p.puntos.size()) {
        p.puntos.at(p.size) = nuevoPunto;
        p.size++;
        isValid = true;
    } else isValid = false;

    return p;
}

Poligono& pop(Poligono& p){
    if(p.size > 0){
        p.size--;
    }

    return p;
}

Poligono& pop(Poligono& p, bool& isValid){
    if(p.size > 0){
        p.size--;
        isValid = true;
    } else isValid = false;

    return p;
}

Poligono& pop(Poligono& p, Punto& puntoRemoved){
    if(p.size > 0){
        p.size--;
        puntoRemoved = p.puntos.at(p.size);
    }

    return p;
}

Poligono& pop(Poligono& p, Punto& puntoRemoved, bool& isValid){
    if(p.size > 0){
        p.size--;
        puntoRemoved = p.puntos.at(p.size);
        isValid = true;
    } else isValid = false;

    return p;
}

Poligono& add(Poligono& p, Punto punto, unsigned index){
    for(unsigned i{p.size}; i > index; i--)
        p.puntos.at(i) = p.puntos.at(i - 1);
    
    p.puntos.at(index) = punto;

    if(p.size < index) p.size = index + 1;
    else p.size++;
        
    return p;
}

Punto top(const Poligono& p) {
    return p.puntos.at(p.size - 1);
}

bool remove(Poligono& p, Punto puntoToRemove){
    bool flag = false;
    
    for(unsigned i{}; i < p.size; i++){
        if (flag) p.puntos.at(i - 1) = p.puntos.at(i);                  // Desde el punto a borrar al final hay que mover todo una posicion para abajo (ya que saco un elemento)
        if(p.puntos.at(i).IsEqual(puntoToRemove) && !flag) flag = true; // encuentro el punto a borrar
    }

    if(flag) p.size--; // Si encontre un elemento, al finalzar todo borro la posicion extra que me queda, no lo hago antes por el for

    return flag;
}

bool get(const Poligono& p, unsigned index, Punto& punto){
    if (index >= p.size) return false;
    
    punto = p.puntos.at(index);

    return true;
}  

bool set(Poligono& p, Punto oldPunto, Punto newPunto){
    for(unsigned i{}; i < p.size; i++){
        if (p.puntos.at(i).IsEqual(oldPunto)) {
            p.puntos.at(i) = newPunto;
            return true;
        }
    }

    return false;
}  

bool setByIndex(Poligono& p, unsigned index, Punto newPunto){
    if(index > p.size) return false;

    p.puntos.at(index) = newPunto;

    return true;
}

int getIndex(Poligono& p, Punto punto){
    for(unsigned i{}; i < p.size; i++)
        if (p.puntos.at(i).IsEqual(punto)) return i;
    
    return -1;
}

bool removeByIndex(Poligono& p, unsigned index, Punto& puntoRemoved){
    if (index >= p.size) return false; 

    puntoRemoved = p.puntos.at(index);

    for(unsigned i{index + 1}; i < p.size; i++)
        p.puntos.at(i - 1) = p.puntos.at(i);       

    p.size--;

    return true;          
}