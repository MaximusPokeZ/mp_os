#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H

#include <logger_builder.h>
#include "client_logger.h"
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <list>

class client_logger_builder final:
    public logger_builder
{
private:

    std::string format_string;
    std::unordered_map<logger::severity, std::pair<std::list<std::pair<std::string, std::ofstream*>>, bool>> available_streams ; //key: severity  value: pair<list of pair<path, ptr for thi file>, exist or not>

    void parsing_severity(logger::severity severity, nlohmann::json& json);

public:

	client_logger_builder();

	explicit client_logger_builder(std::string format_string);

	client_logger_builder(
			client_logger_builder const &other) = default;

	client_logger_builder &operator=(
			client_logger_builder const &other) = default;

	client_logger_builder(
			client_logger_builder &&other) noexcept = default;

	client_logger_builder &operator=(
			client_logger_builder &&other) noexcept = default;

	~client_logger_builder() noexcept override = default;

public:

    logger_builder *add_file_stream(
            std::string const &stream_file_path,
            logger::severity severity) override;

    logger_builder *add_console_stream(
            logger::severity severity) override;

    logger_builder *transform_with_configuration(
            std::string const &configuration_file_path,
            std::string const &configuration_path) override;

    logger_builder *clear() override;

    [[nodiscard]] logger *build() const override;

    logger_builder* set_format(const std::string& str) override;


};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H