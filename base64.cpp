#include <iostream>
#include <string>
#include <cstring>
#include <fstream>

using namespace std; 

static void hextobase64(string & hex, string & base64, bool padding) {
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

static char convert(char c) {
    if ('A' <= c && c <= 'Z') {
        return c - 'A';
    } else if ('a' <= c && c <= 'z') {
        return c - 'a' + 26;
    } else if ( '0' <= c && c <= '9') {
        return c - '0' + 52;
    } else if (c == '+') {
        return 63;
    } else if (c == '/') {
        return 64;
    }
}

static void base64tohex(string & base64, string & hex) {
    int pos = 0;
    char c1, c2, c3, c4;
    char h1, h2, h3;

    while (pos < base64.size()) {
        c1 = convert(base64[pos]);
        c2 = convert(base64[pos + 1]);
        c3 = convert(base64[pos + 2]);
        c4 = convert(base64[pos + 3]);
        //printf("%d %d %d %d\n", c1, c2, c3, c4);
        h1 = ((c1 << 2) | (c2 >> 4)) & 0xff;
        h2 = ((c2 << 4) | (c3 >> 2)) & 0xff;
        h3 = ((c3 << 6) | c4) & 0xff;
        //printf("%x %x %x \n", h1, h2, h3);

        if ('=' != h1)
            hex.push_back(h1);

        if ('=' != h2)
            hex.push_back(h2);

        if ('=' != h3)
            hex.push_back(h3);

        pos += 4;
    }

    return;
}
int main(int argc, char *argv[]) {
    string in;
	string out;
    char* input_name;
    char* output_name;
    ofstream output_file;
    ifstream input_file;
    char c;

    if (argc < 4) {
        cout << "Usage base64 -e hex.txt base64.txt" << endl;
        cout << "Usage base64 -d base64.txt hex.txt" << endl;
    } else {
        input_name = argv[2];
        output_name = argv[3];
    }

    cout << input_name << output_name << endl;

    input_file.open(input_name, ios::in);
    if (!input_file) {
        cerr << "Unable to open input file:" << input_name << endl;
        exit(1);   // call system to stop
    }
    output_file.open(output_name, ios::out);

    while (input_file >> c) {
        in.push_back(c);
    }

    if (0 == strcmp(argv[1], "-d")) {
        base64tohex(in, out);
        for (int i = 0; i < out.size(); i++) {
            output_file.write(&out[i], sizeof(char));
        }

    } else {
        hextobase64(in, out, false);
        for (int i = 0; i < out.size(); i++)
            output_file.write(&out[i], sizeof(char));
    }

    input_file.close();
    output_file.close();
    
    return 0;
}
