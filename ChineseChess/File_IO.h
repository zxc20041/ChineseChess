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
			content = make_shared<vector<std::string>>();
		}
		FILE_INFO(bool _valid)
		{
			line_num = 0;
			valid = _valid;
			content = make_shared<vector<std::string>>();
		}
		void AppendLine(const std::string& content);
		void AppendKeyValue(const std::string& key, const std::string& value);
		std::string GetValueByKey(const std::string& key);
		int line_num;
		shared_ptr<vector<std::string>> content;
		bool valid;
	private:
		std::string Key_Format(const std::string& key);
	};

	class FileManager
	{
	public:
		FileManager();
		~FileManager();


		FILE_INFO ReadFile(const std::string& filename, const std::string& expected_md5);

		void SaveConfig();
		void ReadConfig();

		void RemoveFile(const std::string& filename);
		//void AddFile(const std::string& filename);
		
		
		//void FinishFile(const std::string& filename, bool certify = 0);
		void WriteFile(const std::string& filename, FILE_INFO file_content, bool certify = 0);
		
	private:
		FILE_INFO ReadFile_impl(const std::string& filename);
		void WriteFile_impl(const std::string& filename, FILE_INFO file_content);
		bool Certfile(const std::string& filename);
		bool md5_verify(const std::string& filename, const std::string& expected_md5);
		
	};
}