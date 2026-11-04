#include "paket.h"
#include <iostream>
#include <new>

void createQueue(Queue& q)
{
    q.head = nullptr;
    q.tail = nullptr;
}

bool isEmptyQueue(const Queue& q)
{
    return q.head == nullptr;
}

Node* alokasi(const std::string& kode, int berat)
{
    Node* p = new (std::nothrow) Node;
    if (p == nullptr)
    {
        return nullptr;
    }
    p->info.kode = kode;
    p->info.berat = berat;
    p->next = nullptr;
    return p;
}

void dealokasi(Node*& p)
{
    if (p != nullptr)
    {
        delete p;
        p = nullptr;
    }
}

void enqueue(Queue& q, Node* p)
{
    if (p == nullptr)
    {
        return;
    }
    p->next = nullptr;
    if (isEmptyQueue(q))
    {
        q.head = p;
        q.tail = p;
    }
    else
    {
        q.tail->next = p;
        q.tail = p;
    }
}

Node* dequeue(Queue& q)
{
    if (isEmptyQueue(q))
    {
        return nullptr;
    }
    Node* p = q.head;
    q.head = q.head->next;
    if (q.head == nullptr)
    {
        q.tail = nullptr;
    }
    p->next = nullptr;
    return p;
}

Node* findElm(const Queue& q, const std::string& kode)
{
    Node* p = q.head;
    while (p != nullptr)
    {
        if (p->info.kode == kode)
        {
            return p;
        }
        p = p->next;
    }
    return nullptr;
}

bool fFindElm(const Queue& q, const Node* p)
{
    if (p == nullptr)
    {
        return false;
    }
    const Node* cur = q.head;
    while (cur != nullptr)
    {
        if (cur == p)
        {
            return true;
        }
        cur = cur->next;
    }
    return false;
}

int nbList(const Queue& q)
{
    int n = 0;
    const Node* p = q.head;
    while (p != nullptr)
    {
        n++;
        p = p->next;
    }
    return n;
}

int posisiElm(const Queue& q, const std::string& kode)
{
    int pos = 1;
    const Node* p = q.head;
    while (p != nullptr)
    {
        if (p->info.kode == kode)
        {
            return pos;
        }
        pos++;
        p = p->next;
    }
    return 0;
}

long long totalBerat(const Queue& q)
{
    long long total = 0;
    const Node* p = q.head;
    while (p != nullptr)
    {
        total += p->info.berat;
        p = p->next;
    }
    return total;
}

void printInfo(const Queue& q)
{
    if (isEmptyQueue(q))
    {
        std::cout << "(kosong)\n";
        return;
    }
    const Node* p = q.head;
    while (p != nullptr)
    {
        std::cout << p->info.kode << "(" << p->info.berat << ")";
        if (p->next != nullptr)
        {
            std::cout << " -> ";
        }
        p = p->next;
    }
    std::cout << "\n";
}

void delAll(Queue& q)
{
    while (!isEmptyQueue(q))
    {
        Node* p = dequeue(q);
        dealokasi(p);
    }
}
