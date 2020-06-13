// Proyecto numero 3 de windows desktop.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <chrono>
#include <iomanip>
#include<Windows.h>


//ostream operator << for time_point
//Function to print points in time
template <typename ClockType, typename Duration>
std::ostream &operator<<(std::ostream &output_stream, const std::chrono::time_point<ClockType, Duration> &time_point)
{
    auto ttime = ClockType::to_time_t(time_point);

    auto buffer = ctime(&ttime);
    buffer[24] = 0; //Remove newline that is added

    return output_stream << buffer;
}

//ostream operator << for durtation
//Function to print durations (time lapses) with nanosecond precision
template <typename Rep, typename Period>
std::ostream & operator <<(std::ostream &output_stream, const std::chrono::duration<Rep, Period> &timeSpan)
{
    auto hours = std::chrono::duration_cast<std::chrono::hours>(timeSpan);
	auto minutes = (std::chrono::duration_cast<std::chrono::minutes>(timeSpan) % std::chrono::hours{1});
	auto seconds = (std::chrono::duration_cast<std::chrono::seconds>(timeSpan) % std::chrono::minutes{1});
	auto nanos = timeSpan - hours - minutes - seconds;

    return output_stream << std::setw(2) << std::setfill('0') << hours.count() << ":" << std::setw(2) << minutes.count() << "." << std::setw(2) << seconds.count() << " + " << nanos.count() << " nanos";
}



int main()
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

    typedef int(__cdecl* _add)(int a, int b);
    typedef bool(__cdecl* saySomething)();

    auto pAdd = (_add)GetProcAddress(handle_to_dll, "_add");
    int c = pAdd(5, 7);

    auto pSaySomething = (saySomething)GetProcAddress(handle_to_dll, "saySomething");
    bool dlgres = pSaySomething();
    std::cin >> a;
}
