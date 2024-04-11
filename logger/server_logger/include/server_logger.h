#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_H

#define MAX_MSG_SIZE 1024
#define MAX_PATH_SIZE 512

#include <logger.h>
#include "server_logger_builder.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <unordered_map>
#include <mutex>
#include <set>

class server_logger final:
    public logger
{
	friend class server_logger_builder;

private:
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

	std::string _format;
	std::unordered_map<severity, std::pair<bool, std::set<std::string>>> available_streams;

private:

	static std::mutex mutex;

#ifdef _WIN32
	HANDLE hPipe;
#else
	int msg_descriptor_id;
#endif

private:

	explicit server_logger(const std::unordered_map<severity, std::pair<bool, std::set<std::string>>> & streams);

public:

    server_logger(
        server_logger const &other) = default;

    server_logger &operator=(
        server_logger const &other) = default;

    server_logger(
        server_logger &&other) noexcept = default;

    server_logger &operator=(
        server_logger &&other) noexcept = default;

    ~server_logger() noexcept final = default;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_H