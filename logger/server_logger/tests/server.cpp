#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <thread>

#define MAX_MSG_SIZE 1024
#define MAX_PATH_SIZE 512
#define MSG_KEY 123
#define CLOSE_MSG_Q 1
#define SEND_STATUS_ONLY_CONSOLE 2


struct msg_pack
{
	int msg_status;
	int severity;
	pid_t pid;
	size_t packet_id;
	size_t packet_cnt;
	bool to_console;
	char file_path[MAX_PATH_SIZE];
	char msg_text[MAX_MSG_SIZE];
};

#ifdef _WIN32
#include <windows.h>
HANDLE hPipe;
bool read_win32_slot(HANDLE hSlot, msg_t &msg);
void cleanup() {
	CloseHandle(hPipe);
}
bool read_win32_slot(HANDLE hPipe, msg_pack &msg) {
	DWORD bytesRead;
	BOOL result = ReadFile(hPipe, &msg, sizeof(msg_pack), &bytesRead, NULL);
	return result && bytesRead == sizeof(msg_pack);
}
#else
#include <sys/msg.h>
int msg_descriptor_id = -1;
#endif

bool work = true;

std::string severity_to_string(int severity)
{
	switch (severity)
	{
		case 0:
			return "trace";
		case 1:
			return "debug";
		case 2:
			return "information";
		case 3:
			return "warning";
		case 4:
			return "error";
		case 5:
			return "critical";
		default:
			return "non";
	}
}

void for_close()
{
	msg_pack msg{};
	std::string cmd;

	while (std::cin >> cmd)
	{
		if (cmd == "Close")
		{
			msg.msg_status = CLOSE_MSG_Q;
			msgsnd(msg_descriptor_id, &msg, sizeof(msg_pack), 0);
			work = false;
			break;
		}
	}
}



int main()
{
	msg_pack msg {};
	std::unordered_map<std::string, std::ofstream> streams;
	size_t const max_msg_size = sizeof(msg_pack);


#ifdef _WIN32
	hPipe = CreateNamedPipe("\\\\.\\pipe\\mp_os_srvr_lgr", PIPE_ACCESS_INBOUND,
                            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                            1, 0, 0, 0, NULL);
    if (hPipe == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Failed to create named pipe");
    }
    std::atexit(cleanup);
#else
	msg_descriptor_id = msgget(MSG_KEY, IPC_CREAT | 0666);
	if (msg_descriptor_id == -1) {
		throw std::runtime_error("Failed to create message queue descriptor");
	}

#endif
	std::thread cmd_thread(for_close);
	while (work)
	{
#ifdef _WIN32
        if (!read_win32_slot(hPipe, msg)) {
            break;
        }
#else
		ssize_t status_code = msgrcv(msg_descriptor_id, &msg, max_msg_size, 0,
									 MSG_NOERROR); //0 - будет получено первое сообщение из очереди |
		if (status_code == -1) {
			break;
		}
#endif

		if (msg.msg_status == CLOSE_MSG_Q) { break; }

		if (streams.count(msg.file_path) == 0 && msg.msg_status != SEND_STATUS_ONLY_CONSOLE) {
			std::ofstream _stream = std::ofstream(msg.file_path, std::ios::app); //добавляем в конец
			if (!_stream.is_open()) {
				std::cout << "The file cannot be opened\n";
				break;
			}
			streams[msg.file_path] = std::move(_stream);
		}

		if (msg.packet_cnt == 1) // Если сообщение помещается в одном пакете, его можно обработать сразу
		{
			std::ofstream *log_stream_ptr = nullptr;
			if (msg.file_path[0]) log_stream_ptr = &streams[msg.file_path];
			std::string severity = severity_to_string(msg.severity);
			if (log_stream_ptr) {
				// Записываем сообщение в поток вывода
				*log_stream_ptr << "[" << severity << "] " << msg.msg_text << '\n';
				log_stream_ptr->flush(); // Сбрасываем буфер потока
			}
			if (msg.to_console) {
				std::cout << "[" << severity << "] " << msg.msg_text << '\n';
			}
		}
		else
		{
			// Если сообщение разбито на несколько пакетов, обрабатываем его частями
			std::string temp_file = "temp_" + std::to_string(msg.pid);
			std::ofstream &stream = streams[temp_file];

			if (msg.packet_id == 1) {
				// Если получен первый пакет, закрываем и открываем файл для записи
				stream.close();
				stream.open(temp_file,
							std::ios::out | std::ios::trunc); // Открываем файл для записи, удаляя предыдущее содержимое
			}

			stream << msg.msg_text;

			if (msg.packet_id == msg.packet_cnt) {
				// Если получен последний пакет, выводим содержимое временного файла
				stream.flush(); // Сбрасываем буфер потока

				std::ifstream crnt_stream(temp_file); // Открываем временный файл для чтения

				if (crnt_stream.is_open())
				{
					std::stringstream Message;
					std::ofstream *log_stream_ptr = nullptr;
					if (msg.file_path[0]) log_stream_ptr = &streams[msg.file_path];
					std::string severity = severity_to_string(msg.severity);
					if (log_stream_ptr)
					{
						char ch;
						while (crnt_stream >> ch) { Message << ch; }
						// Записываем сообщение в поток вывода
						*log_stream_ptr << "[" << severity << "] " << msg.msg_text << Message.str() <<'\n';
						if (msg.to_console)
						{
							std::cout << "[" << severity << "] " << msg.msg_text << Message.str() <<'\n';
						}
						log_stream_ptr->flush(); // Сбрасываем буфер потока
						crnt_stream.close(); // Закрываем файл после чтения
					}

				}
			}
		}
	}
	// закрытие потоков и освобождение ресурсов
	for (auto &_stream : streams)
	{
		_stream.second.flush();
		_stream.second.close();
	}

	#ifdef _WIN32
	CloseHandle(hPipe);
	#else
	msgctl(msg_descriptor_id, IPC_RMID, nullptr); //RemoveID
	#endif
	cmd_thread.detach();
	std::cout << "The server has finished its work" << '\n';

}
