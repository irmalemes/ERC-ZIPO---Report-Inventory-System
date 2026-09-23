# Report Inventory System - Build i Deployment

## 1. Zahtjevi

Za build i pokretanje projekta korišteni su:

- C++17
- CMake
- Qt 6.11.2
- MinGW 13.1.0
- SQLite
- Windows operativni sistem

Qt putanja korištena tokom razvoja:

`C:\Qt\6.11.2\mingw_64`

MinGW alat:

`C:\Qt\Tools\mingw1310_64`

## 2. Build projekta

Iz root direktorija projekta prvo je potrebno konfigurirati CMake:

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH=C:/Qt/6.11.2/mingw_64
```

Nakon konfiguracije projekat se builda komandom:

```powershell
cmake --build build
```

Glavni executable se nakon uspješnog builda nalazi na:

```text
build\report_inventory.exe
```

Ako je potrebno pokrenuti aplikaciju direktno iz development builda, Qt i MinGW putanje se mogu dodati u trenutnu PowerShell sesiju:

```powershell
$env:Path = "C:\Qt\6.11.2\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;$env:Path"
```

Aplikacija se zatim pokreće komandom:

```powershell
.\build\report_inventory.exe
```

## 3. SMTP konfiguracija

Email funkcionalnost koristi SMTP konfiguraciju učitanu iz environment variables. Osjetljivi podaci, kao što su password i SMTP credentials, ne čuvaju se direktno u source kodu.

Potrebne environment variables su:

```text
RIS_SMTP_HOST
RIS_SMTP_PORT
RIS_SMTP_USERNAME
RIS_SMTP_PASSWORD
RIS_SMTP_FROM
```

Primjer konfiguracije za Gmail SMTP:

```powershell
$env:RIS_SMTP_HOST = "smtp.gmail.com"
$env:RIS_SMTP_PORT = "465"
$env:RIS_SMTP_USERNAME = "test@example.com"
$env:RIS_SMTP_FROM = "test@example.com"
$env:RIS_SMTP_PASSWORD = "<APP_PASSWORD>"
```

Za Gmail se koristi App Password umjesto običnog passworda korisničkog računa.

Vrijednost `RIS_SMTP_PASSWORD` ne smije se zapisivati u source kod, Git repozitorij ili dokumentaciju projekta.

Environment variables postavljene na ovaj način vrijede za trenutnu PowerShell sesiju, pa aplikaciju treba pokrenuti iz iste sesije.

## 4. Release build i deployment

Za finalnu verziju aplikacije koristi se poseban Release build:

```powershell
cmake -S . -B build-release -G "MinGW Makefiles" `
  -DCMAKE_MAKE_PROGRAM=C:/Qt/Tools/mingw1310_64/bin/mingw32-make.exe `
  -DCMAKE_C_COMPILER=C:/Qt/Tools/mingw1310_64/bin/gcc.exe `
  -DCMAKE_CXX_COMPILER=C:/Qt/Tools/mingw1310_64/bin/g++.exe `
  -DCMAKE_PREFIX_PATH=C:/Qt/6.11.2/mingw_64 `
  -DCMAKE_BUILD_TYPE=Release
```

Release verzija se zatim builda komandom:

```powershell
cmake --build build-release
```

Finalni executable nalazi se na:

```text
build-release\report_inventory.exe
```

Za deployment se kreira `dist` direktorij i kopira executable:

```powershell
New-Item -ItemType Directory -Force .\dist
Copy-Item .\build-release\report_inventory.exe .\dist\
```

Potrebne Qt biblioteke i pluginovi kopiraju se pomoću `windeployqt`:

```powershell
C:\Qt\6.11.2\mingw_64\bin\windeployqt.exe .\dist\report_inventory.exe
```

Potrebno je kreirati i direktorij za bazu podataka:

```powershell
New-Item -ItemType Directory -Force .\dist\data
```

Finalna aplikacija se pokreće iz `dist` direktorija:

```powershell
cd .\dist
.\report_inventory.exe
```

Deployment verzija je testirana pokretanjem direktno iz `dist` direktorija.

## 5. Testiranje i riješeni problemi

Tokom razvoja izvršeni su unit i regresijski testovi za funkcionalnosti implementirane kroz milestone-e projekta.

Testirane su sljedeće komponente:

- Report model
- FileManager
- ReportParser
- DatabaseManager
- UploadManager
- ReportCalendar
- ReminderManager
- EmailManager

Svi testovi su uspješno prošli nakon finalne integracije.

Email funkcionalnost dodatno je ručno testirana slanjem testnog emaila preko Gmail SMTP servisa. Testni email je uspješno poslan i primljen.

Tokom finalne integracije riješeni su problemi vezani za:

- validaciju SMTP konfiguracije
- sigurno učitavanje SMTP porta
- Qt build upozorenja u source kodu
- email preview i validaciju primaoca
- Release build i Qt deployment

SMTP password i ostali osjetljivi podaci nisu spremljeni u source kod niti Git repozitorij.