#include <iostream>
#include <limits>
#include <vector>
#include <cmath>

using namespace std;


template<typename T>
struct BinaryMax:public binary_function<T, T, T>{
    T operator()(const T& x, const T& y) const{return max(x, y);}
    const static T neutral;
};

template <typename T>
const T BinaryMax<T>::neutral = std::numeric_limits<T>::min();

template<typename T>
struct BinaryMin:public binary_function<T, T, T>{
    T operator()(const T& x, const T& y) const{return min(x, y);}
    const static T neutral;
};

template <typename T>
const T BinaryMin<T>::neutral = std::numeric_limits<T>::max();

template<typename T, typename Operation>
class SegmentTree{
private:
    void build(Operation);
    void add_neutrals();
    int query_helper(int, int, Operation);
    vector<int> tree;
    vector<T> v;
public:
    explicit SegmentTree(vector<T>&);
    pair<int, T> query(int x, int y);
};

template<typename T, typename Operation>
SegmentTree<T, Operation>::SegmentTree(vector<T>& segment){
    v = segment;
    Operation func;
    add_neutrals();
    build(func);
};

template <typename T, typename Operation>
void SegmentTree<T, Operation>::build(Operation func){
    size_t n = v.size();
    v.push_back(Operation::neutral);
    tree.resize(2 * n);
    for (int i = 0; i < n; ++i)
        tree[i + n] = i;
    for(int i = n - 1; i > 0; --i) {
        if (func(v[tree[2 * i]], v[tree[2 * i + 1]]) == v[tree[2 * i]]) {
            tree[i] = tree[2 * i];
        } else {
            tree[i] = tree[2 * i + 1];
        }
    }
};

template<typename T, typename Operation>
void SegmentTree<T, Operation>::add_neutrals(){
    int n = v.size(), p = (int)pow(2, floor(log2(n - 0.5) + 1));
    for(int i = n; i < p; ++i)
        v.push_back(Operation::neutral);
};

template<typename T, typename Operation>
int SegmentTree<T, Operation>::query_helper(int l, int r, Operation op) {
    int n = v.size() - 1;
    int ans = n;
    l += n - 1;
    r += n - 1;
    while(l <= r){
        if (l & 1)
            if (v[tree[l]] == op(v[ans], v[tree[l]]))
                ans = tree[l];
        if(!(r & 1))
            if (v[tree[r]] == op(v[ans], v[tree[r]]))
                ans = tree[r];
        l = (l + 1) / 2;
        r = (r - 1) / 2;
    }
    return ans;
};

template<typename T, typename Operation>
pair<int, T> SegmentTree<T, Operation>::query(int x, int y){
    Operation op;
    int pos = query_helper(x, y, op);
    return make_pair(pos + 1, v[pos]);
};

int main() {
    vector<int> c;
    int n, p, q;
    cin >> n;
    for (int i = 0; i < n; ++i){
        cin >> q;
        c.push_back(q);
    }
    pair<int, int> ans;
    SegmentTree<int, BinaryMax<int> > b(c);
    cin >> n;
    for (int i = 0; i < n; ++i){
        cin >> p >> q;
        ans = b.query(p, q);
        cout << ans.second << ' ' << ans.first << endl;
    }
    return 0;
}
