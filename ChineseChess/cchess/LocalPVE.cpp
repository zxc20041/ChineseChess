#include"LocalPVE.h"
#include"..\framework_base.h"

using namespace std;
using namespace debugger;
using namespace CChessBase;

CChessLocalPVE::CChessLocalPVE() :CChessLocalPVP()
{
}

CChessLocalPVE::~CChessLocalPVE()
{
}

void CChessLocalPVE::Update()
{
	if (update_check_win)
	{
		EngineResult r = engineAdapter.GetResult();
		if (r.valid)
		{
			if (result.result == RESULT_MATE)
			{
				win_side = !current_side_red;
				match_over = 1;
				r.win_side = !current_side_red;
			}
			result = r;
			update_check_win = 0;
		}
	}

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
