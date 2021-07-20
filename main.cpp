#include <bits/stdc++.h>
using namespace std;
/******************************************************************************/
bool bellman_ford(vector<vector<pair<int, int>>> edges, vector<int> &h){
    int n = edges.size();
    bool neg_cycle = false, flag = false;
    vector<pair<int, int>> new_vertex;

    for(int i = 0; i < n; i++){
        new_vertex.push_back({i, 0});
    }

    edges.push_back(new_vertex);

    h[n] = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j <= n; j++){
            for(int k = 0; k < edges[j].size(); k++){
                int u = j, v = edges[j][k].first, w = edges[j][k].second;
                if(h[u] != 999999 && h[u] + w < h[v]){
                    h[v] = h[u] + w;
                    flag = true;
                }
            }
        }

        if(flag == true){
            flag = false;
        }
        else{
            break;
        }
    }

    for(int j = 0; j <= n; j++){
        for(int k = 0; k < edges[j].size(); k++){
            int u = j, v = edges[j][k].first, w = edges[j][k].second;
            if(h[u] != 999999 && h[u] + w < h[v]){
                neg_cycle = true;
                break;
            }
        }
    }

    return neg_cycle;
}
/******************************************************************************/
int find_min(vector<int> path, vector<bool> included){
    int min = 999999, index = -1;

    for(int i = 0; i < path.size(); i++){
        if(!included[i] && path[i] < min){
            min = path[i];
            index = i;
        }
    }

    return index;
}

void dijkstra_using_array(vector<vector<pair<int, int>>> edges, vector<int> h, bool flag){
    int n = edges.size();

    for(int i = 0; i < n; i++){
        vector<int> path(n, 999999);
        vector<bool> included(n, false);
        path[i] = 0; //Set path of source vertex to zero

        //Dijkstra
        for(int j = 0; j < n - 1; j++){
            int u = find_min(path, included);
            if(u == -1) break;
            included[u] = true;

            for(int k = 0; k < edges[u].size(); k++){
                int v = edges[u][k].first, w = edges[u][k].second;
                if(!included[v] && path[u] != 999999 && path[u] + w < path[v]){
                    path[v] = path[u] + w;
                }
            }
        }

        //Print output
        for(int j = 0; j < n; j++){
            if(flag && path[j] != 999999){
                cout << path[j] + h[j] - h[i];
            }
            else{
                cout << path[j];
            }
            cout << " ";
        }

        cout << endl;
    }
}
/******************************************************************************/
void heapify(vector<pair<int, int>> &heap, int i, vector<int> &in_heap){
    int l = 2*i + 1, r = 2*i + 2, s = i;

    if(l < heap.size() && heap[l].first < heap[s].first){
        s = l;
    }

    if(r < heap.size() && heap[r].first < heap[s].first){
        s = r;
    }

    if(s != i){
        swap(heap[i], heap[s]);
        swap(in_heap[heap[i].second], in_heap[heap[s].second]);
        heapify(heap, s, in_heap);
    }
}

int extract_min(vector<pair<int, int>> &heap, vector<int> &in_heap){
    int x = heap[0].second;
    in_heap[x] = -1;

    if(heap.size() == 1){
        heap.pop_back();
    }
    else{
        heap[0] = heap[heap.size() - 1];
        in_heap[heap[0].second] = 0;
        heap.pop_back();
        heapify(heap, 0, in_heap);
    }

    return x;
}

void insert(vector<pair<int, int>> &heap, pair<int, int> p, vector<int> &in_heap){
    heap.push_back(p);
    int i = heap.size() - 1;
    in_heap[p.second] = i;

    while(i != 0 && heap[(i-1)/2].first > heap[i].first){
        swap(heap[i], heap[(i-1)/2]);
        swap(in_heap[heap[i].second], in_heap[heap[(i-1)/2].second]);
        i = (i-1)/2;
    }
}

void decrease_key(vector<pair<int, int>> &heap, pair<int, int> p, vector<int> &in_heap){
    heap[in_heap[p.second]].first = p.first;
    int i = in_heap[p.second];

    while(i != 0 && heap[(i-1)/2].first > heap[i].first){
        swap(heap[i], heap[(i-1)/2]);
        swap(in_heap[heap[i].second], in_heap[heap[(i-1)/2].second]);
        i = (i-1)/2;
    }
}

