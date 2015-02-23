#include <boost/program_options.hpp>
#include <boost/program_options/errors.hpp>
#include <iostream>

namespace opt = boost::program_options;

int main(int argc, char* argv[])
{
	opt::options_description desc("All options");

	desc.add_options()
		("apples,a", opt::value<int>()->default_value(10), "how many apples do you have")
		("oranges,o", opt::value<int>()->default_value(20), "how many oranges do you have")
		("name", opt::value<std::string>(), "your name")
		("help", "produce help message")
		;

	opt::variables_map vm;

	opt::store(opt::parse_command_line(argc, argv, desc), vm);

	opt::notify(vm);

	if (vm.count("help"))
	{
		std::cout << desc << "\n";
		return 1;
	}

	if (vm.count("name"))
	{
		std::cout << "Hi, " << vm["name"].as<std::string>() << "!\n";
	}

	// opt::value<int>()->default_value() 설정을 안했다면 이 부분에서 exception 뜸.
	std::cout << "Fruits count: "
		<< vm["apples"].as<int>() + vm["oranges"].as<int>()
		<< std::endl;

	return 0;
}