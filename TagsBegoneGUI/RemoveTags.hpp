#pragma once

#include <filesystem>
#include <fstream>
#include <bitset>
#include <Windows.h>
#include <vector>
#include <array>
#include <QLineEdit>


class RemoveTags
{
	const int		_ID3V1_TAG_SIZE = 128;
	const int		_ID3V2_HEADER_SIZE = 10;
	const int		_APE_FOOTER_SIZE = 32;


	struct id3v1_tag
	{
		std::array<char, 3>		tag_id = {};
		std::array<char, 30>	song_name = {};
		std::array<char, 30>	artist = {};
		std::array<char, 30>	album_name = {};
		std::array<char, 4>		year = {};
		std::array<char, 30>	comment = {};
		uint8_t					genre_id = {};
	};

	struct id3v2_header
	{
		std::array<char, 3>		tag_id = {};
		uint8_t					version = {};
		uint8_t					revision = {};
		uint8_t					flags = {};
		std::array<uint8_t, 4>	tag_size = {};
	};

	struct ape_footer
	{
		std::array<char, 8>		tag_id = {};
		uint32_t				version = {};
		uint32_t				tag_size = {};
		uint32_t				item_count = {};
		uint32_t				flags = {};
		uint64_t				reserved = {};
	};


  public:

	RemoveTags();


	int decode_synchsafe(std::array<uint8_t, 4> _byte);
	
	int get_id3v1_size(std::ifstream& _file_stream);
	int get_id3v2_size(std::ifstream& _file_stream);
	int get_ape_size(std::ifstream& _file_stream, std::streamsize _id3v1_offset);
	
	int removeTags(std::filesystem::path path);
	
	int processFolder(QLineEdit& lineEdit, std::filesystem::path input_path);
};