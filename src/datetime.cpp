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

#include "datetime.hpp"

using namespace fisa;

//#########################################################################################################
/*
  OpenSourceTime
*/

#ifdef OPENSOURCE_PLATFORM_TIME
// ---------------------------------------------------------------------------------------------------------------
DateTime OpenSourceTime::now()
{
  timeval now;
  gettimeofday(&now, NULL);
  return DateTime(now);
}
#endif

//#########################################################################################################
/*
WindowsTime
*/

#ifdef WINDOWS_PLATFORM_TIME
// ---------------------------------------------------------------------------------------------------------------
DateTime WindowsTime::now()
{
	SYSTEMTIME now;
	GetSystemTime(&now);
	return DateTime(now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond, now.wMilliseconds * 1000);
}
#endif

//#########################################################################################################
/*
  DateTime
*/

// ---------------------------------------------------------------------------------------------------------------
DateTime::DateTime() : _year(1970), _month(1), _dayOfMonth(1), _hour(0), _minute(0), _second(0), _usecond(0)
{
  computeDayOfYear();
  computeMonthName();
}

// ---------------------------------------------------------------------------------------------------------------
DateTime::DateTime(int in_year, int in_month, int in_day, int in_hour, int in_minute, int in_second, int in_usecond)
  : _year(in_year), _month(in_month), _dayOfMonth(in_day), _hour(in_hour), _minute(in_minute), _second(in_second),
    _usecond(in_usecond)
{
  computeDayOfYear();
  computeMonthName();
}

// ---------------------------------------------------------------------------------------------------------------
DateTime::DateTime(int in_year, int in_day_of_year, int in_hour, int in_minute, int in_second, int in_usecond)
  : _year(in_year), _dayOfYear(in_day_of_year), _hour(in_hour), _minute(in_minute), _second(in_second), _usecond(in_usecond)
{
  computeDayOfMonth();
  computeMonthName();
}

// ---------------------------------------------------------------------------------------------------------------
DateTime::DateTime(const char *in_datetime)
{
  std::string str(in_datetime);
  
  std::istringstream strstream0(str.substr(0, 4)); 
  strstream0 >> std::dec >> this->_year;

  std::istringstream strstream1(str.substr(5, 7));
  strstream1 >> std::dec >> this->_month;

  std::istringstream strstream2(str.substr(8, 10));
  strstream2 >> std::dec >> this->_dayOfMonth;

  std::istringstream strstream3(str.substr(11, 13));
  strstream3 >> std::dec >> this->_hour;

  std::istringstream strstream4(str.substr(14, 16));
  strstream4 >> std::dec >> this->_minute;

  std::istringstream strstream5(str.substr(17, 19));
  strstream5 >> std::dec >> this->_second;

  this->_usecond = 0;

  computeDayOfYear();
  computeMonthName();
}

#ifdef OPENSOURCE_PLATFORM_TIME
// ---------------------------------------------------------------------------------------------------------------
DateTime::DateTime(timeval &in_tv)
{
  *this = DateTime(1970, 1, 0, 0, 0, in_tv.tv_usec);
  
  long int second = in_tv.tv_sec;
  while (second > 59)
    {
      if (!isLeapYear())
	{
	  if (second > 365 * 24 * 60 * 60)
	    {
	      *this = *this + DateTime(1, 0, 0, 0, 0, 0);
	      second -= 365 * 24 * 60 * 60;
	    }
	  else if (second > 24 * 60 * 60)
	    {
	      *this = *this + DateTime(0, 1, 0, 0, 0, 0);
	      second -= 24 * 60 * 60;
	    }
	  else if (second > 60 * 60)
	    {
	      *this = *this + DateTime(0, 0, 1, 0, 0, 0);
	      second -= 60 * 60;
	    }
	  else
	    {
	      *this = *this + DateTime(0, 0, 0, 1, 0, 0);
	      second -= 60;
	    }
	}
      else
	{
	  if (second > 366 * 24 * 60 * 60)
	    {
	      *this = *this + DateTime(1, 0, 0, 0, 0, 0);
	      second -= 366 * 24 * 60 * 60;
	    }
	  else if (second > 24 * 60 * 60)
	    {
	      *this = *this + DateTime(0, 1, 0, 0, 0, 0);
	      second -= 24 * 60 * 60;
	    }
	  else if (second > 60 * 60)
	    {
	      *this = *this + DateTime(0, 0, 1, 0, 0, 0);
	      second -= 60 * 60;
	    }
	  else
	    {
	      *this = *this + DateTime(0, 0, 0, 1, 0, 0);
	      second -= 60;
	    }
	}
    }    
  this->_second = second;
  
  computeDayOfMonth();
  computeMonthName();
}
#endif

