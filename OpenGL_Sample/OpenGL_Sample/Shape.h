#pragma once

#include <memory>

// �}�`�f�[�^
#include "Object.h"

//=== �}�`�̕`�� ===//
class Shape
{
private:

	// �}�`�f�[�^
	std::shared_ptr<const Object> object;

protected:

	// �`��Ɏg�����_�̐�
	const GLsizei vertexCount;

public:

	//--- �R���X�g���N�^ ---//
	// size :        ���_�ʒu�̎���
	// vertexCount : ���_�̐�
	// vertex :      ���_�������i�[�����z��
	// indexCount :  ���_�C���f�b�N�X�̗v�f��
	// index :       ���_�̃C���f�b�N�X���i�[�����z��
	Shape(GLint size, GLsizei vertexCount, const Object::Vertex* vertex,
		GLsizei indexCount = 0, const GLuint* index = NULL)
		: object(new Object(size, vertexCount, vertex, indexCount, index))
		, vertexCount(vertexCount)
	{

	}

	//--- �`�� ---//
	void draw() const
	{
		// ���_�I�u�W�F�N�g����������
		object->bind();

		// �`������s����
		execute();
	}

	//--- �`��̎��s ---//
	virtual void execute() const
	{
		// �܂���ŕ`�悷��
		glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
	}
};
