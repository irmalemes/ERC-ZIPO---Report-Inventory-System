# Report Inventory System – Final Project Report

## 1. Uvod

Report Inventory System je desktop aplikacija razvijena za organizaciju, evidenciju i praćenje izvještaja koji se izrađuju na redovnoj osnovi.

Glavni cilj projekta je pojednostaviti upravljanje izvještajima tako što se podaci o njima čuvaju na jednom mjestu i smanjuje potreba za ručnim praćenjem. Sistem obuhvata izvještaje iz kategorija PIO/MIO, CBBH i HALCOM.

Aplikacija omogućava dodavanje PDF i DOCX izvještaja, automatsko prepoznavanje podataka iz naziva datoteke, čuvanje podataka u bazi, pretragu i filtriranje postojećih izvještaja, pregled nedostajućih izvještaja za odabrani period, praćenje važnih rokova te pripremu i slanje CBBH email najave.

Projekat je realizovan kroz 10 razvojnih faza, od analize zahtjeva i dizajna sistema do integracije email funkcionalnosti, testiranja i pripreme finalne verzije desktop aplikacije.



## 2. Zahtjevi

Zahtjevi sistema definisani su na početku projekta kako bi se odredile glavne funkcionalnosti aplikacije i način rada sa izvještajima.

### Funkcionalni zahtjevi

- dodavanje PDF i DOCX izvještaja
- automatsko prepoznavanje kategorije, tipa i datuma izvještaja iz naziva datoteke
- podrška za PIO/MIO, CBBH i HALCOM kategorije
- čuvanje podataka o izvještajima u bazi podataka
- pretraga i filtriranje postojećih izvještaja
- pregled izvještaja po izvještajnom periodu
- prepoznavanje nedostajućih izvještaja
- prikaz podsjetnika za rokove 12. i 22. u mjesecu
- priprema i slanje CBBH email najave

### Nefunkcionalni zahtjevi

Aplikacija treba imati jednostavan i pregledan desktop interfejs, trajno čuvati podatke te omogućiti pouzdan rad sa datotekama i bazom podataka. Sistem treba pravilno obrađivati neispravne ili nepoznate nazive datoteka i spriječiti dodavanje duplikata.

## 3. Arhitektura

Aplikacija je organizovana kroz više odvojenih komponenti, pri čemu svaka komponenta ima jasno definisanu odgovornost.

- **Model izvještaja** predstavlja osnovne podatke o izvještaju, kao što su naziv datoteke, kategorija, tip, datum izvještaja, datum dodavanja, veličina i putanja.
- **ReportParser** analizira naziv datoteke i automatski određuje kategoriju, tip i datum izvještaja.
- **FileManager** upravlja datotekama i direktorijima te omogućava provjeru, kopiranje, premještanje i brisanje datoteka.
- **DatabaseManager** omogućava komunikaciju sa SQLite bazom podataka i upravlja čuvanjem, izmjenom, pretragom i brisanjem podataka o izvještajima.
- **UploadManager** povezuje obradu datoteke, parser i bazu podataka prilikom dodavanja novog izvještaja.
- **ReportCalendar** prati očekivane izvještaje za odabrani izvještajni period i određuje koji izvještaji nedostaju.
- **ReminderManager** upravlja rokovima i podsjetnicima vezanim za izvještaje.
- **EmailManager i SmtpClient** omogućavaju pripremu i slanje CBBH email najave.
- **Qt GUI** predstavlja korisnički interfejs kroz koji korisnik pristupa svim glavnim funkcionalnostima sistema.

Ovakva podjela omogućava da se pojedinačni dijelovi sistema razvijaju i testiraju odvojeno, dok zajedno čine jednu funkcionalnu desktop aplikaciju.

## 4. Tehnologije

Za razvoj aplikacije korištene su sljedeće tehnologije i alati:

