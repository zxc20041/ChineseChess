#include"framework_base.h"
#include"CChessEngineAdapter.h"
#include <boost/asio.hpp>
#include <boost/process/windows.hpp>

using namespace CChessBase;
using namespace debugger;
using namespace std;
using namespace std::chrono_literals;
namespace bp = boost::process;
namespace ba = boost::asio;


CChessEngineAdapter::CChessEngineAdapter()
{
    elo = 1500, enable_LimitStrength = 1, hash_size = 64;
    status = ENGINE_STATUS::E_INIT;
    stepTime = 0, targetStepTime = 5;
    bestMoveRecv = 0, noBestMove = 0, mate = 0;
    thread_num = 4;
    currentPosInMove = "position startpos moves";
    //todo: get exeFileNames from text

    exeFileNames[0] = "./game/pikafish230305/pikafish-avx2.exe";
    argument = "";
}

CChessEngineAdapter::~CChessEngineAdapter()
{
    if (proc.running())
    {
        //write quit
        write_input("quit");
    }
}

void CChessEngineAdapter::Reset()
{
    //stop process if started
    if (proc.running())
    {
        //write quit
        write_input("quit");
    }
    

    //start process
    proc = bp::child(
        exeFileNames[0],
        bp::args({ argument }),
        bp::windows::create_no_window,
        bp::std_in < in,
        bp::std_out > out,
        io_ctx
    );
    //proc.reset(new bp::popen(io_ctx, exeFileNames[0], { argument }));
    if (!proc.running())
    {
        debugger_main.writelog(DWARNNING, "failed to start process " + exeFileNames[0], __LINE__);
        return;
    }

    // 创建线程处理异步I/O
    std::thread out_thread([&] 
    {
        std::string line;
        while (proc.running()) 
        {
            std::getline(out, line);
            read_output(line);
            Sleep(1);
        }
        status = ENGINE_STATUS::E_FAILED;
        debugger_main.writelog(DDEBUG, "out_thread stopped running", __LINE__);
        return;
    });
    out_thread.detach();
    status = ENGINE_STATUS::E_READY;
    

    //set option and enter uci
    write_input("uci");

    write_input("setoption name Debug Log File value PikaFishLog.txt");
    write_input("setoption name UCI_LimitStrength value true");
    write_input("setoption name UCI_Elo value " + to_string(elo));
    write_input("setoption name Threads value " + to_string(thread_num));
    write_input("setoption name Hash value " + to_string(hash_size));

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
    //校验参数
    if (move.fromx<BOARD_X_MIN || move.fromx>BOARD_X_MAX || move.tox<BOARD_X_MIN || move.tox>BOARD_X_MAX
        || move.fromy< BOARD_Y_MIN || move.fromy>BOARD_Y_MAX || move.toy<BOARD_Y_MIN || move.toy>BOARD_Y_MAX)
    {
        debugger_main.writelog(DWARNNING, "param move illegal in CChessEngineAdapter::MovePiece() " + to_string(move.fromx) + " " + to_string(move.fromy) + " -> " + to_string(move.tox) + " " + to_string(move.toy), __LINE__);
    }

    currentPosInMove += " ";
    currentPosInMove += (char)move.fromx - '0' + 'a';
    currentPosInMove += to_string(move.fromy);
    currentPosInMove += (char)move.tox - '0' + 'a';
    currentPosInMove += to_string(move.toy);

    //send cmd
    write_input(currentPosInMove);
    return;
}

bool CChessEngineAdapter::GetWin()
{
    return false;
}

CChessBase::PieceMoveDesc CChessEngineAdapter::GetBestMove()
{
    return bestMove;
}

void CChessEngineAdapter::read_output(string line)
{
    
    debugger_main.writelog(DDEBUG, "read_output from proc: " + line, __LINE__);
    size_t find_pos;
    if (line.find("uciok") != string::npos)
    {
        //ok

    }
    else if (line.find("id name") != string::npos)
    {
        find_pos = line.find("id name");
        engineID = line.substr(find_pos + sizeof("id name"));
    }
    else if (line.find("bestmove") != string::npos)
    {
        find_pos = line.find("bestmove");
        bestMove.fromx = line[find_pos + sizeof("bestmove")] - 'a';
        bestMove.fromy = line[find_pos + sizeof("bestmove") + 1] - '0';
        bestMove.tox = line[find_pos + sizeof("bestmove") + 2] - 'a';
        bestMove.toy = line[find_pos + sizeof("bestmove") + 3] - '0';
        bestMoveRecv = 1;
    }
    else if (line.find("bestmove (none)") != string::npos)
    {
        noBestMove = 1;
    }
    else if (line.find("score mate 0") != string::npos)
    {
        mate = 1;
    }
    return;
}


void CChessEngineAdapter::write_input(string cmd)
{
    if (!proc.running())
    {
        debugger_main.writelog(DWARNNING, "failed to write_input while process not running " + cmd, __LINE__);
        status = ENGINE_STATUS::E_FAILED;
        return;
    }
    //cmd += '\n';
    in << cmd << std::endl;

    debugger_main.writelog(DDEBUG, "write_input: " + cmd, __LINE__);
    if (cmd == "quit")
    {
        proc.wait_for(500ms);
        if (proc.running())
        {
            proc.terminate();
        }
    }
    //ba::async_write(in, ba::dynamic_buffer(cmd), [&](boost::system::error_code ec, std::size_t n)
    //    {
    //        if (!ec)
    //        {
    //            //debugger_main.writelog(DDEBUG, "write: " + line, __LINE__);
    //            if (cmd == "quit\n")
    //            {
    //                Sleep(500);
    //                if (proc.running())
    //                {
    //                    proc.terminate();
    //                }
    //            }
    //        }
    //        else
    //        {
    //            //debugger_main.writelog(DWARNNING, "write failed: " + line, __LINE__);
    //        }
    //    }
    //);
    return;
}
