# Proxmox Configuration Manager (C++)

![Proxmox](https://img.shields.io/badge/Proxmox-VE-FF6600?style=for-the-badge&logo=proxmox)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue?style=for-the-badge&logo=c%2B%2B)
![libcurl](https://img.shields.io/badge/libcurl-HTTP-orange?style=for-the-badge)

Client API **Proxmox VE** yang ditulis dalam bahasa **C++17** untuk mengelola konfigurasi Proxmox secara programmatic (membuat, membaca, dan mengubah VM, serta resource lainnya).

```

## ✨ Fitur

- Autentikasi ke Proxmox REST API
- Membuat Virtual Machine (QEMU/KVM) dengan konfigurasi lengkap
- Membaca konfigurasi VM yang sudah ada
- Dukungan request HTTP (GET, POST, PUT, DELETE)
- Menggunakan library modern:
  - **libcurl** untuk HTTP request
  - **nlohmann/json** untuk parsing JSON
- Mudah dikembangkan dan ditambahkan fitur baru
- Support self-signed certificate (default Proxmox)

---

## 📋 Persyaratan

### Sistem Operasi
- Linux (Debian, Ubuntu, atau Proxmox VE itu sendiri direkomendasikan)

### Library yang Dibutuhkan

```bash
sudo apt update
sudo apt install libcurl4-openssl-dev nlohmann-json3-dev build-essential
```

---

## 🚀 Cara Penggunaan

### 1. Clone atau Buat Project

```bash
mkdir proxmox-cpp-config && cd proxmox-cpp-config
```

### 2. Compile Program

**Menggunakan Makefile (direkomendasikan):**

```bash
make
```

**Atau compile manual:**

```bash
g++ -std=c++17 -O2 -Wall -o proxmox-config proxmox_config.cpp -lcurl
```

### 3. Jalankan Program

```bash
./proxmox-config
```

---

## ⚙️ Konfigurasi

Sebelum menjalankan, **edit file `proxmox_config.cpp`** dan ubah bagian berikut di dalam fungsi `main()`:

```cpp
std::string host     = "192.168.1.100";     // Ganti dengan IP atau hostname Proxmox kamu
std::string username = "root@pam";          // atau namauser@pve
std::string password = "password_kamu";     // GANTI DENGAN PASSWORD ASLI
std::string node     = "pve";               // Nama node Proxmox (biasanya "pve")
```

> **Catatan Keamanan**: Untuk produksi, sangat disarankan menggunakan **API Token** daripada password langsung.

---

## 📝 Contoh Output

```
========================================
   Proxmox Configuration Manager (C++)
========================================

[+] Login berhasil sebagai root@pam

--- Mengambil konfigurasi VM ID 100 ---
Konfigurasi VM 100 berhasil diambil:
{
    "memory": 4096,
    "cores": 4,
    "net0": "virtio=XX:XX:XX:XX:XX:XX,bridge=vmbr0",
    ...
}
```

---

## 📁 Struktur File Proyek

```
proxmox-cpp-config/
├── proxmox_config.cpp      ← File utama program
├── README.md               ← Dokumentasi ini
├── Makefile                ← Untuk compile mudah
└── examples/               ← (akan ditambahkan nanti)
```

---

## 🔧 Fitur yang Sudah Tersedia

- [x] Login ke Proxmox API
- [x] Membuat VM baru
- [x] Mengambil konfigurasi VM
- [x] Request HTTP dengan autentikasi
- [ ] Support API Token
- [ ] Manajemen LXC Container
- [ ] Manajemen Storage & Network
- [ ] Command Line Interface (CLI)

---

## ⚠️ Catatan Penting

1. **SSL Certificate**  
   Proxmox menggunakan sertifikat self-signed. Oleh karena itu, SSL verification dimatikan (`verify_ssl = false`).

2. **Asynchronous Operations**  
   Banyak operasi di Proxmox bersifat asynchronous. Kamu perlu mengecek status task menggunakan endpoint `/tasks`.

3. **Error Handling**  
   Saat ini masih sederhana. Untuk penggunaan produksi, tambahkan retry logic dan logging yang lebih baik.

4. **Keamanan**  
   Jangan hardcode password di kode produksi. Gunakan environment variable atau konfigurasi eksternal.

---

## 🛠️ Cara Menambahkan Fitur

Anda dapat dengan mudah menambahkan fungsi baru di dalam class `ProxmoxAPI`. Contoh:

```cpp
bool createLXC(const std::string& node, int vmid, const json& config);
```

---

## 📄 Lisensi

Proyek ini dilisensikan di bawah **MIT License** — bebas digunakan untuk pribadi maupun komersial.

---

**Dibuat dengan ❤️ menggunakan C++ untuk automation Proxmox VE yang lebih cepat dan efisien.**

---

Jika kamu ingin versi yang lebih advanced (dengan CLI, API Token support, config file, logging, dll), silakan beri tahu saya!

---

**Terima kasih telah menggunakan Proxmox Configuration Manager!**
```

---

### Cara Menggunakan File Ini:

1. Buat file baru bernama **`README.md`** di folder proyek kamu.
2. Copy semua teks di atas.
3. Paste ke dalam file `README.md`.
4. Simpan.

Apakah kamu ingin saya buatkan juga file-file tambahan seperti:
- `.gitignore`
- `Makefile` (versi yang sudah saya berikan sebelumnya)
- `config.example.json` atau `.env.example`

atau mau saya perbaiki lagi bagian tertentu di README ini?

Silakan beri tahu! Saya siap membantu sampai proyek kamu benar-benar rapi dan siap digunakan.
