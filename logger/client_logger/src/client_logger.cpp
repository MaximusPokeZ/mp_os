#include <sstream>
#include <utility>

#include "../include/client_logger.h"

std::unordered_map<std::string, std::pair<size_t, std::ofstream>> client_logger::all_streams;

client_logger::client_logger(
		const std::unordered_map<logger::severity, std::pair<std::list<std::pair<std::string, std::ofstream *>>, bool>> &current_streams,
		std::string format)
		: _format(std::move(format))
{
	for (const auto &[severity, streams] : current_streams)
	{
		for (const auto &[path, file_stream] : streams.first)
		{
			// Проверяем, открыт ли уже поток для данного файла
			auto iter = all_streams.find(path);
			if (iter != all_streams.end()) {
				// Если поток уже открыт, используем его
				available_streams[severity].first.emplace_back(path, &iter->second.second);
				// Увеличиваем счетчик использования потока
				iter->second.first++;
			}
			else
			{
				// Если поток еще не открыт, открываем его
				auto insert = all_streams.emplace(path, std::make_pair(1, std::ofstream(path)));
				if (!insert.second || !insert.first->second.second)
				{
					if(!insert.second)
					{
						all_streams.erase(insert.first);
					}
					throw std::runtime_error("Failed to open file: " + path);
				}
				available_streams[severity].first.emplace_back(path, &insert.first->second.second);
			}
		}
		// Устанавливаем флаги для уровня серьезности
		available_streams[severity].second = streams.second;
	}
}


client_logger::client_logger(const client_logger &other)
		: _format(other._format)
{
	for (const auto &[severity, streams] : other.available_streams)
	{
		for (const auto &[path, file_stream] : streams.first)
		{
			// Проверяем, открыт ли уже поток для данного файла
			auto iter = all_streams.find(path);
			if (iter != all_streams.end())
			{
				// Если поток уже открыт, используем его
				available_streams[severity].first.emplace_back(path, &iter->second.second);
				// Увеличиваем счетчик использования потока
				iter->second.first++;
			}
			else
			{
				// Если поток еще не открыт, открываем его
				auto *new_file_stream = new std::ofstream(path);
				if (!new_file_stream->is_open())
				{
					throw std::runtime_error("Failed to open file: " + path);
				}
				// Записываем в available_streams новый поток для данного файла
				available_streams[severity].first.emplace_back(path, new_file_stream);
				// Записываем открытый поток в all_streams и устанавливаем счетчик использования в 1
				all_streams.emplace(path, std::make_pair(1, std::move(*new_file_stream)));
			}
		}
		// Устанавливаем флаги для уровня серьезности
		available_streams[severity].second = streams.second;
	}
}


client_logger &client_logger::operator=(const client_logger &other)
{
	if (this != &other) {
		// Освобождаем ресурсы текущего объекта
		for (auto &[_, file_stream] : all_streams)
		{
			file_stream.second.close(); // Закрываем все потоки
		}
		all_streams.clear(); // Очищаем список потоков

		// Копируем данные из объекта other
		_format = other._format;
		available_streams = other.available_streams;

		// Копируем потоки из объекта other
		for (const auto &[severity, streams] : other.available_streams)
		{
			for (const auto &[path, file_stream] : streams.first)
			{
				// Открываем потоки для каждого файла, если они еще не открыты
				auto iter = all_streams.find(path);
				if (iter != all_streams.end())
				{
					iter->second.first++;
				}
				else {
					auto *new_file_stream = new std::ofstream(path);
					if (!new_file_stream->is_open()) {
						throw std::runtime_error("Failed to open file: " + path);
					}
					all_streams.emplace(path, std::make_pair(1, std::move(*new_file_stream)));
				}
			}
		}
	}
	return *this;
}



client_logger::client_logger(client_logger&& other) noexcept :
		_format(std::move(other._format)),
		available_streams(std::move(other.available_streams)) {}

client_logger& client_logger::operator=(client_logger&& other) noexcept {
	if (this != &other) {
		// Освобождаем ресурсы текущего объекта
		for (auto &[severity, streams] : available_streams) {
			for (auto &[path, file_stream] : streams.first) {
				if (all_streams[path].first == 1)
				{
					// Закрываем поток, если счетчик ссылок станет нулевым
					file_stream->close();
					delete file_stream;
				}
				all_streams[path].first--;
			}
		}

		// Перемещаем данные из другого объекта
		_format = std::move(other._format);
		available_streams = std::move(other.available_streams);

		// Очищаем ресурсы у другого объекта
		other.available_streams.clear();
	}
	return *this;
}


client_logger::~client_logger() noexcept
{
	// Перебираем все уровни серьезности
	for (auto &[severity, streams] : available_streams) {
		// Перебираем все потоки для текущего уровня серьезности
		for (auto &[path, file_stream] : streams.first) {
			// Уменьшаем счетчик ссылок на данный поток
			if (--all_streams[path].first == 0) {
				// Если счетчик ссылок стал равен нулю, закрываем поток
				file_stream->close();
				//delete file_stream; // Освобождаем память, выделенную для потока
			}
		}
	}
}



std::string client_logger::make_format(const std::string &message, logger::severity severity) const
{

    std::stringstream result_format;
    for(auto iter = _format.begin(), end = _format.end(); iter != end; ++iter)
    {
        flags flag = flags::NOTHING;

        if(*iter == '%')
        {
            flag = char_to_flag(*(iter+1));
        }
        if(flag != flags::NOTHING)
        {
            switch(flag)
            {
                case flags::MESSAGE:
                    result_format << message;
                    break;
                case flags::DATE:
                    result_format << current_date_to_string();
                    break;
                case flags::TIME:
                    result_format << current_time_to_string();
                    break;
                default:
                    result_format << severity_to_string(severity);
                    break;
            }

            ++iter;
        }
        else
        {
            result_format << *iter;
        }
    }
    return result_format.str();
}

logger const *client_logger::log(
        const std::string &text,
        logger::severity severity) const noexcept
{
    auto iter = available_streams.find(severity);

    std::string result_str_log = make_format(text, severity);

    if (iter != available_streams.end())
    {
        if (iter->second.second) { std::cout << result_str_log << '\n'; } //если определен поток вывода
        for (auto &elem : iter->second.first)
        {
            *(elem.second) << result_str_log << '\n'; // записываем во все потоки данной severity
        }
    }
    return this;
}

client_logger::flags client_logger::char_to_flag(char c) noexcept
{
    switch (c)
    {
        case 't': return flags::TIME;
        case 'd': return flags::DATE;
        case 's': return flags::SEVERITY;
        case 'm': return flags::MESSAGE;
        default:
            return flags::NOTHING;

    }
}