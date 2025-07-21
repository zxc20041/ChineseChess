#pragma once
//线程间通信
extern bool thread_IO_request_save_config, thread_IO_config_read, thread_IO_request_update_profile, thread_IO_request_read_config, thread_IO_request_rend_taglines;
extern int thread_IO_request_userinit, thread_IO_request_verify_res, thread_IO_request_read_record;



//extern int thread_IO_request_load_gameGB, thread_IO_request_delete_game, thread_IO_request_record_game, thread_IO_request_read_history, thread_game_automode_status, thread_game_automode_request;
extern float time_used, timebuf;

extern int currentSave;
//extern int thread_Audio_target_music;


extern int main_thread_render_blocked;


