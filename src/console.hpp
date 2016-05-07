//
//  console.hpp
//  diplom
//
//  Created by Максим Кузин on 05.05.16.
//  Copyright © 2016 Максим Кузин. All rights reserved.
//

#ifndef console_hpp
#define console_hpp

#include <string>

class Console {
public:
    void ConsoleOut(std::string text);
    std::string return_date_time();
};

#endif /* console_hpp */
