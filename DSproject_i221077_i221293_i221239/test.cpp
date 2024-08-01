#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <commdlg.h>
#include <iostream>
#include <string>
#include <iostream>
#include "fileHandler.h"
#include "DHT_RING.h"

using namespace std;


//TEST(hashing, sha1xHex_to_Binary) {
//
//    std::cout << "sha1x(''):" << sha1x("") << std::endl;
//   
//    string ans = "1010100110011001001111100011011001000111000001101000000101101010101110100011111000100101011100010111100001010000110000100110110010011100110100001101100010011101x";
//    hashing Hash(2);
//    cout << "\nBINARY CONVERSION:" << endl;
//    cout << Hash.convert_to_binary("a9993e364706816aba3e25717850c26c9cd0d89d") << endl;
//    cout << "\nActuall Binary:" << endl;
//    cout << ans << endl <<  endl;
//   
//    EXPECT_EQ(Hash.convert_to_binary("a9993e364706816aba3e25717850c26c9cd0d89d"), ans);
//
//    BigInteger dec = Hash.convert_to_decimal(Hash.convert_to_binary("a9993e364706816aba3e25717850c26c9cd0d89d"));
//
//    string dec_ans = "968236873715988614170569073515315707566766479517";
//
//    cout << "\nDECIMAl CONVERSION:" << endl;
//    cout << dec.value << endl;
//    cout << "\nACTUAL DecimalValue:" << endl;
//    cout << dec_ans << endl << endl;  
//
//    EXPECT_EQ(dec.value, dec_ans);
//
//    cout << "\nMOD w. 31 for 5 bit system" << endl;
//
//    int bitcount = 5;
//    hashing  SystemHash(bitcount); 
//    BigInteger HASH_KEY = SystemHash.generate_key(sha1x(""));
//    cout << "\nOUR HASH KEY (ADT- BigInteger): " <<  HASH_KEY.value << endl;
//
//    EXPECT_EQ( HASH_KEY.value , "27" );
//
//}



TEST(TestCaseName, TestName) {
	int bits, order;
	cout << "Enter size for DHT ring in bits :";
	cin >> bits;
	cout << "Enter order of B-tree :";
	cin >> order;
std:string display_in_log = "Inter Planetary File System, compiled and runned succesfully.\n\n   Group Members:\n   >1.Abdullah Zubair (22i-1077)  >2.Tauha Imran (22i-1293) >3.Saffi Muhammad Hashir (22i-1293)\n\n   >>>Bits value: " + to_string(bits) + "\n   >>>B-tree order: " + to_string(bits) + "\n\n";
	writeToFile(display_in_log, filename_log_IPFS, false);
	writeToFile(display_in_log, filename_log_ROUTER, false);

	DHT_ring dht(bits);
	bool running = true;
	while (running)
	{
		system("cls");

		cout << "\nDHT RING FUNCTIONALITY MENU" << endl;
		cout << "1.  Add Machine\n";
		cout << "2.  Print DHT Ring (Ascending)\n";
		cout << "3.  Print DHT Ring (Descending)\n";
		cout << "4.  Print Finger Tables\n";
		cout << "5.  Add File to Machine\n";
		cout << "6.  Automatic Machine Generation\n";
		cout << "7.  Delete Machine and automatically transfer files\n";
		cout << "8.  Delete File from Specific Machine\n";
		cout << "9.  Print B-tree of any Machine\n";
		cout << "10. Exit\n";

		int choice;
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice)
		{
		case 1:
		{
			cout << "Enter Machine ID : ";
			string machineId;
			cin >> machineId;
			dht.addmachine(machineId);

			system("pause");

			break;
		}
		case 2:
			dht.print_fwd();

			system("pause");

			break;
		case 3:
			dht.print_rev();

			system("pause");

			break;
		case 4:
			dht.print_fingertable();

			system("pause");

			break;
		case 5:
		{// case to add file...
			cout << "Enter Machine ID to add file from: ";
			string machineId;
			cin >> machineId;


			string filepath = searchfile();

			//if dialouge box closed....
			if (filepath == "File selection canceled") {
				break;
			}

			dht.inputfile(machineId, filepath, true);

			system("pause");
			break;
		}
		case 6:
		{
			cout << "Enter the number of machines to generate automatically: ";
			int numMachines;
			cin >> numMachines;
			dht.automatic_activation(numMachines);
			system("pause");
			break;
		}
		case 7:
		{

			cout << "Enter the machine id to turn off : ";
			string machineId;
			cin >> machineId;
			dht.removemachine(machineId);
			system("pause");
			system("cls");
			break;
		}
		case 8:
		{
			cout << "Enter the machine id to turn off : ";
			string machineId;
			cin >> machineId;

			cout << "Enter file's id: ";
			string filepath;
			cin >> filepath;
			dht.inputfile(machineId, filepath, false);
			system("pause");
			system("cls");
			break;
		}
		case 9: {
			cout << "Enter the machine id to view B-tree : ";
			string machineId;
			cin >> machineId;
			dht.display_node_Btree(machineId);
			cout << "\n\n\n";
			system("pause");
			break;
		}
		case 10:
			system("cls");
			cout << "\n \t you have exited the IPFS system manager ... \n\n";
			system("pause");
			running = false;
			break;
		default:
			cout << "Invalid choice. Please enter a valid option.\n";
			system("pause");
			break;
		}
	}

	EXPECT_TRUE(true);
}


//---------------------------------------------------

//TEST(fileHandling, readfrompath) {
//
//
//    //gets content from the file...
//    //std::string cntxt = read_file_from_path("C:/Users/Admin/Desktop/Semester Project DES.pdf"); 
//    //std::string cntxt = read_file_from_path("C:/Users/Admin/Desktop/test.txt");
//    //std::string cntxt = read_file_from_path("C:/Users/Admin/Pictures/9.png");
//    std::string cntxt = read_file_from_path("F:/i221084_i221239_-_DLD_Project.mp4");
//
//    std::string sha1val = sha1x(cntxt);
//    int bitcount = 5;
//    hashing  SystemHash(bitcount); 
//    BigInteger key = SystemHash.generate_key(sha1val);
//
//    cout << "\n =>>  FILE CONTENTS :: \n " << cntxt << "\n\n" << endl;
//
//    cout << "\n =>> SHA1 - ecnryption :: \n " << sha1val << "\n\n" << endl;
//
//    cout << "\n =>>  HASH KEY of file :: \n " << key.value << "\n\n" << endl;
//
//
//   EXPECT_EQ(1, 1);
//
//}


//---------------------------------------------------