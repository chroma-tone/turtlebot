#!/usr/bin/python

from Crypto.Cipher import XOR
import base64

key = "turtlebot_3129_potatoes"

# decrypt password in wifi module something like
# https://github.com/Urban82/Aes256s
#const char * key = "turtlebot_3129_potatoes";
#ByteArray key;
#Aes256 aes(key);
#ByteArray::size_type dec_len = Aes256::decrypt(buffer);
def encrypt(plaintext):
  cipher = XOR.new(key)
  return base64.b64encode(cipher.encrypt(plaintext))

def main():
    # parse command line options
    username = raw_input("Enter wifi ssid for secure storage : ")
    password = raw_input("Enter wifi password for secure storage : ")

    combined = username + ":" + password
    encryptedStuff = encrypt(combined)
    fileConnection = open("password.txt",'w')
    fileConnection.write(encryptedStuff)
    fileConnection.close()

if __name__ == "__main__":
    main()
