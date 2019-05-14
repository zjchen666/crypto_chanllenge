#include <iostream>
#include <string>

using namespace std;

static void enc(string & plain_text, string & key, string & cipher_text) {
    int size = plain_text.size();
    string digits("0123456789abcdef");

    if (plain_text.size() != key.size()) {
        cout << "Error, input string length are not same" << endl;
    }

    for (int i = 0; i < size; ++i) {
        int index = stoi(plain_text.substr(i, 1), 0, 16) ^ stoi(key.substr(i, 1), 0, 16);
        cipher_text.push_back(digits[index]);
    }

    return;
}


int main(void) {
	string plain_text("1c0111001f010100061a024b53535009181c");
	string key("686974207468652062756c6c277320657965");
	string cipher_text;

    enc(plain_text, key, cipher_text);

    for (int i = 0; i < cipher_text.size(); i++)
    	cout << cipher_text[i];

	return 0;
}
