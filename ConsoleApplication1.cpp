#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <sstream>

class Carte {
public:
    std::string titlu;
    std::string autor;
    std::vector<std::string> cuvinte_cheie;
    int an_aparitie;
    std::string domeniu;
    std::string editura;

    Carte(std::string titlu, std::string autor, std::vector<std::string> cuvinte_cheie, int an_aparitie, std::string domeniu, std::string editura)
        : titlu(titlu), autor(autor), cuvinte_cheie(cuvinte_cheie), an_aparitie(an_aparitie), domeniu(domeniu), editura(editura) {}
};

class Biblioteca {
private:
    std::vector<Carte> carti;

public:
    void adaugaCarte(const Carte& carte) {
        carti.push_back(carte);
    }

    void actualizeazaCarte(int index, const Carte& carteNoua) {
        if (index < 0 || index >= carti.size()) {
            throw std::out_of_range("Index invalid.");
        }
        carti[index] = carteNoua;
    }

    std::vector<Carte> cartiDeAutor(const std::string& autor) {
        std::vector<Carte> rezultat;
        for (const auto& carte : carti) {
            if (carte.autor == autor) {
                rezultat.push_back(carte);
            }
        }
        return rezultat;
    }

    std::vector<Carte> cartiCuCuvinteCheie(const std::vector<std::string>& cuvinte) {
        std::vector<Carte> rezultat;
        for (const auto& carte : carti) {
            bool toateCuvintelePrezente = std::all_of(cuvinte.begin(), cuvinte.end(), [&](const std::string& cuvant) {
                return std::find(carte.cuvinte_cheie.begin(), carte.cuvinte_cheie.end(), cuvant) != carte.cuvinte_cheie.end();
                });
            if (toateCuvintelePrezente) {
                rezultat.push_back(carte);
            }
        }
        return rezultat;
    }

    std::vector<Carte> cartiIntreAni(int an_inceput, int an_sfarsit) {
        std::vector<Carte> rezultat;
        for (const auto& carte : carti) {
            if (carte.an_aparitie >= an_inceput && carte.an_aparitie <= an_sfarsit) {
                rezultat.push_back(carte);
            }
        }
        return rezultat;
    }

    std::vector<Carte> cartiDinDomeniu(const std::string& domeniu) {
        std::vector<Carte> rezultat;
        for (const auto& carte : carti) {
            if (carte.domeniu == domeniu) {
                rezultat.push_back(carte);
            }
        }
        return rezultat;
    }

    std::vector<Carte> cartiDeLaEditura(const std::string& editura) {
        std::vector<Carte> rezultat;
        for (const auto& carte : carti) {
            if (carte.editura == editura) {
                rezultat.push_back(carte);
            }
        }
        return rezultat;
    }

    // Stocare și restaurare date
    void salveazaDatele(const std::string& fisier) {
        std::ofstream fout(fisier);
        for (const auto& carte : carti) {
            fout << carte.titlu << "," << carte.autor << "," << carte.an_aparitie << ","
                << carte.domeniu << "," << carte.editura;
            for (const auto& cuvant : carte.cuvinte_cheie) {
                fout << "," << cuvant;
            }
            fout << "\n";
        }
        fout.close();
    }

    void incarcaDatele(const std::string& fisier) {
        std::ifstream fin(fisier);
        std::string linie;
        while (std::getline(fin, linie)) {
            std::istringstream iss(linie);
            std::string titlu, autor, domeniu, editura, cuvant;
            int an_aparitie;
            std::vector<std::string> cuvinte_cheie;

            std::getline(iss, titlu, ',');
            std::getline(iss, autor, ',');
            iss >> an_aparitie;
            iss.ignore(1, ','); // Ignoră separatorul de după anul de apariție
            std::getline(iss, domeniu, ',');
            std::getline(iss, editura, ',');

            while (std::getline(iss, cuvant, ',')) {
                cuvinte_cheie.push_back(cuvant);
            }

            Carte carte(titlu, autor, cuvinte_cheie, an_aparitie, domeniu, editura);
            carti.push_back(carte);
        }
        fin.close();
    }

    class Iterator {
    private:
        typename std::vector<Carte>::iterator it;
    public:
        Iterator(typename std::vector<Carte>::iterator iterator) : it(iterator) {}

        Carte& operator*() {
            return *it;
        }

        Iterator& operator++() {
            ++it;
            return *this;
        }

        bool operator!=(const Iterator& other) {
            return it != other.it;
        }
    };

    Iterator begin() {
        return Iterator(carti.begin());
    }

    Iterator end() {
        return Iterator(carti.end());
    }
};

