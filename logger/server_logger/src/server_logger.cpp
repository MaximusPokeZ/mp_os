#include <not_implemented.h>
#include <unistd.h>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/msg.h>
#endif

#include "../include/server_logger.h"

#define MSG_KEY 123
#define SEND_STATUS_ONLY_CONSOLE 2
#define SEND_STATUS 0

std::mutex server_logger::mutex;

server_logger::server_logger(const std::unordered_map<severity, std::pair<bool, std::set<std::string>>> & streams) : available_streams(streams)
{
#ifdef _WIN32
	hPipe = CreateNamedPipe("\\\\.\\pipe\\mp_os_srvr_lgr", PIPE_ACCESS_OUTBOUND, //только для записи
                            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, //Режим передачи байтов | чтение по байтам | ждем пока данные не будут доступны
                            1, 0, 0, 0, NULL); //Создание одного канала и защита по умолчанию

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        throw std::runtime_error("Failed to create named pipe");
    }
#else
	msg_descriptor_id = msgget(MSG_KEY, 0666);

	if (msg_descriptor_id == -1)
	{
        throw std::runtime_error("Failed to get message queue descriptor");
	}
#endif
}

logger const *server_logger::log(
		const std::string &text,
		logger::severity severity) const noexcept
{
	for (const auto& [severity_level, stream_info] : available_streams)
	{
		const auto& file_paths = stream_info.second;
		const auto& should_log_to_console = stream_info.first;

		if (severity_level == severity)
		{
				mutex.lock();
				msg_pack msg{};
				msg.msg_status = SEND_STATUS;
				msg.pid = getpid();
				msg.packet_cnt = std::ceil(static_cast<double>(text.size())  / MAX_MSG_SIZE);
				msg.severity = static_cast<int>(severity);
				if (should_log_to_console) { msg.to_console = true; }
				else { msg.to_console = false; }
				if (file_paths.empty())
				{
					msg.msg_status = SEND_STATUS_ONLY_CONSOLE;
					msg.packet_cnt = 1; msg.packet_id = 1;
					strcpy(msg.msg_text, text.substr(0, MAX_MSG_SIZE).c_str());
					msgsnd(msg_descriptor_id, &msg, sizeof(msg_pack), 0);
				}
				else {
					for (const auto &file_path: file_paths) {
						strcpy(msg.file_path, file_path.c_str());

						for (size_t i = 0; i < msg.packet_cnt; ++i) {
							msg.packet_id = i + 1;
							strcpy(msg.msg_text, text.substr(i * MAX_MSG_SIZE, MAX_MSG_SIZE).c_str());

							#ifdef _WIN32
							ConnectNamedPipe(hPipe, NULL); //установки соединения с именованным каналом hPipe
							DWORD bytesWritten; // кол-во байтов, записанных в hPipe
							WriteFile(hPipe, &msg, sizeof(msg_pack), &bytesWritten, NULL);
							#else
							msgsnd(msg_descriptor_id, &msg, sizeof(msg_pack), 0);
							#endif
						}
					}
				}
				mutex.unlock();
			}
		}
	return this;
}
