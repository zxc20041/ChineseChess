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
	if (expected_md5.empty())
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


void FileManager::SaveConfig()
{
}

void FileManager::ReadConfig()
{
}

void FileManager::RemoveFile(const std::string& filename)
{
}

FILE_INFO FileManager_ns::FileManager::ReadFile(const std::string& filename, const bool verify)
{
	return FILE_INFO();
}

void FileManager::WriteFile(const std::string& filename, FILE_INFO file_content, bool certify)
{
	
}

void FileManager::Certfile(const std::string& filename)
{
	return;
}

bool FileManager_ns::FileManager::VerifyFile(const std::string& filename, const std::string& expected_md5)
{
	if (expected_md5.empty())
	{
		return true;
	}
	return GetFileMD5(filename) == expected_md5;
}

FILE_INFO FileManager::ReadFile_impl(const std::string& filename)
{
	FILE_INFO fileData;
	string linebuf;
	ifstream file;
	int num = 0;
	file.open(filename, ios::in);
	if (!file.is_open())
	{
		g_am.PlayEffectSound("ioerror");
		g_cm.CreateEffect(301, 400, 50);
		debugger_main.writelog(DWARNNING, "ReadFile failed! " + filename, __LINE__);
		return FILE_INFO(0);
	}
	while (getline(file, linebuf))
	{
		if (linebuf.empty())
		{
			continue;
		}
		else
		{
			fileData.content->push_back(linebuf);
			num++;
			if (num == FILE_LINE_MAX_NUM)
			{
				debugger_main.writelog(DWARNNING, "lines beyond max_num limit in FileManager::ReadFile_impl()! " + to_string(FILE_LINE_MAX_NUM) + " " + string(filename), __LINE__);
				break;
			}
		}
	}
	file.close();
	fileData.line_num = num;
	fileData.valid = true;
	fileData.io_complete.store(1);
	return fileData;
}

void FileManager::WriteFile_impl(const std::string& filename, FILE_INFO file_content)
{
	if (file_content.valid == 0)
	{
		debugger_main.writelog(DWARNNING, "WriteFile Failed since file_content invalid! " + filename, __LINE__);
		return;
	}
	ofstream file;
	string linebuf;
	file.open(filename, ios::out);
	
	if (!file.is_open())
	{
		g_am.PlayEffectSound("ioerror");
		g_cm.CreateEffect(301, 400, 50);
		debugger_main.writelog(DWARNNING, "WriteFile Error! " + filename, __LINE__);
		return;
	}
	for (int i = 0; i < file_content.line_num; i++)
	{
		file << file_content.content->at(i) << endl;
	}
	file.close();
	file_content.io_complete.store(1);
	return;
}

void FileManager_ns::FileManager::Certfile_impl(const std::string& filename)
{
	VERIFY_INFO result = CalcFileCertInfo(filename);
	if (!result.valid)
	{
		debugger_main.writelog(DWARNNING, "Certfile_impl: CalcFileCertInfo failed for file " + filename, __LINE__);
		return;
	}
	FILE_INFO verify_file(1);
	verify_file.AppendLine(result.md5);
	verify_file.AppendLine(to_string(result.private_value));
	//.dat->.check
	string verify_filename = filename.substr(0, filename.length() - 3) + ".check";
	WriteFile_impl(verify_filename, verify_file);
	return;
}


bool FileManager::md5_verify(const std::string& filename, const std::string& expected_md5)
{
	if (expected_md5.empty() || NO_HASH_CHECK)
	{
		return true;
	}
	return GetFileMD5(filename) == filename;
}

string FileManager_ns::FileManager::GetFileMD5(const std::string& filename)
{
	if (NO_HASH_CHECK)
	{
		return string();
	}
	MD5_CTX ctx;
	int len = 0, check = 0;
	unsigned char buffer[1024] = { 0 };
	unsigned char digest[16] = { 0 };

	FILE* pFile = 0;
	errno_t err;

	err = fopen_s(&pFile, filename.c_str(), "rb");

	if (pFile == 0 || err != 0)
	{
		debugger_main.writelog(DWARNNING, "Open file failed! " + filename, __LINE__);
		return string();
	}
	MD5_Init(&ctx);
	while ((len = fread(buffer, 1, 1020, pFile)) > 0)
	{
		MD5_Update(&ctx, buffer, len);
	}

	MD5_Final(digest, &ctx);

	fclose(pFile);

	char buf[33] = { 0 };
	char tmp[3] = { 0 };
	for (int i = 0; i < 16; i++)
	{
		sprintf_s(tmp, "%02X", digest[i]);
		strcat_s(buf, tmp);
	}
	return string(buf);
}

FileManager::VERIFY_INFO FileManager_ns::FileManager::CalcFileCertInfo(const std::string& filename)
{
	VERIFY_INFO info;
	info.md5 = GetFileMD5(filename);
	info.private_value = 0;
	if (info.md5.length() < 32)
	{
		info.valid = 0;
		return info;
	}
	info.private_value = (int)info.md5[1] * (int)info.md5[10] + (int)info.md5[2] * (int)info.md5[9] + (int)info.md5[4] * (int)info.md5[8];
	info.private_value *= (int)info.md5[14] + (int)info.md5[6];
	info.private_value /= (int)info.md5[3] / 3 + 1;
	info.private_value += (int)info.md5[5] * (int)info.md5[7];
	info.valid = 1;
	return info;
}

FileManager::VERIFY_INFO FileManager::GetFileCertInfo(const std::string& filename)
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

