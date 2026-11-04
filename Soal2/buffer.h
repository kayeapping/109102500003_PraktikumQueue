#ifndef BUFFER_H
#define BUFFER_H

const int KAPASITAS = 6;

struct Rekaman
{
    int nomor;
    int nilai;
};

struct Buffer
{
    Rekaman data[KAPASITAS];
    int head;
#ifdef VERSI_CIRCULAR
    int count;
#else
    int tail;
#endif
    int nomorBerikut;
    long long geser;
};

const char* namaVersi();
void createBuffer(Buffer& b);
void kosongkan(Buffer& b);
bool isEmptyBuffer(const Buffer& b);
bool isFullBuffer(const Buffer& b);
int jumlah(const Buffer& b);
int ruangKosong(const Buffer& b);
bool enqueue(Buffer& b, int nilai);
bool dequeue(Buffer& b, Rekaman& keluar);
const Rekaman& elemenKe(const Buffer& b, int urutan);
bool cellAktif(const Buffer& b, int indeks);
void printPenanda(const Buffer& b);

#endif
