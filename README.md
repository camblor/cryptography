# cryptography
Cryptography foundations - Universidad Autónoma de Madrid, 2020-2021

##p1
In this part you can find Affine cipher, Euclid algorithm, Permutation cipher, Vigenere algorithm and RC4 encryption algorithm.

Affine Cipher: This is a type of monoalphabetic substitution cipher, where each letter in an alphabet is mapped to its numeric equivalent, encrypted using a simple mathematical function, and converted back to a letter. The formula used in this encryption is (ax + b) mod m, where a and b are the keys, x is the numerical equivalent of a letter in the alphabet, and m is the size of the alphabet. The keys a and m must be coprime (i.e., their greatest common divisor must be 1) to ensure each letter maps to a unique other letter. The decryption function is the modular multiplicative inverse of the encryption function.

Euclidean Algorithm: This is an efficient method for computing the greatest common divisor (GCD) of two integers, which is the largest number that divides them without leaving a remainder. The algorithm is based on the principle that the GCD of two numbers also divides their difference. So, to find the GCD of a and b, we replace a with b and b with a mod b and repeat the process until b is zero. The Euclidean algorithm is important in number theory and is used in various cryptographic algorithms.

Permutation Cipher: This is a type of cipher that rearranges the letters of the plaintext to create the ciphertext, without changing the actual letters used. The rearrangement is determined by a permutation key, which can be any sequence of numbers, provided that it includes all the positions of the plaintext. For example, the key "4 3 1 2" applied to the plaintext "help" would result in the ciphertext "elhp".

Vigenère Cipher: This is a method of encrypting alphabetic text by using a series of different Caesar ciphers based on the letters of a keyword. It is a form of polyalphabetic substitution. The first letter of the plaintext is encrypted by a Caesar cipher determined by the first letter of the keyword, the second letter of the plaintext by a Caesar cipher determined by the second letter of the keyword, and so on. When the end of the keyword is reached, the process repeats from the start of the keyword.

RC4 Encryption Algorithm: RC4 (Rivest Cipher 4) is a stream cipher, so it encrypts plaintext by mixing it with a series of random bytes, making it very difficult for anyone without the correct key to decode the original message. Despite its simplicity and speed in software, multiple vulnerabilities have been discovered in RC4, making it insecure for most uses. It is notably used in protocols such as Secure Socket Layer (SSL) and Wi-Fi Protected Access (WPA), although it has been deprecated in many of these applications due to security concerns.

##p2
In this section you can find DES, AES and Triple DES.

DES (Data Encryption Standard): DES is a symmetric-key algorithm for the encryption of digital data. It was developed in the early 1970s at IBM and adopted by the U.S. government as an official Federal Information Processing Standard in 1977 for encrypting non-classified information. DES is based on a block cipher that encrypts data in 64-bit blocks, using a 56-bit key. Despite being widely used for many years, DES is now considered insecure for many applications because its key size is too small by today's standards, which makes it vulnerable to brute-force attacks.

Triple DES (3DES): To increase the security of DES, the Triple DES algorithm was introduced. It applies the DES algorithm three times to each data block. The most commonly used version of 3DES uses two 56-bit keys. The data is first encrypted with one key, then decrypted with the second key, and finally encrypted again with the first key. While 3DES is significantly more secure than DES, it is also slower and is now being phased out in favor of AES.

AES (Advanced Encryption Standard): AES is a symmetric encryption standard adopted by the U.S. government in 2001, replacing DES. It was the result of a worldwide call for submissions of encryption algorithms issued by the US National Institute of Standards and Technology (NIST) in 1997. The winning algorithm was Rijndael, developed by two Belgian cryptographers, Vincent Rijmen and Joan Daemen. Unlike DES, AES has a variable block size and key size – it can be chosen as 128, 192, or 256 bits. AES operates on a 4x4 array of bytes, known as a state, and has a series of rounds for which the number depends on the key size. AES is widely regarded as the most secure symmetric encryption standard and is used globally for protecting sensitive data.

##p3

In this section you can find an implementation of a las vegas algorithm and some prime numbers. This is implemented using https://gmplib.org/

A Las Vegas algorithm is a type of randomized algorithm that always gives correct results; that is, it always produces the correct output or it informs about the failure. However, the time it takes to produce the result is not fixed and varies randomly.

A key difference between Monte Carlo algorithms and their counterpart, Las Vegas algorithms, is that while Monte Carlo algorithms always terminate in a set amount of time but have a probability of being incorrect, Las Vegas algorithms always produce the correct result, but their running time is random.
