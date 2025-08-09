#pragma once


#include"CChessBase.h"

#include <string>



class CChessEngineAdapter
{
public:
	CChessEngineAdapter();
	~CChessEngineAdapter();

	//启动并重置引擎
	void Reset();

	//E_READY  elo 1350-2850 大于2850时enable_LimitStrength=false，小于1350时为1350
	void SetElo(int elo);

	void MovePiece(CChessBase::PieceMoveDesc move);


	//一方胜利时退出引擎
	bool GetWin();

	CChessBase::PieceMoveDesc GetBestMove();

	enum class ENGINE_STATUS
	{
		E_INIT,
		E_READY,
		E_BUSY,
		E_FAILED
	};

private:
	constexpr static int ELO_MAX = 2850, ELO_MIN = 1350, EXE_FILENAME_MAX = 16;


	//std::string Move2FenString(CChessBase::PieceMoveDesc move);

	//std::string currentPosInFen;

	std::string currentPosInMove;

	ENGINE_STATUS status;

	bool enable_LimitStrength;

	int elo;

	int thread_num, hash_size;

	float stepTime, targetStepTime;

	std::string exeFileNames[EXE_FILENAME_MAX],argument;


	//ba::streambuf out_buf, err_buf;
	
	
	//boost::asio::posix::stream_descriptor in(ba::io_context);

	std::string line;
	void read_output();
	void write_input(std::string cmd);
};