#include<vector>
#include<queue>
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
		FILE_INFO(const FILE_INFO& other)
		{
			line_num = other.line_num;
			valid = other.valid.load();
			io_complete = other.io_complete.load();
			content = other.content;
		}
		void AppendLine(const std::string& content);
		void AppendKeyValue(const std::string& key, const std::string& value);
		std::string GetValueByKey(const std::string& key);
		int GetIntValueByKey(const std::string& key);
		
		
		//确保IO完成
		void Lock() const;
		bool TryLock() const;

		//file exists and not empty
		atomic<bool> valid;
		//IO是否完成
		atomic<bool> io_complete;
		int line_num;
		shared_ptr<vector<std::string>> content;
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

		//sync verify file, verify md5 if expected_md5 is not empty
		bool VerifyFile(const std::string& filename, const std::string& expected_md5 = "");

		void Certfile(const std::string& filename);
		
		void Init();
	private:
		constexpr static int BUFFER_SIZE = 1024;
		constexpr static int FILE_LINE_MAX_NUM = 10000;
		struct VERIFY_INFO
		{
			int private_value = 0;
			std::string md5;
			bool valid = 0;
		};
		enum IO_TYPE
		{
			ReadFile,
			WriteFile,
			Certfile,
		};
		struct IO_DESC
		{
			string fileName;
			VERIFY_INFO fileData;
			IO_TYPE op_type;
			bool verify = 0;
		};
		FILE_INFO ReadFile_impl(const std::string& filename);
		void WriteFile_impl(const std::string& filename, FILE_INFO file_content);
		void Certfile_impl(const std::string& filename);
		bool md5_verify(const std::string& filename, const std::string& expected_md5);
		string GetFileMD5(const std::string& filename);
		VERIFY_INFO CalcFileCertInfo(const std::string& filename);
		VERIFY_INFO GetFileCertInfo(const std::string& filename);


		shared_ptr<queue<IO_DESC>> asio_queue;
	};
}