// ---------------------------------------------------------------------------------------------------------------
DateTime::DateTime(const DateTime &in_datetime)
{
  this->_year = in_datetime._year;
  this->_month = in_datetime._month;
  this->_smonth = std::make_shared<std::string>(*(in_datetime._smonth));
  this->_dayOfYear = in_datetime._dayOfYear;
  this->_dayOfMonth = in_datetime._dayOfMonth;
  this->_hour = in_datetime._hour;
  this->_minute = in_datetime._minute;
  this->_second = in_datetime._second;
  this->_usecond = in_datetime._usecond;
}

// ---------------------------------------------------------------------------------------------------------------
DateTime::~DateTime() {}

// ---------------------------------------------------------------------------------------------------------------
DateTime& DateTime::operator = (const DateTime &in_datetime)
{
  this->_year = in_datetime._year;
  this->_month = in_datetime._month;
  this->_smonth = std::make_shared<std::string>(*(in_datetime._smonth));
  this->_dayOfYear = in_datetime._dayOfYear;
  this->_dayOfMonth = in_datetime._dayOfMonth;
  this->_hour = in_datetime._hour;
  this->_minute = in_datetime._minute;
  this->_second = in_datetime._second;
  this->_usecond = in_datetime._usecond;

  return *this;
}

// ---------------------------------------------------------------------------------------------------------------
bool DateTime::operator == (const DateTime &in_datetime) 
{
  if ((this->_year == in_datetime._year) && (this->_dayOfYear == in_datetime._dayOfYear) &&
      (this->_hour == in_datetime._hour) && (this->_minute == in_datetime._minute) &&
      (this->_second == in_datetime._second) && (this->_usecond == in_datetime._usecond)) return true;
  else return false;
}

// ---------------------------------------------------------------------------------------------------------------
bool DateTime::operator != (const DateTime &in_datetime)
{
  return !(*this == in_datetime);
}

// ---------------------------------------------------------------------------------------------------------------
DateTime DateTime::operator + (const DateTime &in_datetime) 
{
  int out_usecond = 0;
  int out_second = 0;
  int out_minute = 0;
  int out_hour = 0;
  int out_day_of_year = 0;
  int out_year = 0;

  if (this->_usecond + in_datetime._usecond <= 999999) {out_usecond = this->_usecond + in_datetime._usecond;}
  else {
  out_usecond = this->_usecond + in_datetime._usecond - 1000000;
  out_second++;}
  
  if (this->_second + in_datetime._second <= 59) {out_second = this->_second + in_datetime._second;}
  else {
    out_second = this->_second + in_datetime._second - 60;
    out_minute++;}

  if (this->_minute + in_datetime._minute + out_minute <= 59) {out_minute = this->_minute + in_datetime._minute + out_minute;}
  else {
    out_minute = this->_minute + in_datetime._minute + out_minute - 60;
    out_hour++;}

  if (this->_hour + in_datetime._hour + out_hour <= 23) {out_hour = this->_hour + in_datetime._hour + out_hour;}
  else {
    out_hour = this->_hour + in_datetime._hour + out_hour - 24;
    out_day_of_year++;}

  if (!isLeapYear()) {
    if (this->_dayOfYear + in_datetime._dayOfYear + out_day_of_year <= 365) {out_day_of_year = this->_dayOfYear +
	in_datetime._dayOfYear + out_day_of_year;}
    else {
      out_day_of_year = this->_dayOfYear + in_datetime._dayOfYear + out_day_of_year - 365;
      out_year++;}}
  else {
    if (this->_dayOfYear + in_datetime._dayOfYear + out_day_of_year <= 366) {out_day_of_year = this->_dayOfYear +
	in_datetime._dayOfYear + out_day_of_year;}
    else {
      out_day_of_year = this->_dayOfYear + in_datetime._dayOfYear + out_day_of_year - 366;
      out_year++;}}
  out_year += this->_year + in_datetime._year;

  return DateTime(out_year, out_day_of_year, out_hour, out_minute, out_second, out_usecond);
}

