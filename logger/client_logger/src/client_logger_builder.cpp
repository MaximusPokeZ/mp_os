//#include <not_implemented.h>
#include <filesystem>
#include <list>
#include <fstream>
#include <utility>

#include "../include/client_logger_builder.h"


client_logger_builder::client_logger_builder() : format_string("%m") {}

client_logger_builder::client_logger_builder(std::string str) :
		format_string(std::move(str)) {}

logger_builder *client_logger_builder::set_format(const std::string &str)
{
	format_string = str;
	return this;
}

logger_builder *client_logger_builder::clear()
{
	format_string = "%m";
	available_streams.clear();
	return this;
}

logger *client_logger_builder::build() const
{
	return new client_logger(available_streams, format_string);
}

logger_builder *client_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity)
{
    auto iter = available_streams.find(severity);
    if (iter == available_streams.end()) //если элемента с данным severity нет, то добавляем во все существующие потоки
    {
        auto insert = available_streams.emplace(severity, std::make_pair(std::list<std::pair<std::string, std::ofstream *>>(), false));
        if (!insert.second) //если вставка не произошла (итератор на вставленный эл, true/false)
        {
            throw std::runtime_error("Failed to insert element into unordered_map");
        }
        iter = insert.first;
    }

    iter->second.first.emplace_front(std::filesystem::weakly_canonical(stream_file_path).string(), nullptr); //добавляем новый поток ко всем потокам для данного severity
    return this;
}

logger_builder *client_logger_builder::add_console_stream(
    logger::severity severity)
{
	auto iter = available_streams.find(severity);
	if (iter == available_streams.end()) //если элемента с данным severity нет, то добавляем во все существующие потоки
	{
		auto insert = available_streams.emplace(severity, std::make_pair(std::list<std::pair<std::string, std::ofstream *>>(), true));
		if (!insert.second) //если вставка не произошла (итератор на вставленный эл, true/false)
		{
			throw std::runtime_error("Failed to insert element into unordered_map");
		}
		iter = insert.first;
	}

	iter->second.second = true; // Устанавливаем флаг в true, если severity уже существует

	return this;
}

void client_logger_builder::parsing_severity(logger::severity severity, nlohmann::json &js)
{
	// Проверяем, что данные не пусты и являются объектом JSON
	if (js.empty() || !js.is_object()) { return; }

	// Проверяем, существует ли уже указанный уровень серьезности в текущих потоках
	auto it = available_streams.find(severity);
	if (it == available_streams.end())
	{
		// Если не существует, добавляем новый уровень серьезности в потоки
		auto insert = available_streams.emplace(severity, std::make_pair(std::list<std::pair<std::string, std::ofstream *>>(), false));
		if (!insert.second)
		{
			throw std::runtime_error("Failed to insert element into unordered_map");
		}
		it = insert.first;
	}

	// Проверяем наличие настроек для вывода в консоль
	auto console_iter = js.find("console");
	if (console_iter != js.end() && console_iter->is_boolean())
	{
		it->second.second = console_iter->get<bool>(); // значение устанавливается для уровня логирования, на который указывает it
	}

	// Проверяем наличие путей к файлам для записи логов
	auto path_iter = js.find("path");
	if (path_iter != js.end() && path_iter->is_array())
	{
		nlohmann::json array = *path_iter;
		for (const nlohmann::json& obj : array)
		{
			if (!obj.is_string() || obj.empty()) { continue; }

			std::string stream_file_path = obj.get<std::string>();
			it->second.first.emplace_front(std::filesystem::weakly_canonical(stream_file_path).string(), nullptr);
		}
	}
}

logger_builder* client_logger_builder::transform_with_configuration(
		std::string const &configuration_file_path,
		std::string const &configuration_path)
{
	// Открываем файл с конфигурацией
	std::ifstream json_file(configuration_file_path);
	if (!json_file.is_open())
	{
		throw std::ios_base::failure("Failed to open file: " + configuration_file_path);
	}

	// Парсим JSON данные из файла
	nlohmann::json data = nlohmann::json::parse(json_file);
	json_file.close();

	// Находим указанный путь в конфигурационных данных
	auto iter = data.find(configuration_path);
	if (iter == data.end() || !iter->is_object())
	{
		// Если путь не найден, возвращаем указатель на текущий объект builder
		return this;
	}

	// Парсим данные для различных уровней серьезности сообщений
	for (auto severity : {logger::severity::information, logger::severity::warning, logger::severity::error, logger::severity::critical, logger::severity::debug, logger::severity::trace})
	{
		parsing_severity(severity, (*iter)[severity_to_string(severity)]);
	}

	// Проверяем и применяем формат, если он указан
	auto format = (*iter)["format"];
	if (!format.empty() && format.is_string())
	{
		format_string = format.get<std::string>();
	}

	// Возвращаем указатель на текущий объект builder
	return this;
}

