#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>

struct FrameBufferSpecification 
{
	unsigned int num_color_attachments;
	glm::uvec2 viewport;
	bool SwapChainTarget = false;

	FrameBufferSpecification(unsigned int width, unsigned int height, unsigned int num_color_attachments) noexcept
		: viewport(width, height), num_color_attachments(num_color_attachments)
	{
	}

	FrameBufferSpecification(FrameBufferSpecification&& spec) noexcept
		: viewport(std::move(spec.viewport)), SwapChainTarget(spec.SwapChainTarget), num_color_attachments(num_color_attachments)
	{
	}

	// this gets deleted when we make copy constructor of our own
	FrameBufferSpecification& operator=(const FrameBufferSpecification& other) 
	{
		if (this != &other) 
		{
			viewport = other.viewport;
			SwapChainTarget = other.SwapChainTarget;
			num_color_attachments = other.num_color_attachments;
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
	inline unsigned int get_paint_scene_tex_ID() { return m_ColorAttachments[0]; }
	inline unsigned int get_full_scene_tex_ID() { return m_ColorAttachments[1]; }
	inline unsigned int get_depth_tex_ID() { return m_DepthTexture; }
	inline const FrameBufferSpecification& get_spec() { return Specification; }

	void set_draw_target(unsigned int slot);
	void draw_to_all_targets();

	// make method which passes in all indices to draw to
	void bind() const;
	void unbind() const;
	void resize(unsigned int width, unsigned int height);
	void clear_fbo();
	void clear_fbo_target(uint32_t slot);
	void blit_buffer();
	void bind_scene_tex(int slot);
	void bind_depth_tex(int slot);

private:
	uint32_t m_RenderID = 0;
	uint32_t m_DepthTexture = 0;

	FrameBufferSpecification Specification;
	std::vector<uint32_t> m_ColorAttachments;

private:
	void invalidate(const FrameBufferSpecification& spec);
};


#endif