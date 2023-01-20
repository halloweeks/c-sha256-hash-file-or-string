#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <time.h>
#include <stdbool.h>

void sha256_hash_string(unsigned char hash[SHA256_DIGEST_LENGTH], char *outputBuffer) {
	for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
	}
	outputBuffer[64] = 0;
}

void sha256_string(char *string, char *outputBuffer) {
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, string, strlen(string));
	SHA256_Final(hash, &sha256);
	for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
	}
	outputBuffer[64] = 0;
}

bool sha256_file(const char *filename, char *outputBuffer) {
	int byte = 0;
	unsigned char hash[SHA256_DIGEST_LENGTH] = {0};
	unsigned char *buffer = (unsigned char*) malloc(32768);
	
	if (!buffer) return false;
	
	int file = open(filename, O_RDONLY);
	if (file == -1) {
		return false;
	}
	
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	
	while ((byte = read(file, buffer, 32768)) > 0) {
		SHA256_Update(&sha256, buffer, byte);
	}
	
	if (byte == -1) return false;
	
	SHA256_Final(hash, &sha256);
	
	sha256_hash_string(hash, outputBuffer);
	
	close(file);
	free(buffer);
	return true;
}

int main() {
	clock_t start, end; 
	start = clock();
	
	char hash[65] = {0};
	
	sha256_string("halloweeks", hash);
	
	printf("string hash: %s\n", hash);
	
	if (sha256_file("sha.c", hash) == true) {
		printf("File hash: %s\n", hash);
	}
	
	end = clock();
	double time_taken = (double)(end - start) / (double)(CLOCKS_PER_SEC);
	printf("[TIME] PROCESS COMPLETE IN %f SEC\n", time_taken);
	return 0;
}