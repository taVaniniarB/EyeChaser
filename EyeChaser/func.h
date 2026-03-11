#pragma once
// 자주 쓰이는 전역 함수를 모아두는 헤더


template <typename T>
void Safe_Delete_Vec(vector<T>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
			delete _vec[i];
	}

	_vec.clear();
}

template <typename T1, typename T2>
void Safe_Delete_Map(map<T1, T2>& _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();
	for (; iter != _map.end(); ++iter)
	{
		if (nullptr != iter->second)
			delete iter->second;
	}

	_map.clear();
}


#ifdef _DEBUG
#define LOG(text) WriteLog(text)
#define LOG_CLEAR() ClearLog()
#else
#define LOG(text) ((void)0)
#define LOG_CLEAR() ((void)0)
#endif

inline void WriteLog(const std::string& text) {
	std::ofstream logFile("debug_log.txt", std::ios::app);
	if (logFile.is_open()) {
		logFile << text << std::endl;
		logFile.close();
	}

}
inline void ClearLog()
{
	std::ofstream logFile("debug_log.txt", std::ios::trunc);
}