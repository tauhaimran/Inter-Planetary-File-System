#pragma once

#include <iostream>
#include <Windows.h>
#include <commdlg.h>
#include <iostream>
#include <string>
#include <fstream>

#include "BigInteger.h"
#include "sha1x.h"

class hashing {
private:

	int BITS;
	BigInteger modulous;

public:

//....................................................................

	//constructor...
	hashing(int bits = 0 ):BITS(bits),modulous(BigInteger("2")) {
		modulous = modulous.power(bits);
		modulous = modulous.subtract(modulous.value, "1");
		//modulous = modulous.power(100);
	}

//....................................................................

	//display the value we are taking the modulous with...
	void displaymodval() { std::cout << modulous.value << std::endl; }

	BigInteger generate_key(std::string sha_string) {
		
		string binary = convert_to_binary(sha_string); //converts int binary string

		int totalbits = 160;

		while ( totalbits > BITS ) { //checks if this is our answer...

			for (int i = BITS; i > 0; i--) {
				
				//cout << binary << endl;
				//cout << convert_to_decimal(binary).value << endl;
				//cout << "totalbits = " << totalbits << endl;
				
				totalbits--;
				SHR(binary);
			}
			//cout << "FINISHED A DIV\n";
		}

		return convert_to_decimal(binary);
	}

//....................................................................


	//rotate w.out carry RIGHT
	void SHR(std::string &bits) { //rightshift
		
		int i = size(bits);
		//cout << "x\n";
		while (bits[i] != 'x') { i--; }

		i--;

		while (i > 0) { bits[i] = bits[i - 1]; i--; }

		bits[0] = '0';

	}

	//convert's sha's hex string into binary...
	std::string convert_to_binary(std::string hex_str) {
		std::string binary_str = "";

		int pos = 0;

		while (hex_str[pos] != '\0') {//while the sha_str isn't till it's end

			char val = hex_str[pos];

			switch (val)
			{//this is to check and add the relevent binary_sub_str w.r.t to hex_str
				case '0':
					binary_str += "0000"; //0
					break;

				case '1':
					binary_str += "0001"; //1
					break;

				case '2':
					binary_str += "0010"; //2
					break;

				case '3':
					binary_str += "0011"; //3
					break;

				case '4':
					binary_str += "0100"; //4
					break;

				case '5':
					binary_str += "0101"; //5
					break;

				case '6':
					binary_str += "0110"; //6
					break;

				case '7':
					binary_str += "0111"; //7
					break;

				case '8':
					binary_str += "1000"; //8
					break;

				case '9':
					binary_str += "1001"; //9
					break;

				case 'a':
					binary_str += "1010"; //a=10
					break;

				case 'b':
					binary_str += "1011"; //b=11
					break;

				case 'c':
					binary_str += "1100"; //c=12
					break;

				case 'd':
					binary_str += "1101"; //d=13
					break;

				case 'e':
					binary_str += "1110"; //d=14
					break;

				case 'f':
					binary_str += "1111"; //f=15
					break;

			//if by some misfortune my values are not in hex...
			default:
				break;
			}
		pos++;
		}
		binary_str += 'x';
		return binary_str;
	}
//....................................................................
	BigInteger convert_to_decimal(std::string binary_str) {
		int s = binary_str.size();
		s--;
		if (binary_str[s] == 'x') { // string was correctly proccessed
			BigInteger decimal("0");
			int power = 0; //power of 2
			s--;
			while (s >= 0) { //while reverse tranversing of string is possible...

				if (binary_str[s] == '1') {
					decimal = decimal + BigInteger("2").power(power); 
					//adds orignal + binary bit's translated value...
				}
				power++; //increasing exponent...
				s--; //decreasing address val...
			}

			//returning our Biginteger with decimal value...
			return decimal;

		}
		else {
			return BigInteger("0"); //error call return.
		}

	}

//....................................................................

}; 