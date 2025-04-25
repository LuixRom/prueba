#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include "ChainHash.h"  // Asegúrate de tener ChainHash.h con las funciones 'end' y operator<< definidos

using namespace std;

// Prototipos
vector<pair<string, string>> loadCSV(string file);
void testUnorderedMap();
void testHashTable();

int main() {
    testUnorderedMap();
    testHashTable();
    return 0;
}


void testUnorderedMap() {
    vector<pair<string, string>> data = loadCSV("smalldata.csv");
    unordered_map<string, string> map;

    for (auto& par : data)
        map[par.first] = par.second;

    int maxCount = 0;
    for (unsigned i = 0; i < map.bucket_count(); ++i) {
        cout << "bucket #" << i << " contains " << map.bucket_size(i) << " elements ";
        for (auto it = map.begin(i); it != map.end(i); ++it)
            cout << "[" << it->first << ":" << it->second << "] ";
        cout << "\n";

        if (map.bucket_size(i) > maxCount)
            maxCount = map.bucket_size(i);
    }

    cout << "Size of Hash Table: " << map.bucket_count() << endl;
    cout << "Max count: " << maxCount << endl;
}


void testHashTable() {
    vector<pair<string, string>> data = loadCSV("smalldata.csv");
    ChainHash<string, string> hash(13);

    for (auto& par : data)
        hash.set(par.first, par.second);

    cout << "Size of the hash table: " << hash.bucket_count() << endl;

    for (int i = 0; i < hash.bucket_count(); i++) {
        cout << "Bucket #" << i << " contains " << hash.bucket_size(i) << " elements: ";
        for (auto it = hash.begin(i); it != hash.end(i); ++it)
            cout << "[" << *it << "] ";
        cout << endl;
    }
}


vector<pair<string, string>> loadCSV(string file) {
    vector<pair<string, string>> data;
    fstream fin;
    fin.open(file, ios::in);
    vector<string> row;
    string line, word;
    int i = 0;

    while (getline(fin, line)) {
        if (i++ == 0) continue;  // Saltar encabezado
        row.clear();
        stringstream s(line);
        while (getline(s, word, ';')) {
            row.push_back(word);
        }
        if (row.size() == 2)
            data.push_back({ row[0], row[1] });
    }

    return data;
}


