#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

struct Member {
    string nama;
    string jenisPaket; 
    string idMember;
    int masaAktif; 
};

struct NodeMember {
    Member data;
    NodeMember *prev;
    NodeMember *next;
};

NodeMember *headMember = nullptr;
NodeMember *tailMember = nullptr;


struct Reservasi {
    string namaMember;
    string namaKelas;
    string jamLatihan;
    float biayaTambahan;
};

struct NodeQueue {
    Reservasi data;
    NodeQueue *next;
};

struct Undo {
    string aksi;
    Member data;
};

NodeQueue *depan = nullptr;
Undo stackUndo[100];
int topUndo = -1;
NodeQueue *belakang = nullptr;


void tambahKeLinkedListMember(Member memberBaru) {
    NodeMember *nodeBaru = new NodeMember;
    nodeBaru->data = memberBaru;
    nodeBaru->prev = nullptr;
    nodeBaru->next = nullptr;

    if (headMember == nullptr) {
        headMember = tailMember = nodeBaru;
    } else {
        tailMember->next = nodeBaru;
        nodeBaru->prev = tailMember;
        tailMember = nodeBaru;
    }
}

void sinkronisasiArrayDariLinked(Member *daftar, int &jumlah) {

    NodeMember *current = headMember;
    jumlah = 0;
    while (current != nullptr && jumlah < 100) {
        daftar[jumlah] = current->data;
        jumlah++;
        current = current->next;
    }
}

void sortingNama(Member daftar[], int jumlah) { //Bubble-sort
		for (int i = 0; i < jumlah - 1; i++) {

			for (int j = 0; j < jumlah - i - 1; j++) {

				if (daftar[j].nama > daftar[j + 1].nama) {

					Member temp = daftar[j];
					daftar[j] = daftar[j + 1];
					daftar[j + 1] = temp;
				}
			}
		}
	}
	

void pushUndo(Undo data) { 
    topUndo++;
    stackUndo[topUndo] = data;
}

Undo popUndo() { return stackUndo[topUndo--]; }
bool isEmptyUndo() { return topUndo == -1; }

void undoTerakhir(Member daftar[], int &jumlah) {

    if (isEmptyUndo()) {
        cout << "Tidak ada aksi sebelumnya!" << endl;
        return;
    }

    Undo u = popUndo();
    
    if (u.aksi == "tambah") {

        NodeMember *current = tailMember;

        if (current != nullptr) {

            if (current->prev != nullptr) {
                tailMember = current->prev;
                tailMember->next = nullptr;
            } else {
                headMember = tailMember = nullptr;
            }
            delete current;
        }
        
		cout << "Undo berhasil!" << endl;
        
    } else if (u.aksi == "hapus") {
        tambahKeLinkedListMember(u.data);
        cout << "Undo hapus berhasil!" << endl;
    }

    sinkronisasiArrayDariLinked(daftar, jumlah);
}


void tampilkanMember(Member daftar[], int jumlah) {
    if (jumlah == 0) {
        cout << "Belum ada member terdaftar." << endl;
        return;
    }
    cout << "\n============================  DAFTAR MEMBER GYM  ============================\n";
    cout << "+-----+---------------------+--------------------+-----------------+----------+" << endl;
    cout << "| No  | Nama Member         | Jenis Paket        | ID Member       | Durasi   |" << endl;
    cout << "+-----+---------------------+--------------------+-----------------+----------+" << endl;
	
	sortingNama(daftar, jumlah);
    
    for (int i = 0; i < jumlah; i++) {
    cout << "| " << setw(3) << right << i + 1 << " ";
    cout << "| " << setw(20) << left << daftar[i].nama;
    cout << "| " << setw(19) << left << daftar[i].jenisPaket;
    cout << "| " << setw(16) << left << daftar[i].idMember;
    cout << "| " << right << setw(2) << daftar[i].masaAktif << " bln   |" << endl;
    }
    cout << "+-----+---------------------+--------------------+-----------------+----------+" << endl;
}

void editMember() {
    if (headMember == nullptr) {
        cout << "Data kosong.\n";
        return;
    }
    string idCari;
    cout << "\n==================================" << endl;
	cout << "           Edit Member            " << endl;
	cout << "==================================" << endl;
    cout << "Masukkan ID Member yang ingin diedit: ";
    getline(cin, idCari);

    NodeMember *current = headMember;
    while (current != nullptr) {
        if (current->data.idMember == idCari) {
            cout << "\n--- Data ditemukan ---"<<endl;
            cout << "Nama Baru          : "; getline(cin, current->data.nama);
            cout << "Paket Baru         : "; getline(cin, current->data.jenisPaket);
            cout << "Masa Aktif Baru    : "; cin >> current->data.masaAktif;
            cin.ignore();
            cout << "Data berhasil diperbarui.\n";
            return;
        }
        current = current->next;
    }
    cout << "ID Member tidak ditemukan.\n";
}

