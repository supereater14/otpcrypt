/* One-Time Pad Encryption Library
 * By Alec Hitchiner
 */

#include "otp.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

size_t otp_gen_key_segment(char *buf, size_t len, int allow_pseudo){
	size_t len_generated;
	int rand_handle;

	/* Open random source */
	if(allow_pseudo){
		rand_handle = open("/dev/urandom", O_RDONLY);
	}
	else{
		rand_handle = open("/dev/random", O_RDONLY);
	}

	if(rand_handle < 0){
		return 0;
	}

	/* Read random data */
	len_generated = read(rand_handle, buf, len);

	/* Clean up open files */
	close(rand_handle);

	return len_generated;
}

int otp_crypt(char *data_buf, size_t data_len, char *key_buf, size_t key_len){
	size_t x;

	/* Check if key is valid */	
	if(data_len > key_len){
		return -1;
	}

	/* Perform encryption (XOR all data) */
	for(x = 0; x < data_len; x++){
		data_buf[x] = data_buf[x] ^ key_buf[x];
	}

	return 0;
}

int otp_encrypt_file(char *filename, char *cypher_file, char *key_file,
			size_t block_size, int allow_pseudo){
	int file_handle;
	int crypt_handle;
	int key_handle;
	int success;
	size_t len_read;
	size_t len_written;
	size_t key_generated;
	char *buf;
	char *key_buf;

	/* Open input file */
	file_handle = open(filename, O_RDONLY);
	if(file_handle < 0){
		return -1;
	}

	/* Open encrypted output file */
	crypt_handle = open(cypher_file, O_WRONLY | O_CREAT, 0644);
	if(crypt_handle < 0){
		close(file_handle);
		return -1;
	}

	/* Open key file */
	key_handle = open(key_file, O_WRONLY | O_CREAT, 0644);
	if(key_handle < 0){
		close(file_handle);
		close(crypt_handle);
		return -1;
	}

	/* Allocate buffers for data and key */
	buf = malloc(block_size);
	key_buf = malloc(block_size);

	/* Begin encrypting */
	do{
		/* Read data from input file */
		len_read = read(file_handle, buf, block_size);

		/* Generate enough key to cover the data read */
		key_generated = otp_gen_key_segment(key_buf, len_read,
							allow_pseudo);
		if(key_generated < len_read){
			close(file_handle);
			close(crypt_handle);
			close(key_handle);
			return -1;
		}

		/* Encrypt the data segment */
		success = otp_crypt(buf, len_read, key_buf, len_read);
		if(success){
			close(file_handle);
			close(crypt_handle);
			close(key_handle);
			return -1;
		}

		/* Write encrypted data to output file */	
		len_written = write(crypt_handle, buf, len_read);
		if(len_written < len_read){
			close(file_handle);
			close(crypt_handle);
			close(key_handle);
			return -1;
		}

		/* Write key segment to key file */
		len_written = write(key_handle, key_buf, len_read);
		if(len_written < len_read){
			close(file_handle);
			close(crypt_handle);
			close(key_handle);
			return -1;
		}
	}while(len_read == block_size);

	/* Close open files */
	close(file_handle);
	close(crypt_handle);
	close(key_handle);

	/* Free buffers */
	free(buf);
	free(key_buf);

	return 0;
}

int otp_decrypt_file(char *crypt_file, char *key_file, char *output_file,
			size_t block_size){
	int crypt_handle;
	int key_handle;
	int output_handle;
	char *buf;
	char *key_buf;
	size_t len_read;
	size_t key_read;
	size_t len_written;

	/* Open encrypted file */
	crypt_handle = open(crypt_file, O_RDONLY);
	if(crypt_handle < 0){
		return -1;
	}

	/* Open key file */
	key_handle = open(key_file, O_RDONLY);
	if(key_handle < 0){
		close(crypt_handle);
		return -1;
	}

	/* Open output file */
	output_handle = open(output_file, O_WRONLY | O_CREAT, 0644);
	if(output_handle < 0){
		close(crypt_handle);
		close(key_handle);
		return -1;
	}

	/* Allocate memory for data and key */
	buf = malloc(block_size);
	key_buf = malloc(block_size);

	/* Begin decrypting */
	do{
		/* Read data segment from encrypted file */
		len_read = read(crypt_handle, buf, block_size);

		/* Read key from keyfile */
		key_read = read(key_handle, key_buf, len_read);
		if(key_read != len_read){
			close(crypt_handle);
			close(key_handle);
			close(output_handle);
			return -1;
		}

		/* Decrypt the data segment using the key segment */
		otp_crypt(buf, len_read, key_buf, key_read);

		/* Write decrypted data to output file */
		len_written = write(output_handle, buf, len_read);
		if(len_written != len_read){
			close(crypt_handle);
			close(key_handle);
			close(output_handle);
			return -1;
		}
	}while(len_read == block_size);

	/* Clean up open files */
	close(crypt_handle);
	close(key_handle);
	close(output_handle);

	/* Deallocate buffers */
	free(buf);
	free(key_buf);

	return 0;
}
