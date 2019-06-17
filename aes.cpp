#include <iostream>
#include <vector>

using namespace std;

typedef uint8_t state_t[4][4];
// The lookup-tables are marked const so they can be placed in read-only storage instead of RAM
// The numbers below can be computed dynamically trading ROM for RAM - 
// This can be useful in (embedded) bootloader applications, where ROM is often limited.
static const uint8_t sbox[256] = {
  //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

static const uint32_t rcon[11] = { 0xbeef, 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
                                  0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000 };

void byteSub(state_t & block) {
    uint8_t i = 0, j = 0;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++)
            block[i][j] = sbox[block[i][j]];
    }

    printf("after %s\n", __func__);
    for (int i = 0; i < 4; ++i) {
        printf("0x%x\n", *(uint32_t*)block[i]);
	}

    return;
}

void shiftRows(state_t & block) {
    //row 0 do nothing

    //row 1
    swap(block[1][0], block[1][1]);
    swap(block[1][1], block[1][2]);
    swap(block[1][2], block[1][3]);

    //row 2
    swap(block[2][0], block[2][2]);
    swap(block[2][1], block[2][3]);
    
    //row 3
    swap(block[3][0], block[3][3]);
    swap(block[3][1], block[3][3]);
    swap(block[3][2], block[3][3]);

    printf("after %s\n", __func__);
    for (int i = 0; i < 4; ++i) {
        printf("0x%x\n", *(uint32_t*)block[i]);
	}

    return;
  }

void addRoundKey(state_t & data, uint32_t* key) {

    printf("round key value\n");

    for (int i = 0; i < 4; ++i) {
        printf("0x%x\n", *((uint32_t*)key + i));
	}

	for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            uint32_t tmp = *(key + j);
            tmp = tmp >> (8*(3 - i));
            tmp = tmp & 0xff;
            data[i][j] = data[i][j] ^ tmp;
        }
    }

    printf("after AddRoundKey\n");
    
    for (int i = 0; i < 4; ++i) {
        printf("0x%x\n", *(uint32_t*)data[i]);
	}
	return;
}

/* key sechdule*/
void rotWord(uint32_t* w) {
    uint32_t tmp = 0;

	tmp = (((*w) & 0xff000000) >> 24) |
          (((*w) & 0x00ff0000) << 8) |
          (((*w) & 0x0000ff00) << 8) |
          (((*w) & 0x000000ff) << 8);

    (*w) = tmp;
    return;
}

void subWord(uint8_t* bytes) {
	bytes[0] = sbox[bytes[0]];
	bytes[1] = sbox[bytes[1]];
	bytes[2] = sbox[bytes[2]];
	bytes[3] = sbox[bytes[3]];
	return;
}

#define KEY_LEN  (4)
#define ROUNDS   (11)

