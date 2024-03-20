#include <iostream>
#include <vector>
#include <string> 
#include <cctype>
#include <bitset>
using namespace std;

string caesar(string text, string key) {
    string cipher;
    int shift_digit = 0;
    int overflow_value = 0;
    shift_digit = stoi(key) % 26;
    for (int i = 0; i < text.length(); i++) {

        //check whether the key had upper case 
        if (isupper(text[i])){
            if (text[i] - 65 + shift_digit > 25) {
                overflow_value = text[i] - 65 + shift_digit - 26;
                cipher += char(overflow_value + 65);
            }
            else {
                cipher += char((text[i] - 65 + shift_digit) + 65);
            }
        }
        else {
            if (text[i] - 97 + shift_digit > 25) {
                overflow_value = text[i] - 97 + shift_digit - 26;
                cipher += char(overflow_value + 65);
            }
            else {
                cipher += char((text[i] - 97 + shift_digit) + 65);
            }
        }
    }

    return cipher;
}
string playfair(string text, string key) {
    string cipher;
    vector<vector<char> > table;
    vector<char> alphabet;
    table.resize(5);

    //transfer the key to lower case
    for (int i = 0; i < key.length(); i++) {
        if (isupper(key[i])) {
            key[i] = key[i] - 65 + 97;
        }
    }

    //whether plaintext has same letter
    for (int i = 0; i < text.size(); i++) {
        if (i + 1 == text.size()) {

        }
        else if (text[i] == text[i + 1]) {
            text.insert(text.begin() + i, 'x');
        }
    }
    
    //whether plaintext is odd digits
    if (text.length() % 2 != 0) {
        if (text[text.length() - 1] == 'x') {
            text += 'q';
        }
        else {
            text += 'x';
        }
    }

    //transfer the plaintext to lower key
    for (int i = 0; i < text.length(); i++) {
        if (isupper(text[i])) {
            text[i] = text[i] - 65 + 97;
        }
    }

    //create alphabet list
    alphabet.resize(26);
    for (int i = 0; i < key.size(); i++) {
        for (int j = 0; j < 26; j++) {
            if (key[i] - 97 == j) {
                alphabet[j] = 1;
            }
        }
    }

    //initial the table
    for (int i = 0; i < table.size(); i++) {
        table[i].resize(5);
    }

    //put key in the table
    int x = 0, y = 0;
    for (int i = 0; i < key.length(); i++) {
        table[x][y] = key[i];
        y++;
        if (y == 5) {
            y = 0;
            x++;
        }
    }

    //complete the table
    x = 0, y = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < alphabet.size(); k++) {
                if (table[i][j] == 0 && alphabet[k] == 0 && k != 9) {
                    table[i][j] = char(97 + k);
                    alphabet[k] = 1;
                    break;
                }
            }
        }
    }

    //encrypt
    int pos_x1 = 0, pos_x2 = 0;
    int pos_y1 = 0, pos_y2 = 0;
    for (int i = 0; i < text.length(); i += 2) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                if (text[i] == table[j][k]) {
                    pos_x1 = j;
                    pos_y1 = k;
                }
                if (text[i + 1] == table[j][k]) {
                    pos_x2 = j;
                    pos_y2 = k;
                }
            }
        }
        if (pos_x1 == pos_x2) {
            if ((pos_y1 + 1) > 4) {
                pos_y1 = 0;
                cipher += toupper(table[pos_x1][pos_y1]);
            }
            else {
                cipher += toupper(table[pos_x1][pos_y1 + 1]);
            }
            if ((pos_y2 + 1) > 4) {
                pos_y2 = 0;
                cipher += toupper(table[pos_x2][pos_y2]);
            }
            else {
                cipher += toupper(table[pos_x2][pos_y2 + 1]);
            }
        }
        else if (pos_y1 == pos_y2) {
            if ((pos_x1 + 1) > 4) {
                pos_x1 = 0;
                cipher += toupper(table[pos_x1][pos_y1]);
            }
            else {
                cipher += toupper(table[pos_x1 + 1][pos_y1]);
            }
            if ((pos_x2 + 1) > 4) {
                pos_x2 = 0;
                cipher += toupper(table[pos_x2][pos_y2]);
            }
            else {
                cipher += toupper(table[pos_x2 + 1][pos_y2]);
            }
        }
        else {
            int temp = 0;
            temp = pos_y1;
            pos_y1 = pos_y2;
            pos_y2 = temp;
            cipher += toupper(table[pos_x1][pos_y1]);
            cipher += toupper(table[pos_x2][pos_y2]);
        }
    }
  
    return cipher;
}

