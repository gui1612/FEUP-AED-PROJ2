#pragma once

#include <vector>
#include <unordered_map>

#define LEFT(i) (2*(i))
#define RIGHT(i) (2*(i)+1)
#define PARENT(i) ((i)/2)

using namespace std;

// Binary min-heap to represent integer keys of type K with values (priorities) of type V
template <class K, class V>
class MinHeap {

    /** 
     * An element of the heap: a pair (key, value) 
     */
    struct Node {
        K key;
        V value;
    };

    int size;                  // Number of elements in heap
    int max_size;              // Maximum number of elements in heap
    vector<Node> a;            // The heap array
    unordered_map<K, int> pos; // maps a key into its position on the array a
    const K KEY_NOT_FOUND;

    /**
     * @brief Make a value go "up the tree" until it reaches its position
     * 
     * @param i 
     */
    void up_heap(int i);

    /**
     * @brief Make a value go "down the tree" until it reaches its position
     * 
     * @param i 
     */
    void down_heap(int i);
    
    /**
     * @brief Swap two positions of the heap (update their positions)
     * 
     * @param i1 
     * @param i2 
     */
    void swap(int i1, int i2);

public:
    /**
     * @brief Construct a new MinHeap object for a max of n pair (K, V)
     * 
     * @param n max of (K, V) pairs
     * @param not_found value returned when the MinHeap is empty
     * 
     * @note Returns notFound when empty 
     */
    MinHeap(int n, const K& not_found);

    /**
     * @brief Gets the number of elements in the heap
     * 
     * @return int  Number of elements in the heap
     */
    int get_size();

    /**
     * @brief Returns a boolean that represents wether the heap has a certain `key`
     * 
     * @param key key to be search
     * @return true heap contains the key
     * @return false heap does not contain the key
     */
    bool has_key(const K& key);

    /**
     * @brief Inserts (keey, value) on the heap
     */
    void insert(const K& key, const V& value);

    /**
     * @brief  Decreases value of key
     * 
     * @param key Value of the key
     * @param value Amount to be decreased
     */
    void decrease_key(const K& key, const V& value);

    /**
     * @brief Removes as returns the key with the smallest value
     * 
     * @return K Key with the smallest value
     */
    K remove_min();
};

// ----------------------------------------------

template <class K, class V>
void MinHeap<K,V>::up_heap(int i) {
    while (i>1 && a[i].value < a[PARENT(i)].value) { // while pos smaller than parent, keep swapping to upper position
        swap(i, PARENT(i));
        i = PARENT(i);
    }
}

template <class K, class V>
void MinHeap<K,V>::down_heap(int i) {
    while (LEFT(i) <= size) { // while within heap limits
        int j = LEFT(i);
        if (RIGHT(i)<=size && a[RIGHT(i)].value < a[j].value) j = RIGHT(i); // choose smaller child
        if (a[i].value < a[j].value) break;   // node already smaller than children, stop
        swap(i, j);                    // otherwise, swap with smaller child
        i = j;
    }
}

template <class K, class V>
void MinHeap<K,V>::swap(int i1, int i2) {
    Node tmp = a[i1]; a[i1] = a[i2]; a[i2] = tmp;
    pos[a[i1].key] = i1;
    pos[a[i2].key] = i2;
}

// ----------------------------------------------

template <class K, class V>
MinHeap<K,V>::MinHeap(int n, const K& notFound) : KEY_NOT_FOUND(notFound), size(0), max_size(n), a(n+1) {
}

template <class K, class V>
int MinHeap<K,V>::get_size() {
    return size;
}

template <class K, class V>
bool MinHeap<K, V>::has_key(const K& key) {
    return pos.find(key) != pos.end();
}

template <class K, class V>
void MinHeap<K,V>::insert(const K& key, const V& value) {
    if (size == max_size) return; // heap is full, do nothing
    if (has_key(key)) return;     // key already exists, do nothing
    a[++size] = {key, value};
    pos[key] = size;
    up_heap(size);
}

template <class K, class V>
void MinHeap<K,V>::decrease_key(const K& key, const V& value) {
    if (!has_key(key)) return; // key does not exist, do nothing
    int i = pos[key];
    if (value > a[i].value) return; // value would increase, do nothing
    a[i].value = value;
    up_heap(i);
}

template <class K, class V>
K MinHeap<K,V>::remove_min() {
    if (size == 0) return KEY_NOT_FOUND;
    K min = a[1].key;
    pos.erase(min);
    a[1] = a[size--];
    down_heap(1);
    return min;
}