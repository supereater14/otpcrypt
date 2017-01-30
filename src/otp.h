/* One-Time Pad Encryption Library
 * By Alec Hitchiner
 */

#ifndef OTP_H
#define OTP_H

#include <stdlib.h>

/* Generates a new OTP key segment
 *
 * buf - Buffer to store key segment into, must be at least len bytes long
 * len - Size of key to generate
 * allow_pseudo -	Allow pseudorandom number generation
 * 			lower security, but faster
 * 			PRNG will be cryptographically secure
 * returns - Size of key successfully generated
 */
size_t otp_gen_key_segment(char *buf, size_t len, int allow_pseudo);

/* Performs OTP encryption on given data
 *
 * data_buf - data to be encrypted/decrypted (this will be modified)
 * data_len - the size of data_buf
 * key_buf - The key to be used for encryption/decryption
 * key_len - the length of the key
 * returns - 0 on success, -1 if key invalid
 */
int otp_crypt(char *data_buf, size_t data_len, char *key_buf, size_t key_len);

/* Performs OTP encryption on a file
 *
 * filename - file to encrypt
 * cypher_file - encrypted output file
 * key_file - key output file
 * block_size - size of encryption blocks
 * allow_pseudo - allows pseudorandom number generation when generating key
 * returns - 0 on success, -1 on failure
 */
int otp_encrypt_file(char *filename, char *cypher_file, char *key_file,
			size_t block_size, int allow_pseudo);

/* Performs OTP decryption on a file
 *
 * crypt_file - encrypted data file
 * key_file - key file
 * output_file - decrypted output file
 * block_size - size of decryption blocks
 * returns - 0 on success, 1 on failure
 */
int otp_decrypt_file(char *crypt_file, char *key_file, char *output_file,
			size_t block_size);

#endif
