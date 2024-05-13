#pragma once
#include <memory>
#include <GL/glew.h>

//=== ���j�t�H�[���o�b�t�@�E�I�u�W�F�N�g ===//
template <typename T>
class Uniform
{

private:

	struct UniformBuffer
	{
		// ���j�t�H�[���o�b�t�@�E�I�u�W�F�N�g��
		GLuint ubo;

		// ���j�t�H�[���̃u���b�N�̃T�C�Y
		GLsizeiptr blockSize;

		//--- �R���X�g���N�^ ---//
		//    data �Funiform�u���b�N�Ɋi�[����f�[�^
		//    count�F�m�ۂ��� uniform �u���b�N�̐�
		UniformBuffer(const T* data, unsigned int count)
		{
			// ���j�t�H�[���u���b�N�̃T�C�Y�����߂�
			GLint alignment;
			glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
			blockSize = (((sizeof(T) - 1) / alignment) + 1) * alignment;

			// ���j�t�H�[���o�b�t�@�E�I�u�W�F�N�g���쐬����
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

		//--- �f�X�g���N�^ ---//
		~UniformBuffer()
		{
			// ���j�t�H�[���o�b�t�@�E�I�u�W�F�N�g���폜����
			glDeleteBuffers(1, &ubo);
		}
	};

	// �o�b�t�@�I�u�W�F�N�g
	const std::shared_ptr<const UniformBuffer> buffer;

public:

	//--- �R���X�g���N�^ ---//
	//    data �Funiform�u���b�N�Ɋi�[����f�[�^
	//    count�F�m�ۂ��� uniform �u���b�N�̐�
	Uniform(const T* data = NULL, unsigned int count = 1)
		:buffer(new UniformBuffer(data, count))
	{
	}

	//--- ���j�t�H�[���o�b�t�@�E�I�u�W�F�N�g ---//
	//    data �Funiform�u���b�N�Ɋi�[����f�[�^
	//    start�F�f�[�^���i�[���� uniform �u���b�N�̐擪�ʒu
	//    count�F�m�ۂ��� uniform �u���b�N�̐�
	void set(const T* data, unsigned int start = 0, unsigned int count = 1) const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, buffer->ubo);
		for (unsigned int i = 0; i < count; ++i)
		{
			glBufferSubData(GL_UNIFORM_BUFFER, (start + i) * buffer->blockSize,
				sizeof(T), data + i);
		}
	}

	//--- ���̃��j�t�H�[���o�b�t�@�E�I�u�W�F�N�g���g�p���� ---//
	//    i : �������� uniform �u���b�N�̈ʒu
	//    bp: �����|�C���g
	void select(unsigned int i = 0, GLuint bp = 0) const
	{
		// �����|�C���g�Ƀ��j�t�H�[���o�b�t�@�I�u�W�F�N�g����������
		glBindBufferRange(GL_UNIFORM_BUFFER, bp,
			buffer->ubo, i * buffer->blockSize, sizeof(T));
	}
};
