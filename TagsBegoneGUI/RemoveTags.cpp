#pragma once

#include "RemoveTags.hpp"


RemoveTags::RemoveTags() {}


int RemoveTags::decode_synchsafe(std::array<uint8_t, 4> _byte)
{
	return
		(static_cast<uint32_t>(_byte[3]) |
			(static_cast<uint32_t>(_byte[2]) << 7) |
			(static_cast<uint32_t>(_byte[1]) << 15) |
			(static_cast<uint32_t>(_byte[0]) << 23));
}

int RemoveTags::get_id3v1_size(std::ifstream& _file_stream)
{
	id3v1_tag buffer;
	std::streamsize read_offset = _ID3V1_TAG_SIZE;
	std::streamsize read_size = _ID3V1_TAG_SIZE;

	_file_stream.seekg(-read_offset, std::ios::end);

	if (_file_stream.read(reinterpret_cast<char*>(&buffer), read_size))
	{
		if (std::string(buffer.tag_id.data(), 3) == "TAG")
		{
			return _ID3V1_TAG_SIZE;
		}
	}

	return 0;
}

int RemoveTags::get_id3v2_size(std::ifstream& _file_stream)
{
	id3v2_header buffer;
	std::streamsize read_offset = 0;
	std::streamsize read_size = _ID3V2_HEADER_SIZE;

	_file_stream.seekg(read_offset, std::ios::beg);

	if (_file_stream.read(reinterpret_cast<char*>(&buffer), read_size))
	{
		if (std::string(buffer.tag_id.data(), 3) == "ID3")
		{
			return (_ID3V2_HEADER_SIZE + decode_synchsafe(buffer.tag_size));
		}
	}

	return 0;
}

int RemoveTags::get_ape_size(std::ifstream& _file_stream, std::streamsize _id3v1_offset)
{
	ape_footer buffer;
	std::streamsize read_offset = _APE_FOOTER_SIZE + _id3v1_offset;
	std::streamsize read_size = _APE_FOOTER_SIZE;

	_file_stream.seekg(-read_offset, std::ios::end);

	if (_file_stream.read(reinterpret_cast<char*>(&buffer), read_size))
	{
		if (std::string(buffer.tag_id.data(), 8) == "APETAGEX")
		{
			int tag_size = buffer.tag_size;
			std::bitset<32> flag_bits(buffer.flags);
			tag_size += flag_bits[31] ? _APE_FOOTER_SIZE : 0;

			return tag_size;
		}
	}

	return 0;
}

int RemoveTags::remove_tags(std::filesystem::path filePath, bool id3v1, bool id3v2, bool apev2)
{
	std::ifstream file_stream(filePath, std::ios::binary);
	file_stream.close(); //---------------------------------------------------------->>>>> REMOVE AFTER TESTING
	if (!file_stream.is_open())
	{
		//std::println("Cannot open file!");
		return -1;
	}

	std::streamsize trim_beg = 0;
	std::streamsize trim_end = 0;
	trim_end += id3v1 ? get_id3v1_size(file_stream) : 0;
	trim_beg += id3v2 ? get_id3v2_size(file_stream) : 0;
	trim_end += apev2 ? get_ape_size(file_stream, trim_end) : 0;

	// removing the tags

	if (trim_beg > 0 || trim_end > 0)
	{
		// reading contents of the original file
		file_stream.seekg(0, std::ios::end);
		std::streamsize size_orig = file_stream.tellg();
		std::streamsize size_new = size_orig - trim_beg - trim_end;
		file_stream.seekg(trim_beg, std::ios::beg);
		std::vector<char> buffer(size_new);

		if (!file_stream.read(buffer.data(), size_new))
		{
			//std::println("Error reading the file!");
			file_stream.close();
			return -1;
		}

		// rewriting the contents of the file
		file_stream.close();
		std::ofstream file_stream(filePath, std::ios::binary | std::ios::trunc);
		if (!file_stream.is_open())
		{
			//std::println("Cannot open file for writing!");
			return -1;
		}

		if (!file_stream.write(buffer.data(), size_new))
		{
			//std::println("Error writing to file!");
			file_stream.close();
			return -1;
		}

		file_stream.close();

		return 1;
	}

	file_stream.close();

	return 0;
}

RemoveTags::Files RemoveTags::check_file(std::filesystem::path path, bool id3v1, bool id3v2, bool apev2)
{
	Files file;

	if (!std::filesystem::is_regular_file(path))
	{
		return file;
	}
	
	if (path.extension() == ".mp3" ||
		path.extension() == ".MP3" ||
		path.extension() == ".mP3" ||
		path.extension() == ".Mp3")
	{
		file.found++;

		int returnCode = remove_tags(path, id3v1, id3v2, apev2);
		
		if (returnCode > 0)
		{
			file.updated++;
		}
		else if (returnCode < 0)
		{
			file.ignored++;
		}
	}

	return file;
}

void RemoveTags::process_directory(QStatusBar* statusBar, std::filesystem::path sourcePath, bool inclSubdirs, bool id3v1, bool id3v2, bool apev2)
{
	Files files;

	if (inclSubdirs)
	{
		for (const auto& item : std::filesystem::recursive_directory_iterator(sourcePath, std::filesystem::directory_options::skip_permission_denied))
		{
			Files file = check_file(item.path(), id3v1, id3v2, apev2);
			files += file;
		}
	}
	else
	{
		for (const auto& item : std::filesystem::directory_iterator(sourcePath, std::filesystem::directory_options::skip_permission_denied))
		{
			Files file = check_file(item.path(), id3v1, id3v2, apev2);
			files += file;
		}
	}

	QString message;

	if (files.found == 0)
	{
		message = "No mp3 files found";
	}
	else
	{
		message = "Files found : " + QString::number(files.found) +	" | Files updated : " + QString::number(files.updated);

		if (files.ignored > 0)
		{
			message.append(" | Rear/write errors : " + QString::number(files.ignored));
		}
	}

	statusBar->showMessage(message);
}

void RemoveTags::process_file(QStatusBar* statusBar, std::filesystem::path sourcePath, bool id3v1, bool id3v2, bool apev2)
{
	Files files;

	Files file = check_file(sourcePath, id3v1, id3v2, apev2);
	files += file;
	
	if (files.updated > 0)
	{
		statusBar->showMessage("Tags removed");
	}
	else if (files.ignored > 0)
	{
		statusBar->showMessage("Read/write error");
	}
	else
	{
		statusBar->showMessage("No tags found");
	}
}
