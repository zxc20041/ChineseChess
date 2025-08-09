//#include"framework_base.h"
#define WIN32_LEAN_AND_MEAN
#include"CChessEngineAdapter.h"
#include <boost/process.hpp>


using namespace CChessBase;
//using namespace debugger;
using namespace std;
namespace bp = boost::process;
namespace ba = boost::asio;


CChessEngineAdapter::CChessEngineAdapter()
{
    elo = 0, enable_LimitStrength = 1, hash_size = 64;
    status = ENGINE_STATUS::E_INIT;
    stepTime = 0, targetStepTime = 5;
    thread_num = 4;
    //todo: get exeFileNames from text

    exeFileNames[0] = "./game/pikafish230305/pikafish-avx2.exe";
    argument = "pikafish-avx2.exe";
}

CChessEngineAdapter::~CChessEngineAdapter()
{
    if (0)
    {
        //write quit
        write_input("quit");
    }
}

void CChessEngineAdapter::Reset()
{
    //stop process if started
    if (0)
    {
        //write quit
        write_input("quit");
    }
    

    //start process
    //proc.reset(new bp::popen(io_ctx, exeFileNames[0], { argument }));
    if (0)
    {
        //debugger_main.writelog(DWARNNING, "failed to start process " + exeFileNames[0], __LINE__);
        return;
    }
    status = ENGINE_STATUS::E_READY;
    read_output();
    return;
}

void CChessEngineAdapter::SetElo(int elo)
{
    if (elo > ELO_MAX)
    {
        this->elo = ELO_MAX;
        enable_LimitStrength = 0;
        return;
    }
    if (elo < ELO_MIN)
    {
        elo = ELO_MIN;
        return;
    }
    this->elo = elo;
    return;
}

void CChessEngineAdapter::MovePiece(CChessBase::PieceMoveDesc move)
{
}

bool CChessEngineAdapter::GetWin()
{
    return false;
}

CChessBase::PieceMoveDesc CChessEngineAdapter::GetBestMove()
{
    return CChessBase::PieceMoveDesc();
}

void CChessEngineAdapter::read_output()
{
    if (0)
    {
        status = ENGINE_STATUS::E_FAILED;
        //debugger_main.writelog(DWARNNING, "failed to start process " + exeFileNames[0], __LINE__);
        return;
    }
    //ba::async_read_until(proc, ba::dynamic_buffer(line), '\n',
    //    [&](boost::system::error_code ec, std::size_t n)
    //    {
    //        if (!ec)
    //        {
    //            debugger_main.writelog(DDEBUG, "read: " + line, __LINE__);
    //            read_output(); // 继续读取下一行
    //        }
    //        else
    //        {
    //            debugger_main.writelog(DWARNNING, "read failed: " + line, __LINE__);
    //        }
    //    });
    return;
}

void CChessEngineAdapter::write_input(string cmd)
{
    if (0)
    {
        //debugger_main.writelog(DWARNNING, "failed to write_input while process not ready " + cmd, __LINE__);
        status = ENGINE_STATUS::E_FAILED;
        return;
    }
    cmd += '\n';
    /*ba::async_write(proc, ba::dynamic_buffer(cmd), [&](boost::system::error_code ec, std::size_t n)
        {
            if (!ec)
            {
                debugger_main.writelog(DDEBUG, "write: " + line, __LINE__);
                if (cmd == "quit\n")
                {
                    Sleep(1000);
                    if (proc && proc->is_open())
                    {
                        proc->request_exit();
                        Sleep(100);
                        proc->terminate();
                    }
                }
            }
            else
            {
                debugger_main.writelog(DWARNNING, "write failed: " + line, __LINE__);
            }
        }
    );*/
    return;
}
