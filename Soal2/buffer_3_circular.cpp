#include "buffer.h"
#include <iostream>

const char* namaVersi()
{
    return "Versi 3 - Circular queue";
}

void createBuffer(Buffer& b)
{
    b.head = 0;
    b.count = 0;
    b.nomorBerikut = 1;
    b.geser = 0;
}

void kosongkan(Buffer& b)
{
    b.head = 0;
    b.count = 0;
}

bool isEmptyBuffer(const Buffer& b)
{
    return b.count == 0;
}

int jumlah(const Buffer& b)
{
    return b.count;
}

bool isFullBuffer(const Buffer& b)
{
    return b.count == KAPASITAS;
}

int ruangKosong(const Buffer& b)
{
    return KAPASITAS - b.count;
}

bool enqueue(Buffer& b, int nilai)
{
    if (isFullBuffer(b))
    {
        return false;
    }
    int posTulis = (b.head + b.count) % KAPASITAS;
    b.data[posTulis].nomor = b.nomorBerikut;
    b.data[posTulis].nilai = nilai;
    b.nomorBerikut++;
    b.count++;
    return true;
}

bool dequeue(Buffer& b, Rekaman& keluar)
{
    if (isEmptyBuffer(b))
    {
        return false;
    }
    keluar = b.data[b.head];
    b.count--;
    if (b.count == 0)
    {
        b.head = 0;
    }
    else
    {
        b.head = (b.head + 1) % KAPASITAS;
    }
    return true;
}

const Rekaman& elemenKe(const Buffer& b, int urutan)
{
    return b.data[(b.head + urutan) % KAPASITAS];
}

bool cellAktif(const Buffer& b, int indeks)
{
    int jarak = (indeks - b.head + KAPASITAS) % KAPASITAS;
    return jarak < b.count;
}

void printPenanda(const Buffer& b)
{
    std::cout << "Penanda: head = " << b.head << ", count = " << b.count << "\n";
}
