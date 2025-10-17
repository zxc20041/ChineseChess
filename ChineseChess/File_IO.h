#include<vector>
#include<string>

namespace FileManager_ns
{
	constexpr static const char* config_filename = "./save/config.dat";

	class FILE_INFO
	{
	public:
		FILE_INFO()
		{
			line_num = 0;
			valid = 1;
			io_complete = 0;
			content = make_shared<vector<std::string>>();
		}
		FILE_INFO(bool _valid)
		{
			line_num = 0;
			valid = _valid;
			io_complete = !_valid;
			content = make_shared<vector<std::string>>();
		}
		void AppendLine(const std::string& content);
		void AppendKeyValue(const std::string& key, const std::string& value);
		std::string GetValueByKey(const std::string& key);
		int GetIntValueByKey(const std::string& key);
		int line_num;
		shared_ptr<vector<std::string>> content;
		
		//确保IO完成
		void Lock() const;
		bool TryLock() const;
		//file exists and not empty
		atomic<bool> valid, io_complete;
	private:
		std::string Key_Format(const std::string& key);
	};

	class FileManager
	{
	public:
		FileManager();
		~FileManager();

		//sync read file, verify md5 if expected_md5 is not empty
		FILE_INFO ReadFile(const std::string& filename, const std::string& expected_md5);
		//async read file, verify md5 if verify is true
		FILE_INFO ReadFile(const std::string& filename, const bool verify = 0);

		void SaveConfig();
		void ReadConfig();

		void RemoveFile(const std::string& filename);

		void WriteFile(const std::string& filename, FILE_INFO file_content, bool certify = 0);
		
		void Init();
	private:
		struct VERIFY_INFO
		{
			int private_value;
			std::string md5;
			bool valid;
		};
		FILE_INFO ReadFile_impl(const std::string& filename);
		void WriteFile_impl(const std::string& filename, FILE_INFO file_content);
		bool Certfile(const std::string& filename);
		bool md5_verify(const std::string& filename, const std::string& expected_md5);
		VERIFY_INFO GetFileVerifyInfo(const std::string& filename);
	};
}