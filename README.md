# otpcrypt
## One-time pad data encryption
### By Alec Hitchiner

### Building
```bash
make clean
make
```

### Usage
```
Encryption (auto-generate key):   otpcrypt [input file]
Encryption (provided key):        otpcrypt [input file] [key file]
Decryption:                       otpcrypt [encrypted file] [key file]
```

### Encryption
During encryption, otpcrypt generates a random key with size equal to the
plaintext. This key is generated using data from /dev/urandom. The plaintext is
then XOR'd with the generated key. The result is stored in two files. A file,
ending in ".otp", contains the plaintext XOR'd with the key. A second file,
ending in ".key" contains the generated key.

If the key is provided, the same procedure as is followed for decryption will be
performed, which is functionally identical to encryption.

### Decryption
During decryption, the encrypted file and the key are again XOR'd together. This
yields the original file, which is then stored in a file whose name is based on
the encrypted file's name.
