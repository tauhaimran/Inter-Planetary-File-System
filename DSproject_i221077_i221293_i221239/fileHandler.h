#pragma once
// these will be only functions | made by Tauha Imran 22i1239
#include<iostream>
#include<string>
#include <fstream>
#include <chrono>
#include <ctime>
#include <windows.h>
#include <locale>
#include <codecvt>

std::string searchfile() {
	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH] = { 0 };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile) / sizeof(*szFile);
	ofn.lpstrFilter = L"All Files\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrTitle = L"Select a File";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	GetCurrentDirectory(MAX_PATH, ofn.lpstrFile);

	if (GetOpenFileName(&ofn) == TRUE) {

		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(ofn.lpstrFile);
	}
	else {

		return "File selection canceled";
	}
}

using namespace std;
string filename_log_IPFS = "IPFSlog.txt";
string filename_log_ROUTER = "IPFSRouterlog.txt";
//reads file from a path and returns content.
std::string read_file_from_path(std::string filepath = "?") {

	//if string passed was incorrect...
	if (filepath == "?") { return "?"; }


	std::fstream file; // makes reading object...
	file.open(filepath, std::ios::in); //opens file in reading mode...

	if (!file.is_open()) { // if file error abort reading...
		std::cout << "\n ::: FILE CAN'T BE OPENED ::: return \"?\" ; \n";
		file.close(); //closes file...
		return "?"; //returns exception value...
	}
	else {
		std::string content_str = ""; // making buffer string...
		while (!file.eof()) {// eof() = eof -> boolean for if end of file reached...
			std::string line = "";
			getline(file, line); //put file content in string content_str...
			content_str += line;
		}
		file.close();//closes file...
		//returns the content...
		return content_str;
	}



}
void writeToFile(const std::string& content, const std::string& filename, bool append = true, bool time = true)
{
	ios_base::openmode mode = append ? std::ios::app : std::ios::out;
	ofstream file(filename, mode);
	file.is_open();
	if (time) {

		time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		file << "\n==>" << std::ctime(&now) << "   " << content;
	}
	else {

		file << "\n   " << content;
	}
	file.close();
}
