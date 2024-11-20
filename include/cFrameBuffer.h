// FrameBuffer.h

#pragma once
#include <iostream>
#include <glad/gl.h>
#include <glm/glm.hpp>

class cFrameBuffer
{
public:
	cFrameBuffer(float width, float height);
	~cFrameBuffer();
	unsigned int getFrameTexture();
	void RescaleFrameBuffer(float width, float height);
	void Bind() const;
	void Unbind() const;
	glm::vec2 getSize() const { return size; }
	float getAspectRatio() { return size.x / size.y; }
private:
	glm::vec2 size;
	unsigned int fbo;
	unsigned int texture;
	unsigned int depthTexture;
	unsigned int rbo;
};