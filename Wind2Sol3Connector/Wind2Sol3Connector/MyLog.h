#ifndef MyLog_h__
#define MyLog_h__

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/detail/thread_id.hpp>
#include <boost/program_options.hpp>
#include <boost/move/utility.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>


BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level >);
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_tick_logger, boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level >);
#endif // MyLog_h__