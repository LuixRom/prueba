#include <unordered_map>
#include <forward_list>
#include <vector>

using namespace std;

const float maxFillfactor = 0.5; //Esto me dice después de cuanto llenado
const int maxColision = 4; //Esto sirve para saber cuantas colisiones maximas puede existir

template<typename TK, typename TV>  //Estos son los STL para que salga el ChainHash
class ChainHash {
private:
    struct Entry{
        TK key;         //Este es el key del hash map
        TV value;       //Este es el valor del hash map
        Entry(TK k, TV v){ //Constructor para esta estructura
            key = k; value = v;
        }
        friend ostream& operator<<(ostream& os, const Entry& e) {
            os << e.key << ":" << e.value;
            return os;
        }
    };
    forward_list<Entry>* array; //Nuestro array es un forward_list
    int size;   //Reprenta al numero total de elementos almacenados actualmente
    int capacity;  //Esta es al capacidad que tenemos del array en linkend list
public:
    ChainHash(int _capacity=10){
        array = new forward_list<Entry>[_capacity];
        capacity = _capacity;
        size = 0;
    }

    void set(TK key, TV value){
        if(fillFactor() >= maxFillfactor) rehashing();
        int hashCode = getHashCode(key);//[0 - 10000000]
        int index = hashCode % capacity;//[0 - capacity]
        //TODO: - Insertar el Entry(key, value) en index, manejando colisiones.
        //      - Si el index no tiene elementos, incrementar bucketCount.
        //      - Si la colision es mayor a maxColision, rehashing()
        for(auto& entry : array[index]) {
            if(entry.key == key) {
                entry.value = value;
                return;
            }
        }
        array[index].push_front(Entry(key, value));
        size++;

        int collidionCode= distance(array[index].begin(), array[index].end());
        if(collidionCode >= maxColision) rehashing();
    }

    TV get(TK key){
        int hashCode = getHashCode(key);
        int index = hashCode % capacity;
        //TODO: buscar el Entry(key, value) en index y devolver el value
        for(auto& entry : array[index]) {
            if(entry.key == key) return entry.value;
        }

        throw std::out_of_range("LLave no encontrada");
    }

    bool remove(TK key){
        //TODO: eliminar el Entry(key, value) en index y
        //      devolver true si lo encontro, false si no lo encontro
        int hashCode = getHashCode(key);
        int index = hashCode % capacity;
        for(auto& entry : array[index]) {
            if (key == entry.key) {
                array[index].erase_after(entry);
                size--;
                return true;
            }
        }
        return false;

    }

    typename forward_list<Entry>::iterator begin(int index){//O(1)
        //TODO: devolver el iterador al inicio de la lista en el index
        return array[index].begin();
    }
    typename forward_list<Entry>::iterator end(int index) {
        return array[index].end();
    }


    int bucket_size(int index){//O(1)
        //TODO: devolver el tamaño de la lista en el index
        return distance(array[index].begin(), array[index].end());
    }

    int bucket_count(){//O(1)
        return capacity;
    }

private:
    double fillFactor(){//O(1)
        return double(size) / (capacity * maxColision);
    }

    size_t getHashCode(TK key){   //Así es como hace la fucnión hashing
        std::hash<string> hasher;
        return hasher(key);
    }

    void rehashing(){
        //TODO: crear un nuevo array de forward_list<Entry> con el doble de capacidad
        //      y volver a insertar todos los elementos en el nuevo array
        int capacidad_anterior= capacity;
        capacity*=  2;

        forward_list<Entry>* newArray= new forward_list<Entry>[capacity];
        for(int i=0; i<= capacidad_anterior; i++) {
            for(auto& entry : array[i]) {
                int hashCode = getHashCode(entry.key);
                int newindex = hashCode % capacity;
                newArray[newindex].push_front(Entry(entry.key,entry.value));
            }
        }
        delete[] array;
        array = newArray;
    }
};
