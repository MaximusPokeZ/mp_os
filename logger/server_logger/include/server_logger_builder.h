#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_BUILDER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_BUILDER_H

#include <logger_builder.h>
#include <unordered_map>
#include <set>
#include "nlohmann/json.hpp"

class server_logger_builder final:
    public logger_builder
{

private:

	std::unordered_map<logger::severity, std::pair<bool, std::set<std::string>>> available_streams;
	std::string format_string;
	void parsing_severity(logger::severity severity, nlohmann::json& json);

public:

    server_logger_builder() = default;

    server_logger_builder(
        server_logger_builder const &other) = default;

    server_logger_builder &operator=(
        server_logger_builder const &other) = default;

    server_logger_builder(
        server_logger_builder &&other) noexcept = default;

    server_logger_builder &operator=(
        server_logger_builder &&other) noexcept = default;

    ~server_logger_builder() noexcept override = default;

public:

    logger_builder *add_file_stream(
        std::string const &stream_file_path,
        logger::severity severity) override;

    logger_builder *add_console_stream(
        logger::severity severity) override;

    logger_builder* transform_with_configuration(
        std::string const &configuration_file_path,
        std::string const &configuration_path) override;

    logger_builder *clear() override;

    [[nodiscard]] logger *build() const override;

	logger_builder* set_format(const std::string& str) override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_BUILDER_H