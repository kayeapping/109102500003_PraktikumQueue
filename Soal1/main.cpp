#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <unistd.h>
#include "paket.h"

struct Statistik
{
    int jumlah;
    long long berat;
};

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

static bool bacaN(const std::string t[], int nt, const std::string& nama,
                  const Queue& sumber, const std::string& namaSumber, int& n)
{
    if (nt != 2)
    {
        std::cout << "DITOLAK: format " << nama << " n\n";
        return false;
    }
    long long v;
    if (!parseInt(t[1], v))
    {
        std::cout << "DITOLAK: n harus berupa bilangan bulat\n";
        return false;
    }
    if (v <= 0)
    {
        std::cout << "DITOLAK: n harus positif\n";
        return false;
    }
    int tersedia = nbList(sumber);
    if (v > tersedia)
    {
        std::cout << "DITOLAK: n = " << v << " melebihi jumlah paket di antrean "
                  << namaSumber << " (" << tersedia << " paket)\n";
        return false;
    }
    n = static_cast<int>(v);
    return true;
}

static void cmdMasuk(Queue& A, Queue& B, const std::string t[], int nt)
{
    if (nt != 3)
    {
        std::cout << "DITOLAK: format MASUK kode berat\n";
        return;
    }
    const std::string& kode = t[1];
    if (kode.size() < 1 || kode.size() > 10)
    {
        std::cout << "DITOLAK: kode harus 1-10 karakter\n";
        return;
    }
    long long berat;
    if (!parseInt(t[2], berat))
    {
        std::cout << "DITOLAK: berat harus berupa bilangan bulat\n";
        return;
    }
    if (berat < 1 || berat > 5000)
    {
        std::cout << "DITOLAK: berat harus 1-5000 gram\n";
        return;
    }
    if (findElm(A, kode) != nullptr || findElm(B, kode) != nullptr)
    {
        std::cout << "DITOLAK: kode " << kode << " masih aktif di A atau B\n";
        return;
    }
    Node* p = alokasi(kode, static_cast<int>(berat));
    if (p == nullptr)
    {
        std::cout << "DITOLAK: alokasi memori gagal\n";
        return;
    }
    enqueue(A, p);
    std::cout << "OK: " << kode << " (" << berat << " gram) masuk di belakang A\n";
}

static void cmdPindah(Queue& A, Queue& B, const std::string t[], int nt)
{
    int n;
    if (!bacaN(t, nt, "PINDAH", A, "A", n))
    {
        return;
    }
    for (int i = 0; i < n; i++)
    {
        Node* p = dequeue(A);
        std::cout << "  " << p->info.kode << " dipindahkan dari A ke B\n";
        enqueue(B, p);
    }
    std::cout << "OK: " << n << " paket dipindahkan\n";
}

static void cmdKirim(Queue& B, Statistik& kirim, const std::string t[], int nt)
{
    int n;
    if (!bacaN(t, nt, "KIRIM", B, "B", n))
    {
        return;
    }
    long long totalBatch = 0;
    for (int i = 0; i < n; i++)
    {
        Node* p = dequeue(B);
        std::cout << "  Terkirim: " << p->info.kode << " (" << p->info.berat << " gram)\n";
        kirim.jumlah++;
        kirim.berat += p->info.berat;
        totalBatch += p->info.berat;
        dealokasi(p);
    }
    std::cout << "OK: " << n << " paket terkirim, total " << totalBatch << " gram\n";
}

static void cmdCari(const Queue& A, const Queue& B, const std::string t[], int nt)
{
    if (nt != 2)
    {
        std::cout << "DITOLAK: format CARI kode\n";
        return;
    }
    const std::string& kode = t[1];
    Node* p = findElm(A, kode);
    if (p != nullptr)
    {
        std::cout << "Lokasi A, posisi " << posisiElm(A, kode) << ", berat " << p->info.berat << " gram\n";
        return;
    }
    p = findElm(B, kode);
    if (p != nullptr)
    {
        std::cout << "Lokasi B, posisi " << posisiElm(B, kode) << ", berat " << p->info.berat << " gram\n";
        return;
    }
    std::cout << "TIDAK DITEMUKAN\n";
}

static void cmdAlamat(const Queue& A, const Queue& B, const std::string t[], int nt)
{
    if (nt != 2)
    {
        std::cout << "DITOLAK: format ALAMAT kode\n";
        return;
    }
    Node* p = findElm(A, t[1]);
    if (p == nullptr)
    {
        p = findElm(B, t[1]);
    }
    if (p == nullptr)
    {
        std::cout << "TIDAK DITEMUKAN\n";
        return;
    }
    std::cout << "Alamat node " << t[1] << ": " << static_cast<const void*>(p) << "\n";
    std::cout << "  fFindElm(A) = " << (fFindElm(A, p) ? "ya" : "tidak") << "\n";
    std::cout << "  fFindElm(B) = " << (fFindElm(B, p) ? "ya" : "tidak") << "\n";
}

static void cmdLihat(const Queue& A, const Queue& B)
{
    std::cout << "A: ";
    printInfo(A);
    std::cout << "B: ";
    printInfo(B);
}

static void cmdRekap(const Queue& A, const Queue& B, const Statistik& kirim)
{
    std::cout << "Antrean A: jumlah " << nbList(A) << ", total berat " << totalBerat(A) << " gram\n";
    std::cout << "Antrean B: jumlah " << nbList(B) << ", total berat " << totalBerat(B) << " gram\n";
    std::cout << "Terkirim : jumlah " << kirim.jumlah << ", total berat " << kirim.berat << " gram\n";
}

int main()
{
    Queue A;
    Queue B;
    createQueue(A);
    createQueue(B);
    Statistik kirim = {0, 0};
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
        std::string t[4];
        int nt = bacaToken(baris, t, 4);
        if (nt == 0)
        {
            continue;
        }
        if (!tty)
        {
            std::cout << "> " << baris << "\n";
        }
        std::string cmd = huruf(t[0]);

        if (cmd == "MASUK")
        {
            cmdMasuk(A, B, t, nt);
        }
        else if (cmd == "PINDAH")
        {
            cmdPindah(A, B, t, nt);
        }
        else if (cmd == "KIRIM")
        {
            cmdKirim(B, kirim, t, nt);
        }
        else if (cmd == "CARI")
        {
            cmdCari(A, B, t, nt);
        }
        else if (cmd == "ALAMAT")
        {
            cmdAlamat(A, B, t, nt);
        }
        else if (cmd == "LIHAT")
        {
            cmdLihat(A, B);
        }
        else if (cmd == "REKAP")
        {
            cmdRekap(A, B, kirim);
        }
        else if (cmd == "BERSIHKAN")
        {
            delAll(A);
            delAll(B);
            std::cout << "OK: seluruh paket di A dan B dihapus, statistik terkirim tetap\n";
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

    delAll(A);
    delAll(B);
    std::cout << "Program selesai. Seluruh node sudah dibebaskan.\n";
    return 0;
}
