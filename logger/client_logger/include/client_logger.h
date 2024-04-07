#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H

#include <logger.h>
#include <unordered_map>
#include <list>
#include <fstream>

class client_logger final: public logger
{

private:
    friend class  client_logger_builder;

    std::string _format;
    static std::unordered_map<std::string, std::pair<size_t, std::ofstream>> all_streams; //key: path  value: pair<number of occurrences ptr, this ptr>
    std::unordered_map<logger::severity, std::pair<std::list<std::pair<std::string, std::ofstream*>>, bool>> available_streams ; //key: severity  value: pair<list of pair<path, ptr for thi file>, exist or not>

public:

    client_logger(
            const std::unordered_map<logger::severity, std::pair<std::list<std::pair<std::string, std::ofstream *>>, bool>> &current_streams,
            std::string  format);

	client_logger(
			client_logger const &other);

	client_logger &operator=(
			client_logger const &other);

	client_logger(
			client_logger &&other) noexcept;

	client_logger &operator=(
			client_logger &&other) noexcept;

	~client_logger() noexcept;

    enum class flags
    {
        DATE,
        TIME,
        MESSAGE,
        SEVERITY,
        NOTHING
    };

private:

    static flags char_to_flag(char c) noexcept;

    std::string make_format(const std::string& message, logger::severity severity) const;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H