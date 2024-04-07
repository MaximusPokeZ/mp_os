#include <gtest/gtest.h>

#include "../include/client_logger.h"
#include "../include/client_logger_builder.h"


int main(
    int argc,
    char *argv[])
{
   //testing::InitGoogleTest(&argc, argv);

	client_logger_builder builder;

	builder.add_console_stream(logger::severity::information)
			->add_file_stream("a.txt", logger::severity::information)
			->add_file_stream("a.txt", logger::severity::trace)
			->add_file_stream("b.txt", logger::severity::trace)
			->set_format("[%d %t][%s] %m");
	builder.transform_with_configuration("config.json", "log");
	std::unique_ptr<logger> log(builder.build());
	log->information("Hello");
	log->trace("dasdasdasd");
	log->warning("warning!!!!");
	log->error("puksrenk");



    //return RUN_ALL_TESTS();
}