void hapusMember() {
    if (headMember == nullptr) return;
    string idHapus;
    cout << "\n==================================" << endl;
	cout << "           Hapus Member           " << endl;
	cout << "==================================" << endl;
    cout << "Masukkan ID Member yang ingin dihapus: ";
    getline(cin, idHapus);

    NodeMember *current = headMember;
    while (current != nullptr) {
        if (current->data.idMember == idHapus) {
            if (current->prev) current->prev->next = current->next;
            else headMember = current->next;

            if (current->next) current->next->prev = current->prev;
            else tailMember = current->prev;
			
			Undo u;
			u.aksi = "hapus";
			u.data = current->data;

			pushUndo(u);
			
            delete current;
            cout << "Member berhasil dihapus.\n";
            return;
        }
        current = current->next;
    }
    cout << "Member tidak ditemukan.\n";
}


void buatReservasi() {
    NodeQueue *baru = new NodeQueue;
    cout << "Nama Member      : "; getline(cin, baru->data.namaMember);
    cout << "Pilih Kelas      : "; getline(cin, baru->data.namaKelas);
    cout << "Jam Latihan      : "; getline(cin, baru->data.jamLatihan);
    cout << "Biaya (jika ada) : "; cin >> baru->data.biayaTambahan;
    cin.ignore();
    baru->next = nullptr;

    if (belakang == nullptr) depan = belakang = baru;
    else {
        belakang->next = baru;
        belakang = baru;
    }
    cout << "Reservasi berhasil ditambahkan ke antrian!\n";
}

void lihatAntrianReservasi() {
    if (depan == nullptr) {
        cout << "Antrian reservasi kosong.\n";
        return;
    }
    cout << "\n=== ANTRIAN KELAS HARI INI ===\n";
    NodeQueue *temp = depan;
    int no = 1;
    while (temp != nullptr) {
        cout << no++ << ". " << temp->data.namaMember << " | Kelas: " 
             << temp->data.namaKelas << " | Jam: " << temp->data.jamLatihan << endl;
        temp = temp->next;
    }
}


void simpanReservasiKeFile() {
    FILE *file = fopen("reservasi.txt", "w");
    if (!file) {
        cout << "Gagal menyimpan file reservasi.\n";
        return;
    }

    NodeQueue *temp = depan;
    while (temp != nullptr) {
        fprintf(file, "%s;%s;%s;%.2f\n",
                temp->data.namaMember.c_str(),
                temp->data.namaKelas.c_str(),
                temp->data.jamLatihan.c_str(),
                temp->data.biayaTambahan);
        temp = temp->next;
    }

    fclose(file);
    cout << "Data reservasi disimpan ke file!" << endl;
}

void loadReservasiDariFile() {
    FILE *file = fopen("reservasi.txt", "r");
    if (!file) return;

    char nama[100], kelas[100], jam[100];
    float biaya;

    while (fscanf(file, "%[^;];%[^;];%[^;];%f\n", nama, kelas, jam, &biaya) == 4) {
        NodeQueue *baru = new NodeQueue;
        baru->data.namaMember = nama;
        baru->data.namaKelas = kelas;
        baru->data.jamLatihan = jam;
        baru->data.biayaTambahan = biaya;
        baru->next = nullptr;

        if (!belakang) {
            depan = belakang = baru;
        } else {
            belakang->next = baru;
            belakang = baru;
        }
    }

    fclose(file);
    cout << "Data reservasi berhasil dimuat dari file." << endl;
}


void simpanData(Member daftar[], int jumlah) {
    FILE *f = fopen("gym_members.txt", "w");
    if (!f) return;
    for (int i = 0; i < jumlah; i++) {
        fprintf(f, "%s;%s;%s;%d\n", daftar[i].nama.c_str(), daftar[i].jenisPaket.c_str(), 
                daftar[i].idMember.c_str(), daftar[i].masaAktif);
    }
    fclose(f);
}

void loadData(Member daftar[], int &jumlah) {
    FILE *f = fopen("gym_members.txt", "r");
    if (!f) return;
    char n[100], p[100], id[50];
    int m;
    while (fscanf(f, "%99[^;];%99[^;];%49[^;];%d\n", n, p, id, &m) == 4) {
        daftar[jumlah].nama = n;
        daftar[jumlah].jenisPaket = p;
        daftar[jumlah].idMember = id;
        daftar[jumlah].masaAktif = m;
        tambahKeLinkedListMember(daftar[jumlah]);
        jumlah++;
    }
    fclose(f);
}