string vernam(string text, string key) {
    string cipher;
    string autokey;

    //convert text to lower case
    for (int i = 0; i < text.length(); i++) {
        if (isupper(text[i])) {
            text[i] = text[i] - 65 + 97;
        }
    }

    //create autokey
    autokey = key += text;
    for (int i = 0; i < autokey.length(); i++) {
        if (isupper(autokey[i])) {
            autokey[i] = autokey[i] - 65 + 97;
        }
    }

    //encrypt
    char result;
    int decimal = 0;
    for (int i = 0; i < text.length(); i++) {
        result = char((text[i] - 'a') ^ (autokey[i] - 'a'));
        cipher += char(result + 'A');
    }
    return cipher;
}

string railfence(string text, string key) {
    string cipher;
    vector<vector<char> > fence;
    bool direction_down = false;
    int row = 0, column = 0;

    //convert plaintext to lower case 
    for (int i = 0; i < text.length(); i++) {
        if (isupper(text[i])) {
            text[i] = text[i] - 65 + 97;
        }
    }

    //convert key from string to integer
    int key_int = key[0] - '0';

    //initial the table
    vector<vector<char> > table;
    table.resize(key_int);
    for (int i = 0; i < table.size(); i++) {
        table[i].resize(text.length());
    }


    //encrypt
    for (int i = 0; i < key_int; i++)
        for (int j = 0; j < text.length(); j++)
            table[i][j] = '\n';


    for (int i = 0; i < text.length(); i++)
    {
        if (row == 0 || row == key_int - 1) {
            direction_down = !direction_down;
        }
        table[row][column++] = text[i];
        if (direction_down) {
            row++;
        }
        else {
            row--;
        }
    }

    for (int i = 0; i < key_int; i++) {
        for (int j = 0; j < text.length(); j++) {
            if (table[i][j] != '\n') {
                cipher += toupper(table[i][j]);
            }
        }
    }
            
    return cipher;
}

string row(string text, string key) {
    string cipher;
    vector<vector<char> > table;

    //complete the plaintext
    if (text.length() % key.length() != 0) {
        int difference = text.length() % key.length();
        for (int i = 0; i < difference; i++) {
            text += 'x';
        }
    }

    //initial the table 
    int row = 0;
    row = (text.length() / key.length());
    table.resize(row);
    int k = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < key.length(); j++) {
            table[i].push_back(text[k]);
            k++;
        }
    }

    //sorting
    vector<char> tmp;
    tmp.resize(row);
    for (int i = 0; i < key.length() - 1; i++) {
        for (int j = 0; j < key.length() - i - 1; j++) {
            if (key[j] > key[j + 1]) {
                int temp = key[j];
                key[j] = key[j + 1];
                key[j + 1] = temp;

                //table column switch
                for (int k = 0; k < row; k++) {
                    tmp[k] = table[k][j];
                    table[k][j] = table[k][j + 1];
                    table[k][j + 1] = tmp[k];
                }
            }
        }
    }

    //encrypt
    for (int i = 0; i < key.length(); i++) {
        for (int j = 0; j < row; j++) {
            cipher += toupper(table[j][i]);
        }
    }

    return cipher;
}

int main(int argc, char** argv) { 
    string method;
    string key;
    string plaintext;
    method = argv[2];
    plaintext = argv[4];
    key = argv[6];
    //cin >> method >> plaintext >> key;
    if (method == "caesar") {
        cout << caesar(plaintext, key) << endl;
    }
    else if (method == "playfair") {
        cout << playfair(plaintext, key) << endl;
    }
    else if (method == "vernam") {
        cout << vernam(plaintext, key) << endl;
    }
    else if (method == "railfence") {
        cout << railfence(plaintext, key) << endl;
    }
    else if (method == "row") {
        cout << row(plaintext, key) << endl;
    }
}