void dijkstra_using_binary_heap(vector<vector<pair<int, int>>> edges, vector<int> h, bool flag){
    int n = edges.size();

    for(int i = 0; i < n; i++){
        vector<int> path(n, 999999);
        path[i] = 0; //Set path of source vertex to zero

        vector<pair<int, int>> heap;
        heap.push_back({0, i});

        vector<int> in_heap(n, -1);
        in_heap[i] = 0;

        //Dijkstra
        while(!heap.empty()){
            int x = extract_min(heap, in_heap);

            for(int j = 0; j < edges[x].size(); j++){
                int y = edges[x][j].first;
                int w = edges[x][j].second;

                if(path[x] + w < path[y]){
                    path[y] = path[x] + w;

                    if(in_heap[y] == -1){
                        insert(heap, {path[y], y}, in_heap);
                    }
                    else{
                        decrease_key(heap, {path[y], y}, in_heap);
                    }
                }
            }
        }

        //Print output
        for(int j = 0; j < n; j++){
            if(flag && path[j] != 999999){
                cout << path[j] + h[j] - h[i];
            }
            else{
                cout << path[j];
            }
            cout << " ";
        }

        cout << endl;
    }
}
/******************************************************************************/
typedef struct binomial_heap_node{
    int degree;
    pair<int, int> data;
    struct binomial_heap_node* parent;
    struct binomial_heap_node* child;
    struct binomial_heap_node* sibling;
}node;

node* new_binomial_heap_node(pair<int, int> p){
    node* x = new node;
    x -> degree = 0;
    x -> data = p;
    x -> parent = NULL;
    x -> child = NULL;
    x -> sibling = NULL;
    return x;
}

//Union two binomial heaps in increasing order of degree of binomial trees
list<node*> union_binomial_heaps(list<node*> heap1, list<node*> heap2){
    list<node*> final;
    list<node*> :: iterator h1, h2;
    h1 = heap1.begin(); h2 = heap2.begin();

    while(h1 != heap1.end() && h2 != heap2.end()){
        if((*h1) -> degree <= (*h2) -> degree){
            final.push_back(*h1);
            h1++;
        }
        else{
            final.push_back(*h2);
            h2++;
        }
    }

    while(h1 != heap1.end()){
        final.push_back(*h1);
        h1++;
    }

    while(h2 != heap2.end()){
        final.push_back(*h2);
        h2++;
    }

    return final;
}

//Merge two binomial trees making one child of other
node* merge_binomial_trees(node* tree1, node* tree2){
    if((tree1 -> data).first > (tree2 -> data).first){
        swap(tree1, tree2);
    }

    tree2 -> parent = tree1;
    tree2 -> sibling = tree1 -> child;
    tree1 -> child = tree2;
    (tree1 -> degree) += 1;

    return tree1;
}

//Merge binomial heap such that number of trees of a particular order if any is one
list<node*> merge_binomial_heap(list<node*> heap){
    if(heap.size() <= 1){
        return heap;
    }

    list<node*> :: iterator curr, next, next_next;
    curr = next = next_next = heap.begin();

    if(heap.size() == 2){
        next++;
        next_next = heap.end();
    }
    else{
        next++;
        next_next = next;
        next_next++;
    }

    while(curr != heap.end()){
        if(next == heap.end()){
            curr++;
        }
        else if((*curr) -> degree < (*next) -> degree){
            curr++;
            next++;
            if(next_next != heap.end()){
                next_next++;
            }
        }
        else if((next_next != heap.end()) && (((*curr) -> degree) == ((*next) -> degree)) && (((*next) -> degree) == ((*next_next) -> degree))){
            curr++;
            next++;
            next_next++;
        }
        else if(((*curr) -> degree) == ((*next) -> degree)){
            *curr = merge_binomial_trees(*curr, *next);
            next = heap.erase(next);
            if(next_next != heap.end()){
                next_next++;
            }
        }
    }

    return heap;
}

