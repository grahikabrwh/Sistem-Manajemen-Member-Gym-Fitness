#include <iostream>
using namespace std;

struct Buku {
    string judul;
    string pengarang;
    int tahun;
};

struct Node {
    Buku data;
    Node *left;
    Node *right;
};

struct Undo {
    char aksi;
    Buku data;
};

Undo stackUndo[100];
int top = -1;

void push(Undo data) {
    top++;
    stackUndo[top] = data;
}

Undo pop() {
    return stackUndo[top--];
}

bool isEmpty() {
    return top == -1;
}

Node* createNode(Buku data) {

    Node* baru = new Node;

    baru->data = data;
    baru->left = NULL;
    baru->right = NULL;

    return baru;
}

void insert(Node* &root, Buku data) {

    if (root == NULL) {
        root = createNode(data);
        return;
    }

    if (data.judul == root->data.judul) {
        cout << endl << "Judul sudah ada!"<<endl;
        return;
    }
    
    if (data.judul < root->data.judul) {
        insert(root->left, data);
    } else {
        insert(root->right, data);
    }
}

void inorderDescending(Node* root) { //In-Order Transversal
    if (root == NULL)
        return;
        
    inorderDescending(root->right);

    cout << "==================================" << endl;
    cout << "Judul      : " << root->data.judul << endl;
    cout << "Pengarang  : " << root->data.pengarang << endl;
    cout << "Tahun      : " << root->data.tahun << endl;

    inorderDescending(root->left);
}

Node* search(Node* root, string judul) {

    if (root == NULL)
        return NULL;

    if (judul == root->data.judul)
        return root;

    if (judul < root->data.judul)
        return search(root->left, judul);

    return search(root->right, judul);
}

Node* minValue(Node* root) {

    Node* current = root;

    while (current->left != NULL) {
        current = current->left;
    }

    return current;
}

Node* deleteNode(Node* root, string judul) { 

    if (root == NULL)
        return root;

    if (judul < root->data.judul) {
        root->left = deleteNode(root->left, judul);
    } else if (judul > root->data.judul) {
        root->right = deleteNode(root->right, judul);
    } else {

        if (root->left == NULL && root->right == NULL) { //Node Tanpa anak
            delete root;
            return NULL;
        } else if (root->left == NULL) { //Node Anak Kanan
            Node* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == NULL) { //Node Anak Kiri
            Node* temp = root->left;
            delete root;
            return temp;
        }

        Node* temp = minValue(root->right); //Node 2 Anak
        root->data = temp->data;
        root->right =
        deleteNode(root->right, temp->data.judul);
    }

    return root;
}

void undo(Node* &root) {
    if (isEmpty()) {
        cout << endl << "Tidak ada aksi sebelumnya!" << endl;
        return;
    }

    Undo data = pop();

    if (data.aksi == 'T') {
        root = deleteNode(root, data.data.judul);
        cout << endl << "Undo tambah berhasil!" << endl;
    } else if (data.aksi == 'H') {
        insert(root, data.data);
        cout << endl << "Undo hapus berhasil!" << endl;;
    }
}

int main() {

    Node* root = NULL;
    int pilih;
    do {

        cout << endl << "====================================" << endl;
        cout << "         DATA PERPUSTAKAAN" << endl;
        cout << "====================================" << endl;

        cout << "1. Tambah Buku" << endl;
        cout << "2. Tampilkan Buku" << endl;
        cout << "3. Hapus Buku" << endl;
        cout << "4. Undo" << endl;
        cout << "5. Keluar" << endl;

        cout << "Pilih Menu : ";
        cin >> pilih;
        cin.ignore();

       
        if (pilih == 1) {
            Buku buku;
            cout << endl << "Judul Buku     : ";
            getline(cin, buku.judul);

            cout << "Pengarang      : ";
            getline(cin, buku.pengarang);

            cout << "Tahun Terbit   : ";
            cin >> buku.tahun;
            cin.ignore();

            insert(root, buku);

            Undo simpan;
            simpan.aksi = 'T';
            simpan.data = buku;

            push(simpan);

            cout << endl << "Buku berhasil ditambahkan!" << endl;
        } else if (pilih == 2) {

            cout << endl <<"========== DAFTAR BUKU ==========" << endl;
			cout << endl;
			
            if (root == NULL) {
                cout << "Data kosong!" << endl;
            } else {
                inorderDescending(root);
            }
            
        } else if (pilih == 3) {

            string judul;

            cout << endl << "Masukkan judul buku : ";
            getline(cin, judul);

            Node* ditemukan = search(root, judul);

            if (ditemukan == NULL) {

                cout << endl << "Buku tidak ditemukan!" << endl;
            } else {

                Undo simpan;
                simpan.aksi = 'H';
                simpan.data = ditemukan->data;

                push(simpan);

                root = deleteNode(root, judul);

                cout << endl << "Buku berhasil dihapus!" << endl;
            }
        } else if (pilih == 4) {
            undo(root);
        }

    } while (pilih != 5);

    cout << endl << "Program selesai..." << endl;

    return 0;
}
