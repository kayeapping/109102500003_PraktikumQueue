#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cctype>
#include <unistd.h>
#include "buffer.h"

static int bacaToken(const std::string& baris, std::string t[], int maks)
{
    std::istringstream iss(baris);
    std::string x;
    int n = 0;
    while (iss >> x)
    {
        if (n < maks)
        {
            t[n] = x;
        }
        n++;
    }
    return n;
}

static bool parseInt(const std::string& s, long long& out)
{
    if (s.empty() || s.size() > 12)
    {
        return false;
    }
    std::size_t i = 0;
    bool neg = false;
    if (s[0] == '-' || s[0] == '+')
    {
        neg = (s[0] == '-');
        i = 1;
    }
    if (i >= s.size())
    {
        return false;
    }
    long long v = 0;
    for (; i < s.size(); i++)
    {
        if (s[i] < '0' || s[i] > '9')
        {
            return false;
        }
        v = v * 10 + (s[i] - '0');
    }
    out = neg ? -v : v;
    return true;
}

static std::string huruf(std::string s)
{
    for (std::size_t i = 0; i < s.size(); i++)
    {
        s[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(s[i])));
    }
    return s;
}

static void cmdTulis(Buffer& b, const std::string t[], int nt)
{
    if (nt < 2)
    {
        std::cout << "DITOLAK: format TULIS k v1 ... vk\n";
        return;
    }
    long long k;
    if (!parseInt(t[1], k))
    {
        std::cout << "DITOLAK: k harus berupa bilangan bulat\n";
        return;
    }
    if (k < 1 || k > KAPASITAS)
    {
        std::cout << "DITOLAK: k harus 1-" << KAPASITAS << "\n";
        return;
    }
    if (nt != k + 2)
    {
        std::cout << "DITOLAK: jumlah nilai tidak sama dengan k\n";
        return;
    }
    int nilai[KAPASITAS];
    for (int i = 0; i < k; i++)
    {
        long long v;
        if (!parseInt(t[i + 2], v))
        {
            std::cout << "DITOLAK: nilai ke-" << (i + 1) << " bukan bilangan bulat\n";
            return;
        }
        if (v < -1000 || v > 1000)
        {
            std::cout << "DITOLAK: nilai " << v << " di luar rentang -1000 sampai 1000\n";
            return;
        }
        nilai[i] = static_cast<int>(v);
    }
    if (ruangKosong(b) < k)
    {
        std::cout << "DITOLAK: ruang kosong hanya " << ruangKosong(b) << ", diminta " << k << "\n";
        return;
    }
    std::cout << "OK: masuk";
    for (int i = 0; i < k; i++)
    {
        enqueue(b, nilai[i]);
        const Rekaman& r = elemenKe(b, jumlah(b) - 1);
        std::cout << (i == 0 ? " " : ", ") << r.nomor << ":" << r.nilai;
    }
    std::cout << "\n";
}

static void cmdAmbil(Buffer& b, const std::string t[], int nt)
{
    if (nt != 2)
    {
        std::cout << "DITOLAK: format AMBIL k\n";
        return;
    }
    long long k;
    if (!parseInt(t[1], k))
    {
        std::cout << "DITOLAK: k harus berupa bilangan bulat\n";
        return;
    }
    if (k < 1 || k > KAPASITAS)
    {
        std::cout << "DITOLAK: k harus 1-" << KAPASITAS << "\n";
        return;
    }
    if (k > jumlah(b))
    {
        std::cout << "DITOLAK: data kurang, tersedia " << jumlah(b) << ", diminta " << k << "\n";
        return;
    }
    std::cout << "OK: keluar";
    for (int i = 0; i < k; i++)
    {
        Rekaman r;
        dequeue(b, r);
        std::cout << (i == 0 ? " " : ", ") << r.nomor << ":" << r.nilai;
    }
    std::cout << "\n";
}

static void cmdLihat(const Buffer& b)
{
    std::cout << namaVersi() << "\n";
    int n = jumlah(b);
    std::cout << "FIFO: ";
    if (n == 0)
    {
        std::cout << "(kosong)";
    }
    for (int i = 0; i < n; i++)
    {
        if (i > 0)
        {
            std::cout << ", ";
        }
        std::cout << elemenKe(b, i).nomor << ":" << elemenKe(b, i).nilai;
    }
    std::cout << "\n";
    std::cout << "Indeks fisik:\n";
    for (int i = 0; i < KAPASITAS; i++)
    {
        std::cout << "  [" << i << "] ";
        if (cellAktif(b, i))
        {
            std::cout << b.data[i].nomor << ":" << b.data[i].nilai;
        }
        else
        {
            std::cout << "kosong";
        }
        std::cout << "\n";
    }
    printPenanda(b);
    std::cout << "Nomor berikutnya: " << b.nomorBerikut << "\n";
    std::cout << "Total pergeseran: " << b.geser << " rekaman\n";
}

static void cmdRingkas(const Buffer& b)
{
    int n = jumlah(b);
    if (n == 0)
    {
        std::cout << "Jumlah: 0\nTotal: 0\nMinimum: TIDAK ADA\nMaksimum: TIDAK ADA\nRata-rata: TIDAK ADA\n";
        return;
    }
    long long total = 0;
    int mn = elemenKe(b, 0).nilai;
    int mx = mn;
    for (int i = 0; i < n; i++)
    {
        int v = elemenKe(b, i).nilai;
        total += v;
        if (v < mn)
        {
            mn = v;
        }
        if (v > mx)
        {
            mx = v;
        }
    }
    double rata = static_cast<double>(total) / n;
    std::cout << "Jumlah: " << n << "\n";
    std::cout << "Total: " << total << "\n";
    std::cout << "Minimum: " << mn << "\n";
    std::cout << "Maksimum: " << mx << "\n";
    std::cout << "Rata-rata: " << std::fixed << std::setprecision(2) << rata << "\n";
    std::cout.unsetf(std::ios::floatfield);
}

int main()
{
    Buffer b;
    createBuffer(b);
    bool tty = isatty(STDIN_FILENO) != 0;
    std::string baris;

    while (true)
    {
        if (tty)
        {
            std::cout << "> ";
        }
        if (!std::getline(std::cin, baris))
        {
            break;
        }
        std::string t[8];
        int nt = bacaToken(baris, t, 8);
        if (nt == 0)
        {
            continue;
        }
        if (!tty)
        {
            std::cout << "> " << baris << "\n";
        }
        std::string cmd = huruf(t[0]);

        if (cmd == "TULIS")
        {
            cmdTulis(b, t, nt);
        }
        else if (cmd == "AMBIL")
        {
            cmdAmbil(b, t, nt);
        }
        else if (cmd == "LIHAT")
        {
            cmdLihat(b);
        }
        else if (cmd == "RINGKAS")
        {
            cmdRingkas(b);
        }
        else if (cmd == "KOSONGKAN")
        {
            kosongkan(b);
            std::cout << "OK: buffer dikosongkan, nomor berikutnya " << b.nomorBerikut
                      << ", total pergeseran " << b.geser << "\n";
        }
        else if (cmd == "SELESAI")
        {
            break;
        }
        else
        {
            std::cout << "DITOLAK: perintah tidak dikenal\n";
        }
    }

    std::cout << "Program selesai.\n";
    return 0;
}
