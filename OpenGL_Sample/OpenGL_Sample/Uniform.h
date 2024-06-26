#pragma once
#include <memory>
#include <GL/glew.h>

//=== ユニフォームバッファ・オブジェクト ===//
template <typename T>
class Uniform
{

private:

	struct UniformBuffer
	{
		// ユニフォームバッファ・オブジェクト名
		GLuint ubo;

		// ユニフォームのブロックのサイズ
		GLsizeiptr blockSize;

		//--- コンストラクタ ---//
		//    data ：uniformブロックに格納するデータ
		//    count：確保する uniform ブロックの数
		UniformBuffer(const T* data, unsigned int count)
		{
			// ユニフォームブロックのサイズを決める
			GLint alignment;
			glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
			blockSize = (((sizeof(T) - 1) / alignment) + 1) * alignment;

			// ユニフォームバッファ・オブジェクトを作成する
			glGenBuffers(1, &ubo);
			glBindBuffer(GL_UNIFORM_BUFFER, ubo);
			glBufferData(GL_UNIFORM_BUFFER,
				count * blockSize, NULL, GL_STATIC_DRAW);
			for (unsigned int i = 0; i < count; ++i)
			{
				glBufferSubData(GL_UNIFORM_BUFFER, i * blockSize,
					sizeof(T), data + i);
			}
		}

		//--- デストラクタ ---//
		~UniformBuffer()
		{
			// ユニフォームバッファ・オブジェクトを削除する
			glDeleteBuffers(1, &ubo);
		}
	};

	// バッファオブジェクト
	const std::shared_ptr<const UniformBuffer> buffer;

public:

	//--- コンストラクタ ---//
	//    data ：uniformブロックに格納するデータ
	//    count：確保する uniform ブロックの数
	Uniform(const T* data = NULL, unsigned int count = 1)
		:buffer(new UniformBuffer(data, count))
	{
	}

	//--- ユニフォームバッファ・オブジェクト ---//
	//    data ：uniformブロックに格納するデータ
	//    start：データを格納する uniform ブロックの先頭位置
	//    count：確保する uniform ブロックの数
	void set(const T* data, unsigned int start = 0, unsigned int count = 1) const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, buffer->ubo);
		for (unsigned int i = 0; i < count; ++i)
		{
			glBufferSubData(GL_UNIFORM_BUFFER, (start + i) * buffer->blockSize,
				sizeof(T), data + i);
		}
	}

	//--- このユニフォームバッファ・オブジェクトを使用する ---//
	//    i : 結合する uniform ブロックの位置
	//    bp: 結合ポイント
	void select(unsigned int i = 0, GLuint bp = 0) const
	{
		// 結合ポイントにユニフォームバッファオブジェクトを結合する
		glBindBufferRange(GL_UNIFORM_BUFFER, bp,
			buffer->ubo, i * buffer->blockSize, sizeof(T));
	}
};
