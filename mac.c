//#


#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#include "md5.h" 

unsigned int getHash(char* pass, int length)
{
	MD5_CTX mdContext;  // needed to compute MD5
	unsigned int* temp;

	MD5Init(&mdContext);  // compute MD5 of password
	MD5Update(&mdContext, pass, length);
	MD5Final(&mdContext);
	temp = (int*)& mdContext.digest[12];

	return *temp;
}

// encrypts a file and stores the result 
int encrypt(unsigned char input[], unsigned char output[], int length, int key)
{
	struct stat st;
	int size;
	int* temp, result;
	int rollingkey;
	int buf, n;
	MD5_CTX mdContext;  // needed to compute MD5
	// priliminaries, get files ready and sized

	// do the encryption, buf contains plaintext, and rollingkey contains key
	buf = 0;
	rollingkey = key;
	for (int i = 0; i < length; i += 4)
	{
		buf = buf ^ rollingkey; //XOR with key, and put ciphertext in buf
		MD5Init(&mdContext);  // compute MD5 of rollingkey
		MD5Update(&mdContext, &rollingkey, 4);
		MD5Final(&mdContext);
		temp = (int*)& mdContext.digest[12];
		result = *temp; // result is 32 bits of MD5 of buf

		rollingkey = rollingkey ^ result; // new key
		//write(outfile, &buf, 4);  // write ciphertext
		for (int j = 0; j < 4; ++j)
		{
			output[i + j] = (buf & 255);
			buf = buf >> 8;
		}
		buf = 0; // rinse and repeat
	};

	return(size);
};

int readInput(char name[], unsigned char buffer[1024])
{
	FILE* fptr;
	fptr = fopen(name, "rb");
	if (fptr == NULL)
	{
		printf("Problems opening the rainbow-hashing file!");
	}

	return fread(buffer, 1024, 1, fptr);

	//return read(infile, &buffer, 12);
}

int main(int argc, char* argv[])
{
	if (argc != 3) { printf("Usage: %s file key\n", argv[0]); exit(0); };

	unsigned char input[1024];
	unsigned char extendedOutput[1032];
	unsigned char* output = extendedOutput + 8;
	int bytesRead;
	int infile, outfile;
	int key = strtoul(argv[2], NULL, 16);

	//itoa(key, extendedOutput, 10);
	snprintf(extendedOutput, 9, "%x.8", key);

	printf("Output: %s\n", extendedOutput);

	bytesRead = readInput(argv[1], input);
	
	encrypt(input, output, bytesRead, key);

	/*
	for (int i = 0; i < 1024; ++i) 
	{
		printf("Output: %c\n", output[i]);
	}
	*/

	unsigned int hash = getHash(extendedOutput, bytesRead);

	printf("Hash: %i\n", hash);
	printf("Key: %i\n", key);
	printf("Extended Output: %s\n", extendedOutput);
}