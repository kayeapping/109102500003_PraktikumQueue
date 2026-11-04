#include "buffer.h"
#include <iostream>

const char* namaVersi()
{
    return "Versi 1 - Array geser";
}

void createBuffer(Buffer& b)
{
    b.head = -1;
    b.tail = -1;
    b.nomorBerikut = 1;
    b.geser = 0;
}

void kosongkan(Buffer& b)
{
    b.head = -1;
    b.tail = -1;
}

bool isEmptyBuffer(const Buffer& b)
{
    return b.head == -1;
}

int jumlah(const Buffer& b)
{
    if (isEmptyBuffer(b))
    {
        return 0;
    }
    return b.tail - b.head + 1;
}

bool isFullBuffer(const Buffer& b)
{
    return jumlah(b) == KAPASITAS;
}

int ruangKosong(const Buffer& b)
{
    return KAPASITAS - jumlah(b);
}

bool enqueue(Buffer& b, int nilai)
{
    if (isFullBuffer(b))
    {
        return false;
    }
    if (isEmptyBuffer(b))
    {
        b.head = 0;
        b.tail = 0;
    }
    else
    {
        b.tail++;
    }
    b.data[b.tail].nomor = b.nomorBerikut;
    b.data[b.tail].nilai = nilai;
    b.nomorBerikut++;
    return true;
}

bool dequeue(Buffer& b, Rekaman& keluar)
{
    if (isEmptyBuffer(b))
    {
        return false;
    }
    keluar = b.data[b.head];
    if (b.head == b.tail)
    {
        b.head = -1;
        b.tail = -1;
        return true;
    }
    for (int i = b.head; i < b.tail; i++)
    {
        b.data[i] = b.data[i + 1];
        b.geser++;
    }
    b.tail--;
    return true;
}

const Rekaman& elemenKe(const Buffer& b, int urutan)
{
    return b.data[b.head + urutan];
}

bool cellAktif(const Buffer& b, int indeks)
{
    if (isEmptyBuffer(b))
    {
        return false;
    }
    return indeks >= b.head && indeks <= b.tail;
}

void printPenanda(const Buffer& b)
{
    std::cout << "Penanda: head = " << b.head << ", tail = " << b.tail << "\n";
}
