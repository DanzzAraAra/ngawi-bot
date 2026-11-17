# Ngawi Language

## Daftar Isi

1. [Tentang Project](#tentang-project)
2. [Persyaratan Sistem](#persyaratan-system)
3. [Instalasi](#instalasi)
4. [Konfigurasi](#konfigurasi)
5. [Cara Build](#cara-build)
6. [Cara Menjalankan Project](#cara-menjalankan-project)
7. [Sistem Plugin](#sistem-plugin)
8. [Cara Membuat Plugin Baru](#cara-membuat-plugin-baru)
9. [Cara Mengatur Plugin](#cara-mengatur-plugin)
10. [Struktur Direktori](#struktur-direktori)
11. [Troubleshooting](#troubleshooting)
12. [Contoh Penggunaan](#contoh-penggunaan)
13. [Pengembangan](#pengembangan)
14. [Lisensi](#lisensi)

---

## 1. Tentang Project

Ngawi adalah project bahasa pemrograman eksperimental yang dirancang untuk menghasilkan sintaks ringkas dengan keluaran kode native (mengkompilasi source code ke bahasa C11, kemudian menghasilkan binary mandiri menggunakan GCC).

### Tujuan dan Fungsi Utama
- Menyediakan compiler mandiri (`ngawic`) yang menerjemahkan bahasa Ngawi ke C11 dan mengeksekusi kompilasi native.
- Mendukung fitur bahasa modern seperti struktur kontrol (`if`, `elif`, `else`, `while`, `for`, `match`), array (scalar dan nested scalar dengan operasi `len`, `push`, `pop`), operator aritmatika/logika/compound assignment, serta manipulasi string bawaan.
- Menyediakan sistem modul dengan instruksi `import` tingkat atas beserta deteksi siklus impor.
- Menyediakan sistem plugin otomatis untuk pengelolaan command bot berbasis fungsi dengan awalan `cmd_`.

### Teknologi dan Bahasa Pemrograman
- **Bahasa Pemrograman Compiler:** C (Standar C11).
- **Build System:** GNU Make.
- **Backend Compiler:** GCC (GNU Compiler Collection).
- **Bahasa Target:** C11 (dapat mengeluarkan file sumber C melalui opsi `-S`).

### Gambaran Umum Arsitektur Project
Pipeline compiler dalam project ini mengalir melalui tahapan berikut:
```
Source (.ngawi) -> Lexer -> Parser (AST) -> Sema (Semantic Analysis) -> C11 Codegen -> GCC -> Binary Native
```

- **Frontend (`src/lexer/`, `src/parser/`):** Melakukan tokenisasi kode sumber, pengenalan keyword/operator, dan parsing rekursif ke dalam Abstract Syntax Tree (AST).
- **Analisis Semantik (`src/sema/`):** Memeriksa tabel simbol, pengecekan tipe data, validasi skop kontrol (`break`/`continue`), serta validasi signature fungsi plugin.
- **Backend (`src/codegen/`):** Menghasilkan kode C11 dari AST, menangani penurunan fungsi built-in, array, serta menyisipkan registrasi plugin otomatis.
- **Runtime (`src/runtime/`):** Menyediakan helper fungsi cetak, manipulasi string, dan pengelolaan array di runtime C.

---

## 2. Persyaratan Sistem

Untuk membangun dan menjalankan project Ngawi, pastikan sistem Anda memenuhi persyaratan berikut:
- **Compiler:** GCC dengan dukungan penuh standar C11 (`-std=c11`).
- **Build Tool:** GNU `make`.
- **Sistem Operasi:** Linux (direkomendasikan) atau macOS dengan environment POSIX-compliant shell.
- **Tools Pendukung:** `coreutils`, `git`, `libc6-dev`.

---

## 3. Instalasi

Langkah-langkah untuk mendapatkan dan menyiapkan source code project di local workspace:

1. Clone repository project:
   ```bash
   git clone https://github.com/DanzzAraAra/ngawi-bot.git
   ```
2. Masuk ke direktori project:
   ```bash
   cd ngawi-bot
   ```
3. Project ini tidak memerlukan instalasi package manager eksternal (seperti npm, pip, atau go modules) karena compiler ditulis dalam bahasa C murni dan dikelola menggunakan GNU Make.

---

## 4. Konfigurasi

Project Ngawi menggunakan file konfigurasi berformat JSON (seperti `examples/settings.json` atau file konfigurasi utama yang dibaca saat runtime) untuk menyimpan parameter operasional bot dan API.

### File Konfigurasi (`settings.json`)
Contoh struktur config:
```json
{
  "prefix": "!",
  "bot": {
    "token": "YOUR_BOT_TOKEN_HERE",
    "name": "NgawiBot"
  },
  "owner": {
    "name": "Dandy"
  },
  "api": {
    "baseUrl": "https://api.example.com",
    "key": "YOUR_API_KEY_HERE",
    "header": "Authorization"
  }
}
```

### Penjelasan Parameter Konfigurasi
- `prefix`: Karakter atau string awalan untuk memicu command bot (default: `"`).
- `bot.token`: Token autentikasi bot (misalnya token Telegram API). Gunakan placeholder saat membagikan konfigurasi.
- `bot.name`: Nama identitas bot yang digunakan dalam log dan pesan respon.
- `owner.name`: Nama pemilik atau administrator bot.
- `api.baseUrl`: URL dasar endpoint layanan API eksternal yang digunakan oleh plugin.
- `api.key`: Kunci akses API eksternal.
- `api.header`: Nama HTTP header untuk autentikasi API.

Catatan: Project ini tidak menggunakan file `.env` terpisah; seluruh konfigurasi dibaca langsung melalui fungsi pembaca JSON bawaan runtime Ngawi (`json_read` dan `json_get`).

---

## 5. Cara Build

Compiler Ngawi dapat dibangun menggunakan GNU Make melalui Makefile yang tersedia di root direktori.

### Perintah Build
- **Build mode Debug (default):**
  ```bash
  make debug
  ```
  Atau cukup menjalankan:
  ```bash
  make
  ```
- **Build mode Release (dengan optimasi `-O2`):**
  ```bash
  make release
  ```

### Lokasi Hasil Build
Hasil kompilasi berupa executable compiler dengan nama `ngawic` yang terletak di root direktori project (`/root/ngawi-lang/ngawic`).

### Emit Kode C11
Jika Anda ingin melihat hasil terjemahan kode sumber Ngawi ke bahasa C11 tanpa langsung menghasilkan binary final, gunakan opsi `-S`:
```bash
./ngawic build examples/hello.ngawi -o hello -S
```
Command ini akan menghasilkan file `hello.c` di samping helper runtime (`src/runtime/ngawi_runtime.h` dan `src/runtime/ngawi_runtime.c`).

### Mengatasi Error Build Umum
- **Error `gcc: command not found`:** Pastikan GCC telah terpasang di sistem (`sudo apt install build-essential` pada Ubuntu/Debian).
- **Error standar C11:** Pastikan versi GCC mendukung flag `-std=c11`.

---

## 6. Cara Menjalankan Project

### Menjalankan Program Contoh
1. Kompilasi file contoh menggunakan `ngawic`:
   ```bash
   ./ngawic build examples/hello.ngawi -o hello
   ```
2. Jalankan binary hasil build:
   ```bash
   ./hello
   ```

### Menjalankan Aplikasi Utama (`main.ngawi`)
1. Build aplikasi utama bot:
   ```bash
   ./ngawic build main.ngawi -o bot_app
   ```
2. Jalankan binary aplikasi:
   ```bash
   ./bot_app
   ```

### Menjalankan Project di Linux VPS sebagai Systemd Service
Untuk menjalankan aplikasi bot secara berkelanjutan di Linux VPS, Anda dapat mendaftarkannya sebagai systemd service:

1. Buat file service systemd baru, misalnya `/etc/systemd/system/ngawi-bot.service`:
   ```ini
   [Unit]
   Description=Ngawi Lang Bot Service
   After=network.target

   [Service]
   Type=simple
   User=root
   WorkingDirectory=/root/ngawi-lang
   ExecStart=/root/ngawi-lang/bot_app
   Restart=on-failure
   RestartSec=5

   [Install]
   WantedBy=multi-user.target
   ```
2. Reload daemon systemd dan aktifkan service:
   ```bash
   sudo systemctl daemon-reload
   sudo systemctl enable ngawi-bot.service
   sudo systemctl start ngawi-bot.service
   ```
3. Memeriksa status service:
   ```bash
   sudo systemctl status ngawi-bot.service
   ```
4. Memeriksa log aplikasi:
   ```bash
   sudo journalctl -u ngawi-bot.service -f
   ```
5. Menghentikan dan memulai ulang service:
   ```bash
   sudo systemctl stop ngawi-bot.service
   sudo systemctl restart ngawi-bot.service
   ```

---

## 7. Sistem Plugin

Sistem plugin pada Ngawi dirancang secara modular dan otomatis berdasarkan konvensi penamaan fungsi dan direktori.

### Penjelasan Sistem Plugin
Plugin adalah file kode sumber Ngawi (`.ngawi`) yang diletakkan di dalam direktori `plugin/` (serta subdirektorinya). Setiap fungsi di dalam file plugin yang diawali dengan nama `cmd_` (contoh: `cmd_ping`, `cmd_menu`) otomatis dikenali sebagai command plugin.

### Mekanisme Registrasi dan Pemuatan
1. **Pemindaian (Scanning):** Saat proses build (`load_source_with_imports`), compiler secara otomatis memindai seluruh file `.ngawi` di dalam direktori `plugin/` secara rekursif.
2. **Pengurutan:** Daftar file plugin diurutkan secara alfabetis menggunakan `qsort` untuk memastikan urutan pemuatan yang konsisten.
3. **Pemuatan Modul:** Setiap file plugin dimuat dan digabungkan ke dalam AST program utama.
4. **Registrasi Otomatis (Codegen):** Backend C11 mendeteksi semua fungsi dengan awalan `cmd_` dan menghasilkan fungsi registrasi internal secara otomatis:
   - `ng_plugin_count()`: Mengembalikan jumlah total plugin yang terdaftar.
   - `ng_plugin_name(index)`: Mengembalikan nama command berdasarkan indeks.
   - `ng_plugin_exists(name)`: Memeriksa apakah suatu command plugin tersedia.

### Struktur Direktori dan File Plugin
- Direktori utama: `plugin/`
- Subdirektori opsional: `plugin/info/`, `plugin/search/`, dll.
- Aturan penamaan file: Bebas (biasanya mencerminkan fungsi, misal `ping.ngawi`, `yts.ngawi`).
- Aturan penamaan fungsi command: Wajib diawali dengan `cmd_` (misalnya `cmd_ping`).

### Aturan Signature Plugin (Semantic Validation)
Compiler (`src/sema/sema.c`) melakukan validasi ketat bahwa setiap fungsi plugin `cmd_*` **wajib** memiliki tipe kembalian `void` dan tepat 6 parameter dengan urutan tipe sebagai berikut:
1. `token`: `string`
2. `chat`: `int`
3. `bot_name`: `string`
4. `owner_name`: `string`
5. `prefix`: `string`
6. `args`: `string`

Jika signature tidak sesuai, compiler akan menolak proses build dan melaporkan error semantik.

---

## 8. Cara Membuat Plugin Baru

Berikut adalah panduan langkah demi langkah untuk menambahkan plugin baru ke dalam project:

1. **Buat file plugin baru** di dalam direktori `plugin/` (misalnya di `plugin/utility/hello_plugin.ngawi`).
2. **Tulis kode plugin** menggunakan template yang kompatibel dengan arsitektur Ngawi:
   ```ngawi
   fn cmd_hello(token: string, chat: int, bot_name: string, owner_name: string, prefix: string, args: string) -> void {
       if (args == "") {
           rusdi_send(token, chat, "Example: " + prefix + "hello <nama>");
           return;
       }
       let pesan = "Halo " + args + "! Salam dari bot " + bot_name + ".";
       rusdi_send(token, chat, pesan);
   }
   ```
3. **Penjelasan Kode:**
   - `cmd_hello`: Otomatis mendaftarkan command `hello` (karena diawali `cmd_`).
   - Parameter `token`, `chat`, `bot_name`, `owner_name`, `prefix`, `args`: Menyediakan konteks eksekusi yang dikirim oleh core aplikasi.
   - `rusdi_send(token, chat, pesan)`: Fungsi runtime untuk mengirimkan pesan ke chat Telegram.
4. **Mendaftarkan dan Build Plugin:**
   Karena compiler secara otomatis memindai direktori `plugin/`, Anda tidak perlu mendaftarkan file plugin secara manual di kode utama. Cukup lakukan build ulang aplikasi:
   ```bash
   ./ngawic build main.ngawi -o bot_app
   ```
5. **Menguji Plugin:**
   Jalankan binary `bot_app` dan uji command di chat bot dengan mengirimkan pesan:
   `!hello Rek`

---

## 9. Cara Mengatur Plugin

- **Mengaktifkan Plugin:** Letakkan file `.ngawi` yang berisi fungsi `cmd_*` di dalam direktori `plugin/`.
- **Menonaktifkan Plugin:** Hapus atau pindahkan file `.ngawi` tersebut keluar dari direktori `plugin/`, lalu lakukan build ulang aplikasi.
- **Pengaturan Konfigurasi Plugin:** Plugin dapat mengakses parameter konfigurasi global yang dimuat di core melalui argumen fungsi atau variabel global yang dibagikan antar modul.
- **Memeriksa Keberhasilan Pemuatan:** Saat aplikasi bot dijalankan, log konsol akan menampilkan daftar plugin yang berhasil dimuat melalui fungsi `log_plugin()` (mencetak output `[PLUGIN] Loaded plugins:` beserta jumlahnya).

---

## 10. Struktur Direktori

Berikut adalah struktur direktori utama project `ngawi-lang`:

```
/root/ngawi-lang/
├── Makefile
├── README.md
├── main.ngawi
├── bot.c
├── LICENSE
├── docs/
│   ├── compiler-architecture.md
│   ├── contributing.md
│   ├── getting-started.md
│   ├── language-spec.md
│   ├── README.md
│   ├── roadmap.md
│   ├── syntax-guide.md
│   └── testing.md
├── examples/
│   ├── array_empty.ngawi
│   ├── array_int.ngawi
│   ├── hello.ngawi
│   └── ...
├── plugin/
│   ├── info/
│   │   ├── example.ngawi
│   │   ├── mwani.ngawi
│   │   └── ping.ngawi
│   └── search/
│       └── yts.ngawi
└── src/
    ├── logger.ngawi
    ├── main.c
    ├── codegen/
    │   ├── cgen.c
    │   └── cgen.h
    ├── common/
    │   ├── diag.c
    │   └── diag.h
    ├── lexer/
    │   ├── lexer.c
    │   ├── lexer.h
    │   └── token.h
    ├── parser/
    │   ├── ast.h
    │   ├── parser.c
    │   └── parser.h
    ├── runtime/
    │   ├── ngawi_runtime.c
    │   └── ngawi_runtime.h
    └── sema/
        ├── sema.c
        ├── sema.h
        ├── symbols.h
        └── types.h
```

### Penjelasan File dan Direktori Penting
- `main.ngawi`: File entry point utama untuk aplikasi bot Ngawi.
- `src/main.c`: Source code utama driver compiler (`ngawic`) yang mengatur pipeline parsing, modul, pemindaian plugin, dan pemanggilan GCC.
- `src/lexer/`, `src/parser/`, `src/sema/`, `src/codegen/`: Modul inti compiler (lexer, parser AST, semantic analyzer, dan C11 code generator).
- `src/runtime/`: Berisi helper runtime C (`ngawi_runtime.c` dan `ngawi_runtime.h`) yang disertakan pada setiap binary hasil kompilasi.
- `plugin/`: Direktori tempat penyimpanan modular plugin command bot.
- `examples/`: Kumpulan kode contoh program Ngawi dan test case fitur bahasa.
- `docs/`: Dokumentasi lengkap arsitektur, spesifikasi bahasa, dan panduan kontribusi.
- `Makefile`: Script otomatisasi build compiler dan pengujian.

---

## 11. Troubleshooting

Berikut adalah daftar kendala umum beserta penyebab dan solusinya:

- **Error: `cannot scan plugin directory`**
  - *Penyebab:* Direktori `plugin/` tidak ditemukan di path project atau izin akses (permission) ditolak.
  - *Solusi:* Pastikan direktori `plugin/` ada di root project dan memiliki izin baca yang sesuai (`chmod 755 plugin`).
- **Error Semantik: `plugin 'cmd_xxx' must have signature (string, int, string, string, string, string) -> void`**
  - *Penyebab:* Fungsi plugin dengan awalan `cmd_` tidak memenuhi 6 parameter wajib atau tipe kembalian bukan `void`.
  - *Solusi:* Sesuaikan signature fungsi plugin agar persis menerima parameter `(token: string, chat: int, bot_name: string, owner_name: string, prefix: string, args: string)` dan mengembalikan tipe `void`.
- **Error Build: `gcc: command not found` atau error sintaks C**
  - *Penyebab:* Compiler GCC belum terpasang atau versi GCC terlalu lama (tidak mendukung C11).
  - *Solusi:* Install build essentials (`sudo apt install build-essential`) atau perbarui GCC ke versi yang mendukung standar C11.
- **Error Konfigurasi: Gagal membaca file JSON settings**
  - *Penyebab:* File konfigurasi (misal `settings.json`) tidak ditemukan atau format JSON tidak valid.
  - *Solusi:* Pastikan file `settings.json` berada di direktori kerja saat aplikasi dijalankan dan strukturnya valid (tanda kurung dan kutipan benar).

---

## 12. Contoh Penggunaan

Berikut adalah alur lengkap penggunaan project dari awal hingga aplikasi berjalan:

1. **Pastikan berada di direktori project:**
   ```bash
   cd /root/ngawi-bot
   ```
2. **Build compiler `ngawic`:**
   ```bash
   make debug
   ```
3. **Uji kompilasi program contoh (Hello World):**
   ```bash
   ./ngawic build examples/hello.ngawi -o hello
   ./hello
   ```
4. **Compile aplikasi bot utama (`main.ngawi`):**
   ```bash
   ./ngawic build main.ngawi -o bot_app
   ```
5. **Jalankan aplikasi bot:**
   ```bash
   ./bot_app
   ```

---

## 13. Pengembangan

Bagi developer yang ingin berkontribusi, menambahkan fitur bahasa, atau memperbaiki bug:

### Menjalankan Test Suite
Project menyediakan suite pengujian lengkap yang mencakup unit test lexer, parser, sema, end-to-end tests, dan golden snapshots:
```bash
make test
```

### Memperbarui Golden Snapshots
Jika Anda melakukan perubahan pada codegen yang mengubah hasil keluaran C11 secara sah dan ingin memperbarui file referensi golden test:
```bash
make update_golden
```

### Panduan Kontribusi
Baca panduan kontribusi lengkap yang tersedia di:
- `docs/contributing.md`
- `docs/testing.md`
- `docs/compiler-architecture.md`

---

## 14. Lisensi

Project ini dilisensikan di bawah **MIT License**. Lihat file [LICENSE](LICENSE) untuk rincian lengkap.