// ---------------------------------------------------------------------------------------------------------------
bool DateTime::operator < (const DateTime &in_datetime) 
{
  if (this->_year < in_datetime._year) {return true;}
  if ((this->_year == in_datetime._year) && (this->_dayOfYear < in_datetime._dayOfYear)) {return true;}
  if ((this->_year == in_datetime._year) && (this->_dayOfYear == in_datetime._dayOfYear) &&
      (this->_hour < in_datetime._hour)) {return true;}
  if ((this->_year == in_datetime._year) && (this->_dayOfYear == in_datetime._dayOfYear) &&
      (this->_hour == in_datetime._hour) && (this->_minute < in_datetime._minute)) {return true;}
  if ((this->_year == in_datetime._year) && (this->_dayOfYear == in_datetime._dayOfYear) &&
      (this->_hour == in_datetime._hour) && (this->_minute == in_datetime._minute) &&
      (this->_second < in_datetime._second)) {return true;}
  if ((this->_year == in_datetime._year) && (this->_dayOfYear == in_datetime._dayOfYear) &&
      (this->_hour == in_datetime._hour) && (this->_minute == in_datetime._minute) &&
      (this->_second == in_datetime._second) && (this->_usecond < in_datetime._usecond)) {return true;}
  return false;
}

// ---------------------------------------------------------------------------------------------------------------
bool DateTime::operator <= (const DateTime &in_datetime) 
{
  return ((*this < in_datetime) || (*this == in_datetime));
}

// ---------------------------------------------------------------------------------------------------------------
bool DateTime::operator > (const DateTime &in_datetime) 
{
  return !((*this < in_datetime) || (*this == in_datetime));
}

// ---------------------------------------------------------------------------------------------------------------
bool DateTime::operator >= (const DateTime &in_datetime) 
{
  return !(*this < in_datetime);
}

// ---------------------------------------------------------------------------------------------------------------
bool DateTime::isLeapYear() 
{
  if ((double) this->_year / 400. == std::floor((double) this->_year / 400.)) {
    return true;}
  else if ((double) this->_year / 100. == std::floor((double) this->_year / 100.)) {
    return false;}
  else if ((double) this->_year / 4. == std::floor((double) this->_year / 4.)) {
    return true;}
  else {
    return false;}
}

// ---------------------------------------------------------------------------------------------------------------
int DateTime::year() {return this->_year;}

// ---------------------------------------------------------------------------------------------------------------
int DateTime::dayOfYear() {return this->_dayOfYear;}

// ---------------------------------------------------------------------------------------------------------------
int DateTime::month() {return this->_month;}

// ---------------------------------------------------------------------------------------------------------------
int DateTime::dayOfMonth() {return this->_month;}

// ---------------------------------------------------------------------------------------------------------------
int DateTime::hour() {return this->_hour;}

// ---------------------------------------------------------------------------------------------------------------
int DateTime::minute() {return this->_minute;}

// ---------------------------------------------------------------------------------------------------------------
int DateTime::second() {return this->_second;}

// ---------------------------------------------------------------------------------------------------------------
int DateTime::usecond() {return this->_usecond;}

// ---------------------------------------------------------------------------------------------------------------
std::string DateTime::yearString() 
{
  std::ostringstream out;
  out << std::dec << std::setw(4) << std::setfill('0') << this->_year;

  return std::string(out.str(), 0, 4);
}

// ---------------------------------------------------------------------------------------------------------------
std::string DateTime::dayOfYearString() 
{
  std::ostringstream out;
  out << std::dec << std::setw(2) << std::setfill('0') << this->_dayOfYear;

  return std::string(out.str(), 0, 2);
}

