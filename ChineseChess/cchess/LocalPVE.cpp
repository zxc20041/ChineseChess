#include"LocalPVE.h"
#include"..\framework_base.h"
#include "File_IO.h"

using namespace std;
using namespace debugger;
using namespace CChessBase;
using namespace FileManager_ns;

CChessLocalPVE::CChessLocalPVE() :CChessLocalPVP()
{
}

CChessLocalPVE::~CChessLocalPVE()
{
}

void CChessLocalPVE::Update()
{
	CChessLocalPVP::Update();
	return;
}

bool CChessLocalPVE::CheckBestMove()
{
    return engineAdapter.CheckBestMove();
}

PieceMoveDesc CChessLocalPVE::GetBestMove()
{
    return engineAdapter.GetBestMove();
}

void CChessLocalPVE::SearchBestMove()
{
	engineAdapter.SearchBestMove();
	return;
}

void CChessLocalPVE::RecordMove(CChessBase::PieceMoveDesc move)
{
	if (!piece_moved)
	{
		piece_moved = 1;
		recordFilename = g_fm.GetBiggestIndexedFilename("record", ".dat", "pve");
		FILE_INFO file_head;

		file_head.AppendKeyValue("username", usernameC);
		file_head.AppendKeyValue("gamemode", "PVE");
		file_head.AppendKeyValue("start_time", getTimeStr());
		file_head.AppendLine("[moves]");
		file_head.line_num = file_head.content->size();
		g_fm.WriteFile(recordFilename, file_head, 1);
	}
	FILE_INFO file_content;
	file_content.line_num = 1;
	file_content.content->emplace_back(string(1, (char)move.fromx + 'a') + to_string(move.fromy) + string(1, (char)move.tox + 'a') + to_string(move.toy));
	g_fm.AppendFile(recordFilename, file_content, 1);
	return;
}
