/*
 * tracer.cxx
 * 
 * Copyright 2018 Darius Musiolik <MrFlyingToasterman>
 * 
 * https://github.com/MrFlyingToasterman/tracer
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <chrono>
#include <thread>

using namespace std;

//Global Varz
string version = "1.0";
int startNet;
int endNet;

void printLog(string msg) {
	cout << "\n[INFO] " << msg;
}

void printWarn(string msg) {
	cout << "\n[WARN] " << msg;
}

void printError(string msg) {
	cout << "\n[ERRO] " << msg;
}

void printOK(string msg) {
	cout << "\n[ OK ] " << msg;
}

void printOnline(string msg) {
	cout << "\n[ONLINE] " << msg;
}

void printOffline(string msg) {
	cout << "\n[OFFLINE] " << msg;
}

int checkroot() {
	if(geteuid() != 0) {
	  //Exit
	  return 1;
	}
	//Root access
	return 0;
}

string readFromConfig(string pattern) {
	string line;
	string returnme = "";
	
	ifstream configfile ("/etc/tracer.conf");
	if (configfile.is_open()) {
		while ( getline (configfile, line) ) {
			//cout << line << '\n';
			if (line.substr(0, pattern.length()) == pattern) {
				//pattern found
				//cout << "\nREAD" << line << endl;
				char value[pattern.length()];
				strcpy(value, line.c_str());
				//return only value of pattern
				for(int i = pattern.length(); i < line.length(); i++) {
					if (value[i] == ';') {
						break;
					}
					returnme += value[i];
					//Verbose
					//cout << "\n[READ] " << value[i] << endl;
				}
				return returnme;
			}
		}
    configfile.close();
	} else {
		printError("Unable to read from config!"); 	
		return "Unable to read from config!";
	}
}

void initConfig() {
	std::ofstream outfile ("/etc/tracer.conf");
	outfile << "# This is the default traver V " << version << " config file" << std::endl;
	outfile << "startNet:0;" << std::endl;
	outfile << "endNet:0;" << std::endl;
	outfile.close();
}

inline bool checkFileExist (const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}

void scanNet() {
	bool yr = true;
	do {
	int e = 0;
		printLog("Starting trace");
	
		//Main Loop for scanning the Net
		for(int oct3 = startNet; oct3 < endNet+1; oct3++) { 
			for(int oct4 = 1; oct4 < 255; oct4++) { 
				printLog("scan: 192.168." + to_string(oct3) + "." + to_string(oct4));
				string scanAddr = "192.168." + to_string(oct3) + "." + to_string(oct4);
				if ( system((std::string("ping -c 1 " + scanAddr).c_str())) == 0) { 
					printOnline(scanAddr + " is reachable!");
				} else {
					printOffline(scanAddr + " is NOT reachable!\n");
				}
			}
		}
		e++;
		printLog("trace complete");
		//std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}while(yr);
}

int main(int argc, char **argv) {
	cout << "Welcome to tracer version " << version << endl;
	printLog("Checking root");
	if (checkroot() == 0) {
		printOK("root access");
		//If config does not exist, create it
		printLog("Checking config files");
		if (checkFileExist("/etc/tracer.conf") == 1) {
			//Config file found
			printLog("Config found");
		} else {
			//Config NOT found
			printWarn("Config NOT found!");
			printLog("Creating new one");
			initConfig();
		}
		//Config accessable
		startNet = atoi(readFromConfig("startNet:").c_str());
		endNet = atoi(readFromConfig("endNet:").c_str());
		scanNet();
		
		return 0;
	} else {
		printWarn("No root access!");
		printError("Please run as root!");
		return 1;
	}
}