// ---------------------------------------------------------------------------------------------------------------
std::string DateTime::monthString() 
{
  std::ostringstream out;
  out << std::dec << std::setw(2) << std::setfill('0') << this->_month;

  return std::string(out.str(), 0, 2);
}

// ---------------------------------------------------------------------------------------------------------------
std::string DateTime::monthName() {return *this->_smonth;}

// ---------------------------------------------------------------------------------------------------------------
std::string DateTime::dayOfMonthString() 
{
  std::ostringstream out;
  out << std::dec << std::setw(2) << std::setfill('0') << this->_dayOfMonth;

  return std::string(out.str(), 0, 2);
}

// ---------------------------------------------------------------------------------------------------------------
std::string DateTime::hourString() 
{
  std::ostringstream out;
  out << std::dec << std::setw(2) << std::setfill('0') << this->_hour;

  return std::string(out.str(), 0, 2);
}

// ---------------------------------------------------------------------------------------------------------------
std::string DateTime::minuteString() 
{
  std::ostringstream out;
  out << std::dec << std::setw(2) << std::setfill('0') << this->_minute;

  return std::string(out.str(), 0, 2);
}

// ---------------------------------------------------------------------------------------------------------------
std::string DateTime::secondString() {
  std::ostringstream out;
  out << std::dec << std::setw(2) << std::setfill('0') << this->_second;

  return std::string(out.str(), 0, 2);
}

// ---------------------------------------------------------------------------------------------------------------
std::string DateTime::usecondString() {
  std::ostringstream out;
  out << std::dec << std::setw(6) << std::setfill('0') << this->_usecond;

  return std::string(out.str(), 0, 6);
}

// ---------------------------------------------------------------------------------------------------------------
std::string DateTime::toIso8601() 
{
  return this->yearString() + "-" + this->monthString() + "-" + this->dayOfMonthString() + "T" +
    this->hourString() + ":" + this->minuteString() + ":" + this->secondString();
}

// ---------------------------------------------------------------------------------------------------------------
unsigned long int DateTime::toSeconds() 
{
  unsigned long int result = 0;
  if (isLeapYear()) {
    result = this->_year * 366 * 24 * 60 * 60 + this->_dayOfYear * 24 * 60 * 60 +
      this->_hour * 60 * 60 + this->_minute * 60 + this->_second;}
  else {
    result = this->_year * 365 * 24 * 60 * 60 + this->_dayOfYear * 24 * 60 * 60 +
      this->_hour * 60 * 60 + this->_minute * 60 + this->_second;}

  return result;
}

