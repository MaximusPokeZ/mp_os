#include <not_implemented.h>
#include <nlohmann/json.hpp>
#include <fstream>

#include "../include/server_logger_builder.h"
#include "../include/server_logger.h"


logger_builder *server_logger_builder::set_format(const std::string &str)
{
	format_string = str;
	return this;
}

logger_builder *server_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity)
{
	std::string path = std::filesystem::weakly_canonical(stream_file_path).string();

	auto iter = available_streams.find(severity);
	if (iter == available_streams.end())
	{
		auto insert = available_streams.emplace(severity, std::make_pair(false, std::set<std::string>()));
		if(!insert.second)
		{
			throw std::runtime_error("Failed to insert element into unordered_map");
		}
		iter = insert.first;
	}
	iter->second.second.emplace(path);

	return this;
}

logger_builder *server_logger_builder::add_console_stream(
    logger::severity severity)
{
	auto iter = available_streams.find(severity);
	if (iter == available_streams.end())
	{
		auto insert = available_streams.emplace(severity, std::make_pair(true, std::set<std::string>()));
		if(!insert.second)
		{
			throw std::runtime_error("Failed to insert element into unordered_map");
		}
		iter = insert.first;
	}
	iter->second.first = true;

	return this;
}

void server_logger_builder::parsing_severity(logger::severity severity, nlohmann::json &js)
{
	// Проверяем, что данные не пусты и являются объектом JSON
	if (js.empty() || !js.is_object()) { return; }

	// Проверяем, существует ли уже указанный уровень серьезности в текущих потоках
	auto iter = available_streams.find(severity);
	if (iter == available_streams.end())
	{
		// Если не существует, добавляем новый уровень серьезности в потоки
		auto insert = available_streams.emplace(severity, std::make_pair(true, std::set<std::string>()));
		if(!insert.second)
		{
			throw std::runtime_error("Failed to insert element into unordered_map");
		}
		iter = insert.first;
	}

	// Проверяем наличие настроек для вывода в консоль
	auto console_iter = js.find("console");
	if (console_iter != js.end() && console_iter->is_boolean())
	{
		iter->second.first = console_iter->get<bool>(); // значение устанавливается для уровня логирования, на который указывает it
	}

	// Проверяем наличие путей к файлам для записи логов
	auto path_iter = js.find("path");
	if (path_iter != js.end() && path_iter->is_array())
	{
		// Итерируемся по массиву путей
		nlohmann::json array = *path_iter;
		for (const nlohmann::json& obj : array)
		{
			if (!obj.is_string() || obj.empty()) { continue; }

			// Получаем абсолютный путь к файлу и добавляем его в список для данного уровня серьезности
			std::string stream_file_path = obj.get<std::string>();
			iter->second.second.emplace(std::filesystem::weakly_canonical(stream_file_path).string());
		}
	}
}

logger_builder* server_logger_builder::transform_with_configuration(
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


logger_builder *server_logger_builder::clear()
{
	available_streams.clear();
	return this;
}

logger *server_logger_builder::build() const
{
	return new server_logger(available_streams);
}