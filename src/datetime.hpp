/*                                                                                    
*  FInite State Automata library                                                     
*                                                                                    
*  Copyright (c) 2015, Jean Ahmad (https://www.linkedin.com/in/jeanahmad)                 
*  All rights reserved.                                                              
*                                                                                    
*  Redistribution and use in source and binary forms, with or without modification,  
*  are permitted provided that the following conditions are met:                     
*                                                                                    
*  - Redistributions of source code must retain the above copyright notice, this     
*  list of conditions and the following disclaimer.                                  
*                                                                                    
*  - Redistributions in binary form must reproduce the above copyright notice, this  
*  list of conditions and the following disclaimer in the documentation and/or       
*  other materials provided with the distribution.                                   
*                                                                                    
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND   
*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED     
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE            
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR  
*  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES    
*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;      
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON    
*  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT           
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS     
*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                      
*/                                                                                  

#ifndef DATETIME_H
#define DATETIME_H

#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

#include <iostream>

#ifdef GCCTIME_SUPPORT
#include <sys/time.h>
#endif

namespace fisa
{

  //#########################################################################################################
  /*
    DateTime
  */
  //! Class to manage date and time.
  /**
   * To work properly, input parameters constructors must match following conditions:
   * - 1 <= in_month <= 12,
   * - 1 <= in_day_of_year <= (365 or 366),
   * - 1 <= in_day <= (28, 29, 30 or 31),
   * - 0 <= in_hour <= 23,
   * - 0 <= in_minute <= 59,
   * - 0 <= in_second <= 59,
   * - 0 <= in_usecond <= 999999.
   **/
  
  class DateTime
  {
  public:
    //! Constructor
    /** 
     * Construct an object initialized with:
     * day = 1970,
     * month = 1,
     * day_of_month = 1,
     * hour = 0,
     * minute = 0,
     * second = 0,
     * usecond = 0.
    **/
    DateTime();

    //! Constructor
    /** Specify the date with the day of month. **/
    DateTime(int in_year, int in_month, int in_day, int in_hour, int in_minute, int in_second, int usecond);

    //! Constructor
    /** Specify the date with the day of year. **/
    DateTime(int in_year, int in_day_of_year, int in_hour, int in_minute, int in_second, int usecond);
    
    //! Constructor
    /** Construct an object from an ISO 8060 string (2010-02-10T12:53:00). **/
    /** Microseconds are set to 0. **/
    DateTime(const char *in_datetime);

#ifdef GCCTIME_SUPPORT
    DateTime(timeval &in_tv);
#endif
    
  
    //! \private
    ~DateTime();

    DateTime operator + (const DateTime &in_datetime);
    bool operator == (const DateTime &in_datetime);
    bool operator != (const DateTime &in_datetime);
    bool operator < (const DateTime &in_datetime);
    bool operator <= (const DateTime &in_datetime);
    bool operator > (const DateTime &in_datetime);
    bool operator >= (const DateTime &in_datetime);

    bool isLeapYear();

    int year();
    int dayOfYear();
    int month();    
    int dayOfMonth();
    int hour();
    int minute();
    int second();
    int usecond();

    std::string yearString();
    std::string dayOfYearString();
    std::string monthString();
    std::string monthName();
    std::string dayOfMonthString();
    std::string hourString();
    std::string minuteString();
    std::string secondString();
    std::string usecondString();

    std::string toIso8061();
    unsigned long int toSeconds();
  
  private:
    void computeDayOfMonth();
    void computeDayOfYear();
    void computeMonthName();
    
    int _year;
    int _month;
    std::string _smonth;
    int _dayOfYear;
    int _dayOfMonth;
    int _hour;
    int _minute;
    int _second;
    int _usecond;
  };
}
  
#endif /* DATETIME_H */
