#pragma once
#include"CChessBase.h"

#include <string>
//#include <boost/asio.hpp>
#include <boost/process.hpp>


class CChessEngineAdapter
{
public:
	CChessEngineAdapter();
	~CChessEngineAdapter();

	//��������������
	void Reset();

	//E_READY  elo 1350-2850 ����2850ʱenable_LimitStrength=false��С��1350ʱΪ1350
	void SetElo(int elo);

	void MovePiece(CChessBase::PieceMoveDesc move);


	//һ��ʤ��ʱ�˳�����
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
	std::string engineID;
	std::string currentPosInMove;

	CChessBase::PieceMoveDesc bestMove;

	ENGINE_STATUS status;

	bool enable_LimitStrength, bestMoveRecv, noBestMove, mate;

	int elo;

	int thread_num, hash_size;

	float stepTime, targetStepTime;

	std::string exeFileNames[EXE_FILENAME_MAX],argument;


	//ba::streambuf out_buf, err_buf;
	boost::asio::io_context io_ctx;
	boost::process::child proc;
	// �����ܵ�
	boost::process::opstream in;      // ���뵽�ӽ���
	boost::process::ipstream out;     // �ӽ��̵����
	
	//boost::asio::posix::stream_descriptor in(ba::io_context);

	//std::string line;
	void read_output(std::string line);
	
	void write_input(std::string cmd);
};