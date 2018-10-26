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

using namespace std;

string version = "1.0";

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

int checkroot() {
	if(geteuid() != 0) {
	  //Exit
	  return 1;
	}
	//Root access
	return 0;
}

void readFromConfig(string pattern) {
	string line;
	
	ifstream configfile ("/etc/tracer.conf");
	if (configfile.is_open()) {
		while ( getline (configfile, line) ) {
			//cout << line << '\n';
			if (line.substr(0, pattern.length()) == pattern) {
				//pattern found
				cout << "\n" << line << endl;
				//return only value of pattern
				//TODO
			}
		}
    configfile.close();
	} else {
		printError("Unable to read from config!"); 	
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
		readFromConfig("startNet:");
		return 0;
	} else {
		printWarn("No root access!");
		printError("Please run as root!");
		return 1;
	}
}

