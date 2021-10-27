#ifndef LOG_LOGGER_HPP
#define LOG_LOGGER_HPP


#include <boost/log/trivial.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/core.hpp>
#include <boost/log/common.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/utility/exception_handler.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/filter_parser.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/log/utility/setup/settings.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <fstream>
#include <string>
#include <string>


//BOOST_LOG_GLOBAL_LOGGER(dataLoggerIn,boost::log::sources::severity_channel_logger_mt<boost::log::trivial::severity_level>);

// Custom formatter factory to add TimeStamp format support in config ini file.
// Allows %TimeStamp(format=\"%Y.%m.%d %H:%M:%S.%f\")% to be used in ini config file for property Format.
class TimeStampFormatterFactory :
  public boost::log::basic_formatter_factory<char, boost::posix_time::ptime>
{
public:
  formatter_type create_formatter(const boost::log::attribute_name& name, const args_map& args) {
    args_map::const_iterator it = args.find("format");
    if (it != args.end()) {
      return boost::log::expressions::stream 
        << boost::log::expressions::format_date_time<boost::posix_time::ptime>(
             boost::log::expressions::attr<boost::posix_time::ptime>(name), it->second);
    } else {
      return boost::log::expressions::stream 
        << boost::log::expressions::attr<boost::posix_time::ptime>(name);
    }
  }
};

// Custom formatter factory to add Uptime format support in config ini file.
// Allows %Uptime(format=\"%O:%M:%S.%f\")% to be used in ini config file for property Format.
// boost::log::attributes::timer value type is boost::posix_time::time_duration
class UptimeFormatterFactory :
  public boost::log::basic_formatter_factory<char, boost::posix_time::time_duration>
{
public:
  formatter_type create_formatter(const boost::log::attribute_name& name, const args_map& args)
  {
    args_map::const_iterator it = args.find("format");
    if (it != args.end()) {
      return boost::log::expressions::stream
        << boost::log::expressions::format_date_time<boost::posix_time::time_duration>(
        boost::log::expressions::attr<boost::posix_time::time_duration>(name), it->second);
    } else {
      return boost::log::expressions::stream
        << boost::log::expressions::attr<boost::posix_time::time_duration>(name);
    }
  }
};

class Logger {
public:

  /// @param configFileName config ini file that contains boost logging properties.
  ///        If configFileName.empty() then default initialization.
  static void initFromConfig(const std::string& configFileName){
        // Disable all exceptions
        boost::log::core::get()->set_exception_handler(boost::log::make_exception_suppressor());

        // Add common attributes: LineID, TimeStamp, ProcessID, ThreadID
        boost::log::add_common_attributes();
        // Add boost log timer as global attribute Uptime
        boost::log::core::get()->add_global_attribute("Uptime", boost::log::attributes::timer());
        // Allows %Severity% to be used in ini config file for property Filter.
        boost::log::register_simple_filter_factory<boost::log::trivial::severity_level, char>("Severity");
        // Allows %Severity% to be used in ini config file for property Format.
        boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
        // Allows %TimeStamp(format=\"%Y.%m.%d %H:%M:%S.%f\")% to be used in ini config file for property Format.
        boost::log::register_formatter_factory("TimeStamp", boost::make_shared<TimeStampFormatterFactory>());
        // Allows %Uptime(format=\"%O:%M:%S.%f\")% to be used in ini config file for property Format.
        boost::log::register_formatter_factory("Uptime", boost::make_shared<UptimeFormatterFactory>());

        if (configFileName.empty()) {
            // Make sure we log to console if nothing specified.
            // Severity logger logs to console by default.
        } else {
            std::ifstream ifs(configFileName);
            if (!ifs.is_open()) {
            // We can log this because console is setup by default
                std::cout << "ERRORR: Cannot open congfig file" << std::endl;
            } else {
            try {
                // Still can throw even with the exception suppressor above.
                boost::log::init_from_stream(ifs);
            } catch (std::exception& e) {
                std::string err = "Caught exception initializing boost logging: ";
                err += e.what();
                // Since we cannot be sure of boost log state, output to cerr and cout.
                std::cerr << "ERRORR: " << err << std::endl;
                std::cout << "ERRORR: " << err << std::endl;
            }
            }
        }
        
    };

  /// Disable logging
  static void disable(){
    boost::log::core::get()->set_logging_enabled(false);
    };

};

#define BUILD_LOGGER(FILTER,LOGGER_NAME) BOOST_LOG_GLOBAL_LOGGER_CTOR_ARGS(LOGGER_NAME, \
boost::log::sources::severity_channel_logger_mt<boost::log::trivial::severity_level>,(boost::log::keywords::channel = FILTER));\


#define ADD_LOGGER(LOGGER_NAME) BOOST_LOG_GLOBAL_LOGGER(LOGGER_NAME, \
boost::log::sources::severity_channel_logger_mt<boost::log::trivial::severity_level>);\




#define TRACE(LOGGER_NAME,ARG) BOOST_LOG_SEV(LOGGER_NAME::get(), boost::log::trivial::trace) << ARG
#define DEBUGG(LOGGER_NAME,ARG) BOOST_LOG_SEV(LOGGER_NAME::get(), boost::log::trivial::debug) << ARG
#define INFO(LOGGER_NAME,ARG)  BOOST_LOG_SEV(LOGGER_NAME::get(), boost::log::trivial::info) << ARG
#define WARN(LOGGER_NAME,ARG)  BOOST_LOG_SEV(LOGGER_NAME::get(), boost::log::trivial::warning) << ARG
#define ERRORR(LOGGER_NAME,ARG) BOOST_LOG_SEV(LOGGER_NAME::get(), boost::log::trivial::error) << ARG
#define FATAL(LOGGER_NAME,ARG) BOOST_LOG_SEV(LOGGER_NAME::get(), boost::log::trivial::fatal) << ARG

#endif /* LOG_LOGGER_HXX */
