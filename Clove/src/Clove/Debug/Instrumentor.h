#pragma once

#include "Clove/Core/Core.h"

namespace Clove {

#ifdef CLOVE_DEBUG
#	define CLOVE_PROFILE 1
#endif

#ifdef CLOVE_PROFILE
#	define CLOVE_PROFILE_BEGIN_SESSION(name, filepath) ::Clove::Instrumentor::Get().BeginSession(name, filepath)
#	define CLOVE_PROFILE_END_SESSION() ::Clove::Instrumentor::Get().EndSession()
#	define CLOVE_PROFILE_SCOPE(name) ::Clove::InstrumentationTimer timer##__LINE__(name)
#	define CLOVE_PROFILE_FUNCTION() ::Clove::InstrumentationTimer timer##__LINE__(__FUNCSIG__)
#else
#	define CLOVE_PROFILE_BEGIN_SESSION(name, filepath)
#	define CLOVE_PROFILE_END_SESSION()
#	define CLOVE_PROFILE_SCOPE(name)
#	define CLOVE_PROFILE_FUNCTION()
#endif

	struct ProfileResult {
		const char* name;
		long long start, end;
		uint32_t threadID;
	};



	struct InstrumentationSession {
		std::string name;
	};



	class Instrumentor {
	public:
		Instrumentor()
			: m_current_session(nullptr), m_profile_count(0)
		{  }

		void BeginSession(const std::string& name, const std::string& filepath = "results.json") {
			m_out_stream.open(filepath);
			Instrumentor::WriteHeader();
			m_current_session = new InstrumentationSession{ name };
		}

		void EndSession() {
			Instrumentor::WriteFooter();
			m_out_stream.close();
			delete m_current_session;
			m_current_session = nullptr;
			m_profile_count = 0;
		}

		void Write(std::string data) {
			m_out_stream << data;
			m_out_stream.flush();
		}

		static Instrumentor& Get() {
			static Instrumentor* instance = new Instrumentor();
			return *instance;
		}

		void WriteProfile(const ProfileResult& result) {
			if (m_profile_count++ > 0) m_out_stream << ',';

			std::string name = result.name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_out_stream << "{ \n";
			m_out_stream << "	\"cat\":\"function\", \n";
			m_out_stream << "	\"dur\":" << (result.end - result.start) << ", \n";
			m_out_stream << "	\"name\":\"" << name << "\", \n";
			m_out_stream << "	\"ph\":\"X\", \n";
			m_out_stream << "	\"pid\":0, \n";
			m_out_stream << "	\"tid\":" << result.threadID << ", \n";
			m_out_stream << "	\"ts\":" << result.start << " \n";
			m_out_stream << "} \n";

			m_out_stream.flush();
		}

	private:

		void WriteHeader() {
			m_out_stream << "{ \n	\"otherData\": {}, \n	\"traceEvents\":[ \n\n";
			m_out_stream.flush();
		}

		void WriteFooter() {
			m_out_stream << "]}";
			m_out_stream.flush();
		}

	private:
		InstrumentationSession* m_current_session;
		std::ofstream m_out_stream;
		int m_profile_count;
	};




	class InstrumentationTimer {
	public:
		InstrumentationTimer(const char* name)
			: m_name(name), m_stopped(false)
		{
			int cdecl_where = m_name.find("__cdecl ");
			int thiscall_where = m_name.find("__thiscall ");
			if (cdecl_where != std::string::npos) {
				m_name.erase(cdecl_where, strlen("__cdecl ") - 1);
			}
			else if (thiscall_where != std::string::npos) {
				m_name.erase(thiscall_where, strlen("__thiscall ") - 1);
			}

			m_start_point = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer() {
			if (!m_stopped) InstrumentationTimer::Stop();
		}

		void Stop() {
			auto end_point = std::chrono::high_resolution_clock::now();
			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start_point).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(end_point).time_since_epoch().count();
			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::Get().WriteProfile({ m_name.c_str(), start, end, threadID});
			m_stopped = true;
		}

	private:
		std::string m_name;
		bool m_stopped;
		std::chrono::time_point<std::chrono::steady_clock> m_start_point;
	};


}

