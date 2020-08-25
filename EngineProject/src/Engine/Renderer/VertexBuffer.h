#pragma once
class VertexBuffer
{
public:
	VertexBuffer();

public:
	void Bind() const;
	void SetData(const float* Vertices, const int size, const int DrawType);

public:
	inline int GetHandle() const { return Handle; }

private:



private:
	unsigned int Handle;
};

