// -------------------------------------------------------------------------
// This software is furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// speck.c 
//    This file holds speck128x128 encryption/decryption functions
//
// Author: Vedvyas Shanbhogue
// ------------------------------------------------------------------------*/

#ifdef IDENT_C
static const char* const <filename>_c_Id = "$Id$";
#endif

#include "sp.h"

#define ROR(x, r) ( ((x) >> (r)) | ((x) << (64 - (r))) )
#define ROL(x, r) ( ((x) << (r)) | ((x) >> (64 - (r))) )

// -- global -----------------------------------------------------------------
// speck_key_expand_128x128()
//     Does key schedule expansion - see https://eprint.iacr.org/2013/404.pdf
//
// Returns:
//     None
//     
// 
// -------------------------------------------------------------------------*/
void speck_key_expand_128x128(
    uint64_t *key)
{
    int i;
    uint64_t l[32] = { 0 };

    l[0] = key[0];
    key[0] = key[1];
    key[1] = 0;

    for ( i = 0; i < 31; i++ ) {
        l[i + 1] = (key[i] + ROR(l[i], 8)) ^ i;
        key[i + 1] = ROL(key[i], 3) ^ l[i+1];
    }
    return;
}
// -- global -----------------------------------------------------------------
// encrypt_speck_128x128()
//     Encrypt 128 bits of plain text to cipher text with specified key
//     https://eprint.iacr.org/2013/404.pdf
// Returns:
//     None
//     
// 
// -------------------------------------------------------------------------*/
void encrypt_speck_128x128(
    uint64_t *plain_text, uint64_t *cipher_text, uint64_t *key)
{
    int i;
    cipher_text[0] = plain_text[0]; 
    cipher_text[1] = plain_text[1];
    for ( i = 0; i < 32; i++ ) {
        cipher_text[0] = (ROR(cipher_text[0], 8) + cipher_text[1]) ^ key[i];
        cipher_text[1] = (ROL(cipher_text[1], 3) ^ cipher_text[0]);
    }
}
// -- global -----------------------------------------------------------------
// decrypt_speck_128x128()
//     Decrypt 128 bits of cipher text to plain text with specified key
//     https://eprint.iacr.org/2013/404.pdf
// Returns:
//     None
//     
// 
// -------------------------------------------------------------------------*/
void decrypt_speck_128x128(
    uint64_t *cipher_text, uint64_t *plain_text, uint64_t *key)
{
    int i;
    plain_text[0] = cipher_text[0]; 
    plain_text[1] = cipher_text[1];
    for ( i = 0; i < 32; i++ ) {
        plain_text[1] = ROR((plain_text[0] ^ plain_text[1]), 3);
        plain_text[0] = ROL((plain_text[0] ^ key[31 - i]) - plain_text[1], 8);
    }
    return;
}
/* -- uncomment to test standalone 
int main()
{
    uint64_t plain_text[2] = { 0x6c61766975716520, 0x7469206564616d20 };
    uint64_t cipher_text[2] = { 0, 0 };
    uint64_t key[32] = { 0 };
    key[0] = 0x0f0e0d0c0b0a0908;
    key[1] = 0x0706050403020100;

    speck_key_expand_128x128(key);
    encrypt_speck_128x128(plain_text, cipher_text, key);
    printf("plain : %08llx %08llx\ncipher: %08llx %08llx\n", 
            plain_text[0], plain_text[1], cipher_text[0], cipher_text[1]);
    if ( cipher_text[0] != 0xa65d985179783265 && cipher_text[1] != 0x7860fedf5c570d18 ) {
        printf("Error encrypting test vectors\n");
    }
    plain_text[0] = 0;
    plain_text[1] = 0;
    decrypt_speck_128x128(cipher_text, plain_text, key);
    printf("plain : %08llx %08llx\ncipher: %08llx %08llx\n", 
            plain_text[0], plain_text[1], cipher_text[0], cipher_text[1]);
    if ( plain_text[0] != 0x6c61766975716520 && plain_text[1] != 0x7469206564616d20 ) {
        printf("Error decrypting test vectors\n");
    }
}
*/
