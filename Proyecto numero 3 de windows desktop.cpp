// Proyecto numero 3 de windows desktop.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <chrono>
#include <iomanip>
#include <iostream>


//ostream operator << for time_point
//Function to print points in time
template<typename ClockType, typename Duration>
std::ostream&
operator<<(std::ostream& output_stream, const std::chrono::time_point<ClockType, Duration>& time_point)
{
	auto ttime = ClockType::to_time_t(time_point);

	auto buffer = ctime(&ttime);
	buffer[24] = 0; //Remove newline that is added

	return output_stream << buffer;
}

//ostream operator << for durtation
//Function to print durations (time lapses) with nanosecond precision
template<typename Rep, typename Period>
std::ostream&
operator<<(std::ostream& output_stream, const std::chrono::duration<Rep, Period>& timeSpan)
{
	auto hours = std::chrono::duration_cast<std::chrono::hours>(timeSpan);
	auto minutes = (std::chrono::duration_cast<std::chrono::minutes>(timeSpan) % std::chrono::hours{1});
	auto seconds = (std::chrono::duration_cast<std::chrono::seconds>(timeSpan) % std::chrono::minutes{1});
	auto nanos = timeSpan - hours - minutes - seconds;

	return output_stream << std::setw(2) << std::setfill('0') << hours.count() << ":" << std::setw(2) << minutes.count() << "." << std::setw(2) << seconds.count() << " + " << nanos.count() << " nanos";
}



int
main()
{
	int a;

	auto startTime = std::chrono::high_resolution_clock::now();

	std::cout << "Hello World!\n";
	std::cout << "Another Hello World! :)\n";

	//Used ostream operator << on a time_point
	std::cout << std::chrono::system_clock::now() << "\n";

	auto endTime = std::chrono::high_resolution_clock::now();

	//substracting 2 time points yields a duration.
	//Used ostream operator << on a duration
	std::cout << "Execution of program took about " << (endTime - startTime) << "\n";

	std::cout << "\n";


	HMODULE handle_to_dll = LoadLibrary(L"lib.dll");
	if (handle_to_dll == NULL) //Library not loaded
	{
		auto error_code = GetLastError();

        /*
            *****
            I recommend you to set a breakpoint here as well as in the other error handling sections
            and add a watch for the variable erro_code.
            Add it like shown in the line below, so that you can get a more descriptive error text instead of just an error number.
                error_code,hr
            The ",hr" at the end tells Visual Studio to treat it as an error code and it provides the corresponding text for you.

            Otherwise, the second best way to know what a specific error code means is to google it
        */

		std::cerr << "LoadLibrary failed with error code: " << error_code << "\n";

		std::cin >> a;
		return 0;
	}

	typedef int(__cdecl * _add)(int a, int b); //This defines _add as the type of a pointer to a function with the specified calling convention (__cdecl) and arguments
	typedef bool(__cdecl * saySomething)();	   //This defines saySomething as the type of a pointer to a function returning a bool and taking no arguments, with __cdecl (C calling convention)

	auto pAdd = (_add)GetProcAddress(handle_to_dll, "_add");
	if (pAdd == NULL) //Failed to get function address (maybe wrong spelling?)
	{
        auto error_code = GetLastError();
		std::cerr << "GetProcAddress failed with error code: " << error_code << "\n";

		std::cin >> a;
		return 0;
	}

	int c = pAdd(5, 7); //Call function using a function pointer (pAdd is a pointer)

	auto pSaySomething = (saySomething)GetProcAddress(handle_to_dll, "saySomething");
    if (pSaySomething == NULL) //Failed to get function address (maybe wrong spelling?)
	{
        auto error_code = GetLastError();
		std::cerr << "GetProcAddress failed with error code: " << error_code << "\n";

		std::cin >> a;
		return 0;
	}

	bool dlgres = pSaySomething(); //Call function using a function pointer (pSaySomething is a pointer)
	std::cin >> a;
}