- **C++17** – osnovni programski jezik korišten za implementaciju aplikacije
- **Qt 6** – razvoj grafičkog korisničkog interfejsa i mrežnih funkcionalnosti
- **SQLite** – lokalna baza podataka za trajno čuvanje podataka o izvještajima
- **CMake** – konfiguracija i izgradnja projekta
- **std::filesystem** – rad sa datotekama i direktorijima
- **Qt Network / SMTP** – slanje email najava
- **Git** – kontrola verzija i rad sa razvojnim granama
- **GitHub** – čuvanje repozitorija, Pull Requestovi i praćenje milestoneova i zadataka
- **Visual Studio Code** – razvoj i uređivanje izvornog koda

Aplikacija je razvijena i testirana na Windows okruženju, a za kompajliranje Qt aplikacije korišten je MinGW toolchain.

## 5. Implementacija

Razvoj projekta podijeljen je na 10 milestoneova, pri čemu je svaki milestone predstavljao jednu funkcionalnu cjelinu.

1. **Analiza zahtjeva i dizajn sistema** – definisani su funkcionalni i nefunkcionalni zahtjevi te izrađeni dijagrami sistema.

2. **C++ OOP model** – implementirane su klase i enumeracije za predstavljanje izvještaja, njihovih kategorija i tipova.

3. **Rad sa datotekama** – implementiran je FileManager za provjeru i upravljanje PDF i DOCX datotekama te njihovo organizovanje u odgovarajuće direktorije.

4. **Parser naziva datoteka** – implementiran je ReportParser koji iz naziva datoteke prepoznaje tip, kategoriju i datum izvještaja te provjerava ispravnost datuma.

5. **SQLite baza podataka** – dodano je trajno čuvanje podataka o izvještajima, zajedno sa funkcijama za unos, izmjenu, brisanje, pretragu i filtriranje podataka.

6. **Qt desktop GUI** – razvijen je grafički korisnički interfejs sa prikazom izvještaja, pretragom, filtriranjem i podrškom za svijetlu i tamnu temu.

7. **Dodavanje i automatska organizacija izvještaja** – omogućen je unos PDF i DOCX izvještaja kroz aplikaciju, automatska obrada naziva, provjera duplikata, kopiranje datoteke i upis u bazu podataka.

8. **Pregled perioda i nedostajućih izvještaja** – implementiran je pregled očekivanih izvještaja za prvu i drugu polovinu mjeseca te određivanje statusa perioda i nedostajućih izvještaja.

9. **Podsjetnici** – implementirano je praćenje rokova 12. i 22. u mjesecu, prikaz preostalog broja dana i podsjetnik za CBBH najavu.

10. **Email integracija i finalizacija** – implementirana je priprema i slanje CBBH email najave putem SMTP-a, završno testiranje sistema te priprema Release verzije aplikacije.

## 6. Testiranje

Tokom razvoja testirane su pojedinačne komponente sistema kako bi se provjerila njihova ispravnost prije finalne integracije.

Testiran je rad modela izvještaja, FileManagera, parsera naziva datoteka, SQLite baze podataka, dodavanja izvještaja, pregleda izvještajnih perioda, sistema podsjetnika i email funkcionalnosti.

Pored testova pojedinačnih komponenti, izvršeno je i ručno testiranje desktop aplikacije. Provjereni su unos i prikaz izvještaja, pretraga i filtriranje, prepoznavanje nedostajućih izvještaja, prikaz podsjetnika, promjena teme i rad korisničkog interfejsa.

SMTP funkcionalnost dodatno je testirana stvarnim slanjem email poruke. Nakon ispravne konfiguracije SMTP podataka, poruka je uspješno poslana i dostavljena na adresu primaoca.

Na kraju razvoja izvršeni su regresijski testovi kako bi se provjerilo da nove funkcionalnosti nisu narušile prethodno implementirane dijelove sistema.

## 7. Problemi i rješenja

Tokom razvoja projekta pojavilo se nekoliko tehničkih problema koji su zahtijevali dodatnu konfiguraciju i prilagođavanje.

Jedan od prvih izazova bio je pravilno povezivanje Qt-a, CMake-a i MinGW toolchaina kako bi se aplikacija mogla uspješno kompajlirati i pokretati iz Visual Studio Code okruženja. Problem je riješen pravilnom konfiguracijom putanja i CMake postavki.

