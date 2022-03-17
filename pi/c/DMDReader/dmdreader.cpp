﻿// DMDReader.cpp: Definiert den Einstiegspunkt für die Anwendung.
//
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <chrono>
#include <thread>
#include <filesystem>

#include "DMDReader.h"

#include "dmd/dmdframe.h"
#include "dmd/dmddata.h"
#include "dmd/color.h"
#include "dmd/pubcapture.h"
#include "util/crc32.h"
#include "util/bmp.h"
#include "render/framerenderer.h"
#include "render/raylibrenderer.h"

using namespace std;

std::vector<DMDFrame*> readfile(string name) {

	std::vector<DMDFrame*> res;

	ifstream df(name.c_str(), ios::in | ios::binary);
	if (!df) {
		return res;
	}

	df.exceptions(ifstream::failbit | ifstream::badbit);

	int rc = 0;
	int i = 0;

	while (rc==0) {
		DMDFrame* frame = new DMDFrame();
		rc = frame->read_from_stream(df);
		if (rc==0) {
			// cout << frame.str() << "\n";
		};

		// DMDFrame* framegray = frame.to_gray8();

		res.push_back(frame);
	}

	cout << "Loaded " << res.size() << " frames\n";

	return res;
}

int main()
{
	string datadir = "../../../../../samples/";

	// Pubcapture test
	PubCapture* pubcapture = new PubCapture(4, datadir + "gbpub");


	// find the correct palette for a file
	vector<DMDPalette*> palettes = default_palettes();


	RaylibRenderer rr = RaylibRenderer(128 * 11,32 * 11,5,1,8);

	cout << std::filesystem::current_path() << endl;
	std::vector<DMDFrame*> frames=readfile(datadir+"spiimage-ghostbusters3.dat");
	// std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	
	DMDFrame frame = DMDFrame();

	//MaskedDMDFrame mframe = MaskedDMDFrame();
	//mframe.read_from_bmp(datadir+"gbpub/54.bmp", *palette);
	//rr.showImage((DMDFrame*) &mframe);

	int i = 0;
	for (auto& f : frames) {
		i++;
		if (i < 461) {
			continue;
		};

		rr.showImage(f);
		cout << i << "\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//if (mframe.matches(f)) {
		//	cout << f->str() << "\n";
		//}
		//	rr.showImage((DMDFrame*)&mframe);
		//}
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10000));

	return 0;
}
