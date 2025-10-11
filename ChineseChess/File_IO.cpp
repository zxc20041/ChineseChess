#include "framework_base.h"
#include "audio_thread.h"
#include "settings.h"
#include "thread_communicate.h"
#include "File_IO.h"

using namespace std;
using namespace FileManager_ns;

extern debug_ex debugger_audio;

void FILE_INFO::AppendLine(const std::string& content)
{
}

void FILE_INFO::AppendKeyValue(const std::string& key, const std::string& value)
{
}

string FILE_INFO::GetValueByKey(const std::string& key)
{
	return string();
}

string FILE_INFO::Key_Format(const std::string& key)
{
	return string();
}

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

FILE_INFO FileManager::ReadFile(const std::string& filename, const std::string& expected_md5)
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