list<node*> insert_in_binomial_heap(list<node*> heap, pair<int, int> p, vector<node*> &in_heap){
    node* x = new_binomial_heap_node(p);
    in_heap[p.second] = x;

    list<node*> heap2;
    heap2.push_back(x);

    heap = union_binomial_heaps(heap, heap2);
    heap = merge_binomial_heap(heap);

    return heap;
}

node* min_in_binomial_heap(list<node*> heap){
    node* min = *(heap.begin());
    list<node*> :: iterator it = heap.begin();

    while(it != heap.end()){
        if(((*it) -> data).first < (min -> data).first){
            min = *it;
        }
        it++;
    }

    return min;
}

//Returns a binomial heap consisting of the children of the root
list<node*> binomial_heap_from_min(node* min){
    list<node*> heap;
    node *temp1 = min -> child, *temp2;

    while(temp1){
        temp2 = temp1;
        temp1 = temp1 -> sibling;
        temp2 -> sibling = NULL;
        temp2 -> parent = NULL;
        heap.push_front(temp2);
    }

    return heap;
}

list<node*> delete_min_from_binomial_heap(list<node*> heap){
    node* min = min_in_binomial_heap(heap);
    list<node*> heap2 = binomial_heap_from_min(min);

    list<node*> :: iterator it = heap.begin();
    while(it != heap.end()){
        if((*it) == min){
            heap.erase(it);
            break;
        }
        else{
            it++;
        }
    }

    delete min;

    heap = union_binomial_heaps(heap, heap2);
    heap = merge_binomial_heap(heap);

    return heap;
}

void decrease_key_in_binomial_heap(vector<node*> &in_heap, int y, int path_y){
    (in_heap[y] -> data).first = path_y;

    node* child = in_heap[y];
    node* parent = in_heap[y] -> parent;

    while(parent != NULL && (parent -> data).first > (child -> data).first){
        swap(parent -> data, child -> data);
        swap(in_heap[(parent -> data).second], in_heap[(child -> data).second]);
        child = parent;
        parent = parent -> parent;
    }
}

void dijkstra_using_binomial_heap(vector<vector<pair<int, int>>> edges, vector<int> h, bool flag){
    int n = edges.size();
    for(int i = 0; i < n; i++){
        vector<int> path(n, 999999);
        path[i] = 0;

        list<node*> heap;
        heap.push_back(new_binomial_heap_node({0, i}));

        vector<node*> in_heap(n, NULL);
        in_heap[i] = *heap.begin();

        //Dijkstra
        while(!heap.empty()){
            node* z = min_in_binomial_heap(heap);
            int x = (z -> data).second;
            heap = delete_min_from_binomial_heap(heap);
            in_heap[x] = NULL;

            for(int j = 0; j < edges[x].size(); j++){
                int y = edges[x][j].first;
                int w = edges[x][j].second;

                if(path[x] + w < path[y]){
                    path[y] = path[x] + w;

                    if(in_heap[y] == NULL){
                        heap = insert_in_binomial_heap(heap, {path[y], y}, in_heap);
                    }
                    else{
                        decrease_key_in_binomial_heap(in_heap, y, path[y]);
                    }
                }
            }
        }

        //Print Output
        for(int j = 0; j < n; j++){
            if(flag && path[j] != 999999){
                cout << path[j] + h[j] - h[i];
            }
            else{
                cout << path[j];
            }
            cout << " ";
        }

        cout << endl;
    }
}
/******************************************************************************/
typedef struct fibonacci_heap_node{
    int key;
    int degree;
    bool marked;
    struct fibonacci_heap_node* parent;
    struct fibonacci_heap_node* child;
    struct fibonacci_heap_node* left;
    struct fibonacci_heap_node* right;
}Node;

Node* new_fibonacci_heap_node(int data){
    Node* x = new Node;
    x -> key = data;
    x -> degree = 0;
    x -> marked = false;
    x -> parent = NULL;
    x -> child = NULL;
    x -> left = x;
    x -> right = x;
    return x;
}

