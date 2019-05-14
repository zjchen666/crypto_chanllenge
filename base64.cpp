```cpp
#include <iostream>
#include <string>

using namespace std;

static void hex2base64(string & hex, string & base64, bool padding) {
    int pos = 0, j = 0, val = 0;
    string sub;
    int v;

    while (pos < hex.size()) {
        sub = hex.substr(pos, 6);
        val = stoi(sub, 0, 16);
        j = sub.size() / 2;
        while (j >= 0) {
        	v = ((val >> (j * 6)) & 0x3F);
        	if (v <= 26) {
                base64.push_back(v + 'A');
            } else if (v <= 52) {
            	base64.push_back(v - 26 + 'a');
            } else if (v <= 62) {
            	base64.push_back(v - 52 + '0');
            } else if (v == 63) {
            	base64.push_back('+');
            } else if (v == 64) {
            	base64.push_back('-');
            }
            j--;
        }
        pos += 6;
    }

    // padding
    if (padding) {
        int pad_size = 4 - base64.size() % 4;
        while (pad_size) {
            base64.push_back('=');
            pad_size--;
        }	
    }

    return;
}

int main(void) {
    string hex("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d");
    string base64;
	
    hex2base64(hex, base64, false);
    for (int i = 0; i < base64.size(); i++)
        cout << base64[i];
    return 0;
}
```
