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
