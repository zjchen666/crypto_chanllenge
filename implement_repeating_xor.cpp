#include <iostream>
#include <string>

using namespace std;

void encrypt(string & msg, string & cipher) {
    string key("ICE");
    uint i = 0;

    for (i = 0; i < msg.size(); i++) {
    	int val = (msg[i] ^ key[i % 3]);
        cipher += ((val / 16) >= 10) ? ((val / 16 - 10) + 'a') : ((val / 16) + '0');
        cipher += ((val % 16) >= 10) ? ((val % 16 - 10) + 'a') : ((val % 16) + '0');
    }
    return;
}

int main(void)
{
    string msg("Burning 'em, if you ain't quick and nimble I go crazy when I hear a cymbal");
    string cipher;

    encrypt(msg, cipher);

    for(uint i = 0; i < cipher.size(); ++i) {
	cout << cipher[i];
    }
	
    return 0;
}
