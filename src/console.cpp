//
//  console.cpp
//  diplom
//
//  Created by Максим Кузин on 05.05.16.
//  Copyright © 2016 Максим Кузин. All rights reserved.
//

#include "console.hpp"

#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>

void Console::ConsoleOut(std::string text) {
    std::cout << return_date_time() << " " << text << std::endl;
}

std::string Console::return_date_time() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return "[" + ss.str() + "]";
}