// Funcție pentru a interacționa cu utilizatorul
void interfataUtilizator(Biblioteca& biblioteca) {
    int optiune;
    do {
        std::cout << "1. Adauga carte\n";
        std::cout << "2. Actualizeaza carte\n";
        std::cout << "3. Afiseaza carti de autor\n";
        std::cout << "4. Afiseaza carti cu cuvinte cheie\n";
        std::cout << "5. Afiseaza carti intre ani\n";
        std::cout << "6. Afiseaza carti din domeniu\n";
        std::cout << "7. Afiseaza carti de la editura\n";
        std::cout << "8. Salveaza datele\n";
        std::cout << "9. Incarca datele\n";
        std::cout << "0. Iesire\n";
        std::cin >> optiune;

        try {
            if (optiune == 1) {
                std::string titlu, autor, domeniu, editura;
                int an_aparitie;
                std::vector<std::string> cuvinte_cheie;
                std::cout << "Titlu: ";
                std::cin >> titlu;
                std::cout << "Autor: ";
                std::cin >> autor;
                std::cout << "An aparitie: ";
                std::cin >> an_aparitie;
                std::cout << "Domeniu: ";
                std::cin >> domeniu;
                std::cout << "Editura: ";
                std::cin >> editura;
                std::cout << "Introdu cuvintele cheie (terminate cu '-1'): ";
                std::string cuvant;
                while (std::cin >> cuvant && cuvant != "-1") {
                    cuvinte_cheie.push_back(cuvant);
                }

                Carte carte(titlu, autor, cuvinte_cheie, an_aparitie, domeniu, editura);
                biblioteca.adaugaCarte(carte);
            }
            else if (optiune == 2) {
                int index;
                std::string titlu, autor, domeniu, editura;
                int an_aparitie;
                std::vector<std::string> cuvinte_cheie;
                std::cout << "Index carte: ";
                std::cin >> index;
                std::cout << "Titlu nou: ";
                std::cin >> titlu;
                std::cout << "Autor nou: ";
                std::cin >> autor;
                std::cout << "An aparitie nou: ";
                std::cin >> an_aparitie;
                std::cout << "Domeniu nou: ";
                std::cin >> domeniu;
                std::cout << "Editura noua: ";
                std::cin >> editura;
                std::cout << "Introdu cuvintele cheie noi (terminate cu '-1'): ";
                std::string cuvant;
                while (std::cin >> cuvant && cuvant != "-1") {
                    cuvinte_cheie.push_back(cuvant);
                }

                Carte carteNoua(titlu, autor, cuvinte_cheie, an_aparitie, domeniu, editura);
                biblioteca.actualizeazaCarte(index, carteNoua);
            }
            else if (optiune == 3) {
                std::string autor;
                std::cout << "Autor: ";
                std::cin >> autor;
                auto carti = biblioteca.cartiDeAutor(autor);
                for (const auto& carte : carti) {
                    std::cout << "Titlu: " << carte.titlu << ", An: " << carte.an_aparitie << "\n";
                }
            }
            else if (optiune == 4) {
                std::vector<std::string> cuvinte;
                std::string cuvant;
                std::cout << "Introdu cuvintele cheie (terminate cu '-1'): ";
                while (std::cin >> cuvant && cuvant != "-1") {
                    cuvinte.push_back(cuvant);
                }
                auto carti = biblioteca.cartiCuCuvinteCheie(cuvinte);
                for (const auto& carte : carti) {
                    std::cout << "Titlu: " << carte.titlu << ", Autor: " << carte.autor << "\n";
                }
            }
            else if (optiune == 5) {
                int an_inceput, an_sfarsit;
                std::cout << "An inceput: ";
                std::cin >> an_inceput;
                std::cout << "An sfarsit: ";
                std::cin >> an_sfarsit;
                auto carti = biblioteca.cartiIntreAni(an_inceput, an_sfarsit);
                for (const auto& carte : carti) {
                    std::cout << "Titlu: " << carte.titlu << ", An: " << carte.an_aparitie << "\n";
                }
            }
            else if (optiune == 6) {
                std::string domeniu;
                std::cout << "Domeniu: ";
                std::cin >> domeniu;
                auto carti = biblioteca.cartiDinDomeniu(domeniu);
                for (const auto& carte : carti) {
                    std::cout << "Titlu: " << carte.titlu << ", Domeniu: " << carte.domeniu << "\n";
                }
            }
            else if (optiune == 7) {
                std::string editura;
                std::cout << "Editura: ";
                std::cin >> editura;
                auto carti = biblioteca.cartiDeLaEditura(editura);
                for (const auto& carte : carti) {
                    std::cout << "Titlu: " << carte.titlu << ", Editura: " << carte.editura << "\n";
                }
            }
            else if (optiune == 8) {
                std::string fisier;
                std::cout << "Nume fisier pentru salvare: ";
                std::cin >> fisier;
                biblioteca.salveazaDatele(fisier);
            }
            else if (optiune == 9) {
                std::string fisier;
                std::cout << "Nume fisier pentru incarcare: ";
                std::cin >> fisier;
                biblioteca.incarcaDatele(fisier);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Eroare: " << e.what() << "\n";
        }
    } while (optiune != 0);
}

int main() {
    Biblioteca biblioteca;

    interfataUtilizator(biblioteca);

    return 0;
}
