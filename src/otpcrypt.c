/* OTPcrypt - One-time pad based encryption
 * By Alec Hitchiner
 */

#include "otp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OTP_BLOCK_SIZE 4096

int main(int argc, char **argv){
	char *key_name;
	char *crypt_name;
	char *output_name;

	/* Check Args */
	if(argc < 3){
		printf(	"Usage:\n"
			"Encryption (auto-generate key): %s e [input file]\n"
			"Encryption (provided key):      %s e [input file] "
			"[key file]\n"
			"Decryption:                     %s d [crypt file] "
			"[key file]\n",
			argv[0], argv[0], argv[0]);
		return -1;
	}

	/* Encryption routine */
	if(!strcmp(argv[1], "e")){
		if(argc == 3){ 
			/* Generate filenames for output files */
			key_name = strdup(argv[2]);
			crypt_name = realloc(key_name, strlen(key_name) + 5);
			strcat(key_name, ".key");
	
			crypt_name = strdup(argv[2]);
			crypt_name = realloc(crypt_name, strlen(crypt_name) + 5);
			strcat(crypt_name, ".otp");
	
			/* Perform encryption */
			if(otp_encrypt_file(argv[2], crypt_name,
						key_name, OTP_BLOCK_SIZE, 1)){
				fprintf(stderr, "Error ocurred during encryption.\n");
				return -1;
			}
	
			/* Free name buffers */
			free(key_name);
			free(crypt_name);
		}
		else if(argc == 4){
			/* Generate name for output file */
			crypt_name = strdup(argv[2]);
			crypt_name = realloc(crypt_name, strlen(crypt_name) + 5);
			strcat(crypt_name, ".otp");

			/* Perform encryption */
			if(otp_decrypt_file(argv[2], argv[3],
						crypt_name, OTP_BLOCK_SIZE)){
				fprintf(stderr, "Error ocurred during encryption.\n");
				return -1;
			}

			/* Free name buffer */
			free(crypt_name);
		}
		else{
			fprintf(stderr, "Error: Invalid args for encryption\n");
			return -1;
		}
	}
	/* Decryption routine */
	else if(!strcmp(argv[1], "d")){
		if(argc == 4){
			/* Generate output name */
			output_name = strdup(argv[2]);
			output_name[strlen(output_name) - 4] = '\0';
	
			/* Perform decryption */
			if(otp_decrypt_file(argv[2], argv[3], output_name, 100)){
				fprintf(stderr, "Error ocurred during decryption.\n");
				return -1;
			}
	
			/* Free name buffer */
			free(output_name);
		}
		else{
			fprintf(stderr, "Error: invalid args for decryption.\n");
		}
	}
	else{
		fprintf(stderr, "Error: no valid operation given.\n");
		return -1;
	}
		
	return 0;
}