SQLite je integrisan direktno u projekat, nakon čega je implementiran DatabaseManager koji omogućava aplikaciji da upravlja bazom podataka kroz jasno definisane funkcije.

Kod email funkcionalnosti bilo je potrebno podesiti SMTP komunikaciju preko Qt Network modula. Osjetljivi podaci, kao što je App Password, nisu sačuvani u izvornom kodu nego se učitavaju iz environment varijabli.

Na kraju projekta bilo je potrebno pripremiti aplikaciju za pokretanje izvan razvojnog okruženja. Kreiran je Release build, a potrebne Qt zavisnosti dodane su pomoću alata windeployqt.

## 8. Git Development historija

Za kontrolu verzija tokom cijelog projekta korišteni su Git i GitHub.

Razvoj je organizovan kroz odvojene grane za pojedinačne milestoneove. Nakon završetka određene faze, promjene su commitane i pushane na GitHub, a zatim su kroz Pull Request spojene u glavnu `main` granu.

Na GitHubu su također korišteni Issues i Project board za praćenje 10 definisanih milestoneova i napretka projekta.

Ovakav način rada omogućio je preglednu historiju razvoja, odvajanje pojedinačnih faza implementacije i kontrolisano spajanje novih funkcionalnosti u finalnu verziju aplikacije.


## 9. Finalna aplikacija

Finalna verzija Report Inventory Systema predstavlja funkcionalnu desktop aplikaciju koja objedinjuje sve implementirane dijelove sistema.

Korisnik kroz grafički interfejs može dodavati i pregledati izvještaje, vršiti pretragu i filtriranje, pratiti kompletnost izvještaja za odabrani period te pregledati rokove i podsjetnike. Aplikacija također omogućava pripremu i slanje CBBH email najave.

Korisnički interfejs podržava svijetlu i tamnu temu, a raspored elemenata prilagođen je kako bi pregled većeg broja izvještaja, perioda i podsjetnika bio što jednostavniji.

Pored razvojne verzije, pripremljen je i Release build aplikacije. Potrebne Qt zavisnosti uključene su pomoću alata `windeployqt`, čime je omogućeno pokretanje aplikacije iz pripremljenog distribucijskog direktorija bez potrebe za pokretanjem kroz razvojno okruženje.

Detaljne upute za build, konfiguraciju i deployment nalaze se u dokumentu `build-deployment.md`.


## 10. Buduća poboljšanja

Iako finalna verzija aplikacije ispunjava definisane zahtjeve projekta, sistem se u budućnosti može dodatno proširiti.

Moguća poboljšanja uključuju:

- naprednije filtriranje i sortiranje izvještaja
- dodatne opcije za prikaz statistike i historije izvještaja
- automatsko slanje podsjetnika i email najava
- dodatno prilagođavanje korisničkog interfejsa
- izvoz podataka i pregleda izvještaja u druge formate
- proširenje sistema novim kategorijama i tipovima izvještaja

Arhitektura aplikacije omogućava dodavanje novih funkcionalnosti bez potrebe za većim promjenama postojećih komponenti.

## 11. Zaključak

Razvojem Report Inventory Systema realizovana je desktop aplikacija koja omogućava jednostavniju organizaciju, evidenciju i praćenje redovnih izvještaja.

Kroz projekat su povezane različite oblasti razvoja softvera, uključujući objektno orijentisano programiranje u C++, rad sa datotekama, SQLite bazom podataka, razvoj grafičkog interfejsa pomoću Qt-a, testiranje, SMTP komunikaciju i kontrolu verzija pomoću Git-a i GitHub-a.

Razvoj kroz 10 milestoneova omogućio je postepenu implementaciju i testiranje funkcionalnosti, od početne analize zahtjeva do finalne integracije i pripreme Release verzije aplikacije.

Konačni rezultat je funkcionalan sistem koji ispunjava definisane zahtjeve projekta, a stečeno iskustvo obuhvata kompletan proces razvoja desktop aplikacije – od planiranja i implementacije do testiranja, verzionisanja i deploymenta.
