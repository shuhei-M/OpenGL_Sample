#pragma once

// �}�`�̕`��
#include "Shape.h"

//=== �C���f�b�N�X���g�����}�`�̕`�� ===//
class ShapeIndex : public Shape
{
protected:

	// �`��Ɏg�����_�̐�
	const GLsizei indexCount;

public:

	// �R���X�g���N�^
	// size :        ���_�ʒu�̎���
	// vertexCount : ���_�̐�
	// vertex :      ���_�������i�[�����z��
	// indexCount :  ���_�C���f�b�N�X�̗v�f��
	// index :       ���_�̃C���f�b�N�X���i�[�����z��
	ShapeIndex(GLint size, GLsizei vertexCount, const Object::Vertex* vertex,
		GLsizei indexCount, const GLuint* index)
		:Shape(size, vertexCount, vertex, indexCount, index)
		,indexCount(indexCount)
	{

	}

	virtual void execute() const
	{
		// �����Q�ŕ`�悷��
		glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, 0);
	}
};