// ---------------------------------------------------------------------------------------------------------------
void DateTime::computeDayOfMonth() 
{
  // 30 days for September, April, June and November
  // All the rest have 31,
  // Excepting February alone (And that has 28 days clear, with 29 in each leap year)

  int february_days = 28;
  if (isLeapYear()) february_days = 29;

  if (this->_dayOfYear <= 31) {
    this->_dayOfMonth = this->_dayOfYear;
    this->_month = 1;
    return;}
  if (this->_dayOfYear <= 31 + february_days) {
    this->_dayOfMonth = this->_dayOfYear - 31;
    this->_month = 2;
    return;}
  if (this->_dayOfYear <= 31 + february_days + 31) {
    this->_dayOfMonth = this->_dayOfYear - 31 - february_days;
    this->_month = 3;
    return;}
  if (this->_dayOfYear <= 31 + february_days + 31 + 30) {
    this->_dayOfMonth = this->_dayOfYear - 31 - february_days - 31;
    this->_month = 4;
    return;}
  if (this->_dayOfYear <= 31 + february_days + 31 + 30 + 31) {
    this->_dayOfMonth = this->_dayOfYear - 31 - february_days - 31 - 30;
    this->_month = 5;
    return;}
  if (this->_dayOfYear <= 31 + february_days + 31 + 30 + 31 + 30) {
    this->_dayOfMonth = this->_dayOfYear - 31 - february_days - 31 - 30 - 31;
    this->_month = 6;
    return;}
  if (this->_dayOfYear <= 31 + february_days + 31 + 30 + 31 + 30 + 31) {
    this->_dayOfMonth = this->_dayOfYear - 31 - february_days - 31 - 30 - 31 - 30;
    this->_month = 7;
    return;}
  if (this->_dayOfYear <= 31 + february_days + 31 + 30 + 31 + 30 + 31 + 31) {
    this->_dayOfMonth = this->_dayOfYear - 31 - february_days - 31 - 30 - 31 - 30 - 31;
    this->_month = 8;
    return;}
  if (this->_dayOfYear <= 31 + february_days + 31 + 30 + 31 + 30 + 31 + 31 + 30) {
    this->_dayOfMonth = this->_dayOfYear - 31 - february_days - 31 - 30 - 31 - 30 - 31 - 31;
    this->_month = 9;
    return;}
  if (this->_dayOfYear <= 31 + february_days + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31) {
    this->_dayOfMonth = this->_dayOfYear - 31 - february_days - 31 - 30 - 31 - 30 - 31 - 31 - 30;
    this->_month = 10;
    return;}
  if (this->_dayOfYear <= 31 + february_days + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30) {
    this->_dayOfMonth = this->_dayOfYear - 31 - february_days - 31 - 30 - 31 - 30 - 31 - 31 - 30 - 31;
    this->_month = 11;
    return;}
  this->_dayOfMonth = this->_dayOfYear - 31 - february_days - 31 - 30 - 31 - 30 - 31 - 31 - 30 - 31 - 30;
  this->_month = 12;
}

// ---------------------------------------------------------------------------------------------------------------
void DateTime::computeDayOfYear() 
{
  int february_days = 28;
  if (isLeapYear()) {february_days = 29;}

  this->_dayOfYear = this->_dayOfMonth;
  if (this->_month >= 2) {this->_dayOfYear = this->_dayOfYear + 31;}
  if (this->_month >= 3) {this->_dayOfYear = this->_dayOfYear + february_days;}
  if (this->_month >= 4) {this->_dayOfYear = this->_dayOfYear + 31;}
  if (this->_month >= 5) {this->_dayOfYear = this->_dayOfYear + 30;}
  if (this->_month >= 6) {this->_dayOfYear = this->_dayOfYear + 31;}
  if (this->_month >= 7) {this->_dayOfYear = this->_dayOfYear + 30;}
  if (this->_month >= 8) {this->_dayOfYear = this->_dayOfYear + 31;}
  if (this->_month >= 9) {this->_dayOfYear = this->_dayOfYear + 31;}
  if (this->_month >= 10) {this->_dayOfYear = this->_dayOfYear + 30;}
  if (this->_month >= 11) {this->_dayOfYear = this->_dayOfYear + 31;}
  if (this->_month >= 12) {this->_dayOfYear = this->_dayOfYear + 30;}
}

// ---------------------------------------------------------------------------------------------------------------
void DateTime::computeMonthName() 
{
  if (this->_month == 1) {this->_smonth = std::make_shared<std::string>("January");}
  else if (this->_month == 2) {this->_smonth = std::make_shared<std::string>("February");}
  else if (this->_month == 3) {this->_smonth = std::make_shared<std::string>("March");}
  else if (this->_month == 4) {this->_smonth = std::make_shared<std::string>("April");}
  else if (this->_month == 5) {this->_smonth = std::make_shared<std::string>("May");}
  else if (this->_month == 6) {this->_smonth = std::make_shared<std::string>("June");}
  else if (this->_month == 7) {this->_smonth = std::make_shared<std::string>("July");}
  else if (this->_month == 8) {this->_smonth = std::make_shared<std::string>("August");}
  else if (this->_month == 9) {this->_smonth = std::make_shared<std::string>("September");}
  else if (this->_month == 10) {this->_smonth = std::make_shared<std::string>("October");}
  else if (this->_month == 11) {this->_smonth = std::make_shared<std::string>("November");}
  else if (this->_month == 12) {this->_smonth = std::make_shared<std::string>("December");}
}
