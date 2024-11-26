#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

string accountsFile = "ListAccount.txt";
void loading(WINDOW *menu_win){
	initscr();
	wclear(menu_win);
	box(menu_win, 0, 0);
	
	for (int i= 0; i < 3; i++){
		mvprintw(12.5,41,"Loading...");
		mvprintw(13.5,41,"-----------------------------------");
		mvprintw(14.5,41,"|                                 |");
		mvprintw(15.5,41,"-----------------------------------");
		
		for (int f = 1; f<34; f++){
			mvprintw(14.5,41 + f, ">");
			refresh();
			Sleep(50);
		}
	}
	clear();
	refresh();
	endwin();
}

void login_ncurses(WINDOW *menu_win) {
    string userLog, passLog, userData;
    ifstream myFile;
    bool exist = false;

    wclear(menu_win);
    box(menu_win, 0, 0);
    mvwprintw(menu_win, 1, 2, "----> LOGIN <----");

    mvwprintw(menu_win, 3, 2, "Masukkan Username: ");
    wrefresh(menu_win);
    echo();
    char username[50];
    wgetstr(menu_win, username);
    noecho();
    userLog = username;

    if (userLog.empty()) {
        mvwprintw(menu_win, 5, 2, "Username tidak boleh kosong!");
        wrefresh(menu_win);
        wgetch(menu_win);
        return;
    }

    mvwprintw(menu_win, 5, 2, "Masukkan Password: ");
    wrefresh(menu_win);
    char password[50];
    wgetstr(menu_win, password);
    passLog = password;

    if (passLog.empty()) {
        mvwprintw(menu_win, 7, 2, "Password tidak boleh kosong!");
        wrefresh(menu_win);
        wgetch(menu_win);
        return;
    }

    myFile.open(accountsFile);
    if (!myFile) {
        mvwprintw(menu_win, 7, 2, "Kesalahan dalam membaca list akun!");
        wrefresh(menu_win);
        wgetch(menu_win);
        return;
    }

    while (myFile >> userData) {
        if (userData == userLog) {
            exist = true;
            break;
        }
    }
    myFile.close();
    loading(menu_win);
    clear();
    refresh();

    if (exist) {
        mvwprintw(menu_win, 7, 2, "Login Berhasil!");
    } else {
        mvwprintw(menu_win, 7, 2, "------------------------------------------");
        mvwprintw(menu_win, 8, 2, "                 LOGIN GAGAL              ");
        mvwprintw(menu_win, 9, 2, "Periksa kembali username dan password anda");
        mvwprintw(menu_win, 10, 2, "------------------------------------------");
    }

    mvwprintw(menu_win, 11, 2, "Tekan Enter untuk kembali ke menu.");
    wrefresh(menu_win);
    wgetch(menu_win);
}

void registration_ncurses(WINDOW *menu_win) {
    string userReg, passReg;
    ofstream myFile, accList;

    wclear(menu_win);
    box(menu_win, 0, 0);
    mvwprintw(menu_win, 1, 2, "----> REGISTER <----");

    mvwprintw(menu_win, 3, 2, "Masukkan Username: ");
    wrefresh(menu_win);
    echo();
    char username[50];
    wgetstr(menu_win, username);
    noecho();
    userReg = username;

    if (userReg.empty()) {
        mvwprintw(menu_win, 5, 2, "Username tidak boleh kosong!");
        wrefresh(menu_win);
        wgetch(menu_win);
        return;
    }

    mvwprintw(menu_win, 5, 2, "Masukkan Password: ");
    wrefresh(menu_win);
    char password[50];
    wgetstr(menu_win, password);
    passReg = password;

    if (passReg.empty()) {
        mvwprintw(menu_win, 7, 2, "Password tidak boleh kosong!");
        wrefresh(menu_win);
        wgetch(menu_win);
        return;
    }

    myFile.open(userReg + ".txt");
    if (!myFile) {
        mvwprintw(menu_win, 7, 2, "Kesalahan dalam membuat file!");
        wrefresh(menu_win);
        wgetch(menu_win);
        return;
    }
    myFile << 0; 
    myFile.close();
    loading(menu_win);
    clear();
    refresh();

    accList.open(accountsFile, ios::app);
    if (!accList) {
        mvwprintw(menu_win, 7, 2, "Kesalahan dalam menambahkan list akun");
        wrefresh(menu_win);
        wgetch(menu_win);
        return;
    }
    accList << userReg << endl;
    accList.close();

    mvwprintw(menu_win, 7, 2, "Registrasi Berhasil!");
    mvwprintw(menu_win, 8, 2, "Tekan Enter untuk kembali ke menu.");
    wrefresh(menu_win);
    wgetch(menu_win);
}

void print_menu(WINDOW *menu_win, int pilihanTerpilih, char *opsi[], int jumlahopsi) {
    wclear(menu_win);
    box(menu_win, 0, 0);
    mvwprintw(menu_win, 1, 2, "---- MENU ---");

    for (int i = 0; i < jumlahopsi; ++i) {
        if (i == pilihanTerpilih) {
            wattron(menu_win, A_REVERSE); 
        }
        mvwprintw(menu_win, i + 3, 2, opsi[i]);
        if (i == pilihanTerpilih) {
            wattroff(menu_win, A_REVERSE);
        }
    }
    wrefresh(menu_win);
}

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    int lebar = 45, tinggi = 13;
    int awalx = (COLS - lebar) / 2;  
    int awaly = (LINES - tinggi) / 2;

    WINDOW *menu_win = newwin(tinggi, lebar, awaly, awalx);
    keypad(menu_win, TRUE);

    char *opsi[] = {
        (char *)"Register",
        (char *)"Login",
        (char *)"Exit"
    };

    int jumlahopsi = sizeof(opsi) / sizeof(char *);
    int pilihanTerpilih = 0;
    int pilihan = -1;

    while (1) {
        print_menu(menu_win, pilihanTerpilih, opsi, jumlahopsi);

        int tombol = wgetch(menu_win);

        switch (tombol) {
             case KEY_UP:
                if (pilihanTerpilih == 0) {
                    pilihanTerpilih = jumlahopsi - 1; 
                } else {
                    --pilihanTerpilih; 
                }
                break;

            case KEY_DOWN:
                if (pilihanTerpilih == jumlahopsi - 1) {
                    pilihanTerpilih = 0; 
                } else {
                    ++pilihanTerpilih;
                }
                break;

            case 10: 
                pilihan = pilihanTerpilih; 
                break;
        }

        if (pilihan == 0) {
            registration_ncurses(menu_win);
            pilihan = -1; 
        } else if (pilihan == 1) {
            login_ncurses(menu_win);
            pilihan = -1;
        } else if (pilihan == 2) {
            break;
        }
    }

    endwin();
    return 0;
}