Node* insert_in_fibonacci_heap(Node* heap, int key, vector<int> path, int* number_of_nodes, vector<Node*> &in_heap){
    Node* x = new_fibonacci_heap_node(key);
    in_heap[key] = x;

    if(heap == NULL){
        heap = x;
    }
    else{
        //Insert
        (heap -> left) -> right = x;
        x -> right = heap;
        x -> left = heap -> left;
        heap -> left = x;

        if(path[x -> key] < path[heap -> key]){
            heap = x;
        }
    }

    (*number_of_nodes)++;
    return heap;
}

//Makes one node child of another after removing from main circular doubly linked list
Node* link_fibonacci_nodes(Node* n2, Node* n1, vector<int> path){
    //Remove
    (n2 -> left) -> right = n2 -> right;
    (n2 -> right) -> left = n2 -> left;

    n2 -> left = n2;
    n2 -> right = n2;
    n2 -> parent = n1;

    if(n1 -> child == NULL){
        n1 -> child = n2;
    }
    else{
        //Insert
        n2 -> right = n1 -> child;
        n2 -> left = (n1 -> child) -> left;
        ((n1 -> child) -> left) -> right = n2;
        (n1 -> child) -> left = n2;

        if(path[n2 -> key] < path[(n1 -> child) -> key]){
            n1 -> child = n2;
        }
    }

    (n1 -> degree)++;
    n2 -> marked = false;

    return n1;
}

//Consolidates fibonacci heap
Node* consolidate_fibonacci_heap(Node* heap, vector<int> path, int* number_of_nodes){
    int r = (int)(log2(*number_of_nodes) / log2(1.618));

    Node* arr[r+1];
    for(int i = 0; i <= r; i++){
        arr[i] = NULL;
    }

    Node* temp1 = heap, *temp2;
    bool flag = false;

    while(true){
        int x = temp1 -> degree;

        while(arr[x] != NULL){
            temp2 = arr[x];

            if(temp1 == temp2){
                flag = true;
                break;
            }

            if(path[temp1 -> key] > path[temp2 -> key]){
                swap(temp1, temp2);
            }

            temp1 = link_fibonacci_nodes(temp2, temp1, path);
            arr[x++] = NULL;
        }
        if(flag) break;

        arr[x] = temp1;
        temp1 = temp1 -> right;
    }

    heap = temp1;
    temp2 = temp1;

    do{
        if(path[temp2 -> key] < path[heap -> key]){
            heap = temp2;
        }
        temp2 = temp2 -> right;
    }while(temp2 != temp1);

    return heap;
}

Node* delete_min_from_fibonacci_heap(Node* heap, vector<int> path, int* number_of_nodes){
    Node* min = heap, *below, *next;

    if(min -> child != NULL){
        below = min -> child;
        do{
            //Insertion in main heap
            next = below -> right;
            (heap -> left) -> right = below;
            below -> right = heap;
            below -> left = heap -> left;
            heap -> left = below;
            //
            below -> parent = NULL;
            below = next;
        } while(next != min -> child);
    }

    //Remove
    (min -> left) -> right = min -> right;
    (min -> right) -> left = min -> left;

    if(min == min -> right){
        heap = NULL;
    }
    else{
        heap = min -> right;
        heap = consolidate_fibonacci_heap(heap, path, number_of_nodes);
    }

    delete min;

    (*number_of_nodes)--;
    return heap;
}

void cut_fibonacci_node(Node* heap, Node* child, Node* parent){
    //Remove
    (child -> left) -> right = child -> right;
    (child -> right) -> left = child -> left;

    if(child -> right == child){
        parent -> child = NULL;
    }
    else if(child == (parent -> child)){
        parent -> child = child -> right;
    }

    child -> left = child;
    child -> right = child;

    (parent -> degree)--;
    child -> parent = NULL;
    child -> marked = false;

    //Insert
    heap -> left -> right = child;
    child -> right = heap;
    child -> left = heap -> left;
    heap -> left = child;
}

void cascade_cut_fibonacci_heap(Node* heap, Node* node){
    Node* temp = node -> parent;

    if(temp != NULL){
        if(node -> marked == false){
            node -> marked = true;
        }
        else{
            cut_fibonacci_node(heap, node, temp);
            cascade_cut_fibonacci_heap(heap, temp);
        }
    }
}

