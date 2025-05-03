#ifndef MODEL_BUFFER_H
#define MODEL_BUFFER_H

#include "Model.h"
#include "RunPy.h"
#include <queue>



class ModelBuffer
{
public:

	
	ModelBuffer() = default;
	~ModelBuffer() = default;

	inline static ModelBuffer& get_instance()
	{
		static ModelBuffer instance;
		return instance;
	}

	inline void push_model()
	{
		m_ModelQueue.emplace(std::make_unique<AsyncPy>("python/generate_model.py"));
	}

	inline void pop_model()
	{
		if (m_ModelQueue.empty())
			return;

		m_ModelQueue.pop();
	}

	inline bool peek_buffer()
	{
		if (m_ModelQueue.empty())
			return false;

		auto& script_ptr = m_ModelQueue.front();
		return script_ptr->check_done();
	}



private:
	std::queue<std::unique_ptr<AsyncPy>> m_ModelQueue;

};


#endif