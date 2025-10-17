#include "framework_base.h"
#include "audio_thread.h"
#include "settings.h"
#include "thread_communicate.h"
#include "File_IO.h"

using namespace std;
using namespace FileManager_ns;
using namespace debugger;

extern debug_ex debugger_audio;

void FILE_INFO::AppendLine(const std::string& _content)
{
	content->push_back(_content);
	line_num++;
	return;
}

void FILE_INFO::AppendKeyValue(const std::string& key, const std::string& value)
{
	content->push_back(Key_Format(key));
	content->push_back(value);
	line_num += 2;
	return;
}

string FILE_INFO::GetValueByKey(const std::string& key)
{
	for (size_t i = 0; i < content->size(); i++)
	{
		if ((*content)[i] == Key_Format(key)) 
		{
			return (*content)[i + 1];
		}
	}
	return string();
}

int FileManager_ns::FILE_INFO::GetIntValueByKey(const std::string& key)
{
	for (size_t i = 0; i < content->size(); i++)
	{
		if ((*content)[i] == Key_Format(key))
		{
			return atoi((*content)[i + 1].c_str());
		}
	}
	return 0;
}

void FileManager_ns::FILE_INFO::Lock() const
{
	while (!io_complete.load());
	return;
}

inline bool FileManager_ns::FILE_INFO::TryLock() const
{
	return io_complete.load();
}

string FILE_INFO::Key_Format(const std::string& key)
{
	return "[" + key + "]";
}

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

void FileManager::Init()
{
	//创建读写线程

}

FILE_INFO FileManager::ReadFile(const std::string& filename, const std::string& expected_md5)
{
	if(expected_md5.empty())
	{
		return ReadFile_impl(filename);
	}
	//verify md5
	if (!md5_verify(filename, expected_md5))
	{
		return FILE_INFO(false);
	}
	
	return ReadFile_impl(filename);
}

FILE_INFO FileManager_ns::FileManager::ReadFile(const std::string& filename, const bool verify)
{
	return FILE_INFO();
}

void FileManager::SaveConfig()
{
}

void FileManager::ReadConfig()
{
}

void FileManager::RemoveFile(const std::string& filename)
{
}

void FileManager::WriteFile(const std::string& filename, FILE_INFO file_content, bool certify)
{
}

FILE_INFO FileManager::ReadFile_impl(const std::string& filename)
{
	return FILE_INFO();
}

void FileManager::WriteFile_impl(const std::string& filename, FILE_INFO file_content)
{
}

bool FileManager::Certfile(const std::string& filename)
{
	return false;
}

bool FileManager::md5_verify(const std::string& filename, const std::string& expected_md5)
{
	return false;
}

FileManager::VERIFY_INFO FileManager::GetFileVerifyInfo(const std::string& filename)
{
	VERIFY_INFO result;
	//.dat->.check
	string verify_filename = filename.substr(0, filename.length() - 3) + ".check";
	FILE_INFO verify_file = ReadFile_impl(verify_filename);
	if (verify_file.valid && verify_file.line_num > 2)
	{
		result.private_value = verify_file.GetIntValueByKey("private_value");
		result.md5 = verify_file.GetValueByKey("md5");
		result.valid = 1;
	}
	else if (verify_file.valid && verify_file.line_num == 2)
	{
		result.valid = 1;
		result.md5 = verify_file.content->at(0);
		try
		{
			result.private_value = stoi(verify_file.content->at(1));
		}
		catch (exception e)
		{
			result.private_value = 0;
			debugger_main.writelog(DWARNNING, "GetFileVerifyInfo: stoi failed for file " + verify_filename);
			result.valid = 0;
		}
	}
	else
	{
		result.private_value = 0;
		result.md5 = "";
		result.valid = 0;
	}
	return result;
}
