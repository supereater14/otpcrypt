/* OTPcrypt - One-time pad based encryption
 * By Alec Hitchiner
 */

#include "otp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OTP_BLOCK_SIZE 128

int main(int argc, char **argv){
	char *key_name;
	char *crypt_name;
	char *output_name;

	/* Check Args */
	if(argc < 2 || argc > 3){
		printf(	"Usage:\n"
			"Encryption: %s [input file]\n"
			"Decryption: %s [crypt file] [key file]\n",
			argv[0], argv[0]);
		return -1;
	}

	/* Encryption routine */
	if(argc == 2){
		/* Generate filenames for output files */
		key_name = strdup(argv[1]);
		crypt_name = realloc(key_name, strlen(key_name) + 5);
		strcat(key_name, ".key");

		crypt_name = strdup(argv[1]);
		crypt_name = realloc(crypt_name, strlen(crypt_name) + 5);
		strcat(crypt_name, ".otp");

		/* Perform encryption */
		if(otp_encrypt_file(argv[1], crypt_name,
					key_name, OTP_BLOCK_SIZE, 1)){
			fprintf(stderr, "Error ocurred during encryption.\n");
			return -1;
		}

		/* Free name buffers */
		free(key_name);
		free(crypt_name);
	}
	/* Decryption routine */
	else{
		/* Generate output name */
		output_name = strdup(argv[1]);
		output_name[strlen(output_name) - 4] = '\0';

		/* Perform decryption */
		if(otp_decrypt_file(argv[1], argv[2], output_name, 100)){
			fprintf(stderr, "Error ocurred during decryption.\n");
			return -1;
		}

		/* Free name buffer */
		free(output_name);
	}
		
	return 0;
}
