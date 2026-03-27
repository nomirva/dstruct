#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> ivec;
    // cout << "ivec -> len: " << ivec.size() << " cap: "  << ivec.capacity() << endl;
    
    for (size_t i = 0; i < 10; i++) {
    ivec.push_back(i);
    cout << "ivec -> len: " << ivec.size() << " cap: "  << ivec.capacity() << endl;
    }
    
    
    cout << "ivec -> cap: " << ivec.capacity() << endl;
    ivec.reserve(1200);
    cout << "ivec -> cap: " << ivec.capacity() << endl;
    
    for (int ix = 0; ix < 1200; ++ix) {
        ivec.push_back(ix);
    }
    cout << "ivec -> len: " << ivec.size() << " cap: "  << ivec.capacity() << endl;

    for (int ix = 0; ix < 10; ++ix) {
        ivec.push_back(ix);
    }
    cout << "ivec -> len: " << ivec.size() << " cap: "  << ivec.capacity() << endl;


    ivec.erase(ivec.begin(), ivec.end());
    cout << "ivec -> len: " << ivec.size() << " cap: "  << ivec.capacity() << endl;
    ivec.shrink_to_fit();
    cout << "ivec -> len: " << ivec.size() << " cap: "  << ivec.capacity() << endl;
}