void KeyExpansion(uint32_t* init_key,  uint32_t* round_keys) {
    uint32_t temp;
    uint32_t i = 0;

    while (i < KEY_LEN) {
       round_keys[i] = init_key[i];
       i++;
    }

    for (i = 4; i < 44; ++i) {
        temp = round_keys[i-1];
        if (i % KEY_LEN == 0) {
            rotWord(&temp);
            //cout << "rotword:" << hex << temp << endl;
            subWord((uint8_t*)&temp);
            //cout << "subword:" << hex << temp << endl;
            temp = temp ^ rcon[i/KEY_LEN];
            //cout << "rcon:" << hex << temp << endl;
        }
        round_keys[i] = round_keys[i-KEY_LEN] ^ temp;
        //cout << "round:" << hex << round_keys[i] << endl;
    }

    return;
}
#if 0
static uint8_t xtime(uint8_t x)
{
  return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

// MixColumns function mixes the columns of the state matrix
static void MixColumns(state_t & state)
{
    uint8_t i;
    uint8_t Tmp, Tm, t;
    for (i = 0; i < 4; ++i)
    {  
        t   = state[i][0];
        Tmp = state[i][0] ^ state[i][1] ^ state[i][2] ^ state[i][3] ;
        Tm  = state[i][0] ^ state[i][1] ; Tm = xtime(Tm);  state[i][0] ^= Tm ^ Tmp ;
        Tm  = state[i][1] ^ state[i][2] ; Tm = xtime(Tm);  state[i][1] ^= Tm ^ Tmp ;
        Tm  = state[i][2] ^ state[i][3] ; Tm = xtime(Tm);  state[i][2] ^= Tm ^ Tmp ;
        Tm  = state[i][3] ^ t ;              Tm = xtime(Tm);  state[i][3] ^= Tm ^ Tmp ;
    }
}
#else

void gmix_column(unsigned char *r) {
    unsigned char a[4];
    unsigned char b[4];
    unsigned char c;
    unsigned char h;
    /* The array 'a' is simply a copy of the input array 'r'
     * The array 'b' is each element of the array 'a' multiplied by 2
     * in Rijndael's Galois field
     * a[n] ^ b[n] is element n multiplied by 3 in Rijndael's Galois field */ 
    for (c = 0; c < 4; c++) {
        a[c] = r[c];
        /* h is 0xff if the high bit of r[c] is set, 0 otherwise */
        h = (unsigned char)((signed char)r[c] >> 7); /* arithmetic right shift, thus shifting in either zeros or ones */
        b[c] = r[c] << 1; /* implicitly removes high bit because b[c] is an 8-bit char, so we xor by 0x1b and not 0x11b in the next line */
        b[c] ^= 0x1B & h; /* Rijndael's Galois field */
    }
    r[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]; /* 2 * a0 + a3 + a2 + 3 * a1 */
    r[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]; /* 2 * a1 + a0 + a3 + 3 * a2 */
    r[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; /* 2 * a2 + a1 + a0 + 3 * a3 */
    r[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; /* 2 * a3 + a2 + a1 + 3 * a0 */
}

static void MixColumns(state_t & state)
{
    uint8_t tmp[4];
    
    for (int i = 0; i < 4; ++i)
    {  
        tmp[0] = state[0][i];
        tmp[1] = state[1][i];
        tmp[2] = state[2][i];
        tmp[3] = state[3][i];
        gmix_column(&tmp[0]);
        state[0][i] = tmp[0];
        state[1][i] = tmp[1];
        state[2][i] = tmp[2];
        state[3][i] = tmp[3];
    }

    printf("after %s\n", __func__);
    for (int i = 0; i < 4; ++i) {
        printf("0x%x\n", *(uint32_t*)state[i]);
	}

}
#endif
void Cipher(uint8_t* in, uint8_t* out, uint32_t* round_key) {

    state_t state;

    for (int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            state[i][j] = in[i + j*4];
        }
    }
    cout << " ===== round 0 ===== " << endl;

    for (int i = 0; i < 4; ++i) {
        printf("0x%x\n", *(uint32_t*)state[i]);
	}

    addRoundKey(state, round_key);

    for (int i = 1; i < 10; ++i) {
        printf(" ====== Round %d =======\n", i);

        byteSub(state);

        shiftRows(state);

        MixColumns(state);

        addRoundKey(state, round_key + i*4);
    }
    
    cout << "===== round 10 =====" << endl;

    byteSub(state);

    shiftRows(state);

    addRoundKey(state, (round_key + 40));

    for (int j = 0; j < 4; ++j) {
        for(int i = 0; i < 4; ++i) {
            out[j*4 + i] = state[i][j];
        }
    }

    return;
}

int main(void) {
    uint32_t init_key[4] = {0x2b7e1516, 0x28aed2a6, 0xabf71588, 0x09cf4f3c};
    uint8_t in[16] = {0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34};
    uint8_t out[16];
    uint32_t round_keys[11][4];

    // init
    for (int i = 0; i < 11; ++i) {
	for (int j = 0; j < 4; ++j)
	    round_keys[i][j] = 0;
    }

    // key Expansion
    KeyExpansion(&init_key[0], (uint32_t*)round_keys);

    for (int i = 0; i < 11; ++i) {
         cout << "key:" << i << "--------" << endl;
	 for (int j = 0; j < 4; ++j)
	    cout << hex
            << ((round_keys[i][j] >> 24) & 0xff)
            << ((round_keys[i][j] >> 16) & 0xff)
            << ((round_keys[i][j] >> 8) & 0xff)
            << (round_keys[i][j] & 0xff) << endl;
	}
	
    // AES Cipher
    Cipher(in, out, (uint32_t*)round_keys);

    cout << " ==== output ==== " << endl;
    for (int i = 0; i < 16; ++i) {
	printf("0x%x\n", out[i]);
    }

    return 0;
}
