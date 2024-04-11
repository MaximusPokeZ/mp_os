#include <gtest/gtest.h>
#include <fstream>
#include <server_logger_builder.h>
#include <server_logger.h>



int main(
    int argc,
    char *argv[])
{
    //testing::InitGoogleTest(&argc, argv);

	server_logger_builder builder;

	builder.add_file_stream("b.txt", logger::severity::error)
	->add_file_stream("a.txt", logger::severity::error)
	->add_console_stream(logger::severity::information);

	std::unique_ptr<logger> log(builder.build());
	log->error("dasdasdasd");
	log->information("info");



    //return RUN_ALL_TESTS();
}