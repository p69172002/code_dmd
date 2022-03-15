#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include "util/crc32.h"

#define  PIXVAL uint8_t  // we don't deal with more than 8 bits/pixel

using namespace std;

class DMDFrame {

protected:

	int columns;
	int rows;
	int bitsperpixel;
	uint8_t* data;

public:

	DMDFrame(int columns1 = 0, int rows1 = 0, int bitsperpixel1 = 0, uint8_t* data1 = NULL);
	~DMDFrame();

	PIXVAL getPixel(int x, int y);

	int read_from_stream(std::ifstream& fis);

	bool same_size(DMDFrame* f2);
	bool equals_fast(DMDFrame* f2);

	int get_width();
	int get_height();
	int get_bitsperpixel();
	uint8_t get_pixelmask();

	uint8_t* get_data();

	string str();

	/*
	 * Convert to 8 bits per pixel
	 */
	DMDFrame* to_gray8();
	DMDFrame* to_gray1(int threshold = 1);
	

protected:

	void recalc_checksum();

	void init_mem(uint8_t *data1 = NULL);

	// cache some stuff
	int datalen;
	int rowlen;
	uint8_t pixel_mask;
	uint32_t checksum; // uses for fast equality check

	uint8_t next_pixel(uint8_t **buf, int *bit_index);
};

class MaskedDMDFrame : DMDFrame {

public:

	MaskedDMDFrame();
	~MaskedDMDFrame();

	bool matches(DMDFrame* f);

	/**
	 * Read a frame from a BMP file
	 *
	 * grayindex: offset of the color to use as the gray channel
	 * B=0, G=1, R=2
	 */
	int read_from_bmp(string filename, int grayoffset = 2, int maskoffset = 0);

private:

	uint8_t* mask;

};