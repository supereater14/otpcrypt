/* OTPcrypt - One-time pad based encryption
 * By Alec Hitchiner
 */

#include "otp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
	char *key_name;
	char *crypt_name;
	char *output_name;

	if(argc < 2 || argc > 3){
		printf(	"Usage:\n"
			"Encryption: %s [input file]\n"
			"Decryption: %s [crypt file] [key file]\n",
			argv[0], argv[0]);
		return -1;
	}

	if(argc == 2){
		key_name = strdup(argv[1]);
		crypt_name = realloc(key_name, strlen(key_name) + 5);
		strcat(key_name, ".key");

		crypt_name = strdup(argv[1]);
		crypt_name = realloc(crypt_name, strlen(crypt_name) + 5);
		strcat(crypt_name, ".otp");

		if(otp_encrypt_file(argv[1], crypt_name, key_name, 100, 1)){
			fprintf(stderr, "Error ocurred during encryption.\n");
			return -1;
		}

		free(key_name);
		free(crypt_name);
	}
	else{
		output_name = strdup(argv[1]);
		output_name[strlen(output_name) - 4] = '\0';

		if(otp_decrypt_file(argv[1], argv[2], output_name, 100)){
			fprintf(stderr, "Error ocurred during decryption.\n");
			return -1;
		}
	}
		
	return 0;
}