void menuAdmin(Member daftar[], int &jumlah) {
    int pil;
    do {
		system("cls");
		cout << "\n==================================" << endl;
		cout << "       DASHBOARD ADMIN GYM        " << endl;
		cout << "==================================" << endl;
        cout << "1. Lihat Semua Member\n";
		cout << "2. Edit Member\n";
		cout << "3. Hapus Member\n";
		cout << "4. Tambah Member\n";
		cout << "5. Undo\n";
		cout << "6. Logout\n";
		cout << "Pilihan: ";
		
        cin >> pil; cin.ignore();
        switch(pil) {

			case 1:
				system("cls");
				tampilkanMember(daftar, jumlah);
				cout << "\nTekan enter untuk kembali...";
				cin.get();
				break;

			case 2:
				system("cls");
				editMember();
				sinkronisasiArrayDariLinked(daftar, jumlah);
				cout << endl;
				cout << "\nTekan enter untuk kembali...";
				cin.get();
				break;

			case 3:
				system("cls");
				hapusMember();
				sinkronisasiArrayDariLinked(daftar, jumlah);
				cout << endl;
				cout << "\nTekan enter untuk kembali...";
				cin.get();
				break;

			case 4: {
				system("cls");
				Member m;

				cout << "\n======================================" << endl;
				cout << "         TAMBAH MEMBER BARU           " << endl;
				cout << "======================================" << endl;

				cout << "Nama Member        : "; getline(cin, m.nama);
				cout << "Jenis Paket        : "; getline(cin, m.jenisPaket);
				cout << "ID Member          : "; getline(cin, m.idMember);
				cout << "Masa Aktif (Bulan) : "; cin >> m.masaAktif;
				cin.ignore();
				cout << "======================================" << endl;
				cout << " Member berhasil ditambahkan!" << endl;
				cout << "======================================" << endl;
				cout << endl;				
				cout << "\nTekan enter untuk kembali...";
				
				tambahKeLinkedListMember(m);
				Undo u; u.aksi = "tambah"; u.data = m;
				pushUndo(u);
				sinkronisasiArrayDariLinked(daftar, jumlah);
				
				cin.get();
				break;
			}
			case 5:
				undoTerakhir(daftar, jumlah);
				cin.get();
				break;
			case 6:
				system("cls");
				cout << "Logout berhasil..." << endl;
				break;

			default:
				cout << "Pilihan tidak valid!\n";
		}
    } while (pil != 6);
}

void menuUser() {
    int pil;
    do {

    system("cls");
    cout << "\n==================================" << endl;
    cout << "       SELF SERVICE MEMBER        " << endl;
    cout << "==================================" << endl;
    cout << "1. Reservasi Kelas" << endl;
    cout << "2. Lihat Antrian" << endl;
    cout << "3. Logout" << endl;
    cout << "==================================" << endl;
    cout << "Pilihan : "; cin >> pil;
    cin.ignore();

    switch(pil) {
        case 1:
            system("cls");
            buatReservasi();
            cout << "\nTekan enter untuk kembali...";
            
            cin.get();
            break;
        case 2:
            system("cls");
            lihatAntrianReservasi();
            cout << "\nTekan enter untuk kembali...";
           
            cin.get();
            break;
        case 3:
            system("cls");
            cout << "Logout berhasil..." << endl;
            break;
        default:
            cout << "Pilihan tidak valid!" << endl;
            cin.get();
    }

	} while (pil != 3);
}


int main() {
    Member daftarMember[100];
    int jumlahMember = 0;

    loadData(daftarMember, jumlahMember);
    loadReservasiDariFile();

    int role;
    do {

    system("cls");

    cout << "======================================" << endl;
    cout << "      SISTEM MEMBERSHIP SMART-GYM    " << endl;
    cout << "======================================" << endl;
    cout << "1. Admin" << endl;
    cout << "2. Member" << endl;
    cout << "3. Keluar" << endl;
    cout << "======================================" << endl;
    cout << "Pilihan : "; cin >> role;
    cin.ignore();

		switch(role) {
			case 1: {
				system("cls");
				string user, pass;
				cout << "======================================" << endl;
				cout << "            LOGIN ADMIN              " << endl;
				cout << "======================================" << endl;
				cout << "Username : "; getline(cin, user);
				cout << "Password : "; getline(cin, pass);

				if (user == "admin" && pass == "gym123") {
					cout << "\nLogin berhasil!\n";
					cout << "Tekan enter untuk lanjut...";
					cin.get();
					menuAdmin(daftarMember, jumlahMember);
				} else {
					cout << "\nLogin gagal!\n";
					cout << "Tekan enter untuk kembali...";
					cin.get();
				}

				break;
			}

			case 2:
				system("cls");
				menuUser();
				cin.get();
				break;
			case 3:
				system("cls");
				cout << "======================================" << endl;
				cout << "     DATA TERSIMPAN. SAMPAI JUMPA    " << endl;
				cout << "======================================" << endl;

				break;
			default:
				cout << "\nPilihan tidak valid!" << endl;
				cout << "Tekan enter untuk kembali...";
				cin.get();
		}

	} while (role != 3);

    simpanData(daftarMember, jumlahMember);
    simpanReservasiKeFile();
    cout << "Data tersimpan. Sampai jumpa!"<<endl;
    return 0;
}