Node* decrease_key_in_fibonacci_heap(Node* heap, vector<int> path, Node* node){
    Node* parent = node -> parent;

    if(parent != NULL && (path[parent -> key] > path[node -> key])){
        cut_fibonacci_node(heap, node, parent);
        cascade_cut_fibonacci_heap(heap, parent);
    }

    if(path[node -> key] < path[heap -> key]){
        heap = node;
    }

    return heap;
}

void dijkstra_using_fibonacci_heap(vector<vector<pair<int, int>>> edges, vector<int> h, bool flag){
    int n = edges.size();
    for(int i = 0; i < n; i++){
        vector<int> path(n, 999999);
        path[i] = 0;

        vector<Node*> in_heap(n, NULL);

        Node* heap_min = NULL;
        int number_of_nodes = 0;
        heap_min = insert_in_fibonacci_heap(heap_min, i, path, &number_of_nodes, in_heap);

        //Dijkstra
        while(heap_min != NULL){
            int x = heap_min -> key; //get_min
            heap_min = delete_min_from_fibonacci_heap(heap_min, path, &number_of_nodes);
            in_heap[x] = NULL;

            for(int j = 0; j < edges[x].size(); j++){
                int y = edges[x][j].first;
                int w = edges[x][j].second;

                if(path[x] + w < path[y]){
                    path[y] = path[x] + w;

                    if(in_heap[y] == NULL){
                        heap_min = insert_in_fibonacci_heap(heap_min, y, path, &number_of_nodes, in_heap);
                    }
                    else{
                        heap_min = decrease_key_in_fibonacci_heap(heap_min, path, in_heap[y]);
                    }
                }
            }
        }

        //Print Output
        for(int j = 0; j < n; j++){
            if(flag && path[j] != 999999){
                cout << path[j] + h[j] - h[i];
            }
            else{
                cout << path[j];
            }
            cout << " ";
        }

        cout << endl;
    }
}
/******************************************************************************/
int main(int argc, char** argv){
    //Getting type of implementation to be used
    int type = (argc == 1) ? 4 : stoi(argv[1]);

    int t, tt;
    cin >> t;
    tt = t;

    //For measuring execution times
    vector<double> exec_time;

    while(t--){
        //Start clock
        double time_spent = 0.0;
        clock_t begin = clock();

        int n, d, x;
        cin >> n >> d;

        vector<vector<pair<int, int>>> edges(n);
        bool flag = false;

        //Getting graph input
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                cin >> x;
                if(x < 0) flag = true;
                if(x < 999999 && i != j) edges[i].push_back({j, x});
            }
        }

        //If undirected graph with negative edges
        if(d == 0 && flag == true){
            cout << -1 << endl;
        }
        else{
            vector<int> h(n + 1, 999999);
            bool neg_cycle = false;

            if(flag){
                //Check for negative cycle and also get vertices weights
                neg_cycle = bellman_ford(edges, h);
            }

            if(neg_cycle){
                //If negative cycle
                cout << -1 << endl;
            }
            else{
                h.pop_back();

                //Update graph in case of directed graph having negative edges
                if(d == 1 && flag){
                    for(int i = 0; i < n; i++){
                        for(int j = 0; j < edges[i].size(); j++){
                            edges[i][j].second = edges[i][j].second + h[i] - h[edges[i][j].first];
                        }
                    }
                }

                //Apply dijkstra
                if(type == 1){
                    dijkstra_using_array(edges, h, flag);
                }
                else if(type == 2){
                    dijkstra_using_binary_heap(edges, h, flag);
                }
                else if(type == 3){
                    dijkstra_using_binomial_heap(edges, h, flag);
                }
                else{
                    dijkstra_using_fibonacci_heap(edges, h, flag);
                }
            }
        }

        //Stop clock and calculate execution time
        clock_t end = clock();
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
        exec_time.push_back(time_spent);
    }

    //Print execution times
    for(int i = 0; i < tt; i++){
        cout << fixed << setprecision(6) << exec_time[i] << " ";
    }

    return 0;
}
