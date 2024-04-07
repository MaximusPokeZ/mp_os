#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_LOGGER_BUILDER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_LOGGER_BUILDER_H

#include "logger.h"

class logger_builder
{

public:

    virtual ~logger_builder() noexcept = default; // Виртуальный деструктор по умолчанию

public:

    // Чисто виртуальные функции для добавления различных потоков вывода логгера и настройки логгера
    virtual logger_builder *add_file_stream(
        std::string const &stream_file_path,
        logger::severity severity) = 0;

    virtual logger_builder *add_console_stream(
        logger::severity severity) = 0;

    virtual logger_builder* transform_with_configuration(
        std::string const &configuration_file_path,
        std::string const &configuration_path) = 0;

    virtual logger_builder *clear() = 0; // Чисто виртуальная функция для очистки настроек

    virtual logger_builder *set_format(const std::string &str) = 0;

    virtual logger *build() const = 0; // Чисто виртуальная функция для построения логгера

protected:

    // Статическая вспомогательная функция для преобразования строки в уровень серьёзности
    static logger::severity string_to_severity(
        std::string const &severity_string);

	static std::string severity_to_string(
		logger::severity severity);

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_LOGGER_BUILDER_H 
