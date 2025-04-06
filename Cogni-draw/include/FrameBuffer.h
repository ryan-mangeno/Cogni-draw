#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <glm/glm.hpp>

struct FrameBufferSpecification 
{
	glm::uvec2 viewport;
	bool SwapChainTarget = false;

	FrameBufferSpecification(unsigned int width, unsigned int height) noexcept
		: viewport(width, height)
	{
	}

	FrameBufferSpecification(FrameBufferSpecification&& spec) noexcept
		: viewport(std::move(spec.viewport)), SwapChainTarget(spec.SwapChainTarget)
	{
	}

	// this gets deleted when we make copy constructor of our own
	FrameBufferSpecification& operator=(const FrameBufferSpecification& other) 
	{
		if (this != &other) 
		{
			viewport = other.viewport;
			SwapChainTarget = other.SwapChainTarget;
		}

		// for chaining
		return *this;
	}
};

class FrameBuffer
{
public:
	FrameBuffer(FrameBufferSpecification&& spec);
	~FrameBuffer();
	inline unsigned int get_scene_tex_ID() { return m_SceneTexture; }
	inline unsigned int get_depth_tex_ID() { return m_DepthTexture; }
	inline const FrameBufferSpecification& get_spec() { return Specification; }
	void bind() const;
	void unbind() const;
	void resize(unsigned int width, unsigned int height);
	void clear_fbo();
	void bind_scene_tex(int slot);
	void bind_depth_tex(int slot);

private:
	uint32_t m_RenderID = 0;
	uint32_t m_SceneTexture = 0;
	uint32_t m_DepthTexture = 0;

	FrameBufferSpecification Specification;

private:
	void invalidate(const FrameBufferSpecification& spec);
};


#endif